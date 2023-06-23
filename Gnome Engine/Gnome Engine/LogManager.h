#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "stopWatch.h"

using namespace std;

class LogManager
{
public:
	~LogManager(void);
	static LogManager* getInstance(void);
	void startUp(void);
	void shutDown(void);
	void writeLog(string message);

private:
	LogManager(void);
	LogManager(LogManager const&);
    void operator=(LogManager const&);

	ofstream log;
};

