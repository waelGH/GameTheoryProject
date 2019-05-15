// HedonicGameProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <iostream>

#include "stdafx.h"
#include "pch.h"
#include "RandomGame.h"
#include "PartitionClass.h"
#include "PartitionGenerator.h"
//#include "PrintingOut.h"
#include "SubsetClass.h"
#include <random> // Standard random number generator
#include <string> 

#include <chrono> //Standard clockfunctions
#include <ctime>
#include <cmath>  //standard math functions (could have said math.h instead)
typedef unsigned int MYINT;

int main(int argc, char *argv[])
{
	cout << "GitHub Update ";
	cout << "Command Arguments: " << argc << std::endl;
	for (int i = 0; i < argc; i++) {
		cout << "Arguments " << i << ": " << argv[i] << std::endl;
	}

	// 0 file location
	MYINT n = std::stoi(argv[1]);

	//MYINT n = 4;
	//MYINT RunNumber = 1;
	//MYINT NumRepeats = 10; //Number of seconds till stop

	MYINT RunNumber = std::stoi(argv[2]);
	MYINT NumRepeats = std::stoi(argv[3]); //Number of seconds till stop
	MYINT SubRunNumber = 10;

	if (n > 11) { n = 4; } //just in case make mistake

	// Set up time
	//update
	time_t t = time(0); //is a pointer really
	tm* myStartTime = new tm();
	//localtime(&t);    
	localtime_s(myStartTime, &t);

	// set up file name
	// location is C:\Users\ajcollin\source\repos\AndyMain\AndyMain
	std::string myFileName;
	myFileName = "run_" + to_string(RunNumber) + "_" + to_string(myStartTime->tm_mon + 1) +
		to_string(myStartTime->tm_mday) + to_string(myStartTime->tm_year + 1900) + "_" + to_string(myStartTime->tm_hour) + to_string(myStartTime->tm_min) + to_string(myStartTime->tm_sec) + ".txt";
	std::cout << "text file: " << myFileName << std::endl;

	std::ofstream myfile;
	myfile.open(myFileName);

	//Random number generator
	//unsigned mySeed = std::chrono::system_clock::now().time_since_epoch().count();
	//std::mt19937_64 myGen;
	//myGen.seed(mySeed);

	MyRandomGame* g;
	PartitionGenerator* p; //This actually finds those in the core

						   // Complete one iteration before main run
	g = new MyRandomGame(n);
	//Debugging
	//g->PrintOutArray(n);

	p = new PartitionGenerator(n, g, RunNumber, SubRunNumber, myfile); //This actually finds those partitions in the core
	SubRunNumber++;

	delete g; //Trying to clean up
	delete p; //Trying to clean up


	//*/
	int count = 0;
	while (difftime(time(0), t) < NumRepeats) {
		count++;
		std::cout << count << endl;

		g = new MyRandomGame(n);
		p = new PartitionGenerator(n, g, RunNumber, SubRunNumber, myfile); //This actually finds those in the core	
		SubRunNumber++;

		delete g; //Trying to clean up
		delete p; //Trying to clean up
	}
	///*/


	std::cout << "time past: " << difftime(std::time(0), t) << std::endl; //gives seconds since 1970

	myfile.close();
	delete myStartTime;


	// ???? cannot get this to work
	//_CrtDumpMemoryLeaks(); //for debugging
	//_CrtMemDumpAllObjectsSince(NULL);

	cout << "hello world";
	n = 10;
	cout << n;

	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
