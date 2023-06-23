#pragma once

#include "Creature.h"
//#include "Pile.h"
#include "Map.h"
#include "LinkedList.h"
#include "Dungeon.h"
#include "Creature.h"

class Dungeon;
class Creature;

class Cell
{
public:
	Cell(Dungeon* dungeon, int x, int y);
	~Cell(void);
	virtual ColorChar getColorChar(); //returns the color and character of the occupant of the cell
	bool hasOccupant(); //returns true if there's a dude in this space
	void makeCellAdjecent(Cell* newCell); //just adds it to the list
	void removeAdjecency(Cell* newCell);
	bool tryToMoveToCell(Cell* newCell, bool mustBeAdjecent); //attempts to move to a new cell if it's empty
	bool spawn(Creature* newCreature);
	void despawn();
	LinkedList<Cell>* getAdjecentCells();  //make sure you free the list this gives you when you're done
	void step(); //one game step happens to the cell
	void takeInput(char in);
	Dungeon* getDungeon();
	Creature* getOccupant();
	int getMyX();
	int getMyY();
protected:
	Creature* occupant; //the creature in this cell
	//Pile* junk; //the items in this cell
	int ground; //the color of the floor in this tile
	LinkedList<Cell>* adjecentCells;
	void moveToCell(Cell* newCell); //move the occupant to another cell (not necesarilly adjecent)
	int myX; //the cell's position in the dungeon
	int myY;
	Dungeon* myDungeon; //the dungeon this cell is in

friend class Creature;
};

