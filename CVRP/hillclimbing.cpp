#include "hillclimbing.h"
using namespace std;
						//steepest decent

HillClimbing::HillClimbing(algorithmCategory cat, Utility& u, Solution& currentSolution) : Solver(cat, u, currentSolution) {
	setSolverName("Hill Climbing");
	u.setSOLVER_NAME(*getSolverName());
	algoritmType = cat;
	CurrentSolution = &currentSolution;
	selectedStrategy = NULL;
	mNeighbor = new Neighbor(u, cat);
	TerminationIteration = u.SINGLE.getTERMINATION_CRITERIA();
};

void HillClimbing::run(){
	for (unsigned int i = 0; i < TerminationIteration; i ++ )
	while (steps ());
};

bool HillClimbing::steps(){

	mNeighbor->setIntitialSolution(*CurrentSolution);

	neighborStructureType randomNeighorhood = static_cast<neighborStructureType>(mNeighbor->neighborStructure.at( rand() % mNeighbor->neighborStructure.size()));

	switch (randomNeighorhood){
	case SHIFT_1_0: mNeighbor->Shift_1_0(); break;
	case SHIFT_2_0: mNeighbor->Shift_2_0(); break;
	case SWAP_1_1:	mNeighbor->Swap_1_1();	break;
	case SWAP_1_2:	mNeighbor->Swap_2_1();	break;
	case SWAP_2_2:	mNeighbor->Swap_2_2();	break;
	case CROSS:		mNeighbor->Cross();		break;
	case K_SHIFT:	mNeighbor->K_Shift();	break;
	case REINSERTION:mNeighbor->Reinsertion();break;
	case OR_OPT2:	mNeighbor->Or_opt2();	break;
	case OR_OPT3:	mNeighbor->Or_opt3();	break;
	case TWO_OPT:	mNeighbor->Two_opt();	break;
	case EXCAHNGE:	mNeighbor->Exchange();	break;
	default:
		break;
	}

	if (CurrentSolution->customerSizeIsViolated()) cout << "Customer size is valuated in Hill climbing";

	if (mNeighbor->obtainedNeighbor.size() == 0) return false; 
	
	SaveNeighborhood bestOrFirstNeighbor;

	if (mNeighbor->neighborSelector == FIRST){
		bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
	} else {
		SaveNeighborhood::sortNeighborByImproveValue(&mNeighbor->obtainedNeighbor);
		bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
	}

	if (acceptanceCriteria(bestOrFirstNeighbor.getImprovmentValue()) && bestOrFirstNeighbor.getNeighborSelector() != CONSTRAINT_VIOLATED) {
		if (bestOrFirstNeighbor.getARoute() == bestOrFirstNeighbor.getBRoute())	{
			CurrentSolution->routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
		}
		else{
			CurrentSolution->routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
			CurrentSolution->routes.at(bestOrFirstNeighbor.getBRoute()) = bestOrFirstNeighbor.getSecondRoute();
		}
		
		CurrentSolution->calculatSolutionFitness();
		cout << CurrentSolution->getSolutionFitness() << "  ** Using: " << Utility::itgetNeighborStructureAsName(static_cast<neighborStructureType>(bestOrFirstNeighbor.getNeighborStructure())) << endl;
		//return true; //if true proceed with the same state
	}
	return false;
};

bool HillClimbing::acceptanceCriteria(double ImprovmentValue){
	if (ImprovmentValue >= 0)
		return true;
	else
		return false;
}