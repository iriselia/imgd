#pragma once
#include "FEBattleField.h"

//AIs should extend this abstract class
struct FEMoveOrder
{
	FEUnit* unitToMove;
	int endX;
	int endY;
	FEUnit* attackTarget;
};

class FEBattleField;

class FEAIInterface
{
public:
	FEAIInterface(void);
	virtual ~FEAIInterface(void);
	//this function takes a list of units that can move and a snapshot of the current battlefield; it returns a struct encoding the next move
	virtual FEMoveOrder getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove) = 0;
};