#include "StdAfx.h"
#include "GeneticAI.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

GeneticAI::GeneticAI(StatBlock** _idStats, int _size)
{
	idStats = _idStats;
	size = _size;
	//randomly generate everything else
	initiativeOrder = new StatBlock*[size];
	counterTolerance = new int[size];
	bloodLust = new int[size];
	deathLust = new int[size];
	for(int counter = 0; counter < size; counter++)
	{
		initiativeOrder[counter] = idStats[counter];
		counterTolerance[counter] = rand() % 19 - 9;
		bloodLust[counter] = rand() % 19 - 4;
		deathLust[counter] = rand() % 19 - 4;
	}
	std::random_shuffle(initiativeOrder, &(initiativeOrder[size - 1]));
	foeMovementAttraction = new int[size * size];
	foeAttackAttraction = new int[size * size];
	friendAttraction = new int[size * size];
	for(int counter = 0; counter < size * size; counter++)
	{
		foeMovementAttraction[counter] = rand() % 19 - 9;
		foeAttackAttraction[counter] = rand() % 19 - 9;
		friendAttraction[counter] = rand() % 19 - 9;
	}
}

GeneticAI::GeneticAI(GeneticAI** parents, int numParents)
{
	idStats = parents[0]->idStats;
	size = parents[0]->size;
	initiativeOrder = new StatBlock*[size];
	counterTolerance = new int[size];
	bloodLust = new int[size];
	deathLust = new int[size];
	foeMovementAttraction = new int[size * size];
	foeAttackAttraction = new int[size * size];
	friendAttraction = new int[size * size];
	int initParent = rand() % numParents;
	for(int counter = 0; counter < size; counter++)
	{
		initiativeOrder[counter] = parents[initParent]->initiativeOrder[counter];
		counterTolerance[counter] = maybeMutate(parents[rand() % numParents]->counterTolerance[counter]);
		bloodLust[counter] = maybeMutate(parents[rand() % numParents]->bloodLust[counter]);
		deathLust[counter] = maybeMutate(parents[rand() % numParents]->deathLust[counter]);
	}
	//maybe Mutate init
	if(rand() % 10 == 7)
	{
		int slot = rand() % (size - 1);
		StatBlock* temp = initiativeOrder[slot];
		initiativeOrder[slot] = initiativeOrder[slot + 1];
		initiativeOrder[slot + 1] = temp;
	}
	for(int counter = 0; counter < size * size; counter++)
	{
		foeMovementAttraction[counter] = maybeMutate(parents[rand() % numParents]->foeMovementAttraction[counter]);
		foeAttackAttraction[counter] = maybeMutate(parents[rand() % numParents]->foeAttackAttraction[counter]);
		friendAttraction[counter] = maybeMutate(parents[rand() % numParents]->friendAttraction[counter]);
	}

}

GeneticAI::~GeneticAI(void)
{
	delete[] counterTolerance;
	delete[] bloodLust;
	delete[] deathLust;
	delete[] foeMovementAttraction;
	delete[] foeAttackAttraction;
	delete[] friendAttraction;
	delete[] initiativeOrder;
}

inline int GeneticAI::maybeMutate(int in)
{
	if((rand() % 8) == 7)
	{
		return in + (rand() % 9 - 4);
	}
	else
	{
		return in;
	}
}

FEMoveOrder GeneticAI::getNextMove(FEBattleField* currentBattleField, LinkedList<FEUnit>* unitsToMove)
{
	FEMoveOrder retVal;
	//find the activeUnit with the highest initiative
	retVal.unitToMove = nullptr;
	int movingUnitType = -1;
	for(int counter = 0; counter < size; counter++)
	{
		forEach(FEUnit, counte, unitsToMove->getFirst())
		{
			if(counte->first->getStats() == initiativeOrder[counter])
			{
				retVal.unitToMove = counte->first;
				movingUnitType = counter;
				goto doubleBreak;
			}
		}
	}
doubleBreak:
	double bestSpaceValue = -1000.0; //I think that's a reasonable floor probably
	retVal.endX = retVal.unitToMove->getMyX();
	retVal.endY = retVal.unitToMove->getMyY();
	retVal.attackTarget = nullptr;
	bool* reachableSpaces = currentBattleField->getValidFinalPositions(retVal.unitToMove);
	for(int counter = 0; counter < currentBattleField->getMySize(); counter++)
	{
		if(reachableSpaces[counter])
		{
			//eval this space
			double thisSpaceValue = 0.0;
			FEUnit* bestTarget = nullptr;
			double bestTargetValue = 0.0;
			for(int counte = 1; counte < currentBattleField->getNumPlayers(); counte++)
			{
				forEach(FEUnit, count, currentBattleField->getPlayerUnits(counte)->getFirst())
				{
					//consider this position relative to every other unit
					int unitTypeIndex = -1;
					for(int coun = 0; coun < size; coun++)
					{
						if(idStats[coun] == count->first->getStats())
						{
							unitTypeIndex = coun;
							break;
						}
					}
					if(unitTypeIndex = -1)
					{
						//don't know what it is so continue
						continue;
					}
					if(count->first->getPlayer() == retVal.unitToMove->getPlayer())
					{
						//maybe change coefficient from 10 to something else
						thisSpaceValue += friendAttraction[movingUnitType * size + unitTypeIndex] * 10.0 /
							currentBattleField->getDistance(retVal.unitToMove->getMyX(), retVal.unitToMove->getMyY(),
															count->first->getMyX(), count->first->getMyY());
					}
					else
					{
						thisSpaceValue += foeMovementAttraction[movingUnitType * size + unitTypeIndex] * 10.0 /
							currentBattleField->getDistance(retVal.unitToMove->getMyX(), retVal.unitToMove->getMyY(),
															count->first->getMyX(), count->first->getMyY());
					}
				}
				//now consider all possible attack targets
				LinkedList<FEUnit>* targets = currentBattleField->getPossibleAttackTargets(counter % currentBattleField->getMyX(),
					counter / currentBattleField->getMyX(), retVal.unitToMove->getPlayer(), retVal.unitToMove->getEquipped());
				forEach(FEUnit, count, targets->getFirst())
				{
					double thisTargetValue = bloodLust[movingUnitType];
					if(currentBattleField->getCurrentTurn() > 9) //this is a hack to reduce peace
					{
						thisTargetValue += 40;
					}
					int thisTargetIndex = getIndexOfStat(count->first->getStats());
					thisTargetValue += foeAttackAttraction[movingUnitType * size + thisTargetIndex];
					if(count->first->inRange(currentBattleField->getDistance(counter % currentBattleField->getMyX(),
						counter / currentBattleField->getMyX(), count->first->getMyX(), count->first->getMyY())) &&
						retVal.unitToMove->getMight() <= (count->first->getDamageReduction(retVal.unitToMove->getEquipped()->type)
						+ count->first->getCurrentHP()))
					{
						//counterattack possible
						thisTargetValue += counterTolerance[movingUnitType];
					}
					if((retVal.unitToMove->getMight() - (count->first->getDamageReduction(retVal.unitToMove->getEquipped()->type))
						* (retVal.unitToMove->getStats()->speed  - 4 >= count->first->getStats()-> speed ? 2 : 1)) >
						count->first->getCurrentHP())
					{
						//kill possible w/o crits
						thisTargetValue += deathLust[movingUnitType];
					}
					if(thisTargetValue > bestTargetValue)
					{
						bestTargetValue = thisTargetValue;
						bestTarget = count->first;
					}
				}
			}
			thisSpaceValue += bestTargetValue;
			if(thisSpaceValue > bestSpaceValue)
			{
				bestSpaceValue = thisSpaceValue;
				retVal.endX = counter % currentBattleField->getMyX();
				retVal.endY = counter / currentBattleField->getMyX();
				retVal.attackTarget = bestTarget;
			}
		}
	}
	return retVal;
}

int GeneticAI::getIndexOfStat(StatBlock* stats)
{
	for(int coun = 0; coun < size; coun++)
	{
		if(idStats[coun] == stats)
		{
			return coun;
		}
	}
	return -1;
}

void GeneticAI::outputAIToCSV(string name)
{
	ofstream svFile;
	svFile.open(name);
	svFile << size << '\n';
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			if(idStats[counte] == initiativeOrder[counter])
			{
				svFile << counte << ',';
				break;
			}
		}
	}
	svFile << '\n';
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			svFile << foeMovementAttraction[counter * size + counte] << ',';
		}
		svFile << '\n';
	}
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			svFile << foeAttackAttraction[counter * size + counte] << ',';
		}
		svFile << '\n';
	}
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			svFile << friendAttraction[counter * size + counte] << ',';
		}
		svFile << '\n';
	}
	for(int counter = 0; counter < size; counter++)
	{
		svFile << counterTolerance[counter] << ',';
	}
	svFile << '\n';
	for(int counter = 0; counter < size; counter++)
	{
		svFile << bloodLust[counter] << ',';
	}
	svFile << '\n';
	for(int counter = 0; counter < size; counter++)
	{
		svFile << deathLust[counter] << ',';
	}
	svFile.close();
}

//finish this func
GeneticAI::GeneticAI(StatBlock** _idStats, int _size, string csvFile)
{
	idStats = _idStats;
	size = _size;
	//randomly generate everything else
	initiativeOrder = new StatBlock*[size];
	counterTolerance = new int[size];
	bloodLust = new int[size];
	deathLust = new int[size];
	foeMovementAttraction = new int[size * size];
	foeAttackAttraction = new int[size * size];
	friendAttraction = new int[size * size];
	ifstream svFile;
	std::ifstream in(csvFile, std::ios::in | std::ios::binary);
	string contents;
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
	int nextCharIndex = 2; //after the size
	int readNum;
	int charIndexHolder;
	for(int counter = 0; counter < size; counter++)
	{
		charIndexHolder = contents.find(',', nextCharIndex);
		string nextNum = contents.substr(nextCharIndex, charIndexHolder);
		istringstream tempStream(nextNum);
		tempStream >> readNum;
		initiativeOrder[counter] = idStats[readNum];
		nextCharIndex = charIndexHolder + 1;
	}
	nextCharIndex++; //pass the newline
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			charIndexHolder = contents.find(',', nextCharIndex);
			string nextNum = contents.substr(nextCharIndex, charIndexHolder);
			istringstream tempStream(nextNum);
			tempStream >> readNum;
			foeMovementAttraction[counter * size + counte] = readNum;
			nextCharIndex = charIndexHolder + 1;
		}
		nextCharIndex++; //pass the newline
	}
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			charIndexHolder = contents.find(',', nextCharIndex);
			string nextNum = contents.substr(nextCharIndex, charIndexHolder);
			istringstream tempStream(nextNum);
			tempStream >> readNum;
			foeAttackAttraction[counter * size + counte] = readNum;
			nextCharIndex = charIndexHolder + 1;
		}
		nextCharIndex++; //pass the newline
	}
	for(int counter = 0; counter < size; counter++)
	{
		for(int counte = 0; counte < size; counte++)
		{
			charIndexHolder = contents.find(',', nextCharIndex);
			string nextNum = contents.substr(nextCharIndex, charIndexHolder);
			istringstream tempStream(nextNum);
			tempStream >> readNum;
			friendAttraction[counter * size + counte] = readNum;
			nextCharIndex = charIndexHolder + 1;
		}
		nextCharIndex++; //pass the newline
	}
	for(int counter = 0; counter < size; counter++)
	{
		charIndexHolder = contents.find(',', nextCharIndex);
		string nextNum = contents.substr(nextCharIndex, charIndexHolder);
		istringstream tempStream(nextNum);
		tempStream >> readNum;
		counterTolerance[counter] = readNum;
		nextCharIndex = charIndexHolder + 1;
	}
	nextCharIndex++; //pass the newline
	for(int counter = 0; counter < size; counter++)
	{
		charIndexHolder = contents.find(',', nextCharIndex);
		string nextNum = contents.substr(nextCharIndex, charIndexHolder);
		istringstream tempStream(nextNum);
		tempStream >> readNum;
		bloodLust[counter] = readNum;
		nextCharIndex = charIndexHolder + 1;
	}
	nextCharIndex++; //pass the newline
	for(int counter = 0; counter < size; counter++)
	{
		charIndexHolder = contents.find(',', nextCharIndex);
		string nextNum = contents.substr(nextCharIndex, charIndexHolder);
		istringstream tempStream(nextNum);
		tempStream >> readNum;
		deathLust[counter] = readNum;
		nextCharIndex = charIndexHolder + 1;
	}
}