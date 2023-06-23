#pragma once
#include "stdafx.h"
#include "FEUnit.h"

class FEEnemyUnit : FEUnit {
public:
	FEEnemyUnit(char _face, int _skin, int _team, StatBlock* _stats, Item* _weapon, Cell** _patrolPath, string _name) : FEUnit(_face, _skin, _team, _stats, _weapon, _name) {
		patrolState = 0;
		patrolPath = _patrolPath;
	}
	~FEEnemyUnit() {

	}
private:
	int patrolState;
	Cell** patrolPath;

};