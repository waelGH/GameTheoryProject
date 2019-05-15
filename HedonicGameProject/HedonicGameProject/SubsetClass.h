#pragma once
#pragma once
// File Name : subsetclass.
// Version: 1
// Description:  Used for converting binary representation of subset into integer version

#ifndef SUBSETCLASS_H
#define SUBSETCLASS_H

#include <cmath>  //standard math functions (could have said math.h instead)

typedef unsigned int MYINT;
using namespace std;

//*****************************************************************************
// The Main part of a file
//*****************************************************************************
class SubsetClass
{
public:
	SubsetClass();
	SubsetClass(MYINT);
	SubsetClass(MYINT, bool*);
	~SubsetClass();
	MYINT ConvertSubsetToInteger(MYINT, bool*);
	bool* ConvertIntegerToSubset(MYINT, MYINT);

	MYINT SubsetNum; //is the number the represents a subset

};


SubsetClass::SubsetClass() { SubsetNum = 0; }
SubsetClass::SubsetClass(MYINT n) { SubsetNum = n; }
SubsetClass::SubsetClass(MYINT m, bool* s) { ConvertSubsetToInteger(m, s); }
SubsetClass::~SubsetClass() {  }

//*****************************************************************************
// Converts a boolean subset to a integer
//*****************************************************************************
MYINT SubsetClass::ConvertSubsetToInteger(MYINT n, bool* s) {
	SubsetNum = 0;

	for (MYINT i = 0; i < n; i++) {
		if (s[i]) { SubsetNum += (MYINT)std::pow(2, i); }
	}

	return SubsetNum;
};

//*****************************************************************************
// Converts a boolean integer to a subset
//*****************************************************************************
bool* SubsetClass::ConvertIntegerToSubset(MYINT n, MYINT m) {
	SubsetNum = m;
	bool*  s = new bool[n];


	for (MYINT i = 0; i < n; i++) {

		if (m & (MYINT)std::pow(2, i)) { s[i] = 1; }
		else { s[i] = 0; }
	}

	return s;
	//AJC Need to delete where used
};

#endif  //End of Inclusion Guard