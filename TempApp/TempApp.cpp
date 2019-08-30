// TempApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

class ObjectToTest
{
public:
	virtual void Testfunct(int value, LPVOID returnVal)
	{
		string val = to_string(value);
		strcpy_s((char*)returnVal,val.length()+1, val.c_str());
	}
};

class ObjectHolder
{
	friend class MockObjectHolderFixure;
public:
	ObjectToTest* objectToTest;
	void Create()
	{
		objectToTest = new ObjectToTest();
	}
	void RunTestFunct(int value, char* returnVal)
	{
		objectToTest->Testfunct(value,(LPVOID) returnVal);
		
	}
};



int main()
{
	char expected[1024] = { 0 };

	char* src = (char*) "abcde";

	strncpy_s(expected,  (const char*)src, strlen(src));
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
