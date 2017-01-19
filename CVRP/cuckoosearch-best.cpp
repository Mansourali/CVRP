#include "cuckoosearch_best.h"

CuckooSearch_best::CuckooSearch_best(algorithmCategory cat, Utility& u, vector<Solution>& currentSolution, TerminationCriteria& t, double pa) : Solver(cat, u, currentSolution) {
	selectedStrategy = new Selection(currentSolution);
	setSolverName("Cuckoo Search");
	u.setSOLVER_NAME(*getSolverName());
	algoritmType = cat;
	fractionOfWorseNests = pa;
	solutionToBeDeletedOrCreated = static_cast<int>(selectedStrategy->solutios->size() * fractionOfWorseNests);
	selectionMethod = u.POPULATION.getSELECTION_STRATEGY();
	mNeighbor = new Neighbor(u, cat);
	this->t = &t;
	neighborcount.resize(mNeighbor->neighborStructure.size());
	for (unsigned i = 0; i < mNeighbor->neighborStructure.size(); i++){
		neighborcount.at(i).type_id = mNeighbor->neighborStructure.at(i);
	}
	currentNeigbour = 0;
}

void CuckooSearch_best::selectNestToLayACuckoo() {
	int selectedSolutionIndex = 0;
	switch (selectionMethod)	{
	case selectionStrategy::RANDOM:		selectedSolutionIndex = selectedStrategy->getRandomSolution(); break;
	case selectionStrategy::RANK:		selectedSolutionIndex = selectedStrategy->getSolutionByRank(t->endIteration); break;
	case selectionStrategy::TOURNMENT:	selectedSolutionIndex = selectedStrategy->getSolutionByTournament(); break;
	case selectionStrategy::DISRUPTIVE:	selectedSolutionIndex = selectedStrategy->getSolutionByDisruptive(); break;
	}
	CurrentSolution = &selectedStrategy->solutios->at(selectedSolutionIndex);
}

void CuckooSearch_best::run(){
	//cout << " original fit, perturbation fit, perturbation improve fit, using perturbation, best fit so far " << endl;
	while (!t->terminationCriteriaMet()) {
		while (steps());
	}
	//int n_count, i_count; n_count = i_count = 0;
	//for (unsigned i = 0; i < neighborcount.size(); i++){
	//	cout << setfill(' ') << setw(3) << left << "N: " << setw(11) << left << Utility::itgetNeighborStructureAsName(neighborcount.at(i).type_id)
	//		<< " Count: " << setw(4) << left << neighborcount.at(i).ne_count
	//		 << " Improved: " << neighborcount.at(i).ne_count_succ << endl;
	//	n_count += neighborcount.at(i).ne_count;
	//	i_count += neighborcount.at(i).ne_count_succ;
	//}
	//cout << " Neighbor all count: " << n_count << " Neighbor improvement count: " << i_count << endl;
}

bool CuckooSearch_best::steps(){ // improveNewCuckoo

	selectNestToLayACuckoo();

	Solution pertubedSolution = *CurrentSolution;
	int oldFitness = CurrentSolution->getSolutionFitness();

	mNeighbor->setIntitialSolution(pertubedSolution);

	int levyFlightNo = Utility::getRandomLevyNoBetween(1, 5);

	switch (levyFlightNo){
	case 1: mNeighbor->Rinsertion_perturbation(1);  break; 
	case 2: mNeighbor->Rinsertion_perturbation(2);  break;
	case 3: mNeighbor->Shift_perturbation(1);  break;
	case 4: mNeighbor->Shift_perturbation(2);  break;
	case 5: mNeighbor->Swap_perturbation(1);  break;
	default:
		break;
	}

	double perturbFitness = pertubedSolution.getSolutionFitness();

	switch (currentNeigbour){
	case 0: mNeighbor->Shift_1_0(); break;
	case 5: mNeighbor->Shift_2_0(); break;
	case 1:	mNeighbor->Swap_1_1();	break;
	case 2:	mNeighbor->Swap_2_1();	break;
	case 4:	mNeighbor->Swap_2_2();	break;
	case 3:	mNeighbor->Cross();		break;
	case 6: mNeighbor->K_Shift();	break;
	default:
		break;
	}

	//cout << currentNeigbour << endl;

	if (mNeighbor->obtainedNeighbor.size() == 0) { 
		if (currentNeigbour != 6)	{
			currentNeigbour++;
			//cout << currentNeigbour << endl;
		}
		else currentNeigbour = 0;
		return false; };

	cout << CurrentSolution->getSolutionFitness() << ", " << perturbFitness;

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

	neighborStructureType randomNeighorhood = static_cast<neighborStructureType>(mNeighbor->neighborStructure.at(Utility::getRandomUniformNoBetween(7, mNeighbor->neighborStructure.size())));

	switch (randomNeighorhood){
	case REINSERTION: mNeighbor->Reinsertion(); break;
	case OR_OPT2:	  mNeighbor->Or_opt2();	break;
	case OR_OPT3:	  mNeighbor->Or_opt3();	break;
	case TWO_OPT:	  mNeighbor->Two_opt();	break;
	case EXCAHNGE:	  mNeighbor->Exchange();	break;
	default:
		break;
	}

	if (mNeighbor->obtainedNeighbor.size() != 0 ){
		if (mNeighbor->neighborSelector == FIRST)
			bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
		else {
			SaveNeighborhood::sortNeighborByImproveValue(&mNeighbor->obtainedNeighbor);
			bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
		}
		pertubedSolution.routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
		pertubedSolution.calculatSolutionFitness();
	}

	if (pertubedSolution < *CurrentSolution){

		CurrentSolution = &pertubedSolution;

		//if (bestOrFirstNeighbor.getARoute() == bestOrFirstNeighbor.getBRoute())	{
		//	CurrentSolution->routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
		//}
		//else{
		//	CurrentSolution->routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
		//	CurrentSolution->routes.at(bestOrFirstNeighbor.getBRoute()) = bestOrFirstNeighbor.getSecondRoute();
		//}
		//CurrentSolution->calculatSolutionFitness();

		//cout << CurrentSolution->getSolutionFitness() << endl;
		/*cout << " Before: " << oldFitness << " Now: " << pertubedSolution.getSolutionFitness() << " ** Using: " <<
		Utility::itgetNeighborStructureAsName(static_cast<neighborStructureType>(bestOrFirstNeighbor.getNeighborStructure())) << endl;*/
	}
	//else
	//{
	//	if (currentNeigbour != 6)	{
	//		currentNeigbour++;
	//		//cout << currentNeigbour << endl;
	//	}
	//	else currentNeigbour = 0;
	//}

	t->recordBest(Solution::getCurrentBestSolution(selectedStrategy->solutios));
	//cout << CurrentSolution->getSolutionFitness() << endl;
	cout << ", " << pertubedSolution.getSolutionFitness() << ", " << levyFlightNo << ", " << t->best->getSolutionFitness() << ", " << currentNeigbour << endl;
	abandonWorseCuckoo();
	return false;
}

void CuckooSearch_best::abandonWorseCuckoo(){

	Solution::sortSolutionByFitness(selectedStrategy->solutios);

	//for (unsigned int i = solutionToBeDeletedOrCreated; i != 0; i--) {
	//	vector<Solution>::const_iterator itDel = selectedStrategy->solutios->begin();
	//	advance(itDel, selectedStrategy->solutios->size() - 1);
	//	if (itDel != selectedStrategy->solutios->end()){
	//	selectedStrategy->solutios->erase(itDel);
	//	}
	//}

	createNewCuckoo();

}

//bool CuckooSearch::createNewCuckoo (){
//
//	double improved;
//	int a, b, i, j, nStructure;
//
//	for (unsigned int i = solutionToBeDeletedOrCreated; i != 0; i--) {
//		vector<Solution>::iterator itModified = selectedStrategy->solutios->end()-i;
//		mNeighbor->setIntitialSolution(*itModified);
//		SaveNeighborhood obtained = mNeighbor->makeOneRandomNeighborState();
//
//		if (obtained.getNeighborSelector() != CONSTRAINT_VIOLATED) {
//			switch (obtained.getNeighborStructure()){
//			case SWAP:	 mNeighbor->swap(obtained.getARoute(), obtained.getBRoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
//			case MOVE:	 mNeighbor->move(obtained.getARoute(), obtained.getBRoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
//			case REVERSE:mNeighbor->reversed(obtained.getARoute(), obtained.getICustomer(), obtained.getJCustomer()); break;
//			}
//			CurrentSolution->calculatSolutionFitness();
//		}
//	}
//
//	Solution::sortSolutionByFitness(selectedStrategy->solutios);
//	return true;
//}

bool CuckooSearch_best::createNewCuckoo(){

	double improved;
	int a, b, i, j, nStructure;

	for (unsigned int i = 1; i <= solutionToBeDeletedOrCreated; i++) {
		CurrentSolution = &selectedStrategy->solutios->at(selectedStrategy->solutios->size() - i);

		double oldFitness = CurrentSolution->getSolutionFitness();

		mNeighbor->setIntitialSolution(*CurrentSolution);

		neighborStructureType randomNeighorhood = static_cast<neighborStructureType>(mNeighbor->neighborStructure.at(rand() % mNeighbor->neighborStructure.size()));

		switch (currentNeigbour){
		case SHIFT_1_0:	 mNeighbor->Shift_1_0(); break;
		case SHIFT_2_0:  mNeighbor->Shift_2_0(); break;
		case SWAP_1_1:	 mNeighbor->Swap_1_1();	break;
		case SWAP_1_2:	 mNeighbor->Swap_2_1();	break;
		case SWAP_2_2:	 mNeighbor->Swap_2_2();	break;
		case CROSS:		 mNeighbor->Cross();	break;
		case K_SHIFT:	 mNeighbor->K_Shift();	break;
		case REINSERTION:mNeighbor->Reinsertion(); break;
		case OR_OPT2:	 mNeighbor->Or_opt2();	break;
		case OR_OPT3:	 mNeighbor->Or_opt3();	break;
		case TWO_OPT:	 mNeighbor->Two_opt();	break;
		case EXCAHNGE:	 mNeighbor->Exchange();	break;
		default:
			break;
		}

		if (mNeighbor->obtainedNeighbor.size() == 0) continue;

		SaveNeighborhood bestOrFirstNeighbor;

		if (mNeighbor->neighborSelector == FIRST){
			bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
		}
		else {
			SaveNeighborhood::sortNeighborByImproveValue(&mNeighbor->obtainedNeighbor);
			bestOrFirstNeighbor = mNeighbor->obtainedNeighbor[0];
		}

		if (bestOrFirstNeighbor.getARoute() == bestOrFirstNeighbor.getBRoute())	{
			CurrentSolution->routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
		}
		else{
			CurrentSolution->routes.at(bestOrFirstNeighbor.getARoute()) = bestOrFirstNeighbor.getFirstRoute();
			CurrentSolution->routes.at(bestOrFirstNeighbor.getBRoute()) = bestOrFirstNeighbor.getSecondRoute();
		}
		CurrentSolution->calculatSolutionFitness();
		t->recordBest(Solution::getCurrentBestSolution(selectedStrategy->solutios));
		/*	cout << "      Before: " << oldFitness << " Now: " << CurrentSolution->getSolutionFitness() << "  ** Using: " <<
		Utility::itgetNeighborStructureAsName(static_cast<neighborStructureType>(bestOrFirstNeighbor.getNeighborStructure())) << endl;*/
	}
	Solution::sortSolutionByFitness(selectedStrategy->solutios);
	return true;
}

bool CuckooSearch_best::acceptanceCriteria(double ImprovmentValue){
	if (ImprovmentValue >= 0)
		return true;
	else
		return false;
}