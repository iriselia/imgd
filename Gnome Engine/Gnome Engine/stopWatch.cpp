#include "StdAfx.h"
#include "stopWatch.h"


stopWatch::stopWatch(void)
{
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	ticsPerMilliSec = performanceFrequency.QuadPart / 1000;
}


stopWatch::~stopWatch(void)
{
}

void stopWatch::startCounting(void)
{
	QueryPerformanceCounter(&startTime);
}

LARGE_INTEGER stopWatch::checkCount(void)
{
	QueryPerformanceCounter(&currentTime);
	LARGE_INTEGER temp;
	temp.QuadPart = (currentTime.QuadPart - startTime.QuadPart)/ticsPerMilliSec;
	return temp;
}