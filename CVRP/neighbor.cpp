#include "neighbor.h"

Neighbor::Neighbor(Utility& u, algorithmCategory& cat) {
	if (cat == SINGLE_BASED) {
		neighborSelector  = u.SINGLE.getNEIGHBOR_SELECTOR();
		neighborStructure = u.SINGLE.getNEIGHBORHOOD_STRUCTURE();
		//neighborGenerator = u.SINGLE.getNEIGHBORHOOD_GENERATOR();
	}
	else if (cat == POPULATION_BASED) {
		neighborSelector  = u.POPULATION.getNEIGHBOR_SELECTOR();
		neighborStructure = u.POPULATION.getNEIGHBORHOOD_STRUCTURE();
		//neighborGenerator = u.POPULATION.getNEIGHBORHOOD_GENERATOR();
	}
}

void Neighbor::setIntitialSolution(Solution& solution){
	SelectedSolution = &solution;
}

void printCurrentImprovment(neighborStructureType type, neighborSelectorType t, double m, int i, int j, int a, int b, double fit) {
	switch (type)	{
	case SWAP: printf("swap (%i) maxImprove = %f, i = %i, j = %i Routes = %i,%i rFit = %f\n", t, m, i, j, a, b, fit); break;
	case MOVE: printf("move (%i) maxImprove = %f, i = %i, j = %i Routes = %i,%i rFit = %f\n", t, m, i, j, a, b, fit); break;
	}
}

SaveNeighborhood getPositiveNeighbor(vector<SaveNeighborhood> obtainedNeighbor){
	 for (vector<SaveNeighborhood>::iterator it = obtainedNeighbor.begin(); it != obtainedNeighbor.end(); it++){
		if (it->getNeighborSelector() == POSITIVE_NEIGHBOR || it->getNeighborSelector() == FIRST)
			return *it;
		}
}

bool Neighbor::generateSwapNeighborhood(int a, int b) {

	double maxImprove = 0;
	int iBestImprove = -1, jBestImprove = -1, aBestRoute = a, bBestRoute = b,
		iFirstImprove = -1, jFirstImprove = -1,	aFirstRoute = a, bFirstRoute = b,
		best = -1;

	obtainedNeighbor.clear();
	bool firstFOUND = false, processd = true ;
	
	for (int i = 0; i<SelectedSolution->getRouteAt(a).getCustomerSize() && processd; i++){
		for (int j = 0; j<SelectedSolution->getRouteAt(b).getCustomerSize() && processd; j++){
				if ( a == b && i == j ) continue;
				double improvedScore = scoreImprovementIfSwapped(a, b, i, j);
				if ( improvedScore > 0 && !firstFOUND ) {
					SaveNeighborhood sn = SaveNeighborhood (a,b,i,j,improvedScore,FIRST,SWAP);
					obtainedNeighbor.push_back (sn);
					firstFOUND = true; 
					iFirstImprove = i;	jFirstImprove = j;
					aFirstRoute = a;		bFirstRoute = b;
					if ( neighborSelector == FIRST ) 
						processd = false;
				}   else {
					if (improvedScore != constraintviolated) {
						SaveNeighborhood sn;
						if (improvedScore > 0 )
							sn = SaveNeighborhood (a,b,i,j,improvedScore,POSITIVE_NEIGHBOR,SWAP);
						else if (improvedScore < 0 )
							sn = SaveNeighborhood(a, b, i, j, improvedScore, NEGATIVE_NEIGHBBOR, SWAP);
						obtainedNeighbor.push_back (sn);
				}	}
				if ( maxImprove < improvedScore ){
					maxImprove = improvedScore;
					iBestImprove = i;	jBestImprove = j;
					aBestRoute = a;	 bBestRoute = b;
					best = obtainedNeighbor.size ()-1;
				}
			}
		}

		if ( best != -1 && neighborSelector == BEST )
			obtainedNeighbor.at (best).setNeighborSelectorType (BEST) ;
	
		if (iBestImprove < 0)
		return false;
	
		if (obtainedNeighbor.empty())  return false;

	switch (neighborSelector)	{
	case setting::FIRST:
		if (aFirstRoute == -1 || bFirstRoute == -1 || iFirstImprove == -1 || jFirstImprove == -1) return false;
		swap(aFirstRoute, bFirstRoute, iFirstImprove, jFirstImprove);
		//printCurrentImprovment(SWAP, FIRST, maxImprove, iFirstImprove, jFirstImprove, aFirstRoute, bFirstRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::BEST: default:
		if (aBestRoute == -1 || bBestRoute == -1 || iBestImprove == -1 || jBestImprove == -1) return false;
		swap(aBestRoute, bBestRoute, iBestImprove, jBestImprove);
		//printCurrentImprovment(SWAP, BEST, maxImprove, iBestImprove, jBestImprove, aBestRoute, bBestRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::POSITIVE_NEIGHBOR:
		SaveNeighborhood it = getPositiveNeighbor(obtainedNeighbor);
		if (it.getARoute() < 0) return false;
		swap(it.getARoute(), it.getBRoute(), it.getICustomer(), it.getJCustomer());
		//printCurrentImprovment(SWAP, POSITIVE_NEIGHBOR, it.getImprovmentValue(), it.getICustomer(), it.getJCustomer(), it.getARoute(), it.getBRoute(), SelectedSolution.getSolutionFitness());
		break;
	} 

	SelectedSolution->getSolutionFitness();
	return true;
}

bool Neighbor::generateSwapNeighborhood(){

	double maxImprove = 0;
	int iBestImprove = -1, jBestImprove = -1, aBestRoute = -1, bBestRoute = -1,
		iFirstImprove = -1, jFirstImprove = -1, aFirstRoute = -1, bFirstRoute = -1,
		best = -1;

	obtainedNeighbor.clear();
	for (size_t a = 0; a < SelectedSolution->getRoutesSize(); a++)
	{
		for (size_t b = 0; b < SelectedSolution->getRoutesSize(); b++)
		{
			bool firstFOUND = false, processd = true;

			for (int i = 0; i < SelectedSolution->getRouteAt(a).getCustomerSize() && processd; i++)
			{
				for (int j = 0; j < SelectedSolution->getRouteAt(b).getCustomerSize() && processd; j++)
				{
					if (a == b && i == j) continue;
					double improvedScore = scoreImprovementIfSwapped(a, b, i, j);
					if (improvedScore > 0 && !firstFOUND) {
						SaveNeighborhood sn = SaveNeighborhood(a, b, i, j, improvedScore, FIRST, SWAP);
						obtainedNeighbor.push_back(sn);
						firstFOUND = true;
						iFirstImprove = i;	jFirstImprove = j;
						aFirstRoute = a;		bFirstRoute = b;
						if (neighborSelector == FIRST)
							processd = false;
					}
					else {
						if (improvedScore != constraintviolated) {
						SaveNeighborhood sn;
						if (improvedScore > 0)
							sn = SaveNeighborhood(a, b, i, j, improvedScore, POSITIVE_NEIGHBOR, SWAP);
						else if (improvedScore < 0)
							sn = SaveNeighborhood(a, b, i, j, improvedScore, NEGATIVE_NEIGHBBOR, SWAP);
						obtainedNeighbor.push_back(sn);
						}	
					}
					if (maxImprove < improvedScore){
						maxImprove = improvedScore;
						iBestImprove = i;	jBestImprove = j;
						aBestRoute = a;	 bBestRoute = b;
						best = obtainedNeighbor.size() - 1;
					}
				}
			}

			if (best != -1 && neighborSelector == BEST)
				obtainedNeighbor.at(best).setNeighborSelectorType(BEST);

			if (maxImprove < 0)
				return false;
		}
	}

	if (obtainedNeighbor.empty())  return false;

	switch (neighborSelector)	{
	case setting::FIRST:
		if (aFirstRoute == -1 || bFirstRoute == -1 || iFirstImprove == -1 || jFirstImprove == -1) return false;
		swap(aFirstRoute, bFirstRoute, iFirstImprove, jFirstImprove);
		//printCurrentImprovment(SWAP, FIRST, maxImprove, iFirstImprove, jFirstImprove, aFirstRoute, bFirstRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::BEST: default:
		if (aBestRoute == -1 || bBestRoute == -1 || iBestImprove == -1 || jBestImprove == -1) return false;
		swap(aBestRoute, bBestRoute, iBestImprove, jBestImprove);
		//printCurrentImprovment(SWAP, BEST, maxImprove, iBestImprove, jBestImprove, aBestRoute, bBestRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::POSITIVE_NEIGHBOR:
		SaveNeighborhood it = getPositiveNeighbor(obtainedNeighbor);
		if (it.getARoute() < 0) return false;
		swap(it.getARoute(), it.getBRoute(), it.getICustomer(), it.getJCustomer());
		//printCurrentImprovment(SWAP, POSITIVE_NEIGHBOR, it.getImprovmentValue(), it.getICustomer(), it.getJCustomer(), it.getARoute(), it.getBRoute(), SelectedSolution.getSolutionFitness());
		break;
	}

	SelectedSolution->getSolutionFitness();
	return true;
}
 
double Neighbor::scoreImprovementIfSwapped(int a, int b, int i, int j) {

	Route aRoute = SelectedSolution->getRouteAt(a),
		bRoute = SelectedSolution->getRouteAt(b);
	double removedDist = aRoute.getRouteFitness () + bRoute.getRouteFitness () ;
	int aRouteC = aRoute.getCustomerAt (i),
		bRouteC = bRoute.getCustomerAt (j);
	if ( a == b ){
		bRoute.setCustomerAt (bRouteC,i);
		bRoute.setCustomerAt (aRouteC,j);}
	else {
		aRoute.setCustomerAt (bRouteC,i);
		bRoute.setCustomerAt (aRouteC,j);}

	double addedDist = aRoute.getRouteFitness () + bRoute.getRouteFitness ();
	double improvedD = removedDist - addedDist; 
	if ( aRoute.isCapacityConstraintViolated () ||  bRoute.isCapacityConstraintViolated () )
		return constraintviolated;
	return improvedD;
}

bool Neighbor::generateMoveNeighborhood(int a, int b){

	double maxImprove = 0;
	int iBestImprove = -1, jBestImprove = -1, aBestRoute = a, bBestRoute = b,
		iFirstImprove = -1, jFirstImprove = -1, aFirstRoute = a, bFirstRoute = b,
		best = -1;

	obtainedNeighbor.clear ();
	bool firstFOUND = false, processd = true ;

	for (int i = 0; i<SelectedSolution->getRouteAt(a).getCustomerSize() && processd; i++){
		for (int j = 0; j<SelectedSolution->getRouteAt(b).getCustomerSize() && processd; j++){
			if ( a == b && i == j ) continue;
			double improvedScore = scoreImprovementIfMoved(a, b, i, j);
			if ( improvedScore > 0 && !firstFOUND ) {
				SaveNeighborhood sn = SaveNeighborhood (a,b,i,j,improvedScore,FIRST,MOVE);
				obtainedNeighbor.push_back (sn);
				firstFOUND = true; 
				iFirstImprove = i;	jFirstImprove = j;
				aFirstRoute = a;		bFirstRoute = b;
				if ( neighborSelector == FIRST ) 
					processd = false;
			}   else {
				if (improvedScore != constraintviolated) {
					SaveNeighborhood sn;
					if (improvedScore > 0)
						sn = SaveNeighborhood(a, b, i, j, improvedScore, POSITIVE_NEIGHBOR, MOVE);
					else if (improvedScore < 0)
						sn = SaveNeighborhood(a, b, i, j, improvedScore, NEGATIVE_NEIGHBBOR, MOVE);
					obtainedNeighbor.push_back(sn);
				}	}
			if ( maxImprove < improvedScore ){
				maxImprove = improvedScore;
				iBestImprove = i;	jBestImprove = j;
				aBestRoute = a;	 bBestRoute = b;
				best = obtainedNeighbor.size ()-1;
			}
		}
	}

	if ( best != -1 && neighborSelector == BEST )
		obtainedNeighbor.at (best).setNeighborSelectorType (BEST) ;

	if (maxImprove < 0)
		return false;

	if (obtainedNeighbor.empty())  return false; 

	switch (neighborSelector)	{
	case setting::FIRST:
		if (aFirstRoute == -1 || bFirstRoute == -1 || iFirstImprove==-1 || jFirstImprove==-1) return false;
		move(aFirstRoute, bFirstRoute, iFirstImprove, jFirstImprove);
		//printCurrentImprovment(MOVE, FIRST, maxImprove, iFirstImprove, jFirstImprove, aFirstRoute, bFirstRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::BEST: default:
		if (aBestRoute == -1 || bBestRoute == -1 || iBestImprove == -1 || jBestImprove == -1) return false;
		move(aBestRoute, bBestRoute, iBestImprove, jBestImprove);
		//printCurrentImprovment(MOVE, BEST, maxImprove, iBestImprove, jBestImprove, aBestRoute, bBestRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::POSITIVE_NEIGHBOR:
		SaveNeighborhood it = getPositiveNeighbor(obtainedNeighbor);
		if (it.getARoute() < 0) return false;
		move(it.getARoute(), it.getBRoute(), it.getICustomer(), it.getJCustomer());
		//printCurrentImprovment(MOVE, POSITIVE_NEIGHBOR, it.getImprovmentValue(), it.getICustomer(), it.getJCustomer(), it.getARoute(), it.getBRoute(), SelectedSolution.getSolutionFitness());
		break;
	}

	SelectedSolution->getSolutionFitness();
	return true;
}

bool Neighbor::generateMoveNeighborhood(){

	double maxImprove = 0;
	int iBestImprove = -1, jBestImprove = -1, aBestRoute = -1, bBestRoute = -1,
		iFirstImprove = -1, jFirstImprove = -1, aFirstRoute = -1, bFirstRoute = -1,
		best = -1;

	obtainedNeighbor.clear();
	for (size_t a = 0; a < SelectedSolution->getRoutesSize(); a++)
	{
		for (size_t b = 0; b < SelectedSolution->getRoutesSize(); b++)
		{
			bool firstFOUND = false, processd = true;
			for (int i = 0; i<SelectedSolution->getRouteAt(a).getCustomerSize() && processd; i++){
				for (int j = 0; j<SelectedSolution->getRouteAt(b).getCustomerSize() && processd; j++){
			if ( a == b && i == j ) continue;
			double improvedScore = scoreImprovementIfMoved(a, b, i, j);
			if ( improvedScore > 0 && !firstFOUND ) {
				SaveNeighborhood sn = SaveNeighborhood (a,b,i,j,improvedScore,FIRST,MOVE);
				obtainedNeighbor.push_back (sn);
				firstFOUND = true; 
				iFirstImprove = i;	jFirstImprove = j;
				aFirstRoute = a;		bFirstRoute = b;
				if ( neighborSelector == FIRST ) 
					processd = false;
			}   else {
				if (improvedScore != constraintviolated) {
					SaveNeighborhood sn;
					if (improvedScore > 0)
						sn = SaveNeighborhood(a, b, i, j, improvedScore, POSITIVE_NEIGHBOR, MOVE);
					else if (improvedScore < 0)
						sn = SaveNeighborhood(a, b, i, j, improvedScore, NEGATIVE_NEIGHBBOR, MOVE);
					obtainedNeighbor.push_back(sn);
				}	}
			if ( maxImprove < improvedScore ){
				maxImprove = improvedScore;
				iBestImprove = i;	jBestImprove = j;
				aBestRoute = a;	 bBestRoute = b;
				best = obtainedNeighbor.size ()-1;
			}
		}
	}

	if ( best != -1 && neighborSelector == BEST )
		obtainedNeighbor.at (best).setNeighborSelectorType (BEST) ;

	if (maxImprove < 0)
		return false;
		}
	}

	if (obtainedNeighbor.empty())  return false;

	switch (neighborSelector)	{
	case setting::FIRST:
		if (aFirstRoute == -1 || bFirstRoute == -1 || iFirstImprove == -1 || jFirstImprove == -1) return false;
		move(aFirstRoute, bFirstRoute, iFirstImprove, jFirstImprove);
		//printCurrentImprovment(MOVE, FIRST, maxImprove, iFirstImprove, jFirstImprove, aFirstRoute, bFirstRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::BEST: default:
		if (aBestRoute == -1 || bBestRoute==-1 || iBestImprove==-1 || jBestImprove==-1) return false;
		move(aBestRoute, bBestRoute, iBestImprove, jBestImprove);
		//printCurrentImprovment(MOVE, BEST, maxImprove, iBestImprove, jBestImprove, aBestRoute, bBestRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::POSITIVE_NEIGHBOR:
		SaveNeighborhood it = getPositiveNeighbor(obtainedNeighbor);
		if (it.getARoute() < 0) return false;
		move(it.getARoute(), it.getBRoute(), it.getICustomer(), it.getJCustomer());
		//printCurrentImprovment(MOVE, POSITIVE_NEIGHBOR, it.getImprovmentValue(), it.getICustomer(), it.getJCustomer(), it.getARoute(), it.getBRoute(), SelectedSolution.getSolutionFitness());
		break;
	}

	return true;
}

bool Neighbor::Shift_1_0(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b; a = b = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop ; route01++, a++ ){
		for (auto route02 = SelectedSolution->routes.begin(); route02 != SelectedSolution->routes.end() && !stop; route02++, b++ ){
			if (route01 == route02 || route01->getSmallestCustomerDemand() + route02->getRouteDemand() > instance.capacity) continue;
			else{
				int i = -1;
				for (auto customer01 : route01->customers){
					i++;
					if (instance.customers.at(customer01).getDemand() + route02->routeDemand < instance.capacity){
						int j = -1;
						for (auto customer02 : route02->customers){
							j++;
							auto Oldroutesfitness = route01->getRouteFitness() + route02->getRouteFitness();
							Route route01backup = *route01, route02backup = *route02;
							route01backup.deleteCustomerAt(i);
							route02backup.addCustomerAt(customer01, j);
							route01backup.calculateRouteFitness(); 	route02backup.calculateRouteFitness();
							auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
							auto diff = Oldroutesfitness - Newroutesfitness;
							if ( diff > 0 )  {
								if (neighborSelector == FIRST) stop = true;
								int a = std::distance(SelectedSolution->routes.begin(), route01);
								int b = std::distance(SelectedSolution->routes.begin(), route02);
								obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, diff, POSITIVE_NEIGHBOR, SHIFT_1_0,route01backup, route02backup));
							}
						}
					}
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Shift_2_0(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b; a = b = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		for (auto route02 = SelectedSolution->routes.begin(); route02 != SelectedSolution->routes.end() && !stop; route02++, b++){
			if (route01 == route02 || route01->getSmallestCustomerDemand() + route02->getRouteDemand() > instance.capacity) continue;
			else{
				int i = -1;
				for (auto customer01 = route01->customers.begin()+1; customer01 != route01->customers.end(); customer01++){
					i++;
					auto customer02 = route01->customers.begin()+i;
					auto segmentDemand = instance.customers.at(*customer01).getDemand() + instance.customers.at(*customer02).getDemand();
					if ( segmentDemand + route02->routeDemand < instance.capacity){
						int j = -1;
						for (auto customer03 : route02->customers){
							j++;
							auto Oldroutesfitness = route01->getRouteFitness() + route02->getRouteFitness();
							Route route01backup = *route01, route02backup = *route02;
							route01backup.deleteCustomerAt(i+1);
							route01backup.deleteCustomerAt(i);
							route02backup.addCustomerAt(*customer01, j);
							route02backup.addCustomerAt(*customer02, j);
							route01backup.calculateRouteFitness();
							route02backup.calculateRouteFitness();
							auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
							auto diff = Oldroutesfitness - Newroutesfitness;
							if (diff > 0)  {
								if (neighborSelector == FIRST) stop = true;
								int a = std::distance(SelectedSolution->routes.begin(), route01);
								int b = std::distance(SelectedSolution->routes.begin(), route02);
								obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, diff, POSITIVE_NEIGHBOR, SHIFT_2_0,route01backup, route02backup));
							}
						}
					}
				}
			}
		}
	}

	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Swap_1_1(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		for (auto route02 = SelectedSolution->routes.begin()+a+1; route02 != SelectedSolution->routes.end() && !stop; route02++, b++){
				int i = 0;
				for (auto customer01 = route01->customers.begin(); customer01 != route01->customers.end(); customer01++, i++){
						int j = 0;
						for (auto customer02 = route02->customers.begin(); customer02 != route02->customers.end(); customer02++, j++){
							int customer02Demand = instance.customers.at(*customer02).getDemand();
							int customer01Demand = instance.customers.at(*customer01).getDemand();
							if (customer02Demand + route01->routeDemand - customer01Demand <= instance.capacity && 
								customer01Demand + route02->routeDemand - customer02Demand <= instance.capacity){
								auto Oldroutesfitness = route01->getRouteFitness() + route02->getRouteFitness();
								Route route01backup = *route01, route02backup = *route02;
								auto customer01backup = route02backup.customers.begin() + j;
								auto customer02backup = route01backup.customers.begin() + i;
								int temp = *customer01backup;
								*customer01backup = *customer02backup;
								*customer02backup = temp;
								route01backup.calculateRouteFitness(); route02backup.calculateRouteFitness();
								auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
								auto diff = Oldroutesfitness - Newroutesfitness;
								if (diff > 0)  {
									if (neighborSelector == FIRST) stop = true;
									int a = std::distance(SelectedSolution->routes.begin(), route01);
									int b = std::distance(SelectedSolution->routes.begin(), route02);
									obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, diff, POSITIVE_NEIGHBOR, SWAP_1_1, route01backup, route02backup));
						}
					}
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Swap_2_1(){ 
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		for (auto route02 = SelectedSolution->routes.begin(); route02 != SelectedSolution->routes.end() && !stop; route02++, b++){
			if (route01 == route02 ) continue;
			int i = 0;
			for (auto customer01 = route01->customers.begin()+1; customer01 != route01->customers.end(); customer01++, i++){
				int j = 0;
				auto customer02 = route01->customers.begin() + i;
				auto segmentDemand = instance.customers.at(*customer01).getDemand() + instance.customers.at(*customer02).getDemand();
				for (auto customer03 = route02->customers.begin(); customer03 != route02->customers.end(); customer03++, j++){
					if (instance.customers.at(*customer03).getDemand() + route01->routeDemand - segmentDemand <= instance.capacity &&
						segmentDemand + route02->routeDemand - instance.customers.at(*customer03).getDemand() <= instance.capacity){

						auto Oldroutesfitness = route01->getRouteFitness() + route02->getRouteFitness();
						Route route01backup = *route01, route02backup = *route02;
						auto customer01backup = route02backup.customers.begin() + j;
						auto customer02backup = route01backup.customers.begin() + i;
						int temp = *customer01backup;
						*customer01backup = *customer02backup;
						*customer02backup = temp;
						route01backup.deleteCustomerAt(i + 1);
						route02backup.addCustomerAt( *customer01, j+1);
						route01backup.calculateRouteFitness(); route02backup.calculateRouteFitness();
						auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
						auto diff = Oldroutesfitness - Newroutesfitness;
						if (diff > 0)  {
							if (neighborSelector == FIRST) stop = true;
							int a = std::distance(SelectedSolution->routes.begin(), route01);
							int b = std::distance(SelectedSolution->routes.begin(), route02);
							obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, diff, POSITIVE_NEIGHBOR, SWAP_1_2, route01backup, route02backup));
						}
					}
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Swap_2_2(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		for (auto route02 = SelectedSolution->routes.begin() + a + 1; route02 != SelectedSolution->routes.end() && !stop; route02++, b++){
			int i = 0;
			for (auto customer01 = route01->customers.begin() + 1; customer01 != route01->customers.end(); customer01++, i++){
				int j = 0;
				auto customer02 = route01->customers.begin() + i;
				auto segment01Demand = instance.customers.at(*customer01).getDemand() + instance.customers.at(*customer02).getDemand();
				for (auto customer03 = route02->customers.begin() + 1; customer03 != route02->customers.end(); customer03++, j++){
					auto customer04 = route02->customers.begin() + j;
					auto segment02Demand = instance.customers.at(*customer03).getDemand() + instance.customers.at(*customer04).getDemand();
					if (segment02Demand + route01->routeDemand - segment01Demand <= instance.capacity &&
						segment01Demand + route02->routeDemand - segment02Demand <= instance.capacity){

						auto Oldroutesfitness = route01->getRouteFitness() + route02->getRouteFitness();
						Route route01backup = *route01, route02backup = *route02;
						auto customer01backup = route02backup.customers.begin() + j;
						auto customer02backup = route01backup.customers.begin() + i;
						auto customer03backup = route02backup.customers.begin() + j+1;
						auto customer04backup = route01backup.customers.begin() + i+1;
						int temp = *customer01backup;
						*customer01backup = *customer02backup;
						*customer02backup = temp;

						temp = *customer03backup;
						*customer03backup = *customer04backup;
						*customer04backup = temp;

						route01backup.calculateRouteFitness(); route02backup.calculateRouteFitness();
						auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
						auto diff = Oldroutesfitness - Newroutesfitness;
						if (diff > 0)  {
							if (neighborSelector == FIRST) stop = true;
							int a = std::distance(SelectedSolution->routes.begin(), route01);
							int b = std::distance(SelectedSolution->routes.begin(), route02);
							obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, diff, POSITIVE_NEIGHBOR, SWAP_2_2, route01backup, route02backup));
						}
					}
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}
//TODO :: Error in cross
bool Neighbor::Cross(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		for (auto route02 = SelectedSolution->routes.begin() + a + 1; route02 != SelectedSolution->routes.end() && !stop; route02++, b++){
			int i = 0;
			for (auto customer01 = route01->customers.begin() + 1; customer01 != route01->customers.end(); customer01++, i++){

				auto customer02 = route01->customers.begin() + i;
				int j = route02->customers.size()-1;
				if (route02->customers.size() > 2){
					for (auto customer03 = route02->customers.end() - 2; customer03 != route02->customers.begin(); customer03--, j--){

						auto customer04 = route02->customers.begin() + j;
						if (instance.customers.at(*customer04).getDemand() + route01->routeDemand - instance.customers.at(*customer01).getDemand() <= instance.capacity &&
							instance.customers.at(*customer01).getDemand() + route02->routeDemand - instance.customers.at(*customer04).getDemand() <= instance.capacity){

							auto Oldroutesfitness = route01->getRouteFitness() + route02->getRouteFitness();
							Route route01backup = *route01, route02backup = *route02;
							auto customer01backup = route02backup.customers.begin() + j;
							auto customer02backup = route01backup.customers.begin() + i + 1;
							int temp = *customer01backup;
							*customer01backup = *customer02backup;
							*customer02backup = temp;

							route01backup.calculateRouteFitness(); route02backup.calculateRouteFitness();
							auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
							auto diff = Oldroutesfitness - Newroutesfitness;
							if (diff > 0)  {
								if (neighborSelector == FIRST) stop = true;
								int a = std::distance(SelectedSolution->routes.begin(), route01);
								int b = std::distance(SelectedSolution->routes.begin(), route02);
								obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, diff, POSITIVE_NEIGHBOR, CROSS, route01backup, route02backup));
							}
						}
					}
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::K_Shift(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		for (auto route02 = SelectedSolution->routes.begin(); route02 != SelectedSolution->routes.end() && !stop; route02++, b++){
			if (route01 == route02 || route02->getRouteDemand() > route01->getRouteDemand()) continue;
			for (unsigned int i = 1; i < route01->customers.size(); i++){
					Route segment; segment.problemInstsnce = &instance; 
					segment.customers.insert(segment.customers.begin(),route01->customers.begin()+i,route01->customers.end()); 
					segment.calculateRouteDemand();
					int increment = 0;
					Route route01backup = *route01, route02backup = *route02 ;
					if ( segment.getRouteDemand() + route02->getRouteDemand() < instance.capacity ){
						auto Oldroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
						std::copy(segment.customers.begin(), segment.customers.end(), std::back_inserter(route02backup.customers));
						route01backup.customers.erase(route01backup.customers.begin() + i, route01backup.customers.end());
						route01backup.calculateRouteFitness(); route02backup.calculateRouteFitness();
						auto Newroutesfitness = route01backup.getRouteFitness() + route02backup.getRouteFitness();
						auto diff = Oldroutesfitness - Newroutesfitness;
						if (diff > 0)  {
							if (neighborSelector == FIRST) stop = true;
							int a = std::distance(SelectedSolution->routes.begin(), route01);
							int b = std::distance(SelectedSolution->routes.begin(), route02);
							obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, route02backup.customers.size(), diff, POSITIVE_NEIGHBOR, K_SHIFT, route01backup, route02backup));
						}
					}
				}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

//TODO :: Error in cross
bool Neighbor::Reinsertion(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		if (route01->customers.size() < 2) continue;
		for (unsigned int i = 0; i < route01->customers.size() && !stop; i++){
			for (unsigned int j = 0; j < route01->customers.size() && !stop; j++){
				Route route01New = *route01;
				auto Oldroutesfitness = route01->getRouteFitness();

				int customer = route01New.customers.at(i);
				route01New.deleteCustomerAt(i);
				route01New.addCustomerAt(customer, j);
				route01New.calculateRouteFitness();
				auto Newroutesfitness = route01New.getRouteFitness();
				auto diff = Oldroutesfitness - Newroutesfitness;
				if (diff > 0)  {
					if (neighborSelector == FIRST) stop = true;
					int a = std::distance(SelectedSolution->routes.begin(), route01);
					obtainedNeighbor.push_back(SaveNeighborhood(a, a, i, j, diff, POSITIVE_NEIGHBOR, REINSERTION, route01New));
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Or_opt2(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		if ( route01->customers.size() < 3 ) continue;
		for (unsigned int i = 0; i < route01->customers.size() - 1 && !stop; i++){
			for (unsigned int j = 0; j < route01->customers.size() - 1 && !stop; j++){
				Route route01New = *route01;
				auto Oldroutesfitness = route01->getRouteFitness();
				int customer01 = route01New.customers.at(i);
				int customer02 = route01New.customers.at(i+1);
				route01New.deleteCustomerAt(i+1);
				route01New.deleteCustomerAt(i);
				route01New.addCustomerAt(customer02, j);
				route01New.addCustomerAt(customer01, j);
				route01New.calculateRouteFitness();
				auto Newroutesfitness = route01New.getRouteFitness();
				auto diff = Oldroutesfitness - Newroutesfitness;
				if (diff > 0)  {
					if (neighborSelector == FIRST) stop = true;
					int a = std::distance(SelectedSolution->routes.begin(), route01);
					obtainedNeighbor.push_back(SaveNeighborhood(a, a, i, j, diff, POSITIVE_NEIGHBOR, OR_OPT2, route01New));
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Or_opt3(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		if ( route01->customers.size() < 4 ) continue;
		for (unsigned int i = 0; i < route01->customers.size() - 2 && !stop; i++){
			for (unsigned int j = 0; j < route01->customers.size() - 2 && !stop; j++){
				Route route01New = *route01;
				auto Oldroutesfitness = route01->getRouteFitness();
				int customer01 = route01New.customers.at(i);
				int customer02 = route01New.customers.at(i + 1);
				int customer03 = route01New.customers.at(i + 2);
				route01New.deleteCustomerAt(i + 2);
				route01New.deleteCustomerAt(i + 1);
				route01New.deleteCustomerAt(i);
				route01New.addCustomerAt(customer03, j);
				route01New.addCustomerAt(customer02, j);
				route01New.addCustomerAt(customer01, j);
				route01New.calculateRouteFitness();
				auto Newroutesfitness = route01New.getRouteFitness();
				auto diff = Oldroutesfitness - Newroutesfitness;
				if (diff > 0)  {
					if (neighborSelector == FIRST) stop = true;
					int a = std::distance(SelectedSolution->routes.begin(), route01);
					obtainedNeighbor.push_back(SaveNeighborhood(a, a, i, j, diff, POSITIVE_NEIGHBOR, OR_OPT3, route01New));
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Two_opt(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j; a = b = i = j = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		if ( route01->customers.size() < 3 ) continue;
		for (unsigned int i = 0; i < route01->customers.size() - 1 && !stop; i++){
			for (unsigned int j = i + 1; j <= route01->customers.size() && !stop; j++){
				Route route01New = *route01;
				auto Oldroutesfitness = route01->getRouteFitness();
				std::reverse(route01New.customers.begin()+i,route01New.customers.begin()+j);
				route01New.calculateRouteFitness();
				auto Newroutesfitness = route01New.getRouteFitness();
				auto diff = Oldroutesfitness - Newroutesfitness;
				if (diff > 0)  {
					if (neighborSelector == FIRST) stop = true;
					int a = std::distance(SelectedSolution->routes.begin(), route01);
					obtainedNeighbor.push_back(SaveNeighborhood(a, a, i, j, diff, POSITIVE_NEIGHBOR, TWO_OPT, route01New));
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

bool Neighbor::Exchange(){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, ii, jj; a = b = 0;
	this->obtainedNeighbor.clear();
	bool stop = false;

	for (auto route01 = SelectedSolution->routes.begin(); route01 != SelectedSolution->routes.end() && !stop; route01++, a++){
		if (route01->customers.size() < 3) continue;
		ii = 0;
		for (auto i = route01->customers.begin() + ii; i != route01->customers.end() - 1 && !stop; i++, ii++){
			jj = 0;  jj = ii;
			for (auto j = route01->customers.begin() + 1; j != route01->customers.end() - ii && !stop; j++, jj++){
				Route route01New = *route01;
				auto Oldroutesfitness = route01->getRouteFitness();
				auto customer01 = route01New.customers.begin() + ii;
				auto customer02 = route01New.customers.begin() + jj+1;
				auto tmp = *customer01;
				*customer01 = *customer02;
				*customer02 = tmp;
				route01New.calculateRouteFitness();
				auto Newroutesfitness = route01New.getRouteFitness();
				auto diff = Oldroutesfitness - Newroutesfitness;
				if (diff > 0)  {
					if (neighborSelector == FIRST) stop = true;
					int a = std::distance(SelectedSolution->routes.begin(), route01);
					obtainedNeighbor.push_back(SaveNeighborhood(a, a, ii, jj, diff, POSITIVE_NEIGHBOR, EXCAHNGE, route01New));
				}
			}
		}
	}
	return (obtainedNeighbor.size() > 0) ? true : false;
}

Solution* Neighbor::Rinsertion_perturbation(int repeat){
	Instance instance(*SelectedSolution->problem_instance);
	int a, i, j, two, count; a = i = j = count = two = 0;
	this->obtainedNeighbor.clear();
	bool finish = false;
	Solution newSolution = *SelectedSolution;

	while (!finish && count < 20){
		count = 0;	two++;
		a = rand() % SelectedSolution->routes.size();

			do {
				i = rand() % SelectedSolution->routes.at(a).customers.size();
				j = rand() % SelectedSolution->routes.at(a).customers.size();
				count++;
			} while (i == j && count < 20);
	
		if (count != 20){
			auto customer01 = SelectedSolution->routes.at(a).customers.at(i);
			SelectedSolution->routes.at(a).deleteCustomerAt(i);
			SelectedSolution->routes.at(a).addCustomerAt(customer01, j);
			SelectedSolution->calculatSolutionFitness();
			obtainedNeighbor.push_back(SaveNeighborhood(a, a, i, j, 0.0, setting::neighborSelectorType::RANDOM, MULTIPLE_REINSERTION_1, SelectedSolution->routes.at(a), SelectedSolution->routes.at(a)));
		}
		if (two == repeat) finish = true;
	}
	return &newSolution;
}

Solution* Neighbor::Shift_perturbation(int repeat){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j , two , count ; a = b = i = j = count = two = 0;
	this->obtainedNeighbor.clear();
	bool finish = false;
	Solution newSolution = *SelectedSolution;

	while (!finish && count < 20){
		count = 0;	two++;
		a = rand() % SelectedSolution->routes.size();
		b = rand() % SelectedSolution->routes.size();
		if ( a == b ){
			do {
				i = rand() % SelectedSolution->routes.at(a).customers.size();
				j = rand() % SelectedSolution->routes.at(b).customers.size();
			} while (i == j);
		}
		else {
			int custumerDemand, routeDemand;
			do {
				count++;
				i = rand() % SelectedSolution->routes.at(a).customers.size();
				j = rand() % SelectedSolution->routes.at(b).customers.size();
				custumerDemand = instance.customers.at(SelectedSolution->routes.at(a).customers.at(i)).getDemand();
				routeDemand = newSolution.routes.at(b).routeDemand;
			} while ( custumerDemand + routeDemand > instance.capacity && count < 20 );
		}
		if (count != 20){
			auto customer01 = SelectedSolution->routes.at(a).customers.at(i);
			SelectedSolution->routes.at(a).deleteCustomerAt(i);
			SelectedSolution->routes.at(b).addCustomerAt(customer01, j);
			SelectedSolution->calculatSolutionFitness();
			obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, 0.0, setting::neighborSelectorType::RANDOM, MULTIPLE_SHIFT_1_1, SelectedSolution->routes.at(a), SelectedSolution->routes.at(b)));
		}
		if ( two == repeat ) finish = true;
	}
	return &newSolution;
}

Solution* Neighbor::Swap_perturbation(int repeat){
	Instance instance(*SelectedSolution->problem_instance);
	int a, b, i, j, two, count; a = b = i = j = count = two = 0;
	this->obtainedNeighbor.clear();
	bool finish = false;
	vector<int>::iterator customer01, customer02;
	Solution newsolution = *SelectedSolution;

	while (!finish && count < 20){
		count = 0;	two++;
		vector<Route>::iterator route01, route02;
		a = rand() % newsolution.routes.size(); route01 = newsolution.routes.begin() + a;
		b = rand() % newsolution.routes.size(); route02 = newsolution.routes.begin() + b;
		if ( route01 == route02 ){
			do {
				customer01 = route01->customers.begin() + rand() % route01->customers.size();
				customer02 = route01->customers.begin() + rand() % route01->customers.size();
			} while ( *customer01 == *customer02 );
		}
		else {
			int custumer01Demand, route01Demand, custumer02Demand, route02Demand , oo, rr;
			do {
				count++;
				i = rand() % route01->customers.size(); customer01 = route01->customers.begin() + i;
				j = rand() % route02->customers.size(); customer02 = route02->customers.begin() + j;
				custumer01Demand = instance.customers.at(*customer01).getDemand();
				custumer02Demand = instance.customers.at(*customer02).getDemand();
				oo = custumer01Demand + route02->getRouteDemand() - custumer02Demand, rr = custumer02Demand + route01->getRouteDemand() - custumer01Demand;
			} while ((oo > instance.capacity || rr > instance.capacity) && count < 20);
		}
		if (count != 20){
			auto temp = *customer01;
			*customer01 = *customer02;
			*customer02 = temp;
			//obtainedNeighbor.push_back(SaveNeighborhood(a, b, i, j, 0.0, setting::neighborSelectorType::RANDOM, MULTIPLE_SWAP_1_1, *route01, *route02));
			newsolution.routes.at(a).calculateRouteFitness();
			newsolution.routes.at(b).calculateRouteFitness();
			newsolution.calculatSolutionFitness();
		}
		if (two == repeat) finish = true;
	}
	return &newsolution;
}

void Neighbor::Double_bridge(){

}

double Neighbor::scoreImprovementIfMoved(int a, int b, int i, int j) {

	Route aRoute = SelectedSolution->getRouteAt(a),
		bRoute = SelectedSolution->getRouteAt(b);
	double removedDist = aRoute.getRouteFitness () + bRoute.getRouteFitness () ;
	int removedC = aRoute.getCustomerAt (i);
	if ( a == b )
		bRoute.deleteCustomerAt (i); 
		else aRoute.deleteCustomerAt (i);
	bRoute.addCustomerAt (removedC,j);
	double addedDist = aRoute.getRouteFitness () + bRoute.getRouteFitness ();
	double improvedD = removedDist - addedDist; 
	if ( aRoute.isCapacityConstraintViolated () ||  bRoute.isCapacityConstraintViolated () )
		return constraintviolated;
	return improvedD;
}

bool Neighbor::generateReverseNeighborhood(int a){
	double maxImprove = 0;
	int iBestImprove = -1, jBestImprove = -1, aBestRoute = a, bBestRoute = a,
		iFirstImprove = -1, jFirstImprove = -1, aFirstRoute = a, bFirstRoute = a,
		best = -1;

	obtainedNeighbor.clear();
	bool firstFOUND = false, processd = true;

	for (int i = 0; i < SelectedSolution->getRouteAt(a).getCustomerSize() && processd; i++){
		for (int j = 0; j<SelectedSolution->getRouteAt(a).getCustomerSize() && processd; j++){
			if (i == j || i > j || j - i == 1) continue;
			double improvedScore = scoreImprovementIfReversed(a, i, j);
			if (improvedScore > 0 && !firstFOUND) {
				SaveNeighborhood sn = SaveNeighborhood(a, a, i, j, improvedScore, FIRST, REVERSE);
				obtainedNeighbor.push_back(sn);
				firstFOUND = true;
				iFirstImprove = i;	jFirstImprove = j;
				aFirstRoute = a;		bFirstRoute = a;
				if (neighborSelector == FIRST)
					processd = false;
			}
			else {
				if (improvedScore != constraintviolated) {
					SaveNeighborhood sn;
					if (improvedScore > 0)
						sn = SaveNeighborhood(a, a, i, j, improvedScore, POSITIVE_NEIGHBOR, REVERSE);
					else if (improvedScore < 0)
						sn = SaveNeighborhood(a, a, i, j, improvedScore, NEGATIVE_NEIGHBBOR, REVERSE);
					obtainedNeighbor.push_back(sn);
				}
			}
			if (maxImprove < improvedScore){
				maxImprove = improvedScore;
				iBestImprove = i;	jBestImprove = j;
				aBestRoute = a;	 bBestRoute = a;
				best = obtainedNeighbor.size() - 1;
			}
		}
	}

	if (best != -1 && neighborSelector == BEST)
		obtainedNeighbor.at(best).setNeighborSelectorType(BEST);

	if (maxImprove < 0)
		return false;

	if (obtainedNeighbor.empty())  return false;

	switch (neighborSelector)	{
	case setting::FIRST:
		if (aFirstRoute == -1 || bFirstRoute == -1 || iFirstImprove == -1 || jFirstImprove == -1) return false;
		reversed(aFirstRoute, iFirstImprove, jFirstImprove);
		//printCurrentImprovment(MOVE, FIRST, maxImprove, iFirstImprove, jFirstImprove, aFirstRoute, bFirstRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::BEST: default:
		if (aBestRoute == -1 || bBestRoute == -1 || iBestImprove == -1 || jBestImprove == -1) return false;
		reversed(aBestRoute, iBestImprove, jBestImprove);
		//printCurrentImprovment(MOVE, BEST, maxImprove, iBestImprove, jBestImprove, aBestRoute, bBestRoute, SelectedSolution.getSolutionFitness());
		break;
	case setting::POSITIVE_NEIGHBOR:
		SaveNeighborhood it = getPositiveNeighbor(obtainedNeighbor);
		if (it.getARoute() < 0) return false;
		reversed(it.getARoute(), it.getICustomer(), it.getJCustomer());
		//printCurrentImprovment(MOVE, POSITIVE_NEIGHBOR, it.getImprovmentValue(), it.getICustomer(), it.getJCustomer(), it.getARoute(), it.getBRoute(), SelectedSolution.getSolutionFitness());
		break;
	}

	SelectedSolution->getSolutionFitness();
	return true;
}

bool Neighbor::generateReverseNeighborhood(){
	return false;
}

double Neighbor::scoreImprovementIfReversed(int a, int i, int j) {

	Route aRoute = SelectedSolution->getRouteAt(a);

	double removedDist = aRoute.getRouteFitness() ;

	if ( i < j && j<aRoute.customers.size())	{
		reverse(aRoute.customers.begin() + i, aRoute.customers.begin() + j);
	}

	aRoute.calculateRouteFitness();
	double addedDist = aRoute.getRouteFitness() ;

	double improvedD = removedDist - addedDist;

	return improvedD;
}

void Neighbor::swap(int a, int b, int i, int j) {
	int c = SelectedSolution->routes[a].getCustomerAt(i);
	SelectedSolution->routes[a].customers[i] = SelectedSolution->routes[b].getCustomerAt(j);
	SelectedSolution->routes[b].customers[j] = c;

	SelectedSolution->routes[a].isRouteFitnessCalculated = false;
	SelectedSolution->routes[b].isRouteFitnessCalculated = false;
	SelectedSolution->setFitnessNotCalculated();
}

void Neighbor::move(int a, int b, int i, int j) throw(VRPException){
	if (i >= SelectedSolution->routes[a].customers.size())
		throw VRPException("Can not do move operator, out of range costumer ");
	int c = SelectedSolution->routes[a].customers[i];
	SelectedSolution->routes[a].deleteCustomerAt(i);
	SelectedSolution->routes[b].addCustomerAt(c, j);
	SelectedSolution->setFitnessNotCalculated();
}

void Neighbor::reversed(int a, int i, int j){
	if (i < j && j < SelectedSolution->routes[a].customers.size())
	reverse(SelectedSolution->routes[a].customers.begin() + i, SelectedSolution->routes[a].customers.begin() + j);
	SelectedSolution->setFitnessNotCalculated();
}

vector<SaveNeighborhood> Neighbor::makeNeighborState(){

	int rnd = rand () % neighborStructure.size (); 
	switch ( neighborStructure[rnd] ){
	case SWAP: generateSwapNeighborhood (); break; 
	case MOVE:  generateMoveNeighborhood (); break; 
	case REVERSE: generateReverseNeighborhood (); break; 
	}
	return obtainedNeighbor;
}

vector<SaveNeighborhood> Neighbor::makeNeighborState(neighborStructureType nS){

	switch (nS){
	case SWAP: generateSwapNeighborhood(); break;
	case MOVE:  generateMoveNeighborhood(); break;
	case REVERSE: generateReverseNeighborhood(); break;
	}
	return obtainedNeighbor;
}

SaveNeighborhood Neighbor::makeOneNeighborState(int routeA, int routeB, int i, int j){

	double improvedScore = constraintviolated;
	neighborStructureType nS;
	neighborSelectorType nSE;

	int rnd = rand() % neighborStructure.size();
	switch (neighborStructure[rnd]){
	case SWAP: improvedScore = scoreImprovementIfSwapped(routeA, routeB, i, j); nS = SWAP;
		if (improvedScore > 0) nSE = POSITIVE_NEIGHBOR; 
		else if (improvedScore < 0 && improvedScore != constraintviolated) nSE = NEGATIVE_NEIGHBBOR;
		break;
	case MOVE: improvedScore = scoreImprovementIfMoved(routeA, routeB, i, j); nS = MOVE; 
		if (improvedScore > 0) nSE = POSITIVE_NEIGHBOR;
		else if (improvedScore < 0 && improvedScore != constraintviolated) nSE = NEGATIVE_NEIGHBBOR;
		break;
	}
#pragma warning(suppress: 6001)
	return SaveNeighborhood(routeA, routeB, i, j, improvedScore, nSE, nS);
}

SaveNeighborhood Neighbor::makeOneRandomNeighborState() throw (VRPException){
	double improvedScore = constraintviolated;
	neighborStructureType nS;
	neighborSelectorType nSE;

	if (SelectedSolution->routes.size() == 0)
		cout << "failed" <<endl;

	int a = SelectedSolution->getRandomRouteIndex(),
		b = SelectedSolution->getRandomRouteIndex();
	if (SelectedSolution->routes[a].customers.size() == 0 || 
		SelectedSolution->routes[b].customers.size() == 0)
		throw VRPException("empty route size");

	int	i = rand() % SelectedSolution->routes[a].customers.size(),
		j = rand() % SelectedSolution->routes[b].customers.size();

	int rnd = rand() % 2;
	switch (rnd){
	case 0:	 improvedScore = scoreImprovementIfSwapped (a, b, i, j); nS = SWAP; break;
	case 1:	 improvedScore = scoreImprovementIfMoved   (a, b, i, j); nS = MOVE; break;
	//case 2:  improvedScore = scoreImprovementIfReversed(a, i, j);	 nS = REVERSE; break;
	}
	if (improvedScore >= 0) nSE = POSITIVE_NEIGHBOR;
	if (improvedScore < 0 && improvedScore != constraintviolated) nSE = NEGATIVE_NEIGHBBOR;
	if (improvedScore == constraintviolated) nSE = CONSTRAINT_VIOLATED;
	return SaveNeighborhood(a, b, i, j, improvedScore, nSE, nS);
}

void Neighbor::makeOneRandomNeighbor() throw (VRPException){

	if (SelectedSolution->routes.size() == 0)
		cout << "failed" << endl;

	int rnd = rand() % neighborStructure.size();
	switch (neighborStructure[rnd]){
	case SHIFT_1_0: Shift_1_0(); break;
	case SHIFT_2_0: Shift_2_0(); break;
	case SWAP_1_1:	Swap_1_1();	break;
	case SWAP_1_2:	Swap_2_1();	break;
	case SWAP_2_2:	Swap_2_2();	break;
	case CROSS:		Cross();		break;
	case K_SHIFT:	K_Shift();	break;
	case REINSERTION:Reinsertion(); break;
	case OR_OPT2:	Or_opt2();	break;
	case OR_OPT3:	Or_opt3();	break;
	case TWO_OPT:	Two_opt();	break;
	case EXCAHNGE:	Exchange();	break;
	default:
		break;
	}

}

SaveNeighborhood::SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, neighborStructureType m){
	this->a = a;	this->b = b;
	this->i = i;	this->j = j;
	this->improvmentValue = imp;
	this->neighborSelector = n;
	this->neighborStructure = m;
};

SaveNeighborhood::SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, PerturbationMechanisms m, Route r1, Route r2){
	this->a = a;	this->b = b;
	this->i = i;	this->j = j;
	this->improvmentValue = imp;
	this->neighborSelector = n;
	this->neighborStructure = m;
	this->firstRoute = r1;
	this->secondRoute = r2;
};

SaveNeighborhood::SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, neighborStructureType m, Route r1) :
SaveNeighborhood(a, b, i, j, imp, n, m){
	this->firstRoute = r1;
	this->secondRoute = r1;
};

SaveNeighborhood::SaveNeighborhood(int a, int b, int i, int j, double imp, neighborSelectorType n, neighborStructureType m, Route r1, Route r2):
SaveNeighborhood(a, b, i, j, imp, n, m){
	this->firstRoute = r1;
	this->secondRoute = r2;
};

void SaveNeighborhood::setNeighborSelectorType(neighborSelectorType type){ neighborSelector = type; }

double SaveNeighborhood::getImprovmentValue(){ return improvmentValue; }

int SaveNeighborhood::getNeighborSelector(){ return neighborSelector; }

int SaveNeighborhood::getARoute(){ return a; }

int SaveNeighborhood::getBRoute(){ return b; }

int SaveNeighborhood::getICustomer(){ return i; }

int SaveNeighborhood::getJCustomer(){ return j; }

int SaveNeighborhood::getNeighborStructure(){ return neighborStructure; }

Route SaveNeighborhood::getFirstRoute() { return firstRoute; }

Route SaveNeighborhood::getSecondRoute() { return secondRoute; }

