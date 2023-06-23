#pragma once
#include "LogManager.h"
#include "MapManager.h"
#include "GraphicsManager.h"

class CycleManager
{
public:
	~CycleManager(void);
	static CycleManager* getInstance(void);
	void startUp(void);
	void shutDown(void);
	void runGame(void); //this starts up the game loop and must be terminated from within
	void endGame(void);

private:
	CycleManager(void);
	CycleManager(CycleManager const&);
    void operator=(CycleManager const&);
	bool isRunning;
	LogManager* log;
	MapManager* map;
	GraphicsManager* graph;
	stopWatch timer;
};

