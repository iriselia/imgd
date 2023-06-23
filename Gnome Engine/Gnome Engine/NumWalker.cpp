#include "StdAfx.h"
#include "NumWalker.h"
#include <algorithm>

using namespace std;

NumWalker::NumWalker(void)
{
	counter = 0;
	face = 'N';
	skin = 6;
}


NumWalker::~NumWalker(void)
{
}

void NumWalker::step()
{
	counter--;
}

void NumWalker::takeInput(char in)
{
	if(counter < 1 && in >= '1' && in <= '9')
	{
		counter = 10;
		int newX = getMyX();
		int newY = getMyY();
		if(in % 3 == 1)
		{
			newX--;
		}
		else if(in % 3 == 0)
		{
			newX++;
		}
		if(in < '4')
		{
			newY++;
		}
		else if(in > '6')
		{
			newY--;
		}
		newX = max(min(newX, 5), 0);
		newY = max(min(newY, 5), 0);
		bool debug = here->tryToMoveToCell(here->getDungeon()->getCell(newX, newY), false);
		int x = 1;
	}
}