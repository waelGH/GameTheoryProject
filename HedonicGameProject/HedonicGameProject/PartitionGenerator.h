// File Name: PartitionGenerator.h
// Version: 1
// Description:  Creates all the partitions 
#ifndef PARTITIONGENERATOR_H
#define PARTITIONGENERATOR_H


#include <fstream> // includes istream and ostream
#include <string> 
#include <stdio.h>
#include <random>
#include <chrono> //Standard clockfunctions
#include <cmath>  //standard math functions (could have said math.h instead)

#include "PartitionClass.h"
#include <vector> 

typedef unsigned int MYINT;
using namespace std;

//*****************************************************************************
// The Main part of a file
//*****************************************************************************
class PartitionGenerator
{
public:
	PartitionGenerator(MYINT, MyRandomGame*, MYINT, MYINT, std::ofstream&);
	~PartitionGenerator() ;
private:


};

PartitionGenerator::PartitionGenerator(MYINT myAgentNum, MyRandomGame* g, MYINT RunNumber, MYINT SubRunNumber, std::ofstream& myfile) {
	// N.B. starts at index c[0] not c[1]
	short n = (short)myAgentNum; //note: above 30 is not feasible

	// Setup varibles
	short r = 1;
	short j = 0;
	short i;
	MYINT coreCount = 0;

	short* c = new short[n + 1];
	short* b = new short[n + 1];
	PartitionClass* p = new PartitionClass(n);

	for (i = 0; i < n; i++) { c[i] = 0; b[i] = 0; }
	c[n] = 1; //DEBUGGING note that only went up to "n" in old version, past the post error.

	c[0] = 1;
	b[0] = 1;

	int CoreSize = 0;

	do {
		while (r < (n - 1)) {
			r++;
			c[r - 1] = 1;
			j++;
			b[j] = r;
		}

		for (i = 1; i <= (n - j); i++) {
			c[n - 1] = i;
			
			for (short k = 0; k < n; k++) {
				//myfile << c[k] << '\t';
				//std::cout << c[k] << '\t';
				//cout << "n=" << n << endl;
				//cout << "k=" << k << endl;
				//cout << "c[k]="<< c[k]<<endl;
				//MYINT tt = (MYINT)&c[k];
				//cout << "tt=" << c[k] << endl;
				//cout << endl << endl;

				p->myPartition[k] = (MYINT)c[k];
			}

			//for (short k = 0; k < n; k++) {
			//	cout << p->myPartition[k] << "	";
			//}
			//cout << endl;

			if (p->PartitionValueCheck(g, RunNumber, SubRunNumber, coreCount, myfile)) {
				coreCount++;
			}

		}
		r = b[j];
		c[r - 1]++;
		if (c[r - 1] > (r - j)) { j--; }
		//checker to stop
	} while (r != 1);


	//prints outline if core not found
	if (!coreCount) {
		p->PrintOutIndex(RunNumber, SubRunNumber, coreCount, myfile);
		//	cout << endl; 
		myfile << endl;
	}

	//DEBUGGING
	//So here is the problem, believe that p and c are pointing to same memory space
   //p->myPartition = NULL;

	delete[] b;
	//b = NULL;
	delete p;
	//p = NULL;
	delete[] c;
	//c = NULL;

}
#endif  //End of Inclusion Guard

//*****************************************************************************
// Destructor
//*****************************************************************************
PartitionGenerator::~PartitionGenerator(){

}

