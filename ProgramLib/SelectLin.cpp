#include "pch.h"
#include "SelectLine.h"


StringBuffer SelectLine::SelectLineByLength(StringBuffer& inputStringBuffer, unsigned int lengthToAccept)
{
	StringBuffer processedBuffer;
	unsigned int lineCnt = 0;
	for (unsigned int cnt = 0; cnt < inputStringBuffer.size(); cnt++)
	{
		if (inputStringBuffer[cnt].length() >= lengthToAccept)
		{
			string len = to_string(inputStringBuffer[cnt].length());
			string dash = "-";
			string total = inputStringBuffer[cnt] + dash + len;
			processedBuffer.push_back(total);
			if (++lineCnt >= MAX_LINE) break;
		}
		
	}
	return processedBuffer;
}

