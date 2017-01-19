#include "utility.h"

void Utility::spinner() {
	static char bars[] = { '/', '-', '\\', '|' };
	static int nbars = sizeof(bars) / sizeof(char);
	static int pos = 0;

	printf("%c\r", bars[pos]);
	fflush(stdout);
	pos = (pos + 1) % nbars;
}

double Utility::get_cpu_time(){
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0){
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else{
		//  Handle error
		return 0;
	}
}

double Utility::getRandomLevyNoBetween(int min, int max){

	double shifted, number;
	max--;
	do {
		random_device rd;
		mt19937 gen(rd());
		cauchy_distribution<> distribution(min, max);
		number = distribution(gen);

		int range = max - min;
		int buckets = RAND_MAX / range;

		shifted = static_cast<int>(min + (number / buckets) * 1000);
		if (shifted < 0) shifted = shifted * -1;
	} while (shifted>max);
	//cout << number << endl;
	//cout <<" number=" << number <<" shifted=" << shifted <<endl;
		return shifted+1;
}

Utility::Utility(){
	srand( (unsigned int)time(NULL) );
	cout << "Compiled file is: "		<< __FILE__ << ".\n";
	cout << "Its compilation began: "	<< __DATE__;
	cout << " at: "						<< __TIME__ << ".\n\n";
}

double Utility::getRandomUniformNo (){
	return getRandomUniformNoBetween(0,1);
}

double Utility::getRandomUniformNoBetween(int min, int max){
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(min, max);
	//cout << dis(gen) << endl;

	return dis(gen);
}

void Utility::readAllInstancesName(string folder) throw(VRPException) {
	string instance;
	ifstream infile;
	infile.open(folder + "Instances-names.txt");
	if (!infile)
		throw VRPException("error: can't open file instances name");
	while (!infile.eof())
	{
		getline(infile, instance);
		instancesName.push_back(folder + instance);
	}
	infile.close();
}

int AlgorithmSetting::getTERMINATION_CRITERIA(){
	return TERMINATION_CRITERIA;
}

int Utility::getTERMINATION_CRITERIA () {
	return (POPULATION.getTERMINATION_CRITERIA() != 0) ? POPULATION.getTERMINATION_CRITERIA() : SINGLE.getTERMINATION_CRITERIA();
}

int  PopulationBasedSetting::getPOPULATION_SIZE(){
	return POPULATION_SIZE;
}

//neighborGeneratorType AlgorithmSetting::getNEIGHBORHOOD_GENERATOR(){
//	return static_cast<neighborGeneratorType>(NEIGHBORHOOD_GENERATOR);
//}

vector<neighborStructureType> AlgorithmSetting::getNEIGHBORHOOD_STRUCTURE(){
	vector<neighborStructureType> casting (NEIGHBORHOOD_STRUCTURE.begin (), NEIGHBORHOOD_STRUCTURE.end ()) ;
	return casting ;
}

neighborSelectorType AlgorithmSetting::getNEIGHBOR_SELECTOR(){
	return static_cast<neighborSelectorType>(NEIGHBOR_SELECTOR);
}

selectionStrategy::by PopulationBasedSetting::getSELECTION_STRATEGY(){
	return static_cast<selectionStrategy::by>(SELECTION_STRATEGY);
}

void AlgorithmSetting::setACCEPT_WORSE(int value){
	ACCEPT_WORSE = value;
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6, neighborStructureType value7, neighborStructureType value8, neighborStructureType value9, neighborStructureType value10, neighborStructureType value11, neighborStructureType value12){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
	NEIGHBORHOOD_STRUCTURE.push_back(value12);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6, neighborStructureType value7, neighborStructureType value8, neighborStructureType value9, neighborStructureType value10, neighborStructureType value11){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5, value6, value7, value8, value9, value10);
	NEIGHBORHOOD_STRUCTURE.push_back(value11);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6, neighborStructureType value7, neighborStructureType value8, neighborStructureType value9, neighborStructureType value10){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5, value6, value7, value8, value9);
	NEIGHBORHOOD_STRUCTURE.push_back(value10);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6, neighborStructureType value7, neighborStructureType value8, neighborStructureType value9){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5, value6, value7, value8);
	NEIGHBORHOOD_STRUCTURE.push_back(value9);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6, neighborStructureType value7, neighborStructureType value8){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5, value6, value7);
	NEIGHBORHOOD_STRUCTURE.push_back(value8);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6, neighborStructureType value7){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5, value6);
	NEIGHBORHOOD_STRUCTURE.push_back(value7);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5, neighborStructureType value6){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4, value5);
	NEIGHBORHOOD_STRUCTURE.push_back(value6);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4, neighborStructureType value5){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3, value4);
	NEIGHBORHOOD_STRUCTURE.push_back(value5);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3, neighborStructureType value4){
	setNEIGHBORHOOD_STRUCTURE(value1, value2, value3);
	NEIGHBORHOOD_STRUCTURE.push_back(value4);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2, neighborStructureType value3){
	setNEIGHBORHOOD_STRUCTURE (value1, value2);
	NEIGHBORHOOD_STRUCTURE.push_back (value3);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1, neighborStructureType value2){
	setNEIGHBORHOOD_STRUCTURE (value1);
	NEIGHBORHOOD_STRUCTURE.push_back (value2);
}

void AlgorithmSetting::setNEIGHBORHOOD_STRUCTURE(neighborStructureType value1){
	NEIGHBORHOOD_STRUCTURE.push_back (value1);
}

void PopulationBasedSetting::setSELECTION_STRATEGY(selectionStrategy::by value1){
	SELECTION_STRATEGY = value1;
}

void AlgorithmSetting::setALGORITM_CATEGORY(algorithmCategory value1){
	ALGORITM_CATEGORY = static_cast<int>(value1);
}

//void AlgorithmSetting::setNEIGHBORHOOD_GENERATOR(int value){
//	NEIGHBORHOOD_GENERATOR = value;
//}

void AlgorithmSetting::setNEIGHBOR_SELECTOR(int value){
	NEIGHBOR_SELECTOR = value;
}

void AlgorithmSetting::setTERMINATION_CRITERIA(int value){
	TERMINATION_CRITERIA = value ;
}

void PopulationBasedSetting::setPOPULATION_SIZE(int value){
	POPULATION_SIZE = value;
}

algorithmCategory AlgorithmSetting::getALGORITM_CATEGORY(){
	return static_cast<algorithmCategory>(ALGORITM_CATEGORY);
}

void Utility::setSOLVER_NAME(string& solverName){
	SOVER_NAME.push_back(solverName);
}

string Utility::getSOLVER_NAME(int index) {
	if (index >= SOVER_NAME.size()) return "";
	return SOVER_NAME[index];
}

char* getNeighborSelectorAsName(neighborSelectorType value) {
	switch (value) {
		case BEST: return "BEST";
		case FIRST: return "FIRST";
		case POSITIVE_NEIGHBOR: return "POSITIVE NEIGHBOR";
		case NEGATIVE_NEIGHBBOR: return "NEGATIVE NEIGHBBOR";
		case RANDOM: return "RANDOM";
	}
}

char* Utility::itgetNeighborStructureAsName(neighborStructureType value) {
	switch (value) {
		case SWAP:		return "SWAP";
		case MOVE:		return "MOVE";
		case REVERSE:	return "REVERSE";
		case SHIFT_1_0: return "SHIFT 1 0";
		case SHIFT_2_0: return "SHIFT 2 0";
		case SWAP_1_1:	return "SWAP 1 1";
		case SWAP_1_2:	return "SWAP 1 2";
		case SWAP_2_2:	return "SWAP 2 2";
		case CROSS:		return "CROSS";
		case K_SHIFT:	return "K SHIFT";
		case REINSERTION:return "REINSERTION";
		case OR_OPT2:	return "OR OPT 3";
		case OR_OPT3:	return "OR OPT 4";
		case TWO_OPT:	return "TWO OPT";
		case EXCAHNGE:	return "EXCHANGE";
	}
}

char* getNeighborStructureAsName(neighborStructureType value) {
	switch (value) {
	case SWAP:		return "SWAP";
	case MOVE:		return "MOVE";
	case REVERSE:	return "REVERSE";
	case SHIFT_1_0: return "SHIFT 1 0";
	case SHIFT_2_0: return "SHIFT 2 0";
	case SWAP_1_1:	return "SWAP 1 1";
	case SWAP_1_2:	return "SWAP 1 2";
	case SWAP_2_2:	return "SWAP 2 2";
	case CROSS:		return "CROSS";
	case K_SHIFT:	return "K SHIFT";
	case REINSERTION:return "REINSERTION";
	case OR_OPT2:	return "OR OPT 3";
	case OR_OPT3:	return "OR OPT 4";
	case TWO_OPT:	return "TWO OPT";
	case EXCAHNGE:	return "EXCHANGE";
	}
}

void Utility::printSetting(SingleBasedSetting S) {
	cout << setfill('.') << left << setw(30) << "Algorithm Setting: "	  << left << " " << getSOLVER_NAME(0)										 << endl;
	cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"																 << endl;
	cout << setfill('.') << left << setw(30) << "Termination criteria: "  << left << " " << S.getTERMINATION_CRITERIA ()							 << endl;
	cout << setfill('.') << left << setw(30) << "Neighborhood selector: " << left << " " << getNeighborSelectorAsName(S.getNEIGHBOR_SELECTOR())		 << endl;
	for (unsigned int i = 0; i < S.NEIGHBORHOOD_STRUCTURE.size(); i++)
	cout << setfill('.') << left << setw(30) << "Neighborhood structure: "<< left << " " << getNeighborStructureAsName(S.NEIGHBORHOOD_STRUCTURE[i])  << endl;
	cout << endl << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"														 << endl;
}

void Utility::printSetting(PopulationBasedSetting P) {
	cout << setfill('.') << left << setw(30) << "Algorithm Setting: "	  << left << " " << getSOLVER_NAME(1)										 << endl;
	cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"																 << endl;
	cout << setfill('.') << left << setw(30) << "Population size: "		  << left << " " << P.getPOPULATION_SIZE()									 << endl;
	cout << setfill('.') << left << setw(30) << "Termination criteria: "  << left << " " << P.getTERMINATION_CRITERIA()								 << endl;
	cout << setfill('.') << left << setw(30) << "Neighborhood selector: " << left << " " << getNeighborSelectorAsName(P.getNEIGHBOR_SELECTOR())		 << endl;
	for (unsigned int i = 0; i < P.NEIGHBORHOOD_STRUCTURE.size(); i++)
	cout << setfill('.') << left << setw(30) << "Neighborhood structure: "<< left << " " << getNeighborStructureAsName(P.NEIGHBORHOOD_STRUCTURE[i])  << endl;
	cout << endl << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"														 << endl;
}

void Utility::printSetting(vector<InstanceResult> ir){
	cout << setfill('.') << left << setw(30) << "Statistical report after "<< left << " " << ir.size() << " run" << endl;
	cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
	cout << setfill(' ') << left << setw(15) << "Initial fit:" << left << setw(15) << "Best fit:" << left << setw(15) << "Total Time:" << left << setw(15) << "Time to best:" << left << setw(15) << "Gap:" << endl;
	for (size_t i = 0; i < ir.size(); i++)	{
		cout << setfill(' ') << left << setw(15) << ir[i].initialFittness << left << setw(15) << ir[i].bestFittness << left << setw(15) << ir[i].timeElapsed << left << setw(15) << ir[i].timeToBest   << left << setw(15) << ir[i].gapToBest << endl;
	}
	cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl; 
}

void Utility::printTwoDimintionVector(vector<vector<double>>& mVector){
	for (int i = 0; i < mVector[0].size(); i++){
		for (int j = 0; j < mVector.size(); j++){
			cout << mVector[j][i] << " ";
		}
		cout << endl;
	}
}

//double Utility::getRandomLevyNoBetween(int min, int max){
//	double u, v = 0, t, s, scaled, shifted, range;
//	const double PI = 3.14159265359;
//
//	int c = 1; // scale parameter
//	int alpha = 2;
//	range = max - min;
//
//	u = PI * ( getRandomUniformNo() - 0.5);
//
//	if (alpha == 1){         //CAUCHY
//		scaled = (c*(tan(u)));
//		shifted = scaled * range + min;
//		return shifted;
//	}
//
//	while (v == 0)	
//		v = -log(getRandomUniformNo());
//
//	if (alpha == 2){          //GAUSSIAN
//		scaled = (c*(2 * sin(u)*sqrt(v)))*range;
//		shifted = scaled + min;
//		return shifted;
//	}
//
//	//GENERAL CASE
//	t = sin(alpha*u) / pow(cos(u), 1 / alpha);
//	s = pow(cos((1 - alpha)*u) / v, (1 - alpha) / alpha);
//
//		scaled = (c*t*s)*range; shifted = scaled + min;
//		return shifted;
//}