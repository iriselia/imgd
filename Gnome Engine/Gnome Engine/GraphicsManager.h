#pragma once

class Camera;

#include "LinkedList.h"
#include "Camera.h"
#include "LogManager.h"
#include "MapManager.h"

class GraphicsManager : public LinkedList<Camera>
{
friend class Camera; //let cameras draw characters

public:
	~GraphicsManager(void);
	static GraphicsManager* getInstance(void);
	void startUp(void);
	void shutDown(void);
	void updateScreen(void); //redraws the gameworld onto the screen
	void insert(Camera* obj);
	void remove(Camera* obj);
	void freeAll(void);

private:
	GraphicsManager(void);
	GraphicsManager(GraphicsManager const&);
    void operator=(GraphicsManager const&);
	void drawChar(char shape, int color, int x, int y);
	LogManager* log;
	MapManager* map;
};

