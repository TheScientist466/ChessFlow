#pragma once

#include <string>
#include "../../extlibs/tiny-process-library/process.hpp"

class Engines {

public:
	Engines(std::string path);
	~Engines();

public:
	std::string processStdOut;
	std::string processStdErr;
	std::string processStdOutLast;
	std::string processStdErrLast;

private:
	TinyProcessLib::Process* engineProcess;

public:
	void loadEngine(std::string path);
	void writeToStdIn(std::string toWrite);
	std::string readFromStdOut();
	std::string readFromStdErr();

	void goDepth(int depth = 20);
};