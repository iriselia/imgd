#pragma once
#include "feaiinterface.h"
class SampleFEAI :	public FEAIInterface
{
public:
	//all the units just stand in place
	SampleFEAI(void);
	~SampleFEAI(void);
	FEMoveOrder getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove);
};

