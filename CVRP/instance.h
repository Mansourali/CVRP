#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <map> 
#include "customer.h"
#include "terminationCriteria.h"
using namespace std;

class Instance
{
public:

	string instanceName;
	vector<Customer> customers;
	int capacity;
	int minTrucks;
	int dimension;
	int depot;
	int optimalValue;
	vector<vector<double>> distances;
	double tightness;

	enum distanceCalculationTypeAs {integerType,realType};
	enum TsplibKeyword {
		// The specification part
		NAME, TYPE, COMMENT, DIMENSION, CAPACITY,
		EDGE_WEIGHT_TYPE, EDGE_WEIGHT_FORMAT, EDGE_DATA_FORMAT,
		NODE_COORD_TYPE, DISPLAY_DATA_TYPE, END_OF_FILE,

		// The data part
		NODE_COORD_SECTION, DEPOT_SECTION, DEMAND_SECTION,
		EDGE_DATA_SECTION, EDGE_WEIGHT_SECTION,
	};

	enum EdgeWeightType {EXPLICIT, EUC_2D};
	enum EdgeWeightFormat {LOWER_ROW};
	
	Instance();
	void loadInstance(string s, distanceCalculationTypeAs type);
	bool load(string filename);
	void loadSettings(int tc, string ne, string ng, string ns);
	void callculateDistances(distanceCalculationTypeAs type);
	void read(const string &infile);
};

