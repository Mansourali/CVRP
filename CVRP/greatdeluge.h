#pragma once
#include "solver.h"

class GreatDeluge : public Solver<Solution>
{
protected:
	double level;
	double beta;
	int numOfIter;
	int routeA;
	int routeB;
public:
	GreatDeluge(algorithmCategory cat, Utility& u, Solution& currentSolution);
	double estimatedQuality;
	void calculateForceDecayRate();
	void decrementLevel();
	void run()override;
	bool steps()override;
	bool acceptanceCriteria(double ImprovmentValue) override;

};

