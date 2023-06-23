#include "StdAfx.h"
#include "SampleFEAI2.h"


SampleFEAI2::SampleFEAI2(void)
{
}


SampleFEAI2::~SampleFEAI2(void)
{
}

FEMoveOrder SampleFEAI2::getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove)
{
	FEMoveOrder retVal;
	LinkedList<FEUnit>* PlayerUnits = currentBattleField->getPlayerUnits(0);
	retVal.unitToMove = unitsToMove->getFirst()->first;
	FEUnit* target;
	bool* spotsToMove = currentBattleField->getValidFinalPositions(retVal.unitToMove);
	bool* spotsToStrike = currentBattleField->getValidAttackPositions(retVal.unitToMove);

	/*Find Enemy units within strikemap*/

	







	if (retVal.unitToMove->getPatrolState() != -1) {
		//If unit reaches point of return, reset patrol state.
		if (retVal.unitToMove->getPatrolPoints() == retVal.unitToMove->getPatrolState()) {
			retVal.unitToMove->setPatrolState(0);
		}

		Cell* nextMove = retVal.unitToMove->getPatrolPath()[retVal.unitToMove->getPatrolState() + 1];
		if (spotsToMove[nextMove->getMyX() + nextMove->getMyY() * currentBattleField->getMyX()]) {
			retVal.unitToMove->setPatrolState(retVal.unitToMove->getPatrolState() + 1);
			retVal.endX = nextMove->getMyX();
			retVal.endY = nextMove->getMyY();
			retVal.attackTarget = nullptr;
		}
	}
	 
	else {
		if (currentBattleField->getPlayerUnits(0)->getFirst() != nullptr)
		{
			target = currentBattleField->getPlayerUnits(0)->getFirst()->first;
		}
		else
		{
			//they're all dead
			retVal.endX = retVal.unitToMove->getMyX();
			retVal.endY = retVal.unitToMove->getMyY();
			retVal.attackTarget = nullptr;
			return retVal;
		}

		if (spotsToStrike[target->getMyX() + target->getMyY() * currentBattleField->getMyX()])
		{
			//can attack, now move to position to attack
			//always strike from above for maximum damage if possible
			if (target->getMyY() > 0 && spotsToMove[target->getMyX() + (target->getMyY() - 1) * currentBattleField->getMyX()])
			{
				retVal.endX = target->getMyX();
				retVal.endY = target->getMyY() - 1;
				retVal.attackTarget = target;
			}
			//strike from below; they never see it coming
			else if (target->getMyY() < currentBattleField->getMyY() - 1 && spotsToMove[target->getMyX() + (target->getMyY() + 1) * currentBattleField->getMyX()])
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
	}
	return retVal;
}