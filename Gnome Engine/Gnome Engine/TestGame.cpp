#include "StdAfx.h"
#include "TestGame.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "MapManager.h"
#include "TestMap.h"

TestGame::TestGame(void)
{
}


TestGame::~TestGame(void)
{
}

void TestGame::initialize(void)
{
	MapManager* map = MapManager::getInstance();
	GraphicsManager* graph = GraphicsManager::getInstance();
	Map* testMap = new TestMap();
	Camera* cam = new Camera(testMap, 0, 0, -5, -5, 10, 10);
	graph->insert(cam);
	map->activateMap(testMap);
	map->registerForInput(testMap);
}