#pragma once
#include "feaiinterface.h"
class SampleFEAI2 : public FEAIInterface
{
public:
	//all the units just stand in place
	SampleFEAI2(void);
	~SampleFEAI2(void);
	FEMoveOrder getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove);
};

