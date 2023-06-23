#pragma once
#include "LinkedList.h"
#include "Map.h"
#include "LogManager.h"

class MapManager
{
public:
	~MapManager(void);
	static MapManager* getInstance(void);
	void startUp(void);
	void shutDown(void);
	void activateMap(Map* in);
	void deactivateMap(Map* in);
	void registerForInput(Map* in);
	void deregFromInput(Map* in);
	void introduceMap(Map* in);
	void terminateMap(Map* in);
	void sendInput(char in);
	void advanceActiveMaps(void);
private:
	MapManager(void);
	MapManager(MapManager const&);
    void operator=(MapManager const&);
	LinkedList<Map>* activeMaps; //this will usually be only one map
	LinkedList<Map>* registeredForInput; //this will usually be that one active map
	LinkedList<Map>* allMaps;
	LogManager* log;
};

