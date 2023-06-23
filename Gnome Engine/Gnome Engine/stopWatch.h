#pragma once

#include <Windows.h>

class stopWatch
{
public:
	stopWatch(void);
	~stopWatch(void);
	void startCounting(void);
	LARGE_INTEGER checkCount(void); //returns answer in milliseconds

private:
	private:
	LARGE_INTEGER startTime;
	LARGE_INTEGER currentTime;
	LONGLONG ticsPerMilliSec;
};

