#include "Randomhillclimbing.h"
using namespace std;

//Random

RandomHillClimbing::RandomHillClimbing(algorithmCategory cat, Utility& u, Solution& currentSolution) : Solver(cat, u, currentSolution) {
	setSolverName("Hill Climbing");
	u.setSOLVER_NAME(*getSolverName());
	algoritmType = cat;
	CurrentSolution = &currentSolution;
	selectedStrategy = NULL;
	TerminationIteration = u.SINGLE.getTERMINATION_CRITERIA();
	mNeighbor = new Neighbor(u, cat);
};

void RandomHillClimbing::run(){
	best = *CurrentSolution;
	for (size_t i = 0; i < TerminationIteration; i++)
	while (steps());
	CurrentSolution = &best;
};

bool RandomHillClimbing::steps(){

	mNeighbor->setIntitialSolution(*CurrentSolution);

	SaveNeighborhood obtained = mNeighbor->makeOneRandomNeighborState();

	if (obtained.getNeighborSelector() != CONSTRAINT_VIOLATED) {
		switch (obtained.getNeighborStructure()){
		case SWAP:	 mNeighbor->swap(obtained.getARoute(), obtained.getBRoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
		case MOVE:	 mNeighbor->move(obtained.getARoute(), obtained.getBRoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
		case REVERSE:mNeighbor->reversed(obtained.getARoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
		}
		CurrentSolution->calculatSolutionFitness();
		if ( *CurrentSolution < best )
			best = *CurrentSolution;
		return false; //if true proceed with the same state
	}
	return false;
};

bool RandomHillClimbing::acceptanceCriteria(double ImprovmentValue){
	if (ImprovmentValue >= 0)
		return true;
	else
		return false;
}