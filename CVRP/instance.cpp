#include "instance.h"
#include "utility.h"

#define roundToInt(a) ((a) >= 0.0 ? (int)((a)+0.5) : (int)((a)-0.5));

Instance::Instance() {};

void Instance::loadInstance(string filePath, distanceCalculationTypeAs type){
	read(filePath);
	callculateDistances(type);
}

void Instance::callculateDistances(distanceCalculationTypeAs type) {

	distances.resize( dimension );

	for (unsigned int i = 0; i < dimension; ++i)
		distances[i].resize(dimension);

	switch (type)
	{
	case Instance::integerType:
		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < i; j++) {
				int dx = customers[j].coordinate.first - customers[i].coordinate.first;
				int dy = customers[j].coordinate.second - customers[i].coordinate.second;
				distances[i][j] = distances[j][i] = floor(sqrt(dx*dx + dy*dy) + 0.5);
			}
		}
		break;
	case Instance::realType:
		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < i; j++) {
				int dx = customers[j].coordinate.first - customers[i].coordinate.first;
				int dy = customers[j].coordinate.second - customers[i].coordinate.second;
				distances[i][j] = distances[j][i] = sqrt(dx*dx + dy*dy);
			}
		}
		break;
	default:
		break;
	}
}

void trim(string& str, const string& trim_char) {
	size_t pos;
	while ((pos = str.find_first_of(trim_char)) != string::npos)
		str.erase(pos, 1);
}

string get_parameter(ifstream& ifs) {
	string param;
	ifs >> param;
	while (param == ":") ifs >> param;
	return param;
}

int get_numericalValue(string& strInput, string toFind, int from, int to) throw(VRPException) {
	if (strInput.find(toFind) != std::string::npos){
		std::string strNumber = "0";
		size_t iIndex = strInput.find(toFind);
		strNumber = strInput.substr(iIndex + from, to);
		trim(strNumber, ", :)");
		return stoi(strNumber);
	}
	else throw VRPException("error: can't find Min no. of trucks or optimal value");
}

void Instance::read( const string &infile) throw (VRPException) {

	map<string, Instance::TsplibKeyword> keyword_map = {
		// The specification part
		{ "NAME",                Instance::NAME },
		{ "TYPE",                Instance::TYPE },
		{ "COMMENT",             Instance::COMMENT },
		{ "DIMENSION",           Instance::DIMENSION },
		{ "CAPACITY",            Instance::CAPACITY },
		{ "EDGE_WEIGHT_TYPE",    Instance::EDGE_WEIGHT_TYPE },
		{ "EDGE_WEIGHT_FORMAT",  Instance::EDGE_WEIGHT_FORMAT },
		{ "EDGE_DATA_FORMAT",    Instance::EDGE_DATA_FORMAT },
		{ "NODE_COORD_TYPE",     Instance::NODE_COORD_TYPE },
		{ "DISPLAY_DATA_TYPE",   Instance::DISPLAY_DATA_TYPE },
		{ "EOF",                 Instance::END_OF_FILE },

		// The data part
		{ "NODE_COORD_SECTION",  Instance::NODE_COORD_SECTION },
		{ "DEPOT_SECTION",       Instance::DEPOT_SECTION },
		{ "DEMAND_SECTION",      Instance::DEMAND_SECTION },
		{ "EDGE_DATA_SECTION",   Instance::EDGE_DATA_SECTION },
		{ "EDGE_WEIGHT_SECTION", Instance::EDGE_WEIGHT_SECTION }
	};

	map<string, EdgeWeightType> ew_type_map = {
		{ "EXPLICIT", EXPLICIT },
		{ "EUC_2D", EUC_2D }
	};

	map<string, EdgeWeightFormat> ew_format_map = {
		{ "LOWER_ROW", LOWER_ROW }
	};

	ifstream ifs(infile.c_str());
	if (!ifs)
		throw VRPException("error: can't open file " + infile);

	EdgeWeightType   edge_weight_type;
	EdgeWeightFormat edge_weight_format;
	double totalDemand=0;

	while (ifs) {
		string tsp_keyword;
		ifs >> tsp_keyword;
		if (ifs.eof()) break;
		trim(tsp_keyword, " :");
		switch (keyword_map[tsp_keyword]) {

			// The specification part
		case NAME :
			instanceName = get_parameter(ifs);
			break;
		case TYPE :
			{
				string not_use;
				getline(ifs, not_use);
			}
			break;
		case COMMENT :
			{
				string strInput;
				getline(ifs, strInput);
				try {
					minTrucks = get_numericalValue(strInput, "trucks: ", 7, 3);
					optimalValue = get_numericalValue(strInput, "Optimal value: ", 14, 5);	}
				catch (VRPException& caught) { cout << caught.what() << endl; }
			}
			break;
		case DIMENSION :
			this->dimension = stoi(get_parameter(ifs));
			break;
		case CAPACITY :
			this->capacity = stoi(get_parameter(ifs));
			break;
		case EDGE_WEIGHT_TYPE :
			edge_weight_type = ew_type_map[get_parameter(ifs)];
			break;
		case EDGE_WEIGHT_FORMAT :
			edge_weight_format = ew_format_map[get_parameter(ifs)];
			break;
		case EDGE_DATA_FORMAT :
			{
				string not_use;
				getline(ifs, not_use);
			}
			break;
		case NODE_COORD_TYPE :
			{
				string not_use;
				getline(ifs, not_use);
			}
			break;
		case DISPLAY_DATA_TYPE :
			{
				string not_use;
				getline(ifs, not_use);
			}
			break;
		case END_OF_FILE :
			// do nothing
			break;

			// The data part
		case NODE_COORD_SECTION :
			{
				double m, x, y; // m do not use
				for (int i = 0; i != dimension; i++) {
					ifs >> m >> x >> y;
					Customer c(x,y);
					customers.push_back(c);
				}
			}
			break;
		case DEPOT_SECTION :
			{
				depot = stoi(get_parameter(ifs));
				if (stoi(get_parameter(ifs)) != -1)
					throw VRPException("error:"
					"can't handle multiple depots");
			}
			break;
		case DEMAND_SECTION :
			{
				for (int i = 1; i <= dimension; i++) {
					unsigned int node_id, demand;
					ifs >> node_id >> demand;
					if (node_id != i)
						throw VRPException("error:""DEMAND_SECTION format may be different");
					this->customers.at(i - 1).setDemand(demand);
					totalDemand += demand;
				}
			}
			break;
		case EDGE_DATA_SECTION :
			throw VRPException("Sorry, can not handle 'EDGE_DATA_SECTION'");
			break;
		case EDGE_WEIGHT_SECTION :
			{
#pragma warning(suppress: 6001)
				if (edge_weight_format != LOWER_ROW)
					throw VRPException("Sorry, can not handle except EDGE_WEIGHT_FORMAT == LOWER_ROW");
				for (int i = 0; i < dimension; i++) {
					for (int j=0; j < i; j++) {
						int distance;
						ifs >> distance;
						//distances.push_back(distance);
					}
				}
			}
			break;

		default :
			throw VRPException("error: unknown keyword '" + tsp_keyword);
			break;
		}
	}

	//if (edge_weight_type != EXPLICIT) {

	tightness = totalDemand/( capacity * this->minTrucks);

	//	callculateDistances();
	//}

}