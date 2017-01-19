#include "route.h"

	Route::Route(){};

	Route::Route(Instance* i){
		problemInstsnce=i;
		customers.reserve (0);
		isRouteFitnessCalculated=false;
		routeDemand=0;
		routeFitness=0;
	}

	vector<int> Route::getRoute (){
		return customers;
	}

	int Route::getRouteDemand(){
		return routeDemand;
	};

	double Route::getRouteFitness(){
		if (!isRouteFitnessCalculated)
			calculateRouteFitness ();
		return routeFitness;
	};

	int Route::getCustomerSize() {
		return customers.size ();
	}

	int Route::getCustomerAtCaller(int index, string caller) throw (VRPException) {
		if (index >= customers.size()) {
			printf("getCustomerAt() function being called from %s", caller.c_str());
			throw VRPException("Customer out of range");
		}
		return customers[index];
	}

	int Route::getRandomCustomerPosition() throw (VRPException) {
		if (getCustomerSize() == 0) 
			throw VRPException("getRandomCustomerPosition Customer size is empty");
		return rand () % getCustomerSize ()-1 ;
	}

	void Route::setRouteFitnessNotCalculated(){
		isRouteFitnessCalculated = false;
	}

	void Route::setCustomerAt(int customer, int position) throw (VRPException) {
		if (position > customers.size()) throw VRPException("Position is out of range");
		customers[position]=customer;
		setRouteFitnessNotCalculated();
	}

	void Route::pushToEnd(int customer){ // push back to last position
		isRouteFitnessCalculated=false;
		return addCustomerAt (customer, customers.size ());
	}

	void Route::addCustomerAt (int customer, int insertionPosition) throw (VRPException){ // insert into position
		if (insertionPosition > customers.size()) 
			throw VRPException("The customer index is larger than customer vector");
			customers.insert (customers.begin() + insertionPosition, customer);
		isRouteFitnessCalculated = false;
	}

	void Route::deleteCustomerAt(int deletePosition) throw (VRPException) {
		if (deletePosition > customers.size()) throw VRPException("Delete position is out of range");
		customers.erase (customers.begin () + deletePosition);
		isRouteFitnessCalculated=false;
	}

	void Route::deleteAllCustomer(){
		customers.clear();
		routeDemand = 0; routeFitness = 0;
		isRouteFitnessCalculated = false;
	}

	bool Route::isCapacityConstraintViolated (){
		return ( routeDemand > problemInstsnce->capacity ) ? true : false;
	}

	bool Route::isCapacityConstraintViolated(int customer){
		return ( problemInstsnce->customers[customer].getDemand () + routeDemand > problemInstsnce->capacity ) ? true : false;
	}
	
	void Route::calculateRouteDemand() throw (VRPException) {
		if (customers.empty()) throw VRPException("calculateRouteDemand Customer size is empty");
		routeDemand=0;
		for(unsigned int j = 0; j < customers.size(); ++j)
			routeDemand += problemInstsnce->customers[getCustomerAt (j)].getDemand () ;
	}

	double Route::distanceBetween(int i, int j){
		return problemInstsnce->distances[getCustomerAt (i)][getCustomerAt (j)];
	}

	int Route::getSmallestCustomerDemand() throw (VRPException){
		if (customers.empty()) throw VRPException("getSmallestCustomerDemand Customer size is empty");
		int smallestDemandCustomer = customers[0];
		int smallestDemand = problemInstsnce->customers.at (smallestDemandCustomer).getDemand ();
		for (vector<int>::iterator it=customers.begin ()+1; it!=customers.end ();it++){
			if (problemInstsnce->customers.at (*it).getDemand () < smallestDemand){
				smallestDemandCustomer = *it;
				smallestDemand = problemInstsnce->customers.at (smallestDemandCustomer).getDemand ();
			}
		}
		return smallestDemand;
	}

	void Route::calculateRouteFitness(){

		routeFitness = routeDemand = 0;

		if ( customers.size() > 0 ) {
			int first = getCustomerAt(0);
			routeFitness = problemInstsnce->distances[0][first];
			for(unsigned int j = 0; j < customers.size(); ++j){
				routeDemand += problemInstsnce->customers[getCustomerAt (j)].getDemand () ;
				if( j == customers.size() - 1 )
					routeFitness += problemInstsnce->distances[getCustomerAt (j)][0];
				else
					routeFitness += distanceBetween(j,j+1);
			}
		}
		else {}
		//	throw VRPException("calculateRouteFitness Customer size is empty\n");
		isRouteFitnessCalculated=true;
	}

	void Route::printRoute() const {
		printf( "%2d customers, distance = %3.2f, demand = %d :", customers.size(), routeFitness, routeDemand);
		for(vector<int>::const_iterator it = customers.begin(); it != customers.end(); ++it)
			printf(" %d", *it);
		cout<<endl ;

	}

	bool Route::operator!=(Route lhs){
		return !equal(lhs.customers.begin(), lhs.customers.end(), this->customers.begin());
	}

	bool Route::operator==(Route lhs){
		return equal(lhs.customers.begin(), lhs.customers.end(), this->customers.begin());
	}