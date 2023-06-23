#include "StdAfx.h"
#include "Creature.h"


Creature::Creature(void)
{
	inventory = new Pile();
	here = nullptr;
}

Creature::Creature(char face, int color)
{
	inventory = new Pile();
	here = nullptr;
	this->face = face;
	skin = color;
}


Creature::~Creature(void)
{
	delete inventory;
	if(here != nullptr)
	{
		here->getDungeon()->exit(getMyX(), getMyY());
	}
}

ColorChar Creature::getColorChar(void)
{
	ColorChar retVal = ColorChar();
	retVal.color = skin;
	retVal.glyph = face;
	return retVal;
}

void Creature::step()
{
	//this will be extended by children
}

void Creature::takeInput(char input)
{
	//this will be extended by children
}

int Creature::getMyX()
{
	return here->myX;
}

int Creature::getMyY()
{
	return here->myY;
}

Cell* Creature::getMyLocation()
{
	return here;
}