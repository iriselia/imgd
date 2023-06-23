#pragma once
#include "FEUnit.h"

class FEEnemyUnit : FEUnit {
public:
	FEEnemyUnit(char _face, int _skin, int _team, StatBlock* _stats, Item* _weapon, Cell** _patrolPath, string _name);
	~FEEnemyUnit();
private:
	int patrolState;
	Cell** patrolPath;

};