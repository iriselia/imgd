#pragma once
#include "map.h"
#include "CycleManager.h"
#include "GraphicsManager.h"

class TestMap :
	public Map
{
public:
	TestMap(void);
	~TestMap(void);
	void takeInput(char in);
	void step(void);
	ColorChar getColorChar(int x, int y);
private:
	int animationCounter;
	ColorChar Icon;
	ColorChar Blank;
	CycleManager* cycle;
	GraphicsManager* graph;
};

