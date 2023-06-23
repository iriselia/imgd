#include "StdAfx.h"
#include "SampleFEAI.h"


SampleFEAI::SampleFEAI(void)
{
}


SampleFEAI::~SampleFEAI(void)
{
}

FEMoveOrder SampleFEAI::getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove)
{
	FEMoveOrder retVal;
	LinkedList<FEUnit>* PlayerUnits = currentBattleField->getPlayerUnits(1);
	retVal.unitToMove = unitsToMove->getFirst()->first;
	FEUnit* target;
	if(currentBattleField->getPlayerUnits(1)->getFirst() != nullptr)
	{
		target = currentBattleField->getPlayerUnits(1)->getFirst()->first;
	}
	else
	{
		//they're all dead
		retVal.endX = retVal.unitToMove->getMyX();
		retVal.endY = retVal.unitToMove->getMyY();
		retVal.attackTarget = nullptr;
		return retVal;
	}
	bool* spotsToMove = currentBattleField->getValidFinalPositions(retVal.unitToMove);
	bool* spotsToStrike = currentBattleField->getValidAttackPositions(retVal.unitToMove);
	if(spotsToStrike[target->getMyX() + target->getMyY() * currentBattleField->getMyX()])
	{
		//can attack, now move to position to attack
		//always strike from above for maximum damage if possible
		if(target->getMyY() > 0 && spotsToMove[target->getMyX() + (target->getMyY() - 1) * currentBattleField->getMyX()])
		{
			retVal.endX = target->getMyX();
			retVal.endY = target->getMyY() - 1;
			retVal.attackTarget = target;
		}
		//strike from below; they never see it coming
		else if(target->getMyY() < currentBattleField->getMyY() - 1 && spotsToMove[target->getMyX() + (target->getMyY() + 1) * currentBattleField->getMyX()])
		{
			retVal.endX = target->getMyX();
			retVal.endY = target->getMyY() + 1;
			retVal.attackTarget = target;
		}
		else //too much typing; give up
		{
			retVal.endX = retVal.unitToMove->getMyX();
			retVal.endY = retVal.unitToMove->getMyY();
			retVal.attackTarget = nullptr;
		}
	}
	else
	{
		retVal.endX = retVal.unitToMove->getMyX();
		retVal.endY = retVal.unitToMove->getMyY();
		retVal.attackTarget = nullptr;
	}
	return retVal;
}