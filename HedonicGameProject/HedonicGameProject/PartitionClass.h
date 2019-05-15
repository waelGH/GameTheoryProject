#pragma once
// File Name: PartitionClass.h
// Version: 1
// Description:  Create a place to put a partition
#ifndef PARTITIONCLASS_H
#define PARTITIONCLASS_H

#include "SubsetClass.h"
#include <fstream> // includes istream and ostream
#include <iostream> //include iostream

typedef unsigned int MYINT;
using namespace std;

//*****************************************************************************
// The Main part of a file
//*****************************************************************************
class PartitionClass
{
public:
	PartitionClass(MYINT);
	PartitionClass(MYINT, MYINT*);
	~PartitionClass();
	MYINT* myPartition;
	MYINT* myReward;
	MYINT mySize;
	MYINT myMax;

	bool PartitionChecker();
	void FindMaximum();
	SubsetClass** SplitIntoSubsets();

	bool FindPartitionValue(MyRandomGame*);
	bool PartitionValueCheck(MyRandomGame*, MYINT, MYINT, MYINT, std::ofstream&);


	void PrintOutPartition(std::ofstream&);
	void PrintOutReward(std::ofstream&);
	void PrintOutIndex(MYINT, MYINT, MYINT, std::ofstream&);

	//MYINT* FindSubSetSizeCount();

private:

};

//*****************************************************************************
// Constructor
//*****************************************************************************
PartitionClass::PartitionClass(MYINT n) {
	myPartition = new MYINT[n];
	mySize = n;
	myReward = new MYINT[mySize];
}

PartitionClass::PartitionClass(MYINT n, MYINT* m) {
	myPartition = new MYINT[n];
	mySize = n;
	for (MYINT i = 0; i < n; i++) { myPartition[i] = m[i]; }
	myReward = new MYINT[mySize];
}

//*****************************************************************************
// Destructor
//*****************************************************************************
PartitionClass::~PartitionClass() {
	//Debugging
	delete[] myPartition;
	//this->myPartition = NULL;

	//if (!(myReward == NULL)) {
	delete[] myReward;
	//this->myReward = NULL;
	//}


}

//*****************************************************************************
// Checks partition
//*****************************************************************************
bool PartitionClass::PartitionChecker() {
	bool b = true;
	if (myPartition[0] > 1) { b = false; }

	MYINT j = 1;

	for (MYINT i = 1; i < mySize; i++) {
		if (myPartition[i] == (j + 1)) { j++; }
		if (myPartition[i] > (j + 1)) { b = false; i = mySize; }
	}

	return b;
}

//*****************************************************************************
// Finds max value of partition
//*****************************************************************************
void PartitionClass::FindMaximum() {
	MYINT M = 0;
	for (MYINT i = 1; i < mySize; i++) {
		if (myPartition[i] > M) { M = myPartition[i]; }
	}
	myMax = M;
}

//*****************************************************************************
// Split a partition into it consituate subset (which is represented by a number)
//*****************************************************************************
SubsetClass** PartitionClass::SplitIntoSubsets() {
	FindMaximum();
	//This works
	SubsetClass** x = new SubsetClass*[myMax + 1];

	bool*  s = new bool[mySize];

	MYINT j = 0;
	MYINT i = 0;
	for (i = 0; i <= myMax; i++) {

		for (j = 0; j < mySize; j++) {
			if (myPartition[j] == i) { s[j] = 1; }
			else { s[j] = 0; }
		}
		x[i] = new SubsetClass(mySize, s);
	}
	delete[] s;

	return x;

}

//*****************************************************************************
// Find the value of each subset, then allociates those values
//*****************************************************************************
bool PartitionClass::FindPartitionValue(MyRandomGame* r) {
	MYINT i; MYINT j;
	bool isIRCL = true;

	if (r->CheckNumberPlayers(mySize)) {
		//SubsetClass** x = SplitIntoSubsets(); //find FindMaximum already in their

		//bring the function here
		//////////////////////////////////
		///////////////////////////////////

		FindMaximum();

		SubsetClass** x = new SubsetClass*[myMax + 1];

		bool*  s = new bool[mySize];

		MYINT j = 0;
		MYINT i = 0;
		for (i = 0; i <= myMax; i++) {

			for (j = 0; j < mySize; j++) {
				if (myPartition[j] == i) { s[j] = 1; }
				else { s[j] = 0; }
			}
			x[i] = new SubsetClass(mySize, s);
		}
		delete[] s;

		//////////////////////////////
		//////////////////////////////
		/////////////////////////

		//MYINT* myReward = new MYINT[mySize];

		for (i = 0; i < mySize; i++) { myReward[i] = 0; }

		for (j = 0; j <= myMax; j++) {
			bool* b = x[j]->ConvertIntegerToSubset(mySize, x[j]->SubsetNum);
			for (i = 0; i < mySize; i++) {
				if (b[i]) {
					if (myReward[i] == 0) {
						myReward[i] = r->GetReward(i, x[j]->SubsetNum);
						if (r->GetReward(mySize, x[j]->SubsetNum) == 1) {
							//so subset dominated, so can ignore;
							isIRCL = false;
							i = mySize;
							j = myMax;
						}
					}
					else {
						cout << "Error: Reward already allociated to player " << i << '\t';
						cout << myReward[i];
						cout << " implies an error with subset indexing" << '\n';
						cout << "IN: PartitionClass::FindPartitionValue" << '\n';
					}
				}
			}
			delete[] b;
		}


		//delete myReward

		//DEDUGGING since x is a **, need to delete the individuals ones also
		for (i = 0; i < myMax + 1; i++) {
			delete x[i];                     //AJC not array but subset class
		}
		//delete[] x;
		delete[] x;
		//x = NULL;

	}
	else {
		cout << "Error: Number of players wrong when matching game to a partition" << '\n';
		cout << "IN: PartitionClass::FindPartitionValue" << '\n';
	}

	return isIRCL;
}

//*****************************************************************************
// Compares a partition to the subsets to see if dominated
//*****************************************************************************
bool PartitionClass::PartitionValueCheck(MyRandomGame* g, MYINT RunNumber, MYINT SubRunNumber, MYINT coreCount, std::ofstream& myfile) {


	bool b = FindPartitionValue(g);
	//bool b = false;

	if (b) {
		//because know that partition is dominated by singleton
		MYINT s;
		MYINT i;
		bool c = true;
		MYINT numSubsets = (MYINT)std::pow(2.0, (double)mySize);

		// Singleton Case first
		for (i = 0; i < mySize; i++) {
			if (g->GetReward(i, 0) > myReward[i]) {
				b = false; i = mySize;
			}
		}
		// ??? of for rest
		if (b) {
			for (s = 1; s < numSubsets; s++) {
				c = true;
				for (i = 0; i < mySize; i++) {
					if (g->GetReward(i, s) > 0) { //Makes sure in subset
						if (g->GetReward(i, s) <= myReward[i]) {
							c = false; //know that subset not dominate
							i = mySize; // so no point checking rest
						}
					}
				}
				if (c) {
					b = false; s = numSubsets;
				}
			}
		}

		if (b) {
			PrintOutIndex(RunNumber, SubRunNumber, coreCount + 1, myfile);
			PrintOutPartition(myfile);
			PrintOutReward(myfile);
			//	cout << '\n';
			myfile << '\n';
		}
	}
	return b;
}

//*****************************************************************************
void PartitionClass::PrintOutIndex(MYINT RunNumber, MYINT SubRunNumber, MYINT coreCount, std::ofstream& myfile) {
	//	cout << RunNumber << '\t';
	//	cout << SubRunNumber << '\t';
	//	cout << coreCount << '\t';
	//	cout <<  mySize << '\t';

	myfile << RunNumber << '\t';
	myfile << SubRunNumber << '\t';
	myfile << coreCount << '\t';
	myfile << mySize << '\t';
}

//*****************************************************************************
void PartitionClass::PrintOutPartition(std::ofstream& myfile) {

	for (MYINT i = 0; i < mySize; i++) {
		//	cout << myPartition[i] << '\t';
		myfile << myPartition[i] << '\t';
	}
}

void PartitionClass::PrintOutReward(std::ofstream& myfile) {
	for (MYINT i = 0; i < mySize; i++) {
		//	cout << myReward[i] << '\t';
		myfile << myReward[i] << '\t';
	}
}

//*****************************************************************************
// Finds max value of partition
//*****************************************************************************
/* MYINT* PartitionClass::FindSubSetSizeCount() {

MYINT* x = new MYINT[myMax + 1]; //find FindMaximum alredy in their
MYINT j = 0;
for (j = 0; j <= myMax; j++) {x[j] = 0;}

for (j = 0; j <= myMax; j++) { x[myPartition[j]]++; }
return x;
//DEBUDDING not actually used anywhere
}
*/
#endif  //End of Inclusion Guard

