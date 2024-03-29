#pragma Once
#include "pch.h"
#include <iostream>
#include "ReadFileLine.h"



StringBuffer ReadFileLine::ReadFileAndReturnCertianLength(string myfile, int lineLength)
{
	HANDLE hFile = iWindowsFileSystem.CreateFile(myfile.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	DWORD fileSizeHigh = 0;

	DWORD fileSizeLow = iWindowsFileSystem.GetFileSize(hFile, &fileSizeHigh);

	StringBuffer stringBuf;

	TestClass testClass(8);

	if (fileSizeHigh == 0 && hFile != INVALID_HANDLE_VALUE)
	{
		BOOL readSuccess;
		DWORD byteReading = fileSizeLow;
		char* readBuffer = new char[fileSizeLow];
		readBuffer[fileSizeLow] = 0;
	
		readSuccess = iWindowsFileSystem.ReadFile(hFile, readBuffer, byteReading, &byteReading, nullptr);

		if (readSuccess == TRUE)
		{
			string bufferString = readBuffer;
			StringBuffer breakLine = iProcessInput.BreakLine(bufferString);
			stringBuf = iSelectLine.SelectLineByLength(breakLine, lineLength);
		}
		cout << "-------------" << endl;
		for (unsigned int cnt = 0; cnt < stringBuf.size() ; cnt++)
		{
			cout << stringBuf[cnt] << endl;
		}
		cout << "-------------" << endl;
		iWindowsFileSystem.CloseHandle(hFile);

		iWindowsFileSystem.FakeAPIWithOutputByRef(hFile, testClass);
	}
	return stringBuf;
}