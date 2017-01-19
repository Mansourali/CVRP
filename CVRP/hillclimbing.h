#pragma once
#include "solver.h"

class HillClimbing : public Solver<Solution>
{
protected:
	vector<SaveNeighborhood> neighborStates;
public:
	HillClimbing(algorithmCategory cat, Utility& u, Solution& currentSolution);
	void run() override;
	bool steps() override;
	bool acceptanceCriteria(double ImprovmentValue) override;
};

