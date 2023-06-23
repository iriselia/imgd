#include "StdAfx.h"
#include "ConnectXGame.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "MapManager.h"
#include "ConnectXBoard.h"

ConnectXGame::ConnectXGame(void)
{
}


ConnectXGame::~ConnectXGame(void)
{
}

void ConnectXGame::initialize(void)
{
	MapManager* map = MapManager::getInstance();
	GraphicsManager* graph = GraphicsManager::getInstance();
	Map* cXMap = new ConnectXBoard(WIDTH, HEIGHT, LENGTH);
	Camera* cam = new Camera(cXMap, 0, 0, -1, 0, WIDTH + 3, HEIGHT + 3);
	graph->insert(cam);
	map->activateMap(cXMap);
	map->registerForInput(cXMap);
}