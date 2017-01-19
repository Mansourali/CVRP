#include "iteratedlocalsearch.h"
#include "hillclimbing.h"

IteratedLocalSearch::IteratedLocalSearch(algorithmCategory cat, Utility& u, Solution& currentSolution) : Solver(cat, u, currentSolution) {
	setSolverName("Iterated Local Search");
	u.setSOLVER_NAME(*getSolverName());
	algoritmType = cat;
	CurrentSolution = &currentSolution;
	selectedStrategy = NULL;
	mNeighbor = new Neighbor(u, cat);
};

void IteratedLocalSearch::run(){

	while (steps());
};

bool IteratedLocalSearch::steps(){

	mNeighbor->setIntitialSolution(*CurrentSolution);

	SaveNeighborhood obtained1 = mNeighbor->makeOneRandomNeighborState();
	applyNeighbor(obtained1);
	SaveNeighborhood obtained2 = mNeighbor->makeOneRandomNeighborState();
	applyNeighbor(obtained2);

	for (size_t i = 0; i < 100; i++) {

		SaveNeighborhood obtained = mNeighbor->makeOneRandomNeighborState();

		if (acceptanceCriteria(obtained.getImprovmentValue()) ) {
			applyNeighbor(obtained);
			//return true; //if true proceed with the same state
		}
	}
	return false;
};

bool IteratedLocalSearch::acceptanceCriteria(double ImprovmentValue){
	if (ImprovmentValue >= 0)
		return true;
	else
		return false;
}

bool IteratedLocalSearch::applyNeighbor(SaveNeighborhood neighbor){
	if (neighbor.getNeighborSelector() == CONSTRAINT_VIOLATED) return false;
	switch (neighbor.getNeighborStructure()){
	case SWAP:	 mNeighbor->swap(neighbor.getARoute(), neighbor.getBRoute(), neighbor.getICustomer(), neighbor.getJCustomer()); break;
	case MOVE:	 mNeighbor->move(neighbor.getARoute(), neighbor.getBRoute(), neighbor.getICustomer(), neighbor.getJCustomer()); break;
	case REVERSE:mNeighbor->reversed(neighbor.getARoute(), neighbor.getICustomer(), neighbor.getJCustomer()); break;
	}
	CurrentSolution->calculatSolutionFitness();
	return true;
}