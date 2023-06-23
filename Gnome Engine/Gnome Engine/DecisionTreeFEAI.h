#include "StdAfx.h"
#include "DecisionTreeFEAI.h"

/*


DecisionTreeFEAI::DecisionTreeFEAI(void)
{
	transposition_table = new unordered_map<TranspositionTableKey, TranspositionTableEntry>();
}


DecisionTreeFEAI::~DecisionTreeFEAI(void)
{
}

float DecisionTreeFEAI::negamax(FEBattleField* battlefield, LinkedList<FEUnit>* unitsToMove, int depth, float alpha, float beta, int player)
{
	FEUnit* current_unit = unitsToMove->getFirst()->first;
	LinkedList<FEUnit>* enemy_units = battlefield->getPlayerUnits(player == 2 ? 1 : 2);
	float alpha_origin = alpha;
	TranspositionTableKey key;
	key.battlefield = battlefield;
	key.units_to_move = unitsToMove;
	if(transposition_table->find(key) != transposition_table->end())
	{
		TranspositionTableEntry stored_value = transposition_table->at(key);
		if(depth >= stored_value.depth)
		{
			switch(stored_value.flag)
			{
			case EXACT:
				return stored_value.value;
				break;
			case UPPER:
				if(alpha < stored_value.value) alpha = stored_value.value;
				break;
			case LOWER:
				if(beta < stored_value.value) beta = stored_value.value;
				break;
			}
			if(alpha >= beta) return stored_value.value;
		}
	}
	if(depth == 0 ||  enemy_units->getFirst() == nullptr) return DecisionTreeFEAI::heuristic(key, player);
	float best_value = -10000;
	bool* valid_positions = battlefield->getValidFinalPositions(current_unit);
	LinkedList<FEMoveOrder> possible_moves = LinkedList<FEMoveOrder>();
	for(int i = 0; i < (battlefield->getMyX() * battlefield->getMyY()); i++)
	{
		LinkedList<FEUnit*> possible_targets = battlefield->getValidTargetsFromPosition(current_unit, i);

	}
}

float DecisionTreeFEAI::heuristic(TranspositionTableKey key, int player)
{
	return 0;
}

FEMoveOrder DecisionTreeFEAI::getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove)
{

	FEMoveOrder retVal;
	LinkedList<FEUnit>* PlayerUnits = currentBattleField->getPlayerUnits(1);
	retVal.unitToMove = unitsToMove->getFirst()->first; //always move units in order
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
*/
