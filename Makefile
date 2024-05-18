all: vrp

vrp: main.o readData.o functions.o heuristics.o value.o
	g++ -o vrp main.o readData.o functions.o heuristics.o value.o

main.o: main.cpp
	g++ -c main.cpp

readData.o: readData.cpp readData.h
	g++ -c readData.cpp

functions.o: functions.cpp functions.h
	g++ -c functions.cpp

value.o: value.cpp value.h
	g++ -c value.cpp

heuristics.o: heuristics.cpp heuristics.h
	g++ -c heuristics.cpp
