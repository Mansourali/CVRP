#pragma once
#include <time.h>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <map>
#include <chrono>

namespace setting{   //SWAP, MOVE, REVERSE,
	enum neighborStructureType			{ SHIFT_1_0, SHIFT_2_0, SWAP_1_1, SWAP_1_2, SWAP_2_2, CROSS, K_SHIFT, REINSERTION, OR_OPT2, OR_OPT3, TWO_OPT, EXCAHNGE, SWAP, MOVE, REVERSE, };
	enum PerturbationMechanisms			{ MULTIPLE_SWAP_1_1, MULTIPLE_SHIFT_1_1, MULTIPLE_REINSERTION_1, DOUBLE_PRIDGE };
	enum neighborSelectorType			{ FIRST, BEST, POSITIVE_NEIGHBOR, NEGATIVE_NEIGHBBOR, CONSTRAINT_VIOLATED, RANDOM };
	namespace selectionStrategy			{ enum by { TOURNMENT, DISRUPTIVE, RANK, RANDOM }; }
	enum algorithmCategory				{ POPULATION_BASED, SINGLE_BASED };
	namespace initializationTechnique	{ enum by {			RANDOM, CLARKE_AND_WRIGHT,
															NN_N_DEPOT_NEAREST_ADD,
															NN_F_DEPOT_NEAREST_ADD,
															NN_N_DEPOT_COMPINED_ADD,
															NN_F_DEPOT_COMPINED_ADD,
															NN_NF_DEPOT_COMPINED_ADD,
															NN_NF_DEPOT_NEAREST_ADD,
															NN_NR_DEPOT_COMPINED_ADD,
															NN_NR_DEPOT_NEAREST_ADD,
															NN_FR_DEPOT_COMPINED_ADD,
															NN_FR_DEPOT_NEAREST_ADD,
															NN_R_DEPOT_NEAREST_ADD
	};
	namespace initializationCriteria { enum by { NEAREST, FARTEST, NEAREST_FARTEST, NEAREST_RANDOM , FARTEST_RANDOM , RANDOM }; }
	namespace InsertionCriteria { enum by { NEAREST, COMPINED }; }
	}
	};

using namespace setting;
using namespace std;

#define THROW(VRPException, message) throw VRPException(__FILE__, __LINE__, (message) )

struct VRPException : public exception {
	string s;
	VRPException(string ss) : s(ss) {}
	~VRPException() throw () {} // Updated
	const char* what() const throw() { return s.c_str(); }
};

class AlgorithmSetting {
protected:
	int NEIGHBORHOOD_GENERATOR; // full , random
	int NEIGHBOR_SELECTOR;	    // best , first
	int TERMINATION_CRITERIA;	// iteration based
	int ALGORITM_CATEGORY;
	int ACCEPT_WORSE;
public:
	vector<neighborStructureType> NEIGHBORHOOD_STRUCTURE; // swap , move , reverse
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType, neighborStructureType);
	void setNEIGHBORHOOD_STRUCTURE(neighborStructureType);
	void setALGORITM_CATEGORY(algorithmCategory);
	//void setNEIGHBORHOOD_GENERATOR(int value);
	void setNEIGHBOR_SELECTOR(int);
	void setTERMINATION_CRITERIA(int);
	void setACCEPT_WORSE(int);
	int getTERMINATION_CRITERIA();
	vector<neighborStructureType> getNEIGHBORHOOD_STRUCTURE();
	//neighborGeneratorType getNEIGHBORHOOD_GENERATOR();
	neighborSelectorType getNEIGHBOR_SELECTOR();
	algorithmCategory getALGORITM_CATEGORY();
};

class SingleBasedSetting : public AlgorithmSetting {
public:
	SingleBasedSetting() : AlgorithmSetting() { setALGORITM_CATEGORY(SINGLE_BASED); };
};

class PopulationBasedSetting : public AlgorithmSetting {
public:
	PopulationBasedSetting() : AlgorithmSetting() { setALGORITM_CATEGORY(POPULATION_BASED); };
	int SELECTION_STRATEGY;
	int POPULATION_SIZE;
	void setSELECTION_STRATEGY(selectionStrategy::by value);
	selectionStrategy::by getSELECTION_STRATEGY();
	void setPOPULATION_SIZE(int size);
	int getPOPULATION_SIZE();
};

struct InstanceResult{
	float initialFittness;
	float bestFittness;
	float timeElapsed;
	float timeToBest;
	float gapToBest;
	float optimal;
	void saveInstanceRunDetails(float iF, float bf, float bT, float dif, float tt, float opt){
		initialFittness = iF; bestFittness = bf; timeElapsed = tt; gapToBest = dif; timeToBest = bT; optimal = opt;
	};
};

struct neighborCount{
	neighborStructureType type_id;
	int ne_count_succ; //improve the solution
	int ne_count;		// selected
};

class Utility
{
	public:
		Utility();
		vector<string> SOVER_NAME;
		SingleBasedSetting SINGLE;
		PopulationBasedSetting POPULATION;
		vector<string> instancesName;
		void setSOLVER_NAME(string& solverName);
		string getSOLVER_NAME(int index);
		int getTERMINATION_CRITERIA();
		void printSetting(SingleBasedSetting S);
		void printSetting(PopulationBasedSetting P);
		void printSetting(vector<InstanceResult> ir);
		void static printTwoDimintionVector(vector<vector<double>>& mVector);
		static double get_cpu_time();
		static double getRandomUniformNo();
		static double getRandomUniformNoBetween(int min, int max);
		static double getRandomLevyNoBetween(int min, int max);
		static void spinner();
		void readAllInstancesName(string folder);
		static char* itgetNeighborStructureAsName(neighborStructureType value);
};
