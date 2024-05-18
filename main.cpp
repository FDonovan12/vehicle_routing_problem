#include "readData.h"
#include "functions.h"
#include "heuristics.h"
#include "value.h"
#include <iostream>
#include <string>

using namespace std;

int main() {


		string instName = "vrpnc1.txt";
		Data data;
		readData(&data, instName);
		cout << "Instance: " << data.instanceName <<" "<<CLOCKS_PER_SEC<<" "<< endl;

		// int limitMin = 0;
		// int limitMax = 12;
		// int secondMax = 300;
		// makeTabTest (&data, limitMin, limitMax, secondMax);

		int constructor = 2;
		int limitValueMax = 8;
		int secondMax = 300;
		bool distanceFixed = true;
		allInstances(constructor, limitValueMax, secondMax, distanceFixed);
		// makeSolMin(&data, 0, limitValueMax, secondMax, false);
		// makeSolMin(&data, 1, limitValueMax, secondMax, false);
		// makeSolMin(&data, 2, limitValueMax, secondMax, false);

	return 0;
}
