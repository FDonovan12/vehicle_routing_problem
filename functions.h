#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "readData.h"
#include <time.h>
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include <limits>

struct Solution {
	double cost;		//Solution cost
	double time;		//Running time
	vector <int> capacity_rest;	//capacity rest
	vector <vector <int>> routes;	//Solution
};

void tabDistance(Data* data);
int searchMinDistance(Data* data, std::vector<int> tabIndex, int last_customers);
void endRouteStartNew(Data* data, Solution* sol, int current_vehicle, int last_customers);
void routeOneVehicle(Data* data, Solution* sol, int current_vehicle, int current_customers);
void addCustomerRoute(Data* data, Solution* sol, int current_vehicle, int current_customers, int last_customers);
double distanceMean(Data* data, Solution* sol, double max);
void printVector(vector <int> tab);
void printSolutionInFile(Data* data, Solution* sol);
void makeEchange(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer, double distance);
void makeMove(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer, double distance);
void makeMoveNew(Data* data, Solution* sol, int firstVehicle, int firstCustomer, double distance);
bool verification(Data* data, Solution* sol);

#endif
