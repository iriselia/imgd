#pragma once
#include "map.h"
#include "Cell.h"
#include "Creature.h"
#include "LinkedList.h"

#define side 6

class Cell;
class Creature;

class Dungeon :	public Map
{
public:
	Dungeon(void);
	Dungeon(int height, int width);
	~Dungeon(void);
	void takeInput(char in);
	void step(void);
	ColorChar getColorChar(int x, int y);
	LinkedList<Cell>* getAdjecentCells(int x, int y); //returns all cells in this Dungeon that are adjecent to the given cell
	virtual bool enter(Creature* newCreature, int x, int y); //introduces a new creature to the dungeon
	virtual void exit(int x, int y); //removes the creature at this point from the dungeon
	Cell* getCell(int x, int y);
	Cell** getCells();
	int getMyX();
	int getMyY();
	int getMySize();
protected:
	//lets assume all dungeons are on orthogonal grids
	int height;
	int width;
	int size;
	Cell** contents; //this is an array; assume a null value is impassible, but generally these should be avoided
	LinkedList<Creature> inputListeners;

friend class Cell;
};

