#pragma once
// File Name: RandomGame.h
// Version: 1
// Description:  Creates a random game for analysis
#ifndef RandomGame_H
#define RandomGame_H

#include <fstream> 
#include <iostream> 
#include <string> 
#include <stdio.h>
#include <random> // Standard random number generator
#include <chrono> //Standard clockfunctions
#include <cmath>  //standard math functions (could have said math.h instead)
#include<algorithm>

typedef unsigned int MYINT;

//*****************************************************************************
// Class definition
//*****************************************************************************
class MyRandomGame
{
public:
	MyRandomGame(MYINT); // Constructors
	MyRandomGame(std::string);
	~MyRandomGame();

	MYINT GetReward(MYINT n, MYINT s) { return myRewardArray[n][s]; };

	void PrintOutArray(MYINT);

	//	MYINT ConvertSubsetToInteger(MYINT, bool*);
	//	bool* ConvertIntegerToSubset(MYINT, MYINT);

	void SaveGame(MYINT, std::string);
	int ReadInGame(std::string); //returns number of players

	bool CheckNumberPlayers(MYINT n) { if (n == numPlayers) { return 1; } else { return 0; } }

private:
	MYINT** myRewardArray;
	MYINT numPlayers;
	bool SingletonFirstRow;
	MYINT PositionFinder(MYINT, MYINT);
	void SingletonRewardSubstitution(MYINT);
	void SingletonRewardSubsetCheck(MYINT);
};

//*****************************************************************************
// Constructors
//*****************************************************************************
MyRandomGame::MyRandomGame(MYINT n)
{
	// index variables
	MYINT i;
	MYINT j;
	numPlayers = n;
	SingletonFirstRow = false;

	//Random number generator
	//N.B. Looks like if you pass a random generator, causes problems
	unsigned mySeed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937_64 myGen;
	myGen.seed(mySeed);

	//--------------------------------------------
	// First creates the main array
	//--------------------------------------------
	// n column is binary for the checked
	myRewardArray = new MYINT*[n + 1];
	const MYINT intNumSubsets = (MYINT)std::pow(2.0, (double)n);
	for (i = 0; i < n + 1; i++) {
		myRewardArray[i] = new MYINT[intNumSubsets];
	}

	for (i = 0; i < n + 1; i++) {
		for (j = 0; j < intNumSubsets; j++) {
			myRewardArray[i][j] = (MYINT)0; //initialize
		}
	}

	//--------------------------------------------
	// Then makes reward for each player
	//--------------------------------------------
	const MYINT intNumHalfSubsets = (MYINT)(intNumSubsets / 2.0);


	for (MYINT a = 0; a < n; a++) {
		MYINT* myArray = new MYINT[intNumHalfSubsets];
		for (j = 0; j < intNumHalfSubsets; j++) {
			// Need start at Reward "1" so clear
			myArray[j] = j + 1;
		}

		//Remember looking for "a U {subset}", so 0 refers to {a}
		// N.B. a cool way to reference array, the "+" add to the pointer address by size of object stored
		shuffle(myArray, myArray + intNumHalfSubsets, myGen);

		//--------------------------------------------
		// Add values to Main matrix
		//--------------------------------------------
		for (j = 0; j < intNumHalfSubsets; j++) {
			myRewardArray[a][PositionFinder(a, j)] = myArray[j];
		}

		//DEBUGGING
		//for (j = 0; j <= intNumHalfSubsets; j++) {
		//myArray[j] = 0;
		//}
		// AJC now create a new one each time
		delete[] myArray;
	}

	SingletonRewardSubstitution(n); //puts singleton values in first row
	SingletonRewardSubsetCheck(n);

	//DEBUGGING
	//PrintOutArray(n);

	// delete myArray; 
	//std::cout << "DEBUG: GOT HERE" << std::endl;
	//PrintOutArray(n);

}

// Constructor - read in string name
MyRandomGame::MyRandomGame(std::string s) { numPlayers = ReadInGame(s); }

//*****************************************************************************
// Destructor
//*****************************************************************************
MyRandomGame::~MyRandomGame() {
	// ??? need delete pointers as well

	for (int i = 0; i < numPlayers + 1; i++)
	{
		delete[] myRewardArray[i];
	}
	delete[] myRewardArray;
}

//*****************************************************************************
// Position finder for a game
//*****************************************************************************
// For a given subset with agent s excluded, this function determines the integer
// value of the subset when s is included
MYINT MyRandomGame::PositionFinder(MYINT a, MYINT s) {
	// Assume that s already in 2^(n-1) format

	// so put agent into position
	MYINT k = (MYINT)std::pow(2.0, (double)a) - 1;

	//This is XOR function ^
	MYINT d = ~(~s | ~k); //gets all the bits before the position
	MYINT b1 = (~(~s | k)); //get all bits after
	MYINT b = b1 << 1; //shifts all the after stuff one space
	return d + b + k + 1; // should give the rest
						  // N.B. We do NOT minus one because the first value is the empty set, which should always be zero
}

//*****************************************************************************
// Prints out the matrix representing the payoff
//*****************************************************************************
void MyRandomGame::PrintOutArray(MYINT n) {
	// index variables
	MYINT a;
	MYINT j;

	MYINT intNumSubsets = (MYINT)std::pow(2.0, (double)n);

	std::cout << " \t";
	for (a = 0; a < n; a++) {
		std::cout << a << "\t";
	}
	std::cout << "IR" << "\t";
	std::cout << std::endl;

	for (j = 0; j < intNumSubsets; j++) {
		std::cout << j << "\t";
		for (a = 0; a <= n; a++) {
			std::cout << myRewardArray[a][j] << "\t";
		}
		std::cout << std::endl;
	}
}

/* IN SUBSET CLASS NOW
//*****************************************************************************
// Converts a boolean subset to a integer
//*****************************************************************************
MYINT MyRandomGame::ConvertSubsetToInteger(MYINT n, bool* s) {
MYINT m = 0;

for (MYINT i = 0; i < n; i++) {
if (s[i]){ m += (MYINT) std::pow(2, i); }
}
return m;
}

//*****************************************************************************
// Converts a boolean integer to a subset
//*****************************************************************************
bool* MyRandomGame::ConvertIntegerToSubset(MYINT n, MYINT m) {
bool*  s = new bool[n];

for (MYINT i = 0; i < n; i++) {
if (m & (MYINT)std::pow(2, i)) {s[i] = 1;}
else { s[i] = 0; }
}

return s;
}
*/

//*****************************************************************************
// Substituates the first line of reward matrix with singleton values
//*****************************************************************************
void MyRandomGame::SingletonRewardSubstitution(MYINT n) {

	for (MYINT i = 0; i < n; i++) {
		myRewardArray[i][0] = myRewardArray[i][(MYINT)std::pow(2, i)];
	}
	SingletonFirstRow = true;
}

//*****************************************************************************
// Does a check to see if subset not individually rational
//*****************************************************************************
void MyRandomGame::SingletonRewardSubsetCheck(MYINT n) {
	if (!SingletonFirstRow) { SingletonRewardSubstitution(n); }
	MYINT intNumSubsets = (MYINT)std::pow(2.0, (double)n);

	for (MYINT s = 1; s < intNumSubsets; s++) {
		for (MYINT i = 0; i < n; i++) {
			if (myRewardArray[i][s] > 0) {
				if (myRewardArray[i][0] > myRewardArray[i][s]) {
					myRewardArray[n][s] = 1;
					i = n;
				}
			}
		}
	}

}

//*****************************************************************************
// Saves the game to a file
//*****************************************************************************
// ?? not to check works with extra column
void MyRandomGame::SaveGame(MYINT n, std::string myFileName) {

	std::ofstream myfile;
	myfile.open(myFileName + ".txt");

	MYINT a;
	MYINT j;

	MYINT intNumSubsets = (MYINT)std::pow(2.0, (double)n);

	myfile << " \t";
	for (a = 0; a < n; a++) {
		myfile << a << "\t";
	}
	myfile << std::endl;

	for (j = 0; j < intNumSubsets; j++) {
		myfile << j << "\t";
		for (a = 0; a < n; a++) {
			myfile << myRewardArray[a][j] << "\t";
		}
		myfile << std::endl;
	}
	myfile.close();
}

//*****************************************************************************
// ReadIn a game to a file
//*****************************************************************************
// ??? not sure if working or not
// ??? need to change for extra column now
int MyRandomGame::ReadInGame(std::string myFileName) {

	std::ifstream myfile2;
	myfile2.open(myFileName + ".txt");
	std::string myline;
	MYINT n = 0;

	//First need to determine number of players, this is the last number +1 of first row
	if (myfile2.is_open())
	{
		std::cout << "file open" << '\n';
		// Need to read in
		std::getline(myfile2, myline);
		std::cout << myline << '\n';

		char c;
		MYINT i;
		MYINT l = myline.length();
		std::string strLastNumber;

		//First of all, we work out the last number
		for (i = 0; i < (l - 1); i++) {
			c = myline.at(i); //last element is a return \n
							  //std::cout << c << '\n';
			if (c == '\n') { i = l; }
			else {
				if (c == '\t') { strLastNumber = ""; }
				else { strLastNumber += c; }
			}
		}

		n = std::stoi(strLastNumber, 0, 0) + 1;
		std::cout << n << '\n';

		MYINT a = 0;
		MYINT j = 0;
		std::string strNumber = "";

		//--------------------------------------------
		// First creates the main array
		//--------------------------------------------
		delete myRewardArray;

		myRewardArray = new MYINT*[n];
		const MYINT intNumSubsets = (MYINT)std::pow(2.0, (double)n);
		for (i = 0; i < n; i++) {
			myRewardArray[i] = new MYINT[intNumSubsets];
		}
		std::cout << intNumSubsets << '\n';

		while (getline(myfile2, myline)) //is a member of <string>
		{
			l = myline.length();
			a = 0;
			for (i = 0; i < l; i++) {
				c = myline.at(i); //last element is a return \n
				if (c == '\n') { i = l; }
				if ((c == '\t') || (c == '\n')) {
					if (a > 0) {//Ignore the first case
						myRewardArray[a - 1][j] = std::stoi(strNumber, 0, 0);
						std::cout << myRewardArray[a - 1][j] << '\t';
					}
					strNumber = "";
					a++;
				}
				else { strNumber += c; }
			}
			std::cout << '\n';
			j++;
		}
		std::cout << j << '\n';
	}
	myfile2.close();
	return n;
}

#endif  //End of Inclusion Guard
