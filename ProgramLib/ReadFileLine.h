#pragma once

//#include <windows.h>
#include "framework.h"
#include "ProcessInput.h"
#include "SelectLine.h"
#include"FileSystemStub.h"
using namespace std;


class ReadFileLine
{
	IWindowsFileSystem& iWindowsFileSystem;
	IProcessInput &iProcessInput;
	ISelctLine &iSelectLine;
	
public:
	
	ReadFileLine(IProcessInput &_iProcessInput, ISelctLine & _iSelectLine, IWindowsFileSystem& _iWindowsFileSystem) :
		iProcessInput(_iProcessInput), iSelectLine(_iSelectLine), iWindowsFileSystem(_iWindowsFileSystem)
	{}
	virtual StringBuffer ReadFileAndResturnCertianLength(string directoryToSearch, int folderWithNumberofFiles);
};