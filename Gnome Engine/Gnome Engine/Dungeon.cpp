#include "StdAfx.h"
#include "Dungeon.h"

Dungeon::Dungeon()
{
	Dungeon(side, side);
}

Dungeon::Dungeon(int height, int width)
{
	this->height = height;
	this->width = width;
	size = height * width;
	contents = new Cell*[size];
	for(int counter = 0; counter < height; counter++)
	{
		for(int counte = 0; counte < width; counte++)
		{
			contents[counter * width + counte] = new Cell(this, counte, counter);
		}
	}
	
	//link up all the new cells
	for(int counter = 0; counter < height; counter++)
	{
		for(int counte = 0; counte < width; counte++)
		{
			for(int count = -1; count <= 1; count++)
			{
				for(int coun = -1; coun <= 1; coun++)
				{
					if((counter + count >= 0) && (counter + count < height) &&
						(counte + coun >= 0) && (counte + coun < width))
					{
						contents[counter * width + counte]->
							makeCellAdjecent(contents[(counter + count) * width + counte + counte]);
					}
				}
			}
		}
	}
}


Dungeon::~Dungeon(void)
{
}

void Dungeon::takeInput(char in)
{
	//tell each cell to step
	for(int counter = 0; counter < size; counter++)
	{
		contents[counter]->takeInput(in);
	}
}

void Dungeon::step(void)
{
	//tell each cell to step
	for(int counter = 0; counter < size; counter++)
	{
		contents[counter]->step();
	}
}

ColorChar Dungeon::getColorChar(int x, int y)
{
	if(y * width + x < size)
	{
		return contents[y * width + x]->getColorChar();
	}
	else
	{
		ColorChar retVal;
		retVal.color = 0;
		retVal.glyph = ' ';
		return retVal;
	}
}

LinkedList<Cell>* Dungeon::getAdjecentCells(int x, int y)
{
	return contents[x * y * width]->getAdjecentCells();
}

bool Dungeon::enter(Creature* newCreature, int x, int y)
{
	if(!(contents[y * width + x]->hasOccupant()))
	{
		contents[y * width + x]->spawn(newCreature);
		return true;
	}
	else
	{
		return false;
	}
}

void Dungeon::exit(int x, int y)
{
	contents[x + y * width]->despawn();
}

Cell* Dungeon::getCell(int x, int y)
{
	return contents[x + y * width];
}

Cell** Dungeon::getCells() {
	return contents;
}

int Dungeon::getMyX() {
	return width;
}

int Dungeon::getMyY() {
	return height;
}

int Dungeon::getMySize()
{
	return size;
}