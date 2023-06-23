#include "StdAfx.h"
#include <stdlib.h> //get rand from here
#include "RandomWalker.h"
#include <algorithm>

using namespace std;

RandomWalker::RandomWalker(void)
{
	counter = 10;
	face = 'R';
	skin = 4;
}


RandomWalker::~RandomWalker(void)
{
}

void RandomWalker::step()
{
	if(--counter < 1)
	{
		counter = 10;
		//move in a random direction
		int newX = max(min(rand() % 3 - 1 + getMyX(), 5), 0);
		int newY = max(min(rand() % 3 - 1 + getMyY(), 5), 0);
		here->tryToMoveToCell(here->getDungeon()->getCell(newX, newY), false); //adjecency test is broken
		int x = 1;
	}
}