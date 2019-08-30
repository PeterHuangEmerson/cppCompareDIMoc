#pragma once
#include <string>


class IProcessInput
{
public:
	virtual ~IProcessInput() = default;
	IProcessInput() {}
	IProcessInput(const IProcessInput&) = delete;
	IProcessInput(const IProcessInput&&) = delete;
	IProcessInput& operator=(const IProcessInput&) = delete;
	IProcessInput& operator=(const IProcessInput&&) = delete;


	virtual StringBuffer BreakLine(
			_In_    string &inputline
	) = 0;
};


class ProcessInput final : public virtual IProcessInput
{
public:
	ProcessInput() {}
	StringBuffer BreakLine(string& inputline)override;

};

