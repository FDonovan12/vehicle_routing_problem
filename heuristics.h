#ifndef HEURISTICS_H_INCLUDED
#define HEURISTICS_H_INCLUDED

#include "value.h"
#include <time.h>
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include <limits>

void constructionNextFit(Data* data, Solution* sol);
void constructionBestDistance(Data* data, Solution* sol);
void constructionAllVehicle(Data* data, Solution* sol);
void oneImprove(Data* data, Solution* sol, int limitValue);
void manyImprove(Data* data, Solution* sol, Solution* solMin, int limitValueMax, int secondMax, bool distanceFixed, clock_t timeBefore);
void echange(Data* data, Solution* sol, int vehicle, int customer, int limitValue);
void move(Data* data, Solution* sol, int vehicle, int customer, int limitValue);
void makeTabTest (Data* data, int distanceMin, int distanceMax, int secondMax);
Solution makeSolMin(Data* data, int constructor, int limitValueMax, int secondMax, bool distanceFixed);
void allInstances(int constructor, int limitValueMax, int secondMax, bool distanceFixed);

#endif
