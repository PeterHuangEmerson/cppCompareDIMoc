// ProgramApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include "framework.h"
#include "ProcessInput.h"
#include "SelectLine.h"
#include "ReadFileLine.h"
#include "FilesystemStub.h"
int main()
{
	ProcessInput processInput;
	SelectLine selectLine;
	FileSystemOperation fileSystemOperation;
	try
	{
		ReadFileLine readFileLine(processInput, selectLine, fileSystemOperation);
		string path = "d:\\temp\\test\\readme.txt";
		int length = 20;
		readFileLine.ReadFileAndResturnCertianLength(path, 40);
	}
	catch (exception e)
	{
		printf("exception occured");
	}
}

