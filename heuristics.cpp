#include "heuristics.h"

// Make a construction with the next to fit method
void constructionNextFit(Data* data, Solution* sol) {
	int current_vehicle = 0;
	int last_customers = 0;
	int current_customers;

	sol->capacity_rest.push_back(data->vehicleCapacity);
	std::vector<int> ints;
	sol->routes.push_back(ints);
	sol->routes[current_vehicle].push_back(0);
	for (int i = 1; i <= data->numberCustomers; i++) {
		current_customers = i;
		if (sol->capacity_rest[current_vehicle] < data->demand[current_customers]) {
			endRouteStartNew(data, sol, current_vehicle, last_customers);
			current_vehicle ++;
			last_customers = 0;
		}
		addCustomerRoute(data, sol, current_vehicle, current_customers, last_customers);
		last_customers = current_customers;
	}
	sol->routes[current_vehicle].push_back(0);
	sol->cost += data->distance[last_customers][0];
}

// Make a construction we add the customer with the shortest distance with the last customers
void constructionBestDistance(Data* data, Solution* sol) {
	int current_vehicle = 0;
	int last_customers = 0;
	std::vector<int> tabIndex;
	for (int i = 1; i <= data->numberCustomers; i++) {
		tabIndex.push_back(i);
	}

	sol->capacity_rest.push_back(data->vehicleCapacity);
	std::vector<int> ints;
	sol->routes.push_back(ints);
	sol->routes[current_vehicle].push_back(0);
	int current_customers_index;
	int current_customers;
	for (int i = 1; i <= data->numberCustomers; i++) {
		current_customers_index = searchMinDistance(data, tabIndex, last_customers);
		current_customers = tabIndex[current_customers_index];
		if (sol->capacity_rest[current_vehicle] < data->demand[current_customers]) {
			endRouteStartNew(data, sol, current_vehicle, last_customers);
			current_vehicle ++;
			last_customers = 0;
			current_customers_index = searchMinDistance(data, tabIndex, last_customers);
			current_customers = tabIndex[current_customers_index];
		}
		tabIndex.erase(tabIndex.begin() + current_customers_index);
		addCustomerRoute(data, sol, current_vehicle, current_customers, last_customers);
		last_customers = current_customers;
	}
	sol->routes[current_vehicle].push_back(0);
	sol->cost += data->distance[last_customers][0];
}

// Make a construction with one route/vehicle per customer
void constructionAllVehicle(Data* data, Solution* sol) {
	int current_vehicle = 0;
	int current_customers;

	for (int i = 1; i <= data->numberCustomers; i++) {
		current_customers = i;
		routeOneVehicle(data, sol, current_vehicle, current_customers);
		current_vehicle++;
	}
}

// make an iteration for improve a solution
void oneImprove(Data* data, Solution* sol, int limitValue) {
	for (int firstVehicle = 0; firstVehicle < sol->routes.size() ; firstVehicle++) {
		for (int firstCustomer = 1; firstCustomer < sol->routes[firstVehicle].size()-1 ; firstCustomer++) {
			echange(data, sol, firstVehicle, firstCustomer, limitValue);
			move(data, sol, firstVehicle, firstCustomer, limitValue);
		}
	}
}

// echange two customers if its possible and the distanceValue is inferior to limitValue
void echange(Data* data, Solution* sol, int firstVehicle, int firstCustomer, int limitValue) {
	for (int secondVehicle = firstVehicle; secondVehicle < sol->routes.size() ; secondVehicle++) {
		int memory = 1;
		if(firstVehicle == secondVehicle){
			memory = firstCustomer+1;
		}
		for (int secondCustomer = memory; secondCustomer < sol->routes[secondVehicle].size()-1 ; secondCustomer++) {
			if(echangeDemandValue(data, sol, firstVehicle, secondVehicle, firstCustomer, secondCustomer)) {
				double distanceValue = echangeDistanceValue(data, sol, firstVehicle, secondVehicle, firstCustomer, secondCustomer);
				if(distanceValue < limitValue) {
					makeEchange(data, sol, firstVehicle, secondVehicle, firstCustomer, secondCustomer, distanceValue);
				}
			}
		}
	}
}

// move one customers if its possible and the distanceValue is inferior to limitValue
void move(Data* data, Solution* sol, int firstVehicle, int firstCustomer, int limitValue) {
	for (int secondVehicle = 0; secondVehicle < sol->routes.size() ; secondVehicle++) {
		for (int secondCustomer = 1; secondCustomer < sol->routes[secondVehicle].size()-1 ; secondCustomer++) {
			// can't move if the two customer is the same or the first is just before the second
			if (! (firstVehicle == secondVehicle && (firstCustomer == secondCustomer || firstCustomer == secondCustomer-1))) {
				// If the displacement is feasible in relation to the demand of the various customers
				if (moveDemandValue(data, sol, firstVehicle, secondVehicle, firstCustomer)) {
					double distanceValue = moveDistanceValue(data, sol, firstVehicle, secondVehicle, firstCustomer, secondCustomer);
					if (distanceValue < limitValue) {
						int afterFirstCustomer = sol->routes[firstVehicle][firstCustomer+1];
						int beforeFirstCustomer = sol->routes[firstVehicle][firstCustomer-1];
						makeMove(data, sol, firstVehicle, secondVehicle, firstCustomer, secondCustomer, distanceValue);
						// delete route if we move the last customer of the route
						if (afterFirstCustomer == beforeFirstCustomer) {
							sol->routes.erase(sol->routes.begin() + firstVehicle);
							sol->capacity_rest.erase(sol->capacity_rest.begin() + firstVehicle);
							// cout << "delete route" << endl;
							return;
						}
					}
				}
			}
		}
	}
	double distanceValue = moveNewDistanceValue(data, sol, firstVehicle, firstCustomer);
	if (distanceValue < limitValue) {
		makeMoveNew(data, sol, firstVehicle, firstCustomer, distanceValue);
	}
}

void manyImprove(Data* data, Solution* sol, Solution* solMin, int limitValueMax, int secondMax, bool distanceFixed, clock_t timeBefore) {
	double cost = sol->cost;
	int count = 0;
	int limitValue = 0;
	solMin = sol;
	while ((clock() - timeBefore) < secondMax*CLOCKS_PER_SEC) {
		oneImprove(data, sol, limitValue);
		if(cost <= sol->cost+0.1){
			if (limitValue > limitValueMax) {
				limitValue = 0;
			} else {
				limitValue++;
			}
		}
		count ++;
		cost = sol->cost;
		if (cost < solMin->cost){
			solMin = sol;
			if (!distanceFixed) {
				limitValueMax = solMin->cost/50;
			}
			limitValue = 0;
		}
	}
	cout << " end costMin " << solMin->cost << endl;
	cout << " Count " << count << endl;
	// return solMin;
}

// make construction with the constructor on the parameter after that
// make multiple iteration of improve with different limitValue
// return the solution minimum find after the time was expired
Solution makeSolMin(Data* data, int constructor, int limitValueMax, int secondMax, bool distanceFixed) {
	// Solution sol;
	Solution sol;
	Solution solMin;
	//Get start time
	clock_t timeBefore = clock();
	//Get initial solution
	switch (constructor) {
		case 0:
			constructionNextFit(data, &sol);
			break;
		case 1:
			constructionBestDistance(data, &sol);
			break;
		case 2:
			constructionAllVehicle(data, &sol);
			break;
	}
	// manyImprove(data, &sol, &solMin, limitValueMax, secondMax, distanceFixed, timeBefore);
	double cost = sol.cost;
	int count = 0;
	int limitValue = 0;
	solMin = sol;
	while ((clock() - timeBefore) < secondMax*CLOCKS_PER_SEC) {
		oneImprove(data, &sol, limitValue);
		// if the cost isn't shorter than the last we change limitValue a marge was add cause the floating calcul make wrong comparison
		if(cost <= sol.cost+0.01){
			// we reset limitValue if we have reach the max
			if (limitValue > limitValueMax) {
				limitValue = 0;
			} else {
				limitValue++;
			}
		}
		cost = sol.cost;
		// if we have a better solution than we find before we update solMin
		if (cost < solMin.cost){
			solMin = sol;
			// we update limitValueMax if distanceFixed is false
			if (!distanceFixed) {
				limitValueMax = solMin.cost/50;
			}
			limitValue = 0;
		}
		count ++;
	}
	cout << " end costMin " << solMin.cost << endl;
	cout << " Count " << count << endl;

	solMin.time = (double) (clock() - timeBefore)/CLOCKS_PER_SEC;
	cout << " Time : " << solMin.time << endl;
	verification(data, &solMin);
	printSolutionInFile(data, &solMin);
	return solMin;
}

// make the construcionImprove for all instances
void allInstances(int constructor, int limitValueMax, int secondMax, bool distanceFixed) {
	std::vector<string> instNames;
	instNames.push_back("vrpnc1.txt");
	instNames.push_back("vrpnc2.txt");
	instNames.push_back("vrpnc3.txt");
	instNames.push_back("vrpnc4.txt");
	instNames.push_back("vrpnc5.txt");
	instNames.push_back("vrpnc11.txt");
	instNames.push_back("vrpnc12.txt");

	for (int i = 0 ; i < instNames.size() ; i++) {
		string instName = instNames[i];
		Data data;
		Solution sol;
		readData(&data, instName);
		cout << "Instance: " << data.instanceName <<" "<<CLOCKS_PER_SEC<<" "<< endl;

		makeSolMin(&data, constructor, limitValueMax, secondMax, distanceFixed);

		//Print solution in file
		for (int j = 0; j <= data.numberCustomers; j++) {
			delete[] data.distance[j];
		}
		delete[] data.demand;
		delete[] data.distance;
	}
}

// make multiple solution with different constructor and different
void makeTabTest (Data* data, int distanceMin, int distanceMax, int secondMax) {
	Solution sol;
	Solution solMin;
	double costMin = pow(2,10);
	char tempString[100];
	sprintf(tempString, "Test/%s_sol.csv", data->instanceName.c_str());
	ofstream solution(tempString, ios::out);
	solution << data->instanceName.c_str() << ",Next Fit,Best Distance,All Vehicle" << endl;
	for (int limitValueMax = distanceMin ; limitValueMax < distanceMax ; limitValueMax++) {
		solution << limitValueMax;
		for (int constructor = 0 ; constructor < 3 ; constructor++) {
			sol = makeSolMin(data, constructor, limitValueMax, secondMax, true);
			solution << " , " << sol.cost;
			if (sol.cost < costMin) {
				costMin = sol.cost;
				solMin = sol;
			}
		}
		solution << endl;
	}
	solution.close();
	printSolutionInFile(data, &solMin);
}
