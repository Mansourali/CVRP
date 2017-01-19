#pragma once
#include "solver.h"
#include "selection.h"

using namespace setting::selectionStrategy;

class CuckooSearch_best : public Solver <vector<Solution>> {
protected:
	double fractionOfWorseNests;
	int solutionToBeDeletedOrCreated;
	int nests;
	selectionStrategy::by selectionMethod;
	TerminationCriteria* t;
	vector<neighborCount> neighborcount;
	int currentNeigbour;
public:
	CuckooSearch_best(algorithmCategory, Utility&, vector<Solution>&, TerminationCriteria&, double);
	void run()override;		// improveNewCuckoo
	bool steps()override;	// improveNewCuckoo
	void selectNestToLayACuckoo();
	void abandonWorseCuckoo();
	bool createNewCuckoo();
	bool acceptanceCriteria(double ImprovmentValue) override;
};