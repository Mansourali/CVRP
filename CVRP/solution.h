#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "utility.h"
#include "customer.h"
#include "route.h"
#include "instance.h"
using namespace std;
using namespace setting;
using namespace setting::initializationTechnique;
using namespace setting::initializationTechnique::initializationCriteria;
using namespace setting::initializationTechnique::InsertionCriteria;

#define getRouteAt(index) getRouteAtCaller(index, __FUNCTION__)
#define getRandomRouteIndex() getRandomRouteIndexCaller(__FUNCTION__)

class Utility;
class Route;
class Instance;
class Solution 
{
protected:
	int totalCustomer;
	bool isSolutionFitnessCalculated;
	double solutionFitness;
	double initialFitness;
	double probabilityRank;
public:
	vector<Route> routes;
	Instance* problem_instance;
	Solution();
	Solution(Instance* i);
	int getSolutionRoutesSize() const { return routes.size(); }
	int getSolutionDemand() const;
	double getSolutionFitness();
	double getInitialFitness() const { return initialFitness; }
	void calculatSolutionFitness();
	int getRandomRouteIndexCaller(string caller) const;
	int getTotalCustomer() const { return totalCustomer; }
	int getRoutesSize() const { return routes.size(); }
	double getProbabilityRank() const { return probabilityRank; }
	void setProbabilityRank(double val) { probabilityRank = val; }
	bool customerSizeIsViolated();
	void setFitnessNotCalculated() { isSolutionFitnessCalculated = false; }
	Route getRouteAtCaller(int routeIndex, string caller) const;
	void randomInitialization();
	void nearestNeighbor();
	void nearestneigborInitialization(initializationCriteria::by mInitial, InsertionCriteria::by mInsert);
	void cLarkeAndWright();
	vector<vector<int>>& computeSaving();
	bool mergeFeasibility(int& customerA, int& customerB, vector<Route>& allRoutes);
	void randomnearestneigborInitialization();
	void deleteVisitedCustomer(int customer, vector<int>& remaining);
	int nearestCustomerPair(int currentNearest, vector<int>& remianing);
	int fartestCustomerPair(int currentfartest, vector<int>& remianing);
	int randomCustomerPair(vector<int>& remianing);
	void clear();
	void ready();
	void printSolution() const;
	bool operator<(Solution& toCompare);
	bool operator>(Solution& toCompare);
	bool alreadyFound(Solution& toCompare);
	static vector<double> measureDiversification(vector<Solution>& population, int vehicle);
	static Solution& getCurrentBestSolution(vector<Solution>* initialSolutions);
	static bool compFitness(const Solution& a, const Solution& b);
	static vector<Solution> solutionsFactory(vector<Solution> *initialSolutions, initializationTechnique::by value, Utility& u);
	static void sortSolutionByFitness(vector<Solution> *soutions);
};

