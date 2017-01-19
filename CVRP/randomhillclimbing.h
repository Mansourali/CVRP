#pragma once
#include "solver.h"

class RandomHillClimbing : public Solver < Solution >
{
public:
	Solution best;
protected:
	vector<SaveNeighborhood> neighborStates;
public:
	RandomHillClimbing(algorithmCategory cat, Utility& u, Solution& currentSolution);
	void run() override;
	bool steps() override;
	bool acceptanceCriteria(double ImprovmentValue) override;
};

