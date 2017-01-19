#pragma once
#include "solver.h"

class SimulatedAnnealing : public Solver<Solution>
{
protected:
	double currentTemperature;
	Solution globalBest;
public:
	SimulatedAnnealing(algorithmCategory cat, Utility& u, Solution& currentSolution);
	void decremnetTempreture();
	void run() override;
	bool steps() override;
	bool acceptanceCriteria(double ImprovmentValue) override;
};

