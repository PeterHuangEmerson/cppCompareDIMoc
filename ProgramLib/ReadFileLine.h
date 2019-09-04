#pragma once

#include <windows.h>
#include "framework.h"
#include "ProcessInput.h"
#include "SelectLine.h"
#include"FileSystemStub.h"

class ReadFileLine
{
	IWindowsFileSystem& iWindowsFileSystem;
	IProcessInput &iProcessInput;
	ISelctLine &iSelectLine;
	
public:
	
	ReadFileLine(IProcessInput &_iProcessInput, ISelctLine & _iSelectLine, IWindowsFileSystem& _iWindowsFileSystem) :
		iProcessInput(_iProcessInput), iSelectLine(_iSelectLine), iWindowsFileSystem(_iWindowsFileSystem)
	{}
	virtual StringBuffer ReadFileAndReturnCertianLength(string directoryToSearch, int folderWithNumberofFiles);
};