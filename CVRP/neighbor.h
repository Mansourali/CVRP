#pragma once
#include "solution.h"
#include "utility.h"
using namespace setting;
typedef double constraintViolated ;

class SaveNeighborhood;
class Neighbor {
public:
	Neighbor(){};
	Neighbor(Utility& u, algorithmCategory& cat);
	void setIntitialSolution(Solution &solution);
	bool generateMoveNeighborhood(int a, int b);
	bool generateSwapNeighborhood(int a, int b);
	bool generateReverseNeighborhood(int a);
	bool generateMoveNeighborhood();
	bool generateSwapNeighborhood();
	bool generateReverseNeighborhood();
	double scoreImprovementIfSwapped(int a, int b, int i, int j);
	double scoreImprovementIfMoved	(int a, int b, int i, int j);
	double scoreImprovementIfReversed(int a, int i, int j);
	void swap(int a, int b, int i, int j);
	void move(int a, int b, int i, int j);
	void reversed(int a, int i, int j);
	bool Shift_1_0();
	bool Shift_2_0();
	bool Swap_1_1();
	bool Swap_2_1();
	bool Swap_2_2();
	bool Cross();
	bool K_Shift();
	bool Reinsertion();
	bool Or_opt2();
	bool Or_opt3();
	bool Two_opt();
	bool Exchange();
	Solution* Rinsertion_perturbation(int = 1);
	Solution* Swap_perturbation(int = 1);
	Solution* Shift_perturbation(int = 1);
	void Double_bridge();
	vector<SaveNeighborhood> makeNeighborState ();
	vector<SaveNeighborhood> makeNeighborState(neighborStructureType nS);
	SaveNeighborhood makeOneNeighborState(int routeA, int routeB, int i, int j);
	SaveNeighborhood makeOneRandomNeighborState();
	void makeOneRandomNeighbor();
	vector<neighborStructureType> neighborStructure;
	neighborSelectorType neighborSelector;
	//neighborGeneratorType neighborGenerator;
	vector<SaveNeighborhood> obtainedNeighbor;
	Solution* SelectedSolution;
	int selectedRoute;
	constraintViolated const constraintviolated = -9999.00;
	Instance* instance;
};

class SaveNeighborhood {
public:
	SaveNeighborhood(){};
	SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, PerturbationMechanisms m, Route r1, Route r2);
	SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, neighborStructureType m);
	SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, neighborStructureType m, Route r1);
	SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, neighborStructureType m, Route r1, Route r2);
	void setNeighborSelectorType(neighborSelectorType t);
	double getImprovmentValue();
	int getARoute();
	int getBRoute();
	int getICustomer();
	int getJCustomer();
	int getNeighborStructure();
	int getNeighborSelector();
	Route getFirstRoute();
	Route getSecondRoute();
	static bool compImprovmentValue(SaveNeighborhood& a, SaveNeighborhood& b) { return a.getImprovmentValue() > b.getImprovmentValue(); };
	static void sortNeighborByImproveValue(vector<SaveNeighborhood> *obtained) { sort(obtained->begin(), obtained->end(), &compImprovmentValue); };
protected:
	Route firstRoute;
	Route secondRoute;
	int a, b, i, j;
	double improvmentValue;
	int neighborStructure;
	int neighborSelector;
};