#pragma once

class GraphicsManager;

#include "GraphicsManager.h"
#include "Map.h"

class Camera
{
public:
	Camera(Map* mark, int screenX=0, int screenY=0, int worldX=0, int worldY=0, int width=0, int height=0);
	~Camera(void);
	void drawMark();
private:
	int screenX; //upper left corner of region
	int screenY;
	int worldX;
	int worldY;
	int height;
	int width;
	Map* realm; //the map this camera is looking at
	GraphicsManager* graph;
};

