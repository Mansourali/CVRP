#pragma once
#include <vector>
#include "customer.h"
#include "instance.h"
using namespace std;

#define getCustomerAt(index) getCustomerAtCaller(index, __FUNCTION__)

class Instance;
class Route
{
public:
	vector<int> customers;
	bool isRouteFitnessCalculated;
	int routeDemand;
	double routeFitness;
	Instance* problemInstsnce;

		Route();
		Route(Instance* i);
		int getCustomerSize();
		int getCustomerAtCaller(int index, string caller);
		void setCustomerAt(int customer,int position);
		vector<int> getRoute ();
		int getRouteDemand();
		double getRouteFitness();
		int getRandomCustomerPosition();
		void setRouteFitnessNotCalculated();
		bool isCapacityConstraintViolated();
		bool isCapacityConstraintViolated(int i);
		void pushToEnd(int customer);
		void addCustomerAt(int customer, int position);
		void deleteCustomerAt (int deletePosition);
		void deleteAllCustomer();
		double distanceBetween(int i, int j);
		void calculateRouteDemand();
		void calculateRouteFitness(); 
		int getSmallestCustomerDemand();
		void printRoute() const;
		bool operator!=(Route);
		bool operator==(Route);
		//void getCustomerAtCaller(char const* caller, int index);
};

