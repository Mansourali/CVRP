#pragma once
#include "solver.h"

class TabuSearch : public SingleBasedSolver
{
protected:
	int routeA;
	int routeB;
	int tabutuner;
	vector<vector<int>> tabuList;
public:
	TabuSearch(SingleBasedSetting se, vector<Solution>& currentSolution);
	void createTabulist (int size);
	void tabuMove(int i, int j);
	void decrementTabu();
	bool aspirationCriteria(double improvmentScore);
	void run() override;
	bool steps() override;
};
