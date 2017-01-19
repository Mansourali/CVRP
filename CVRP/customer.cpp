#include "Customer.h"

	Customer::Customer(int x, int y, int demand) {
		this->coordinate.first = x;
		this->coordinate.second = y;
		this->customeDemand = demand;
	};

	Customer::Customer(int x, int y) {
		this->coordinate.first = x;
		this->coordinate.second = y;
		this->customeDemand = 0;
	};

	int Customer::getX(){
		return this->coordinate.first;
	};
	
	int Customer::getY(){
		return this->coordinate.second;
	};

	int Customer::getDemand() const{
		return customeDemand;
	};

	void Customer::setDemand(int demand){
		this->customeDemand = demand;
	}