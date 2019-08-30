#pragma once

#include "framework.h"

class ISelctLine
{
public:
	ISelctLine() {}
	virtual ~ISelctLine() = default;

	ISelctLine(const ISelctLine&) = delete;
	ISelctLine(const ISelctLine&&) = delete;
	ISelctLine& operator=(const ISelctLine&) = delete;
	ISelctLine& operator=(const ISelctLine&&) = delete;

	virtual StringBuffer SelectLineByLength(StringBuffer& inputStringBuffer, unsigned int lengthToAccept) = 0;
};



class SelectLine final : public virtual ISelctLine
{
	const unsigned int MAX_LINE = 5;
public:
	
	SelectLine() { }
	StringBuffer SelectLineByLength(StringBuffer& inputStringBuffer, unsigned int lengthToAccept);
};

