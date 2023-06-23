#include "StdAfx.h"
#include "LogManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <iomanip>

using namespace std;

LogManager::LogManager(void)
{
}

LogManager::~LogManager(void)
{
}

LogManager* LogManager::getInstance(void)
{
	static LogManager instance;
	return &instance;
}

void LogManager::startUp(void)
{
	//open the log file
	log.open("log.log");
	writeLog("LogManager: Started Up");
}

void LogManager::shutDown(void)
{
	//close the log file
	writeLog("LogManager: Shut Down");
	log.close();
}

void LogManager::writeLog(string message)
{
	SYSTEMTIME temp;
	GetSystemTime(&temp);
	log << setfill('0') << temp.wYear << ':' << setw(2) << temp.wMonth << ':' << setw(2) << temp.wDay << ':' << setw(2) << temp.wHour << ':' << setw(2) << temp.wMinute << ':' << setw(2) << temp.wSecond << "::" << message << endl;
}