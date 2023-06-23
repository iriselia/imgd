#include "StdAfx.h"
#include "GraphicsManager.h"
#include <curses.h>
#include "CurrentGameInfo.h"

GraphicsManager::GraphicsManager(void)
{
}


GraphicsManager::~GraphicsManager(void)
{
}

GraphicsManager* GraphicsManager::getInstance(void)
{
	static GraphicsManager instance;
	return &instance;
}

void GraphicsManager::startUp(void)
{
	log = LogManager::getInstance();
	map = MapManager::getInstance();

	initscr(); //start curses mode
	cbreak(); //don't buffer lines
	noecho(); //don't display characters as they're typed
	keypad(stdscr, TRUE); //allow the arrow keys to be read
	nodelay(stdscr, true); //don't wait for input
	start_color();
	COLORMAP //load colors from game file

	log->writeLog("GraphicsManager: Started Up");
}

void GraphicsManager::shutDown(void)
{
	endwin();
	freeAll();

	log->writeLog("GraphicsManager: Shut Down");
}

void GraphicsManager::updateScreen(void)
{
	clear();
	//draw everything
	forEach(Camera, ptr, getFirst())
	{
		ptr->first->drawMark();
	}
	refresh();
}

void GraphicsManager::drawChar(char shape, int color, int x, int y)
{
	attron(COLOR_PAIR(color));
	mvaddch(y, x, shape);
}

void GraphicsManager::insert(Camera* obj)
{
	LinkedList<Camera>::insert(obj);
}

void GraphicsManager::remove(Camera* obj)
{
	LinkedList<Camera>::remove(obj);
}

void GraphicsManager::freeAll(void)
{
	LinkedList<Camera>::freeAll();
}