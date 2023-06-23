#include "StdAfx.h"
#include "CycleManager.h"
#include "curses.h"

CycleManager::CycleManager(void)
{
}


CycleManager::~CycleManager(void)
{
}

CycleManager* CycleManager::getInstance(void)
{
	static CycleManager instance;
	return &instance;
}

void CycleManager::startUp(void)
{
	log = LogManager::getInstance();
	map = MapManager::getInstance();
	graph = GraphicsManager::getInstance();
	isRunning = false;
	timer = stopWatch();
	log->writeLog("CycleManager: Started Up");
}

void CycleManager::shutDown(void)
{
	log->writeLog("CycleManager: Shut Down");
}

void CycleManager::runGame(void)
{
	isRunning = true;
	int inputCountDown = 0; //don't take input for 10 tics after getting input
	int timeLeft;
	char in;
	while(isRunning)
	{
		//start stopwatch
		timer.startCounting();
		//take user input
		if(inputCountDown < 1)
		{
			in = getch();
			if(in != ERR) //don't send messages for no input
			{
				map->sendInput(in);
				timeLeft = 10;
			}
		}
		else
			inputCountDown -= 1;
		//advance game state
		map->advanceActiveMaps();
		//render game world
		graph->updateScreen();
		//wait until 1/30 sec
		timeLeft = 33 - timer.checkCount().QuadPart;
		if(timeLeft > 0)
			Sleep(timeLeft);
	}
}

void CycleManager::endGame(void)
{
	isRunning = false;
}