#ifndef VALUE_H_INCLUDED
#define VALUE_H_INCLUDED

#include "functions.h"
#include <time.h>
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include <limits>

double echangeDistanceValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer);
bool echangeDemandValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer);
double moveDistanceValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer, int secondCustomer);
double moveNewDistanceValue(Data* data, Solution* sol, int firstVehicle, int firstCustomer);
bool moveDemandValue(Data* data, Solution* sol, int firstVehicle, int secondVehicle, int firstCustomer);

#endif
