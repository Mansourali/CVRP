#include "greatdeluge.h"

GreatDeluge::GreatDeluge(algorithmCategory cat, Utility& u, Solution& currentSolution) : Solver(cat, u, currentSolution) {
	numOfIter = u.SINGLE.getTERMINATION_CRITERIA ();
	setSolverName("Great Deluge");
	algoritmType = cat;
	u.setSOLVER_NAME(*getSolverName());
	selectedStrategy = NULL;
	mNeighbor = new Neighbor(u, cat);
	CurrentSolution = &currentSolution;
	level = CurrentSolution->getSolutionFitness();
}

void GreatDeluge::calculateForceDecayRate () {
	estimatedQuality = CurrentSolution->problem_instance->optimalValue;
	beta = estimatedQuality / numOfIter;
	//beta = exp(estimatedQuality) / numOfIter;
}

void GreatDeluge::decrementLevel(){
	level -= beta; //cout << level << endl;
}

void GreatDeluge::run () {
			while (steps ());
}

bool GreatDeluge::steps () {

	calculateForceDecayRate();

	mNeighbor->setIntitialSolution(*CurrentSolution);

	SaveNeighborhood obtained = mNeighbor->makeOneRandomNeighborState();

	double score = CurrentSolution->getSolutionFitness() - obtained.getImprovmentValue();

	if ( acceptanceCriteria(obtained.getImprovmentValue()) || score <= level) {
		switch (static_cast<neighborStructureType>(obtained.getNeighborStructure())){
		case SWAP: mNeighbor->swap(obtained.getARoute(),obtained.getBRoute(),obtained.getICustomer(),obtained.getJCustomer()); break;
		case MOVE: mNeighbor->move(obtained.getARoute(), obtained.getBRoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
		case REVERSE: mNeighbor->reversed(obtained.getARoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
		}
		CurrentSolution->calculatSolutionFitness();
		if (obtained.getImprovmentValue() != 0 ) 
			decrementLevel();
		//return true;
	}
	return false;
}

bool GreatDeluge::acceptanceCriteria(double ImprovmentValue) {
	if (ImprovmentValue >= 0)
		return true;
	else
		return false;
}