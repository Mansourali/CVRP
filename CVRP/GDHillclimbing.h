#pragma once
#include "solver.h"

class GradientDescentHillClimbing : public Solver<Solution>
{
protected:
	vector<SaveNeighborhood> neighborStates;
	int routeA, routeB;
public:
	GradientDescentHillClimbing(algorithmCategory cat, Utility& u, Solution& currentSolution);
	void run() override;
	bool steps() override;
	bool acceptanceCriteria(double improvementValue) override;
	vector<SaveNeighborhood> generateNeighbor();
};