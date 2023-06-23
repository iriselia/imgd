#include "StdAfx.h"
#include "TestMap.h"
#include "curses.h"


TestMap::TestMap(void)
{
	cycle = CycleManager::getInstance();
	graph = GraphicsManager::getInstance();
	animationCounter = 30;
	Icon = ColorChar();
	Icon.glyph = 'a';
	Icon.color = 2;
	Blank = ColorChar();
	Blank.glyph = ' ';
	Blank.color = 0;
}


TestMap::~TestMap(void)
{
}

ColorChar TestMap::getColorChar(int x, int y)
{
	if(x == 0 && y == 0)
		return Icon;
	else
		return Blank;
}

void TestMap::step(void)
{
	if(--animationCounter < 1)
	{
		animationCounter = 30;
		Icon.color = 2;
	}
	if(animationCounter == 15)
		Icon.color = 1;
}

void TestMap::takeInput(char in)
{
	if(in != ERR)
		Icon.glyph = in;
	if(in == 'x')
		cycle->endGame();
}