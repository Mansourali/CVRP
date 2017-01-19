#include "GDHillClimbing.h"

// gradient descent hill climbing is try to chose the best among many neighborhood instead of one in hill climbing

GradientDescentHillClimbing::GradientDescentHillClimbing(algorithmCategory cat, Utility& u, Solution& currentSolution) : Solver(cat, u, currentSolution) {
	setSolverName("Gradient Descent Hill Climbing");
	u.setSOLVER_NAME(*getSolverName());
	algoritmType = cat;
	CurrentSolution = &currentSolution;
	selectedStrategy = NULL;
	mNeighbor = new Neighbor(u,cat);
};

void GradientDescentHillClimbing::run(){
	while (steps());
};

bool GradientDescentHillClimbing::steps(){

	mNeighbor->setIntitialSolution(*CurrentSolution);
	vector<SaveNeighborhood> obtained = generateNeighbor();
	SaveNeighborhood::sortNeighborByImproveValue(&obtained);

	SaveNeighborhood chosen = obtained.at( 0 );

	if (acceptanceCriteria(chosen.getImprovmentValue()) && chosen.getNeighborSelector() != CONSTRAINT_VIOLATED) {
		switch (chosen.getNeighborStructure()){
		case SWAP:	 mNeighbor->swap(chosen.getARoute(), chosen.getBRoute(), chosen.getICustomer(), chosen.getJCustomer()); break;
		case MOVE:	 mNeighbor->move(chosen.getARoute(), chosen.getBRoute(), chosen.getICustomer(), chosen.getJCustomer()); break;
		case REVERSE:mNeighbor->reversed(chosen.getARoute(), chosen.getICustomer(), chosen.getJCustomer()); break;
		}
		CurrentSolution->calculatSolutionFitness();
		return true; //if true proceed with the same state
	}
	return false;
}

bool GradientDescentHillClimbing::acceptanceCriteria(double improvementValue){
	if (improvementValue >= 0)
		return true;
	else
		return false;
}

vector<SaveNeighborhood> GradientDescentHillClimbing::generateNeighbor(){
	vector<SaveNeighborhood> obtained;
	for (size_t i = 0; i < 3; i++ )
		obtained.push_back(mNeighbor->makeOneRandomNeighborState());
	return obtained;
}