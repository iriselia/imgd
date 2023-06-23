#include "StdAfx.h"
#include "Cell.h"


Cell::Cell(Dungeon* dungeon, int x, int y)
{
	occupant = nullptr;
	ground = 16; //green
	adjecentCells = new LinkedList<Cell>();
	myDungeon = dungeon;
	myX = x;
	myY = y;
}


Cell::~Cell(void)
{
	//write this later
}

ColorChar Cell::getColorChar(void)
{
	ColorChar retVal;
	if(occupant != nullptr)
	{
		retVal = occupant->getColorChar();
	}
	else
	{
		retVal = ColorChar();
		retVal.glyph = ' ';
		retVal.color = 0;
	}
	retVal.color += ground;
	return retVal;
}

void Cell::makeCellAdjecent(Cell* newCell)
{
	adjecentCells->insert(newCell);
}

void Cell::removeAdjecency(Cell* newCell)
{
	adjecentCells->remove(newCell);
}

bool Cell::tryToMoveToCell(Cell* newCell, bool mustBeAdjecent)
{
	if(newCell == this)
	{
		return true;
	}
	if(occupant == nullptr || newCell == nullptr || newCell->hasOccupant())
	{
		return false;
	}
	if(mustBeAdjecent)
	{
		bool contains = false;
		forEach(Cell, counter, adjecentCells->getFirst())
		{
			if(counter->first == newCell)
			{
				contains = true;
				break;
			}
		}
		if(!contains)
		{
			return false;
		}
	}
	moveToCell(newCell);
	return true;
}

void Cell::moveToCell(Cell* newCell)
{
	newCell->occupant = occupant;
	occupant->here = newCell;
	occupant = nullptr;
}

bool Cell::spawn(Creature* newCreature)
{
	if(occupant == nullptr)
	{
		occupant = newCreature;
		newCreature->here = this;
		return true;
	}
	else
	{
		return false;
	}
}

void Cell::despawn()
{
	occupant = nullptr;
}

LinkedList<Cell>* Cell::getAdjecentCells()
{
	return adjecentCells->copyList(); //add duplicate function to linkedList so
}

bool Cell::hasOccupant()
{
	return occupant != nullptr;
}

void Cell::step()
{
	//just let the occupant know
	if(occupant != nullptr)
	{
		occupant->step();
	}
}

void Cell::takeInput(char in)
{
	//just let the occupant know
	if(occupant != nullptr)
	{
		occupant->takeInput(in);
	}
}

Dungeon* Cell::getDungeon()
{
	return myDungeon;
}

Creature* Cell::getOccupant()
{
	return occupant;
}

int Cell::getMyX() {
	return myX;
}

int Cell::getMyY() {
	return myY;
}