#include "StdAfx.h"
#include "FEBattleField.h"
#include <algorithm> 
#include "SampleFEAI.h"
#include <sstream>
#include "AIBreeder.h"

using namespace std;

FEBattleField::FEBattleField(int numberOfPlayers, int height, int width, FEStatViewer* statViewer, FEConsole* log) : Dungeon(height, width)
{
	activeUnit = nullptr;
	cursorX = 0;
	cursorY = 0;
	//moveCounter = 0;
	flashCounter = 0;
	currentTurn = 1;
	numPlayers = numberOfPlayers + 1; //zero is reserved for terrain
	//unitsOnField = 0;
	unitCounts = new LinkedList<FEUnit>*[numPlayers];
	factionAIs = new FEAIInterface*[numPlayers];
	for(int counter = 0; counter < numPlayers; counter++)
	{
		unitCounts[counter] = new LinkedList<FEUnit>();
		factionAIs[counter] = nullptr;
	}
	attackMode = false;
	statWindow = statViewer;
	unitsToMove = new LinkedList<FEUnit>();
	attacklog = log;
	turnCounter = 1;
	log->sendMessage("Player 1: Turn 1");
	scoreKeeper = nullptr;
	matchOver = false;
}


FEBattleField::~FEBattleField(void)
{
	for(int counter = 0; counter < numPlayers; counter++)
	{
		delete unitCounts[counter];
	}
	delete[] factionAIs;
	delete[] unitCounts;
}

ColorChar FEBattleField::getColorChar(int x, int y)
{
	if((x == -1 || x == width) && (y == -1 || y == height))
	{
		ColorChar retVal;
		retVal.color = 16; //black on green
		retVal.glyph = '+';
		return retVal;
	}
	else if(y == -1 || y == height)
	{
		ColorChar retVal;
		retVal.color = 16; //black on green
		retVal.glyph = '-';
		return retVal;
	}
	else if(x == -1 || x == width)
	{
		ColorChar retVal;
		retVal.color = 16; //black on green
		retVal.glyph = '|';
		return retVal;
	}

	ColorChar retVal = contents[x + y * width]->getColorChar();
	if(x == cursorX && y == cursorY && flashCounter > 10)
	{
		retVal.color = 56; //black on white
	}
	else if(activeUnit != nullptr)
	{
		if(attackMode)
		{
			if(getDistance(activeUnit->getMyX(), activeUnit->getMyY(), x, y) <= activeUnit->getEquipped()->max_range &&
				canAttack(activeUnit, x, y))
			{
				retVal.color = (retVal.color % 8) + 40; //angry background
			}
		}
		else
		{
			//blue field can be moved to
			//if(!contents[y * width + x]->hasOccupant() && getDistance(activeUnit->getMyX(), activeUnit->getMyY(), x, y) <= activeUnit->getStats()->move)
			if(canMove(activeUnit, x, y))
			{
				retVal.color = (retVal.color % 8) + 24; //cyan background
			}
			else if(canAttackSpace(activeUnit, x, y))
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
	//unitsOnField++;
	unitCounts[(static_cast <FEUnit*>(newCreature))->getPlayer()]->insert(static_cast <FEUnit*>(newCreature));
	if((static_cast <FEUnit*>(newCreature))->getPlayer() == currentTurn && (static_cast <FEUnit*>(newCreature))->getIsActive())
	{
		unitsToMove->insert(static_cast <FEUnit*>(newCreature));
	}
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
		if(contents[x + y * width]->getOccupant() != nullptr) //finish moving might end the game, which means this may have exitted
		{
			unitCounts[static_cast<FEUnit*>(contents[x + y * width]->getOccupant())->getPlayer()]->remove(static_cast<FEUnit*>(contents[x + y * width]->getOccupant()));
			unitsToMove->remove(static_cast<FEUnit*>(contents[x + y * width]->getOccupant()));
		}
	}
	Dungeon::exit(x, y);
}

void FEBattleField::takeInput(char in) //finish this function
{
	if(/*moveCounter > 4 &&*/ factionAIs[currentTurn] == nullptr)
	{
		//moveCounter = 0;
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
			//if we moused over a unit, display it
			if(contents[width * cursorY + cursorX]->hasOccupant())
			{
				statWindow->setUnit(static_cast<FEUnit*>(contents[width * cursorY + cursorX]->getOccupant()));
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
					if(!activeUnit->getIsActive() || activeUnit->getPlayer() != currentTurn)
					{
						activeUnit = nullptr; //can't select inactive units
					}
				}
			}
			else if(attackMode)
			{
				//try to attack the unit at the indicated location
				if(canAttack(activeUnit, cursorX, cursorY))
				{
					activeUnit->attack(static_cast<FEUnit*>(contents[cursorX + cursorY * width]->getOccupant()),
						!canAttack(static_cast<FEUnit*>(contents[cursorX + cursorY * width]->getOccupant()), //why does counter = false mean no counterattack?
						activeUnit->getMyX(),
						activeUnit->getMyY()), attacklog);
					finishMoving();
				}
			}
			else //a unit is selected, so try to move to the indicated location
			{
				//first check if there is enough range
				if(canMove(activeUnit, cursorX, cursorY))
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
			activeUnit = nullptr; //deselect
		}
	}
}

void FEBattleField::step()
{
	if(matchOver)
	{
		matchOver = false;
		endMatch(); //don't end the match midstate
		return;
	}
	flashCounter = (flashCounter + 1) % 15;
	//moveCounter++;
	if(/*moveCounter >= 24 &&*/ factionAIs[currentTurn] != nullptr) //put the moveCounter back in for actual human play
	{
		FEMoveOrder thisOrder = factionAIs[currentTurn]->getNextMove(this, unitsToMove);
		//execute the order
		activeUnit = thisOrder.unitToMove;
		if(canMove(activeUnit, thisOrder.endX, thisOrder.endY))
		{
			activeUnit->getMyLocation()->tryToMoveToCell(contents[thisOrder.endX + thisOrder.endY * width], false);
		}
		if(thisOrder.attackTarget != nullptr && canAttack(activeUnit, thisOrder.attackTarget->getMyX(), thisOrder.attackTarget->getMyY()))
		{
			activeUnit->attack(thisOrder.attackTarget, !canAttack(thisOrder.attackTarget, thisOrder.unitToMove->getMyX(), thisOrder.unitToMove->getMyY()), attacklog);
		}
		finishMoving();
		//moveCounter = 0;
	}
}

inline int FEBattleField::getDistance(int xStart, int yStart, int xEnd, int yEnd)
{
	return abs(xStart - xEnd) + abs(yStart - yEnd);
}

void FEBattleField::finishMoving()
{
	if(activeUnit != nullptr)
	{
		activeUnit->deactivate();
		unitsToMove->remove(activeUnit);
	}
	attackMode = false;
	activeUnit = nullptr;
	//check if there are any activeUnits left of the current team
	if(unitsToMove->getFirst() == nullptr)
	{
		//make all inactive units active
		forEach(FEUnit, counter, unitCounts[currentTurn]->getFirst())
		{
			counter->first->activate();
		}
		int lastTurn = currentTurn;
		do
		{
			currentTurn = (currentTurn + 1) % numPlayers;
			if(currentTurn == 0) //zero is reserved for rocks
			{
				currentTurn++;
				turnCounter++;
			}
		}
		while(unitCounts[currentTurn]->getFirst() == nullptr); //skip the turns of anyone with no units
		if(lastTurn == currentTurn || turnCounter > 15)
		{
			//gameover
			matchOver = true;
		}
		delete unitsToMove;
		unitsToMove = unitCounts[currentTurn]->copyList();
		stringstream ss;
		ss << "Player " << currentTurn << ": Turn " << turnCounter;
		attacklog->sendMessage(ss.str());
	}
}

inline bool FEBattleField::canMove(FEUnit* movingUnit, int x, int y)
{
	bool* inefficient = getValidFinalPositions(movingUnit);
	bool retVal = inefficient[x + y * width];
	delete[] inefficient;
	return retVal;
}

inline bool FEBattleField::canAttack(FEUnit* attackingUnit, int x, int y)
{
	return attackingUnit->inRange(getDistance(attackingUnit->getMyX(), attackingUnit->getMyY(), x, y)) &&
	contents[x + y * width]->hasOccupant() &&
	static_cast<FEUnit*>(contents[x + y * width]->getOccupant())->getPlayer() != attackingUnit->getPlayer() &&
	static_cast<FEUnit*>(contents[x + y * width]->getOccupant())->getPlayer() != 0;
}

inline bool FEBattleField::canAttackSpace(FEUnit* attackingUnit, int x, int y)
{
	bool* inefficient = getValidAttackPositions(activeUnit);
	bool retVal = inefficient[x + y * width];
	delete[] inefficient;
	return retVal;
}

int FEBattleField::getNumPlayers()
{
	return numPlayers;
}

LinkedList<FEUnit>* FEBattleField::getPlayerUnits(int player)
{
	return unitCounts[player]->copyList();
}

int FEBattleField::InitTerrain(int map[], int x, int y)
{
	StatBlock* standard_rock = new StatBlock(10, 0, 0, 0, 0, 0, 0, 0, 0, Proficiency());
	Item* rock = new Item(ITEM, 0, 0, 0, 0, 0, 0);
	if (x != width || y != height)
		return -1;
	else {
		for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++) {
			if (map[i+j*x] == 1) {
				this->enter(new FEUnit('@', 0, 0, standard_rock, rock, "Rock"), i, j);
			}
		}
		return 0; //success
	}
}

bool* FEBattleField::getValidFinalPositions(FEUnit* unitToMove)
{
	int* stepMap = new int[size];
	for(int counter = 0; counter < size; counter++)
	{
		stepMap[counter] = -1;
	}
	stepMap[unitToMove->getMyX() + unitToMove->getMyY() * width] = 0;
	for(int counter = 0; counter < unitToMove->getStats()->move; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			if(stepMap[counte] == counter) //take a step
			{
				//up
				if(counte >= width && stepMap[counte - width] == -1 &&
					(!contents[counte - width]->hasOccupant() ||
					static_cast<FEUnit*>(contents[counte - width]->getOccupant())->getPlayer() == unitToMove->getPlayer()))
				{
					stepMap[counte - width] = counter + 1;
				}
				//down
				if(counte < width * (height - 1) && stepMap[counte + width] == -1 &&
					(!contents[counte + width]->hasOccupant() ||
					static_cast<FEUnit*>(contents[counte + width]->getOccupant())->getPlayer() == unitToMove->getPlayer()))
				{
					stepMap[counte + width] = counter + 1;
				}
				//left
				if(counte % width > 0 && stepMap[counte - 1] == -1 &&
					(!contents[counte - 1]->hasOccupant() ||
					static_cast<FEUnit*>(contents[counte - 1]->getOccupant())->getPlayer() == unitToMove->getPlayer()))
				{
					stepMap[counte - 1] = counter + 1;
				}
				//right
				if(counte % width > 0 && stepMap[counte + 1] == -1 &&
					(!contents[counte + 1]->hasOccupant() ||
					static_cast<FEUnit*>(contents[counte + 1]->getOccupant())->getPlayer() == unitToMove->getPlayer()))
				{
					stepMap[counte + 1] = counter + 1;
				}
			}
		}
	}
	bool* retVal = new bool[size];
	for(int counter = 0; counter < size; counter++)
	{
		if(stepMap[counter] != -1 && !contents[counter]->hasOccupant())
		{
			retVal[counter] = true;
		}
		else
		{
			retVal[counter] = false;
		}
	}
	retVal[unitToMove->getMyX() + unitToMove->getMyY() * width] = true;
	delete stepMap;
	return retVal;
}

bool* FEBattleField::getValidAttackPositions(FEUnit* unitToMove)
{
	bool* standingPlaces = getValidFinalPositions(unitToMove);
	bool* strikeMap = new bool[size];
	for(int counter = 0; counter < size; counter++)
	{
		strikeMap[counter] = false;
	}
	for(int counter = 0; counter < size; counter++)
	{
		if(standingPlaces[counter])
		{
			for(int counte = 0; counte < size; counte++)
			{
				if(counter != counte &&
					unitToMove->inRange(getDistance(counter % width, counter / width, counte % width, counte / width)))
				{
					strikeMap[counte] = true;
				}
			}
		}
	}
	for(int counter = 0; counter < size; counter++)
	{
		if(contents[counter]->hasOccupant() &&
			(static_cast<FEUnit*>(contents[counter]->getOccupant())->getPlayer() == 0 ||
			static_cast<FEUnit*>(contents[counter]->getOccupant())->getPlayer() ==  unitToMove->getPlayer()))
		{
			strikeMap[counter] = false;
		}
	}
	delete[] standingPlaces;
	return strikeMap;
}
