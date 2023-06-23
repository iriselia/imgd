#include "StdAfx.h"
#include "MapManager.h"


MapManager::MapManager(void)
{
}


MapManager::~MapManager(void)
{
}

MapManager* MapManager::getInstance(void)
{
	static MapManager instance;
	return &instance;
}

void MapManager::startUp(void)
{
	log = LogManager::getInstance();
	activeMaps = new LinkedList<Map>();
	registeredForInput = new LinkedList<Map>();
	allMaps = new LinkedList<Map>();
	log->writeLog("MapManager: Started Up");
}

void MapManager::shutDown(void)
{
	allMaps->freeAll();
	log->writeLog("MapManager: Shut Down");
}

void MapManager::activateMap(Map* in)
{
	activeMaps->insert(in);
}

void MapManager::deactivateMap(Map* in)
{
	activeMaps->remove(in);
}

void MapManager::registerForInput(Map* in)
{
	registeredForInput->insert(in);
}

void MapManager::deregFromInput(Map* in)
{
	registeredForInput->remove(in);
}

void MapManager::introduceMap(Map* in)
{
	allMaps->insert(in);
}

void MapManager::terminateMap(Map* in)
{
	allMaps->remove(in);
}

void MapManager::sendInput(char in)
{
	forEach(Map, ptr, registeredForInput->getFirst())
	{
		ptr->first->takeInput(in);
	}
}

void MapManager::advanceActiveMaps(void)
{
	forEach(Map, ptr, activeMaps->getFirst())
	{
		ptr->first->step();
	}
}