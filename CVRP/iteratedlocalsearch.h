#pragma once
#include "solver.h"

class IteratedLocalSearch : public Solver<Solution>
{
protected:
	vector<SaveNeighborhood> neighborStates;
public:
	IteratedLocalSearch(algorithmCategory cat, Utility& u, Solution& currentSolution);
	void run() override;
	bool steps() override;
	bool acceptanceCriteria(double ImprovmentValue) override;
	bool applyNeighbor(SaveNeighborhood neighbor);
};