/*
Authors: Utku Yurter, Michael Figueroa, Puspa Giri, Ryan Hammerly
Course: CS2302 - Digital Logic
School: University of Houston - Downtown
Description: This program simulates a full adder digital circuit and adds two binary numbers.
Input: Binary number of any lenght
Output: Binary number representing the sum of two binary numbers
References: https://www.geeksforgeeks.org/full-adder-digital-electronics/
https://en.wikipedia.org/wiki/Adder_(electronics)
https://en.wikibooks.org/wiki/Digital_Electronics/Digital_Adder
http://www.cplusplus.com/reference/vector/vector/
*/


#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>


using namespace std;

void output_Names()
{
	cout << "Authors:" << endl;
	cout << "-Utku Yurter" << endl;
	cout << "-Michael Figueroa" << endl;
	cout << "-Puspa Giri" << endl;
	cout << "-Ryan Hamerly" << endl << endl;
	cout << "Full Adder" << endl;
	cout << "***********************************" << endl;
}


//Functions for simple boolean operations representing xor, and, or gates
bool f_xor(bool a, bool b) { //XOR ==> (A B') + (A'B)
	return a != b;
}

bool f_or(bool a, bool b) { //OR ==> (A + B) - (A * B)
	return a || b;
}

bool f_and(bool a, bool b) { //AND ==> (A * B)
	return a && b;
}


//Functions for calculating the sum and carry values
bool f_sum(bool a, bool b, bool c) {
	return f_xor(f_xor(a, b), c); //equivalent of logical expression  C-IN XOR (A XOR B)
}

bool f_carry(bool a, bool b, bool c) {
	return f_or((f_and(a, b)), (f_and(c, f_xor(a, b)))); //equivalent of logical expression  (A B) + (C-IN (A XOR B))
}

int max(int a, int b) { //return the maximum of two integers. Used for determining the longer vector size
	if (a > b)
		return a;
	else
		return b;
}

vector<bool> full_adder(vector<bool> a, vector<bool> b) {
	//temporary local vectors for operations
	vector<bool> tempA = a;
	vector<bool> tempB = b;
	int tempSize;
	//This if-else block ensures that both of the vectors are equal in size. It adds 0's to the shorter vector until sizes are equal
	if (max(tempA.size(), tempB.size()) == tempA.size()) {
		tempSize = tempA.size() - tempB.size();
		for (int i = 0; i < tempSize; i++) {
			tempB.insert(tempB.end(), false);
		}
	}
	else if (max(tempA.size(), tempB.size()) == tempB.size()) {
		tempSize = tempB.size() - tempA.size();
		for (int i = 0; i < tempSize; i++) {
			tempA.insert(tempA.end(), false);
		}
	}

	vector<bool> res; //result vector
	bool c_in = false; //initial carry is set to 0 since no operation has taken place yet
	bool sum;
	bool c_out;
	int lenght = max(a.size(), b.size());
	for (int i = 0; i < lenght; i++) { //For loop operates for each bit of the vectors starting from the beginning, and inserts the appropriate result into res
		sum = f_sum(tempA[i], tempB[i], c_in);
		c_out = f_carry(tempA[i], tempB[i], c_in);
		c_in = c_out; //value of carry in for the next loop is set equal to the value of carry out from current loop
		res.insert(res.begin(), sum);
	}
	if (c_in == true) //Checks the last carry value after all the operations are done. If there is a carry remaining, adds it to the beginning of res
		res.insert(res.begin(), true);
	return res;

}

//Function for input. Asks for binary number and stores the input in a string.
string askVector(int vectorNum) { //Value of vectorNum indicates which input (1st or 2nd) that we want to ask from user.
	string in = "";
	if (vectorNum == 1) {
		cout << "Input the first binary number: ";
		getline(cin, in);
	}
	if (vectorNum == 2) {
		cout << "Input the second binary number: ";
		getline(cin, in);
	}
	return in;
}

//Displays the elements of the vector to console
void display(const vector<bool> &vec, bool reverse) {
	int size = vec.size();
	int i;
	if (reverse) { //If the reverse oprion is provided as 1, prints the vector elements in reversed order
		for (i = size - 1; i >= 0; i--) {
			cout << vec[i];
		}
	}
	else { //Prints the vector elements in normal order
		for (i = 0; i < size; i++) {
			cout << vec[i];
		}
	}
}

//If the input contains illegal characters, this function lets the user know and calls the askVector() again to give user a chance to re-enter the values
string wrongInput(string a, int num) {
	cout << "\nThe following characters are invalid! : ";
	for (int i = 0; i < a.size(); i++) {
		cout << a[i];
		if (i < a.size() - 1)
			cout << " , ";
	}
	cout << endl << endl;
	return askVector(num); //Indicates either the 1st or 2nd input contained illegal characters and calls the askVector() with correct value
}

//Converts the strings stored in askVector() to vectors to be used in full_adder()
vector<bool> toVector(string s, int num) {
	vector<bool> temp;
	string trash = ""; //string that stores all the illegal characters in the provided input
	string corrected; //string that is returned from wrongInput(), which may or may not include illegal characters again

	for (int i = 0; i < s.size(); i++) { //Converts the individual characters in string to boolean values and inserts into vector
		if (s[i] == '0')
			temp.insert(temp.begin(), false);
		else if (s[i] == '1')
			temp.insert(temp.begin(), true);
		else  //If value != 1 or value != 0, append to trash since it is an illegal character
			trash += s[i];
	}
	if (trash.compare("")) { //If trash contains even a single illegal character, call wrongInput()
		corrected = wrongInput(trash, num);
		return toVector(corrected, num); //Recursively call toVector() until the input does not contain illegal characters
	}
	return temp;
}

int main() {

	output_Names();
	string response = "Y";

	do {	//main loop of the program
		vector<bool> a;
		vector<bool> b;
		a = toVector(askVector(1), 1); //askVector() for the first input and then convert that input from string to vector. Error checking takes place here too
		cout << endl;
		b = toVector(askVector(2), 2);
		cout << endl << "Result--->   ";
		display(a, 1); //Display the value of the first binary number in reversed because toVector() creates a reversed vector for convenience in the full_adder()
		cout << " + ";
		display(b, 1);
		cout << " = ";
		display(full_adder(a, b), 0);
		cout << endl << endl;
		cout << "Do you want to continue? (Y/y) : ";
		getline(cin, response);
		cout << "****************************************************" << endl;
	} while ((response == "Y") || response == "y");

	return 0;
}

