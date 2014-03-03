#include "StdAfx.h"
#include "FEBattleField.h"
#include <algorithm> 

using namespace std;

FEBattleField::FEBattleField(int numberOfPlayers, int height, int width) : Dungeon(height, width)
{
	activeUnit = nullptr;
	cursorX = 0;
	cursorY = 0;
	moveCounter = 0;
	flashCounter = 0;
	currentTurn = 0;
	numPlayers = numberOfPlayers;
	unitsOnField = 0;
	unitCounts = new LinkedList<FEUnit>*[numberOfPlayers];
	for(int counter = 0; counter < numberOfPlayers; counter++)
	{
		unitCounts[counter] = new LinkedList<FEUnit>();
	}
	attackMode = false;
}


FEBattleField::~FEBattleField(void)
{
	for(int counter = 0; counter < numPlayers; counter++)
	{
		delete unitCounts[counter];
	}
	delete unitCounts;
}

ColorChar FEBattleField::getColorChar(int x, int y)
{
	ColorChar retVal = contents[x + y * width]->getColorChar();
	if(x == cursorX && y == cursorY && flashCounter > 10)
	{
		retVal.color = 56; //black on white
	}
	else if(activeUnit != nullptr)
	{
		if(attackMode)
		{
			if(getDistance(activeUnit->getMyX(), activeUnit->getMyY(), x, y) <= activeUnit->getRange() &&
				(!contents[x + y * width]->hasOccupant() || static_cast<FEUnit*>(contents[x + y * width]->getOccupant())->getTeam() != activeUnit->getTeam()))
			{
				retVal.color = (retVal.color % 8) + 40; //angry background
			}
		}
		else
		{
			//blue field can be moved to
			if(!contents[y * width + x]->hasOccupant() && getDistance(activeUnit->getMyX(), activeUnit->getMyY(), x, y) <= activeUnit->getMove())
			{
				retVal.color = (retVal.color % 8) + 24; //cyan background
			}
			else if(getDistance(activeUnit->getMyX(), activeUnit->getMyY(), x, y) <= activeUnit->getMove() + activeUnit->getRange() &&
				(!contents[x + y * width]->hasOccupant() || static_cast<FEUnit*>(contents[x + y * width]->getOccupant())->getTeam() != activeUnit->getTeam()))
			{
				retVal.color = (retVal.color % 8) + 40; //angry background
			}
		}
	}
	return retVal;
}

bool FEBattleField::enter(Creature* newCreature, int x, int y)
{
	//TODO: check that newCreature is an FE creature, somehow
/*	if(typeid (newCreature) != typeid(FEUnit*)) //extensions should be allowed; I'll have to rewrite this check
	{
		return false;
	}*/
	bool attempt = Dungeon::enter(newCreature, x, y);
	if(!attempt)
	{
		return false;
	}
	unitsOnField++;
	unitCounts[(static_cast <FEUnit*>(newCreature))->getTeam()]->insert(static_cast <FEUnit*>(newCreature));
	return true;
}

void FEBattleField::exit(int x, int y)
{
	if(contents[x + y * width]->getOccupant() != nullptr)
	{
		if(contents[x + y * width]->getOccupant() == activeUnit)
		{
			finishMoving();
		}
		totalUnits--;
		unitCounts[static_cast<FEUnit*>(contents[x + y * width]->getOccupant())->getTeam()]->remove(static_cast<FEUnit*>(contents[x + y * width]->getOccupant()));
	}
	Dungeon::exit(x, y);
}

void FEBattleField::takeInput(char in) //finish this function
{
	if(moveCounter > 8)
	{
		moveCounter = 0;
		//if there's a num key, move the cursor
		if(in >= '1' && in <= '9')
		{
			if(in % 3 == 1)
			{
				cursorX--;
				cursorX = max(cursorX, 0);
			}
			else if(in % 3 == 0)
			{
				cursorX++;
				cursorX = min(cursorX, width - 1);
			}
			if(in < '4')
			{
				cursorY++;
				cursorY = min(cursorY, height - 1);
			}
			else if(in > '6')
			{
				cursorY--;
				cursorY = max(cursorY, 0);
			}
		}
		else if(in == 'a') //select
		{
			//if no unit is selected, select the unit in the current cell
			if(activeUnit == nullptr)
			{
				if(contents[cursorX + cursorY * width]->hasOccupant())
				{
					activeUnit = static_cast<FEUnit*> (contents[cursorX + cursorY * width]->getOccupant());
					if(!activeUnit->getIsActive() || activeUnit->getTeam() != currentTurn)
					{
						activeUnit = nullptr; //can't select inactive units
					}
				}
			}
			else if(attackMode)
			{
				//try to attack the unit at the indicated location
				if(getDistance(activeUnit->getMyX(), activeUnit->getMyY(), cursorX, cursorY) <= activeUnit->getRange() &&
					contents[cursorX + cursorY * width]->hasOccupant() &&
					static_cast<FEUnit*>(contents[cursorX + cursorY * width]->getOccupant())->getTeam() != activeUnit->getTeam())
				{
					activeUnit->attack(static_cast<FEUnit*>(contents[cursorX + cursorY * width]->getOccupant()));
					finishMoving();
				}
			}
			else //a unit is selected, so try to move to the indicated location
			{
				//first check if there is enough range
				if(getDistance(activeUnit->getMyX(), activeUnit->getMyY(), cursorX, cursorY) <= activeUnit->getMove())
				{
					if(activeUnit->getMyLocation()->tryToMoveToCell(contents[cursorX + cursorY * width], false))
					{
						attackMode = true;
					}
				}
			}
		}
		else if(in == 'b') //cancel
		{
			if(attackMode)
			{
				finishMoving();
			}
		}
	}
}

void FEBattleField::step()
{
	flashCounter = (flashCounter + 1) % 15;
	moveCounter++;
}

int FEBattleField::getDistance(int xStart, int yStart, int xEnd, int yEnd)
{
	return abs(xStart - xEnd) + abs(yStart - yEnd);
}

void FEBattleField::finishMoving()
{
	if(activeUnit != nullptr)
	{
		activeUnit->deactivate();
	}
	attackMode = false;
	activeUnit = nullptr;
	//check if there are any activeUnits left of the current team
	bool anyUnitsLeft = false;
	forEach(FEUnit, counter, unitCounts[currentTurn]->getFirst())
	{
		if(counter->first->getIsActive())
		{
			anyUnitsLeft = true;
			break;
		}
	}
	if(!anyUnitsLeft) //no units left to move so advance the turn
	{
		//make all inactive units active
		forEach(FEUnit, counter, unitCounts[currentTurn]->getFirst())
		{
			counter->first->activate();
		}
		do
		{
			currentTurn = (currentTurn + 1) % numPlayers;
		}
		while(unitCounts[currentTurn]->getFirst() == nullptr); //skip the turns of anyone with no units
	}
}