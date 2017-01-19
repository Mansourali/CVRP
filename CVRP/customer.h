#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
using namespace std;

class Customer
{
public:
	pair <int, int> coordinate;
	int customeDemand;
		Customer(int x, int y, int demand);
		Customer(int x, int y);
		int getX();
		int getY();
	int getDemand() const;
	void setDemand(int demand);
};
