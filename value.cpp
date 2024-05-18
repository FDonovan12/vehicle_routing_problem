#include "value.h"

// return the gain distance if an echange bewteen two customer is make
double echangeDistanceValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer) {
	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];
	int indexSecondCustomer = sol->routes[secondVehicle][secondCustomer];

	if ((indexFirstCustomer == 0 || indexSecondCustomer == 0)||(firstVehicle > secondVehicle)||((firstVehicle == secondVehicle)&&(firstCustomer >= secondCustomer))){
		// cout << "error echangeDistanceValue " << endl;
		return pow(2,10);
	}
	double distanceValue = 0;
	if (!(firstVehicle == secondVehicle && secondCustomer - firstCustomer == 1)){
		int afterFirstCustomer = sol->routes[firstVehicle][firstCustomer+1];
		distanceValue -= data->distance[indexFirstCustomer][afterFirstCustomer];
		distanceValue += data->distance[indexSecondCustomer][afterFirstCustomer];

		int beforeSecondCustomer = sol->routes[secondVehicle][secondCustomer-1];
		distanceValue -= data->distance[indexSecondCustomer][beforeSecondCustomer];
		distanceValue += data->distance[indexFirstCustomer][beforeSecondCustomer];
	}
	int beforeFirstCustomer = sol->routes[firstVehicle][firstCustomer-1];
	distanceValue -= data->distance[indexFirstCustomer][beforeFirstCustomer];
	distanceValue += data->distance[indexSecondCustomer][beforeFirstCustomer];

	int afterSecondCustomer = sol->routes[secondVehicle][secondCustomer+1];
	distanceValue -= data->distance[indexSecondCustomer][afterSecondCustomer];
	distanceValue += data->distance[indexFirstCustomer][afterSecondCustomer];

	return distanceValue;
}

// return if an echnage bewteen two customer can be make
bool echangeDemandValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer) {

	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];
	int indexSecondCustomer = sol->routes[secondVehicle][secondCustomer];

	if (indexFirstCustomer == 0 || indexSecondCustomer == 0){
		// cout << "error echangeDemandValue " << endl;
		return false;
	}
	int demandFirstCustomer = data->demand[indexFirstCustomer];
	int demandSecondCustomer = data->demand[indexSecondCustomer];
	int newFirstCapacity = sol->capacity_rest[firstVehicle] + demandFirstCustomer - demandSecondCustomer;
	int newSecondCapacity = sol->capacity_rest[secondVehicle] - demandFirstCustomer + demandSecondCustomer;

	if (newFirstCapacity >= 0 && newSecondCapacity >= 0){
		return true;
	}
	return false;
}

// return the gain distance if a depalcement is make
double moveDistanceValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer) {
	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];
	int indexSecondCustomer = sol->routes[secondVehicle][secondCustomer];

	if ((indexFirstCustomer == 0 || indexSecondCustomer == 0)||((firstVehicle == secondVehicle)&&(secondCustomer - firstCustomer == 1))){
		// cout << "error moveDistanceValue " << endl;
		return pow(2,10);
	}
	double distanceValue = 0;

	int afterFirstCustomer = sol->routes[firstVehicle][firstCustomer+1];
	distanceValue -= data->distance[indexFirstCustomer][afterFirstCustomer];

	int beforeSecondCustomer = sol->routes[secondVehicle][secondCustomer-1];
	distanceValue += data->distance[indexFirstCustomer][beforeSecondCustomer];
	distanceValue -= data->distance[indexSecondCustomer][beforeSecondCustomer];

	int beforeFirstCustomer = sol->routes[firstVehicle][firstCustomer-1];
	if (firstVehicle == secondVehicle && secondCustomer - firstCustomer == -1){
		distanceValue += data->distance[indexSecondCustomer][afterFirstCustomer];
	} else {
		distanceValue -= data->distance[indexFirstCustomer][beforeFirstCustomer];
		distanceValue += data->distance[indexFirstCustomer][indexSecondCustomer];
		if (afterFirstCustomer != beforeFirstCustomer) {
			distanceValue += data->distance[afterFirstCustomer][beforeFirstCustomer];
		}
	}
	return distanceValue;
}

// return if an depalcement can be make
bool moveDemandValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer) {
	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];

	if (indexFirstCustomer == 0){
		// cout << "error moveDemandValue" << endl;
		return false;
	}
	int demandFirstCustomer = data->demand[indexFirstCustomer];

	if (demandFirstCustomer <= sol->capacity_rest[secondVehicle] || firstVehicle == secondVehicle){
		return true;
	}
	return false;
}

// return the gain distance if a depalcement in a new route/vehicle is make
double moveNewDistanceValue(Data* data, Solution* sol, int firstVehicle, int firstCustomer) {
	int indexFirstCustomer = sol->routes[firstVehicle][firstCustomer];

	if (indexFirstCustomer == 0){
		// cout << "error moveNewDistanceValue " << endl;
		return pow(2,10);
	}
	double distanceValue = 0;

	int afterFirstCustomer = sol->routes[firstVehicle][firstCustomer+1];
	distanceValue -= data->distance[indexFirstCustomer][afterFirstCustomer];

	int beforeFirstCustomer = sol->routes[firstVehicle][firstCustomer-1];
	distanceValue -= data->distance[indexFirstCustomer][beforeFirstCustomer];

	distanceValue += (data->distance[indexFirstCustomer][0]);
	distanceValue += (data->distance[indexFirstCustomer][0]);
	distanceValue += (data->distance[beforeFirstCustomer][afterFirstCustomer]);

	return distanceValue;
}
