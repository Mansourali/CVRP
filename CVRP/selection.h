#pragma once
#include "utility.h"
#include "solution.h"
using namespace setting::initializationTechnique;

class Solution;
class Selection {
protected:
	double allSolutionFittness;
public:
	vector<Solution>* solutios;
	Selection();
	Selection(vector<Solution>& s);
	int getRandomSolution();
	int getBestInitialSolution();
	void calculateAllSolutionFittness();
	vector<double> absoulutefitnessCalcuation();
	vector<double> fitnessCalculation();
	int rouletteWheelSelection();
	void tournamentSelection();
	void rankSelection(int);
	void disruptiveSelection();
	int getSolutionByTournament();
	int getSolutionByRank(int);
	int getSolutionByDisruptive();
};

