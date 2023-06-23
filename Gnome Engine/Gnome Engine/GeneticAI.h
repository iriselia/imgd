#pragma once
#include "FEAIInterface.h"
#include <cstdlib>

class GeneticAI :	public FEAIInterface
{
public:
	GeneticAI(StatBlock** _idStats, int _size);
	GeneticAI(GeneticAI** parents, int numParents);
	GeneticAI(StatBlock** _idStats, int _size, string csvFile);
	~GeneticAI(void);
	FEMoveOrder getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove);
	void outputAIToCSV(string name);
private:
	//knight
	//archer
	//calvalier
	//swashbuckler
	//fighter
	//mage
	int size; //how many unit types there are
	StatBlock** idStats; //used to quickly identify which class a unit is; these pointers are compared but never dereferenced
	StatBlock** initiativeOrder; //the order in which the units will act each turn, 
	int* foeMovementAttraction; //this is a matrix of how attracted to each foe each unit is in terms of movement
	int* foeAttackAttraction; //this is a matrix that prioritizes which foes to attack first with each unit
	int* friendAttraction; //this is a matrix too
	int* counterTolerance; //a measurement of how willing each unit is to take counterAttacks
	int* bloodLust; //each unit's desire to make an attack
	int* deathLust; //each unit's desire to make a killing blow
	int maybeMutate(int in);
	int getIndexOfStat(StatBlock*);
};

