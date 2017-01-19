#include "simulatedannealing.h"

SimulatedAnnealing::SimulatedAnnealing(algorithmCategory cat, Utility& u, Solution& currentSolution) : Solver(cat, u, currentSolution) {
	currentTemperature = 10;//  u.SINGLE.getTERMINATION_CRITERIA();
	setSolverName("Simulated Annealing");
	u.setSOLVER_NAME(*getSolverName());
	algoritmType = cat;
	CurrentSolution = &currentSolution;
	globalBest = currentSolution;
	selectedStrategy = NULL;
	mNeighbor = new Neighbor(u, cat);
}

void SimulatedAnnealing::run (){
	//if (currentTemperature == 0) currentTemperature = 100;

	while (currentTemperature>0)
		steps();

	*CurrentSolution = this->globalBest;
}

void SimulatedAnnealing::decremnetTempreture (){
	currentTemperature = (currentTemperature < 1) ? currentTemperature = 0 : currentTemperature *= 0.998;
}

bool SimulatedAnnealing::steps (){

	decremnetTempreture ();
	//printf("Current temp = %f\n", currentTemperature);

	Solution  pertubedSolution = *CurrentSolution;

	double oldfitness = CurrentSolution->getSolutionFitness() , newfitness = CurrentSolution->getSolutionFitness();

	mNeighbor->setIntitialSolution(*CurrentSolution);

	mNeighbor->makeOneRandomNeighbor();

	if (mNeighbor->obtainedNeighbor.size() == 0){
		SaveNeighborhood negativeNeigbhour = mNeighbor->makeOneRandomNeighborState();
		if (negativeNeigbhour.getImprovmentValue() != -9999.00){
			switch (negativeNeigbhour.getNeighborStructure()){
			case SWAP:	 mNeighbor->swap(negativeNeigbhour.getARoute(), negativeNeigbhour.getBRoute(), negativeNeigbhour.getICustomer(), negativeNeigbhour.getJCustomer()); break;
			case MOVE:	 mNeighbor->move(negativeNeigbhour.getARoute(), negativeNeigbhour.getBRoute(), negativeNeigbhour.getICustomer(), negativeNeigbhour.getJCustomer()); break;
			}
			mNeighbor->SelectedSolution->calculatSolutionFitness();
			newfitness = mNeighbor->SelectedSolution->getSolutionFitness();
			//pertubedSolution = *mNeighbor->SelectedSolution;
		}
		else 
			return false;
	}
	else
	{
		SaveNeighborhood bestOrFirstNeighbor;

		if (mNeighbor->neighborSelector == FIRST){
			bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
		}
		else {
			SaveNeighborhood::sortNeighborByImproveValue(&mNeighbor->obtainedNeighbor);
			bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
		}

		if (bestOrFirstNeighbor.getARoute() == bestOrFirstNeighbor.getBRoute())	{
			pertubedSolution.routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
		}
		else{
			pertubedSolution.routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
			pertubedSolution.routes.at(bestOrFirstNeighbor.getBRoute()) = bestOrFirstNeighbor.getSecondRoute();
		}
		pertubedSolution.calculatSolutionFitness();
		if (this->globalBest.getSolutionFitness()>pertubedSolution.getSolutionFitness())
			this->globalBest = pertubedSolution;
		newfitness = pertubedSolution.getSolutionFitness();
	}

	

	bool imp = ( newfitness < oldfitness ) ? true : false;
	double no = Utility::getRandomUniformNo();
	double yy = exp(newfitness / currentTemperature);
	bool expo = ( no < yy ) ? true : false ;

	if (imp || expo ) { //original without *10
		if (imp == true)
		*CurrentSolution = pertubedSolution;
	}
		
	return false;
}

bool SimulatedAnnealing::acceptanceCriteria(double ImprovmentValue){
	if (ImprovmentValue >= 0)
		return true;
	else
		return false;
}