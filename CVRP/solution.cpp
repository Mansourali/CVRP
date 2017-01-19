#include "solution.h"

	Solution::Solution() {};

	Solution::Solution(Instance* i){
		problem_instance=i;
		for (unsigned int j=0; j<this->problem_instance->minTrucks ;j++) routes.push_back (Route(i));
		totalCustomer = problem_instance->dimension;
		isSolutionFitnessCalculated=false;
		solutionFitness=0;
	}

	double Solution::getSolutionFitness() {
		if (!isSolutionFitnessCalculated) 
			calculatSolutionFitness();
		return solutionFitness;
	}

	void Solution::calculatSolutionFitness() throw(VRPException) {

		if (routes.size() < 1)
			throw VRPException ("Solution size Violation < 1") ;
		solutionFitness = 0;
		for (unsigned int j = 0; j < routes.size(); ++j) {
			solutionFitness += routes[j].getRouteFitness();
		}
		isSolutionFitnessCalculated=true;
	}

	void Solution::clear(){
		this->routes.clear();
		this->solutionFitness = 0;
		this->initialFitness = 0;
		this->totalCustomer = 0;
	}

	int Solution::getRandomRouteIndexCaller(string caller) const throw(VRPException) {
		if (routes.size() == 0){
			printf("getRandomRouteIndexCaller() function being called from %s", caller.c_str());
			throw VRPException("The routes size is empty");
		}
		int te = rand () % routes.size();
		return te;
	}

	Route Solution::getRouteAtCaller(int routeIndex, string caller) const throw(VRPException) { 
		if (routeIndex > routes.size())	{
			printf("getCustomerAt() function being called from %s", caller.c_str());
			throw VRPException("Route index is out of range");
		}
		return routes[routeIndex]; 
	}

	void Solution::printSolution () const {
		printf("[%d routes, distance = %3.2f]\n", routes.size (), solutionFitness);
		for(vector<Route>::const_iterator it = routes.begin(); it != routes.end(); ++it)
			it->printRoute();
		cout << endl;
	}

	bool Solution::customerSizeIsViolated(){
		int customerSize = 0;
		for (int i = 0; i < getRoutesSize(); i++)	
			customerSize += getRouteAt(i).getCustomerSize();
		if (customerSize == getTotalCustomer() - 1)
		return false;
		else {
			for (int i = 0; i < getRoutesSize(); i++)
				routes[i].deleteAllCustomer();
			return true;
		}
	}

	vector<Solution> Solution::solutionsFactory(vector<Solution>* initialSolutions, initializationTechnique::by value, Utility &u) {

		switch (value)
		{
		case initializationTechnique::by::RANDOM:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).randomInitialization();
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::CLARKE_AND_WRIGHT:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).cLarkeAndWright();
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_N_DEPOT_NEAREST_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::NEAREST, InsertionCriteria::NEAREST);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_F_DEPOT_NEAREST_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::FARTEST, InsertionCriteria::NEAREST);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_N_DEPOT_COMPINED_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::NEAREST, InsertionCriteria::COMPINED);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_F_DEPOT_COMPINED_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::FARTEST, InsertionCriteria::COMPINED);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_NF_DEPOT_NEAREST_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::NEAREST_FARTEST, InsertionCriteria::NEAREST);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_NF_DEPOT_COMPINED_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::NEAREST_FARTEST, InsertionCriteria::COMPINED);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_NR_DEPOT_NEAREST_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::NEAREST_RANDOM, InsertionCriteria::NEAREST);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_NR_DEPOT_COMPINED_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::NEAREST_RANDOM, InsertionCriteria::COMPINED);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_FR_DEPOT_NEAREST_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::FARTEST_RANDOM, InsertionCriteria::NEAREST);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_FR_DEPOT_COMPINED_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::FARTEST_RANDOM, InsertionCriteria::COMPINED);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		case initializationTechnique::by::NN_R_DEPOT_NEAREST_ADD:
			for (int i = 0; i < u.POPULATION.getPOPULATION_SIZE(); i++)	{
				initialSolutions->at(i).nearestneigborInitialization(initializationCriteria::RANDOM, InsertionCriteria::NEAREST);
				initialSolutions->at(i).initialFitness = initialSolutions->at(i).getSolutionFitness();
			}
			break;
		}

		sortSolutionByFitness(initialSolutions);
		return *initialSolutions;
	}

	Solution& Solution::getCurrentBestSolution(vector<Solution>* initialSolutions) throw (VRPException) {
		sortSolutionByFitness(initialSolutions);
		if (initialSolutions->empty()) throw VRPException("Solution vector is Zero");
		return initialSolutions->at(0);
	}

	void Solution::randomInitialization(){
		do	{
			bool proceed = true; int count = 1;
			for (int id = 1; id < totalCustomer && proceed; ++id){
				int rnd = rand() % routes.size();
				while (routes[rnd].isCapacityConstraintViolated(id) && proceed){
					rnd = rand() % routes.size(); count++;
					if (count > 30) // terminate after certain tries and try again
						proceed = false;}
				count = 1;
				routes[rnd].pushToEnd(id);
				routes[rnd].calculateRouteDemand();
			}
		} while ( customerSizeIsViolated () ); 
		calculatSolutionFitness ();
	}

	void Solution::nearestNeighbor(){
		
	}

	void Solution::nearestneigborInitialization(initializationCriteria::by mInitial, InsertionCriteria::by mInsert){

		int currentAddition, currentRoute, count , maximumTry = totalCustomer; bool routeSizeIsViolated, proceed;

		do	{
			routeSizeIsViolated = proceed = false; currentRoute = count = 0;
			vector<int> remianing(totalCustomer - 1);
			iota(begin(remianing), end(remianing), 1);

			for (auto route : routes){
				if (currentRoute == routes.size()) { routeSizeIsViolated = true; break; }
				double random = Utility::getRandomUniformNoBetween(0, 1);
				switch (mInitial){
				case initializationCriteria::NEAREST:
					currentAddition = nearestCustomerPair(0, remianing);
					break;
				case initializationCriteria::FARTEST:
					currentAddition = fartestCustomerPair(0, remianing);
					break;
				case initializationCriteria::NEAREST_FARTEST:
					if (random < 0.5) currentAddition = nearestCustomerPair(0, remianing);
					else currentAddition = fartestCustomerPair(0, remianing);
					break;
				case initializationCriteria::NEAREST_RANDOM:
					if (random < 0.5) currentAddition = nearestCustomerPair(0, remianing);
					else currentAddition = randomCustomerPair(remianing);
					break;
				case initializationCriteria::FARTEST_RANDOM:
					if (random < 0.5) currentAddition = fartestCustomerPair(0, remianing);
					else currentAddition = randomCustomerPair(remianing);
					break;
				case initializationCriteria::RANDOM:
					currentAddition = randomCustomerPair(remianing);
					break;
				} 
				route.pushToEnd(currentAddition);
				route.calculateRouteDemand();
				deleteVisitedCustomer(currentAddition, remianing);

				while (!remianing.empty()) {
					double random = Utility::getRandomUniformNoBetween(0, 1);
					switch (mInsert){
					case InsertionCriteria::NEAREST:
						currentAddition = nearestCustomerPair(currentAddition, remianing);
						break;
					case InsertionCriteria::COMPINED:
						double random = Utility::getRandomUniformNoBetween(0, 1); 
						if (random < 0.5) currentAddition = randomCustomerPair(remianing);
						else currentAddition = nearestCustomerPair(currentAddition, remianing);
						break;
					}
					
					if (!route.isCapacityConstraintViolated(currentAddition)){
						route.pushToEnd(currentAddition);
						route.calculateRouteDemand();
						deleteVisitedCustomer(currentAddition, remianing);
					}
					//else if (currentRoute == routes.size() && remianing.size() > 0)
					//	proceed = false;
					else 
						break;
				}
				routes.at(currentRoute) = route;
				currentRoute++;
			}
		} while (customerSizeIsViolated() || routeSizeIsViolated);

		calculatSolutionFitness();
	}

	void Solution::deleteVisitedCustomer(int customer, vector<int>& remaining){
		remaining.erase(remove(remaining.begin(), remaining.end(), customer), remaining.end());
	}

	void Solution::randomnearestneigborInitialization(){
		bool routeSizeIsViolated;

		do	{
			vector<int> remianing(totalCustomer - 1);
			iota(begin(remianing), end(remianing), 1);
			int currentNearest, currentRoute = 0, randomCustomer = 0;
			routeSizeIsViolated = false;
			randomCustomer = rand() % remianing.size();
			currentNearest = nearestCustomerPair(remianing.at(randomCustomer), remianing);
			routes[currentRoute].pushToEnd(currentNearest);
			routes[currentRoute].calculateRouteDemand();
			remianing.erase(remove(remianing.begin(), remianing.end(), currentNearest), remianing.end());

			while (!remianing.empty()) {
				currentNearest = nearestCustomerPair(currentNearest, remianing);
				if (!routes[currentRoute].isCapacityConstraintViolated(currentNearest)){
					routes[currentRoute].pushToEnd(currentNearest);
					routes[currentRoute].calculateRouteDemand();}
				else {
					currentRoute++;
					if (currentRoute == routes.size()) 
					{ routeSizeIsViolated = true; break; }
					randomCustomer = rand() % remianing.size();
					currentNearest = nearestCustomerPair(remianing.at(randomCustomer), remianing);
					routes[currentRoute].pushToEnd(currentNearest);
					routes[currentRoute].calculateRouteDemand();
				}
				remianing.erase(remove(remianing.begin(), remianing.end(), currentNearest), remianing.end());
			}
			calculatSolutionFitness();
		} while (customerSizeIsViolated() && routeSizeIsViolated);

	}

	int Solution::nearestCustomerPair(int currentNearest, vector<int>& remianing){

		int nearestCostumer = 0;
		double minCostumerToCostumerDistance = 9999;
		for (auto next : remianing){
			if (minCostumerToCostumerDistance > problem_instance->distances[currentNearest][next] && currentNearest != next){
				nearestCostumer = next;
				minCostumerToCostumerDistance = problem_instance->distances[currentNearest][next];
			}		}
		return nearestCostumer;
	}

	int Solution::fartestCustomerPair(int currentFartest, vector<int>& remianing){

		int fartestCostumer = 0;
		double minCostumerToCostumerDistance = 0;
		for (auto next : remianing){
			if (minCostumerToCostumerDistance < problem_instance->distances[currentFartest][next] && currentFartest != next){
				fartestCostumer = next;
				minCostumerToCostumerDistance = problem_instance->distances[currentFartest][next];
			}
		}
		return fartestCostumer;
	}

	int Solution::randomCustomerPair(vector<int>& remianing){
		return remianing.at(rand() % remianing.size());
	}

	vector<double> Solution::measureDiversification(vector<Solution>& population, int vehicle){
		vector<vector<int>> divector, allDeffrences;
		vector<int> diffrence; vector<double> allAvg;
			divector.resize(vehicle);
			int count = 0, i = 0, sum = 0, totalCustomer = population[0].totalCustomer - 1, total_sum = 0 ;

			for (vector<Solution>::iterator first = population.begin(); first != population.end(); ++first){
				for (vector<Solution>::iterator second = population.begin(); second != population.end(); ++second){
					for (vector<Route>::iterator it_first = first->routes.begin(); it_first != first->routes.end(); ++it_first, i++){
						for (vector<Route>::iterator it_second = second->routes.begin(); it_second != second->routes.end(); ++it_second){
							for (vector<int>::iterator first_cus = it_first->customers.begin(); first_cus != it_first->customers.end(); first_cus++){
								for (vector<int>::iterator second_cus = it_second->customers.begin(); second_cus != it_second->customers.end(); second_cus++){
									if (*first_cus == *second_cus)
										count++;
								}
							}
							divector[i].push_back(count);
							count = 0;
						}
					}
					i = sum = 0; 
					for (vector<vector<int>>::iterator it = divector.begin(); it != divector.end(); ++it){
						auto max = distance(it->begin(), max_element(it->begin(), it->end()));
						sum = sum + it->at(max);
						fill(it->begin(), it->end(), 0);
						for (int k = 0; k < divector.size(); k++)
							divector[k][max] = 0;
					}
					divector.clear(); divector.resize(vehicle);
					int current_diffrence = totalCustomer - sum;
					total_sum = total_sum + current_diffrence;
					diffrence.push_back(current_diffrence);
				}
				double avg = double(total_sum) / double(population.size()); total_sum = 0;
				allDeffrences.push_back(diffrence);
				allAvg.push_back(avg); 
				diffrence.clear();
			}

			//for (int i = 0; i < allAvg.size(); i++)
			//	cout << allAvg.at(i) << "  " << population.at(i).getInitialFitness() << endl;
			//cout << endl;
			
		return allAvg;
	}

	bool Solution::compFitness(const Solution& a, const Solution& b) { return a.solutionFitness < b.solutionFitness; };

	void Solution::sortSolutionByFitness(vector<Solution> *solutions) { sort(solutions->begin(), solutions->end(), &compFitness); };

	bool Solution::operator<(Solution& toCompare){ return (this->solutionFitness < toCompare.solutionFitness) ? true : false; }

	bool Solution::operator>(Solution& toCompare){ return (this->solutionFitness > toCompare.solutionFitness) ? true : false; }

	bool Solution::alreadyFound(Solution& toCompare) { return (this->solutionFitness = toCompare.solutionFitness) ? true : false; }

	bool compSaving(const vector<int>& a, const vector<int>& b) { return a.at(2) > b.at(2); }

	vector<vector<int>>& Solution::computeSaving(){
		vector<int> tEMP;
		vector<vector<int>>* savingMatrix = new vector<vector<int>>;      // compute Saving
		for (unsigned int i = 1; i < problem_instance->dimension-1; i++){
			for (unsigned int j = i + 1; j <= problem_instance->dimension-1; j++){
				tEMP.push_back(i);
				tEMP.push_back(j);
				tEMP.push_back((problem_instance->distances[0][i] + problem_instance->distances[0][j]) - (1 * problem_instance->distances[i][j]));
				savingMatrix->push_back(tEMP);
				tEMP.clear(); tEMP.shrink_to_fit();
			}
		}
		return *savingMatrix;
	}

	int findRouteindex(vector<Route>& allRoutes, int& custumer){

		int i = 0;
		for (vector<Route>::iterator it = allRoutes.begin(); it != allRoutes.end(); it++)	{
			vector<int>::iterator  found = find((*it).customers.begin(), (*it).customers.end(), custumer);
			if (found != it->customers.end())  {
				return i;
			}
			i++;
		}
	}

	bool Solution::mergeFeasibility(int& customerA, int& customerB, vector<Route>& allRoutes){

		int customerAindex = findRouteindex(allRoutes, customerA);
		int customerBindex = findRouteindex(allRoutes, customerB);

		Route routeA = allRoutes[customerAindex];
		Route routeB = allRoutes[customerBindex];

		// Customers both in the same route.
		if (customerBindex == customerAindex)
			return false;

		// Overload of the vehicle capacity.
		if (routeA.routeDemand + routeB.routeDemand > problem_instance->capacity )
			return false;

		int firstA = routeA.customers[0];
		int firstB = routeB.customers[0];
		int lastA = routeA.customers[routeA.customers.size() - 1];
		int lastB = routeB.customers[routeB.customers.size() - 1];

		// Internal customers.
		if ((firstA != customerA && lastA != customerA) ||
			(firstB != customerB && lastB != customerB))
			return false;

		return true;
	}

	void merge(int& customerA, int& customerB, vector<Route>& allRoutes){
		int customerAindex = findRouteindex(allRoutes, customerA);
		int customerBindex = findRouteindex(allRoutes, customerB);

		Route routeA = allRoutes[customerAindex];
		Route routeB = allRoutes[customerBindex];

		int customerPositionA = find(routeA.customers.begin(), routeA.customers.end(), customerA) - routeA.customers.begin();
		int customerPositionB = find(routeB.customers.begin(), routeB.customers.end(), customerB) - routeB.customers.begin();

		if (customerPositionA == 0)
			reverse(routeA.customers.begin(), routeA.customers.end());
		if (customerPositionB == routeB.customers.size() - 1)
			reverse(routeB.customers.begin(), routeB.customers.end());

		for (int i = 0; i < routeB.customers.size(); ++i)
			routeA.customers.push_back(routeB.customers[i]);
		routeA.calculateRouteFitness();

		if (customerBindex > customerAindex) {
			allRoutes.erase(allRoutes.begin() + customerBindex);
			allRoutes.erase(allRoutes.begin() + customerAindex);
		}
		else{
			allRoutes.erase(allRoutes.begin() + customerAindex);
			allRoutes.erase(allRoutes.begin() + customerBindex);
		}
		allRoutes.push_back(routeA);
	}

	void Solution::cLarkeAndWright(){

		vector<vector<int>>& savingMatrix = computeSaving();
		vector<Route> allRoutes(problem_instance->dimension-1, problem_instance);
		vector<int> triplet, ok;

		sort(savingMatrix.begin(), savingMatrix.end(), compSaving); //sort saving

		for (unsigned int i = 0; i < problem_instance->dimension-1; i++){
			allRoutes[i].customers.push_back(i + 1);
			allRoutes[i].calculateRouteFitness();
		}

		triplet.push_back(savingMatrix.at(0).at(0));	triplet.push_back(savingMatrix.at(0).at(1));	triplet.push_back(savingMatrix.at(0).at(2));
			savingMatrix.erase(savingMatrix.begin());

			while ((savingMatrix.size() != 0) && (triplet[2] > 0) && (problem_instance->minTrucks != allRoutes.size())) {
			int customerA = triplet[0];
			int customerB = triplet[1];

			if (mergeFeasibility(customerA, customerB, allRoutes)) {
				merge(customerA, customerB, allRoutes);
			}
			triplet.clear();
			triplet.push_back(savingMatrix.at(0).at(0));	triplet.push_back(savingMatrix.at(0).at(1));	triplet.push_back(savingMatrix.at(0).at(2));
			savingMatrix.erase(savingMatrix.begin());
		}
			int i = 0;
		for (vector<Route>::iterator it = allRoutes.begin(); it != allRoutes.end(); it++, i++)
			routes.at(i) = *it;

		this->calculatSolutionFitness();
	}

	//void Solution::ready(){
	//	
	//	int route1[]={4,5,27,25,30,10}; 
	//	int route2[]={20,32,13,8,7,26,12};
	//	int route3[]={15,17,9,3,16,29};
	//	int route4[]={11,18,31,1,21,14,19,28};
	//	int route5[]={22,23,6,24,2};
	//	vector<int> r1 (route1,route1+6);
	//	vector<int> r2 (route2,route2+7);
	//	vector<int> r3 (route3,route3+6);
	//	vector<int> r4 (route4,route4+8);
	//	vector<int> r5 (route5,route5+5);
	//	
	//	Route r01,r02,r03,r04,r05;
	//	r01.problemInstsnce = this->problem_instance;
	//	r02.problemInstsnce = this->problem_instance;
	//	r03.problemInstsnce = this->problem_instance;
	//	r04.problemInstsnce = this->problem_instance;
	//	r05.problemInstsnce = this->problem_instance;
	//	r01.customers = r1 ; 
	//	r02.customers = r2 ; 
	//	r03.customers = r3 ; 
	//	r04.customers = r4 ; 
	//	r05.customers = r5 ;
	//	routes.at(0) =r01;
	//	routes.at(1) =r02;
	//	routes.at(2) =r03;
	//	routes.at(3) =r04;
	//	routes.at(4) =r05;

	//	calculatSolutionFitness ();

	//}