#include "pch.h"
#include "ProcessInput.h"
#include <fstream>
#include <string>

using namespace std;

StringBuffer ProcessInput::BreakLine( string &inputStream) 
{
	string delimiter = "\r\n";
	string token;
	StringBuffer stringBuffer;
	string s = inputStream;
	size_t pos = 0;
	
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		if (token.length() > 0)
		{
			stringBuffer.emplace_back(token);
		}
		s.erase(0, pos + delimiter.length());
	}

	return stringBuffer;

}