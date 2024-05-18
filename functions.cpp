#include "functions.h"

// print the distance between all customers
void tabDistance(Data* data) {
	for (int i = 0; i <= data->numberCustomers; i++) {
		for (int j = 0; j <= data->numberCustomers; j++) {
			cout << data->distance[i][j]  << setw(7) << " ";
		}
		cout << endl;
	}
}

//  return the index of customers in tabIndex with the shortest distance with last_customers
int searchMinDistance(Data* data, std::vector<int> tabIndex, int last_customers) {
	int current_customers_index = 0;
	double minDistance = pow(2,10);
	for (int i = 0 ; i < tabIndex.size() ; i++) {
		if (data->distance[i][last_customers] < minDistance) {
			minDistance = data->distance[i][last_customers];
			current_customers_index = i;
		}
	}
	return current_customers_index;
}

// make the end of the current route and add a new route to the solution
void endRouteStartNew(Data* data, Solution* sol, int current_vehicle, int last_customers) {
	sol->capacity_rest.push_back(data->vehicleCapacity);
	sol->routes[current_vehicle].push_back(0);
	std::vector<int> ints;
	sol->routes.push_back(ints);
	sol->routes[current_vehicle+1].push_back(0);
	sol->cost += data->distance[last_customers][0];
}

// add a route with only the customers in parameter
void routeOneVehicle(Data* data, Solution* sol, int current_vehicle, int current_customers) {
	sol->capacity_rest.push_back(data->vehicleCapacity);
	sol->capacity_rest[current_vehicle] -= data->demand[current_customers];
	std::vector<int> ints;
	sol->routes.push_back(ints);
	sol->routes[current_vehicle].push_back(0);
	sol->routes[current_vehicle].push_back(current_customers);
	sol->routes[current_vehicle].push_back(0);
	// sol->routes[current_vehicle+1].push_back(0);
	sol->cost += (2*data->distance[current_customers][0]);
}

// add the customer to the current route
void addCustomerRoute(Data* data, Solution* sol, int current_vehicle, int current_customers, int last_customers) {
	sol->routes[current_vehicle].push_back(current_customers);
	sol->capacity_rest[current_vehicle] -= data->demand[current_customers];
	sol->cost += data->distance[last_customers][current_customers];
}

// print and return the mean distance between all customers
double distanceMean(Data* data, Solution* sol, double max) {
	double mean = 0;
	for (int i = 0 ; i < data->numberCustomers ; i++) {
		for (int j = 0 ; j < data->numberCustomers ; j++) {
			if (i != j) {
				mean += data->distance[j][i];
			}
		}
	}
	mean = mean / (data->numberCustomers * (data->numberCustomers-1));
	cout << "distanceMean " << mean << endl;
	return mean;
}

// print the vector in parameter
void printVector(vector <int> tab){
	for (int i = 0; i < tab.size(); i++) {
		cout << i << " " << tab[i] << endl;
	}
}

// make the echange between two customer
// change the demand and the distance with that
void makeEchange(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer, double distance) {
	sol->cost += distance;
	if(firstVehicle != secondVehicle) {
		int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];
		int indexSecondCustomer = sol->routes[secondVehicle][secondCustomer];

		int demandFirstCustomer = data->demand[indexFirstCustomer];
		int demandSecondCustomer = data->demand[indexSecondCustomer];

		sol->capacity_rest[firstVehicle] += (demandFirstCustomer-demandSecondCustomer);
		// sol->capacity_rest[firstVehicle] -= demandSecondCustomer;

		sol->capacity_rest[secondVehicle] += (demandSecondCustomer-demandFirstCustomer);
	}
	int tmp = sol->routes[firstVehicle][firstCustomer];
	sol->routes[firstVehicle][firstCustomer] = sol->routes[secondVehicle][secondCustomer];
	sol->routes[secondVehicle][secondCustomer] = tmp;
}

// make the move of one customer in the new position
// change the demand and the distance with that
void makeMove(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer, double distance) {
	sol->cost += distance;
	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];

	if(firstVehicle != secondVehicle) {
		int demandFirstCustomer = data->demand[indexFirstCustomer];

		sol->capacity_rest[firstVehicle] += demandFirstCustomer;
		sol->capacity_rest[secondVehicle] -= demandFirstCustomer;
	}

	sol->routes[secondVehicle].insert(sol->routes[secondVehicle].begin() + secondCustomer, indexFirstCustomer);

	if (firstVehicle == secondVehicle && secondCustomer < firstCustomer){
		sol->routes[firstVehicle].erase(sol->routes[firstVehicle].begin() + firstCustomer + 1);
	} else {
		sol->routes[firstVehicle].erase(sol->routes[firstVehicle].begin() + firstCustomer);
	}
}

// make the move of one customer in the new route/vehicle
// change the demand and the distance with that
void makeMoveNew(Data* data, Solution* sol, int firstVehicle, int firstCustomer, double distance) {
	sol->cost += distance;
	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];

	int demandFirstCustomer = data->demand[indexFirstCustomer];

	sol->capacity_rest.push_back(data->vehicleCapacity);
	std::vector<int> ints;
	sol->routes.push_back(ints);
	int secondVehicle = sol->routes.size()-1;
	sol->capacity_rest[firstVehicle] += demandFirstCustomer;
	sol->capacity_rest[secondVehicle] -= demandFirstCustomer;

	sol->routes[secondVehicle].push_back(0);
	sol->routes[secondVehicle].push_back(indexFirstCustomer);
	sol->routes[secondVehicle].push_back(0);

	sol->routes[firstVehicle].erase(sol->routes[firstVehicle].begin() + firstCustomer);
}

// make a verifictation if the demand and the cost in the solution was good
bool verification(Data* data, Solution* sol) {
	double distance = 0;
	bool ret = true;
	for (int vehicle = 0; vehicle < sol->routes.size() ; vehicle++) {
		int demand = 0;
		int lastCustomers = 0;
		for (int customer = 1; customer < sol->routes[vehicle].size() ; customer++) {
			int indexCustomer = sol->routes[vehicle][customer];
			demand += data->demand[indexCustomer];
			double tmp = data->distance[lastCustomers][indexCustomer];
			// cout << "distance "  << indexCustomer << " " << lastCustomers << " " << tmp << endl;
			distance += tmp;
			lastCustomers = indexCustomer;
		}
		if (demand != data->vehicleCapacity-sol->capacity_rest[vehicle]) {
			cout << "mauvaise demand " << demand;
			cout << " " << data->vehicleCapacity-sol->capacity_rest[vehicle];
			cout << " vehicle " << vehicle;
			cout << endl;
			ret = false;
		}
	}
	int arrondi = pow(10,4);
	// with the floatong calcul it's necessary to round before the comparaison
	if (round(distance*arrondi)/arrondi != round(sol->cost*arrondi)/arrondi) {
		cout << "mauvaise distance " << distance;
		cout << " " << sol->cost;
		cout << endl;
		ret = false;
	}
	if (ret) {
		cout << " Verification bonne" << endl;
		cout << endl;
		return true;
	}
	return false;
}

// print the solution in a file in the folder Solution
void printSolutionInFile(Data *data, Solution *sol) {

	char tempString[100];
	sprintf(tempString, "Solutions/%s_sol.txt", data->instanceName.c_str());
	ofstream solution(tempString, ios::out);

	solution << "Cost: " << setw(10) << fixed << setprecision(2) << sol->cost;
	solution << "\nRunning time: "<< setw(10) << sol->time << "\n\n";

	solution << "Solution: " << endl;
	for (int r = 0; r < sol->routes.size(); r++) {
		for (int i = 0; i < sol->routes[r].size(); i++) {
			solution << sol->routes[r][i] << setw(7);
		}
		solution << "\n";
	}
	solution.close();
}
