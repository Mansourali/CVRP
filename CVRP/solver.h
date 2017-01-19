#pragma once
#include "neighbor.h"
#include "utility.h"
#include "selection.h"

template <typename T>
class Solver {
public:
	Solver(){};
	Solver(algorithmCategory cat, Utility& u, T& currentSolution){};
	Solution Solver::getCurrentState() const { return *CurrentSolution; };
	void Solver::setCurrentState(Solution *solution) { CurrentSolution = solution; };
	virtual bool steps() = 0;
	virtual void run() = 0;
	virtual bool acceptanceCriteria(double ImprovmentValue) = 0;
	void Solver::setSolverName(string solverName) { this->solverName = solverName; };
	string* Solver::getSolverName()  { return &solverName; };
	void Solver::showResult() const { CurrentSolution->printSolution(); };
	Solution* CurrentSolution;
	//~Solver();

protected:
	int TerminationIteration;
	Neighbor* mNeighbor;
	string solverName;
	Selection* selectedStrategy;
	algorithmCategory algoritmType;
};