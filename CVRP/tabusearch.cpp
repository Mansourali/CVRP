#include "tabusearch.h"

TabuSearch::TabuSearch(SingleBasedSetting se, vector<Solution>& currentSolution) : SingleBasedSolver(se, currentSolution) {
	createTabulist(Neighbor::SelectedSolution->problem_instance->dimension);
	tabutuner = 20 ;
	setSolverName("Tabu Search");
	algoritmType = SINGLE_BASED;
}	

void TabuSearch::createTabulist(int size){
	vector<int> aRoute (size);
	for (int i = 0; i < size; i++)
		tabuList.push_back(aRoute);
}

void TabuSearch::tabuMove(int i, int j){
	tabuList[i][j]+= tabutuner;
	tabuList[j][i]+= tabutuner;
}

void TabuSearch::decrementTabu(){
	for (unsigned int i = 0; i < tabuList.size (); i ++)	{
		for (unsigned int j = 0; j < tabuList.size (); j ++)	{
			tabuList[i][j] -= tabuList[i][j] <= 0 ? 0 : 1 ;
		}
	}
}

bool TabuSearch::aspirationCriteria (double improvmentScore){
	//double bestCostSoFar = TerminationCriteria::getBest ().getSolutionFitness (),
	//	currentBest = solution->getSolutionFitness () - improvmentScore;
	//return ( currentBest < bestCostSoFar) ? true : false;
	return false;
}

void TabuSearch::run (){
	for (int a = 0; a<SelectedSolution->getRoutesSize(); a++){
		for (int b = 0; b<SelectedSolution->getRoutesSize(); b++){
			routeA = a;  routeB = b;
	while (steps()) ;
		}
	}
}

bool TabuSearch::steps (){
	int city1 = -1, city2 = -1;
	bool firstNeighbor = true;

	int aRouteSize = SelectedSolution->getRouteAt(routeA).getCustomerSize(),
		bRouteSize = SelectedSolution->getRouteAt(routeB).getCustomerSize();

		for (int i=0; i < aRouteSize; i++){

			for (int j=0; j < bRouteSize; j++){
				if (routeA == routeB && i == j ) continue; 

				if (SelectedSolution->getRouteAt(routeA).getCustomerSize() != aRouteSize || SelectedSolution->getRouteAt(routeB).getCustomerSize() != bRouteSize) {
					aRouteSize = SelectedSolution->getRouteAt(routeA).getCustomerSize();
					bRouteSize = SelectedSolution->getRouteAt(routeB).getCustomerSize();
				}
				if ( i >= aRouteSize || j >= bRouteSize )	
					continue;

				SaveNeighborhood se = makeOneNeighborState(routeA, routeB, i, j);
		 double improvedScore = se.getImprovmentValue() ;
		 if ( improvedScore == -9999 ) continue;
		 if ( improvedScore == 0) continue;

		 bool d = aspirationCriteria (improvedScore);

		 int aC = SelectedSolution->getRouteAt(routeA).getCustomerAt(i),
			 bC = SelectedSolution->getRouteAt(routeB).getCustomerAt(j);

			if ((improvedScore > 0 || firstNeighbor ) && tabuList[aC-1][bC-1] == 0) { //if better move found, store it
				firstNeighbor = false;
				switch (static_cast<neighborStructureType>(se.getNeighborStructure ())){
				case SWAP: swap(routeA,routeB,i,j); break;
				case MOVE: move(routeA,routeB,i,j); break;
				}
				SelectedSolution->calculatSolutionFitness();
				city1 = aC-1;
				city2 = bC-1;
				}  
			}
		}
		if (city1 != -1) {
			decrementTabu();
			tabuMove(city1, city2);
		}
	return false;
}
 