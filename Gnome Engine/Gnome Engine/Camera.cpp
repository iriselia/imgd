#include "StdAfx.h"
#include "Camera.h"
#include "curses.h"
#include "LinkedList.h"
#include "GraphicsManager.h"

Camera::Camera(Map* mark, int screenX, int screenY, int worldX, int worldY, int width, int height)
{
	realm = mark;
	this->screenX = screenX;
	this->screenY = screenY;
	this->worldX = worldX;
	this->worldY = worldY;
	this->height = height;
	this->width = width;
	graph = GraphicsManager::getInstance();
}


Camera::~Camera(void)
{
}

void Camera::drawMark()
{
	ColorChar stamp;
	for(int counter = 0; counter < height; counter++)
	{
		for(int counte = 0; counte < width; counte++)
		{
			stamp = realm->getColorChar(worldX + counte, worldY + counter);
			graph->drawChar(stamp.glyph, stamp.color, screenX + counte, screenY + counter);
		}
	}
}