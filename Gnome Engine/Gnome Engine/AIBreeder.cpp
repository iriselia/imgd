#include "StdAfx.h"
#include "AIBreeder.h"
#include "LogManager.h"
#include <sstream>

AIBreeder::AIBreeder(int _stockSize, StatBlock** _idStats, int _numBlocks, FEBattleField* _arena, LinkedList<spawnPoint>* _spawnPoints)
{
	currentGeneration = 1;
	stockSize = _stockSize;
	numBlocks = _numBlocks;
	idStats = _idStats;
	breedingStock = new GeneticAI*[stockSize];
	runningScores = new int[stockSize];
	for(int counter = 0; counter < stockSize; counter++)
	{
		breedingStock[counter] = new GeneticAI(idStats, numBlocks);
		runningScores[counter] = 0;
	}
	arena = _arena;
	spawnPoints = _spawnPoints;
	midGeneration = false;
	team1 = -1;
	team2 = -1;
	desiredGenerations = 0;
	log = LogManager::getInstance();
}

AIBreeder::AIBreeder(int _stockSize, StatBlock** _idStats, int _numBlocks, FEBattleField* _arena, LinkedList<spawnPoint>* _spawnPoints, string loadDirectory)
{
	currentGeneration = 1;
	stockSize = _stockSize;
	numBlocks = _numBlocks;
	idStats = _idStats;
	breedingStock = new GeneticAI*[stockSize];
	runningScores = new int[stockSize];
	for(int counter = 0; counter < stockSize; counter++)
	{
		stringstream ss;
		ss << loadDirectory << '/' << counter << ".csv";
		breedingStock[counter] = new GeneticAI(idStats, numBlocks, ss.str());
		runningScores[counter] = 0;
	}
	arena = _arena;
	spawnPoints = _spawnPoints;
	midGeneration = false;
	team1 = -1;
	team2 = -1;
	desiredGenerations = 0;
	log = LogManager::getInstance();
}

AIBreeder::~AIBreeder(void)
{
	for(int counter = 0; counter < stockSize; counter++)
	{
		delete breedingStock[counter];
	}
	delete[] breedingStock;
	delete[] runningScores;
	spawnPoints->freeAll();
	delete[] spawnPoints;
}

void AIBreeder::resolveGeneration()
{
	//round robin, both ways
	team1 = 0;
	team2 = 1;
	midGeneration = true;
	beginMatch();
}

void AIBreeder::beginMatch()
{
	//set up the board
	arena->killAllUnits();
	forEach(spawnPoint, count, spawnPoints->getFirst())
	{
		arena->enter(new FEUnit(count->first->face, count->first->skin, count->first->team, count->first->stats,
			count->first->weapon, count->first->name), count->first->x, count->first->y);
	}
	stringstream ss;
	ss << "Generation " << currentGeneration << ": " << team1 << " vs " << team2;
	log->writeLog(ss.str());
	arena->setAI(breedingStock[team1], 1);
	arena->setAI(breedingStock[team2], 2);
	arena->uUFightToTheDeath(this);
}

void AIBreeder::takeScores(int* scores)
{
	runningScores[team1] += scores[1];
	runningScores[team2] += scores[2];
	delete[] scores;
	//now prepare the next match
	if(advanceTeamCounters())
	{
		//scores complete; breed new generation
		if(desiredGenerations > currentGeneration)
		{
			getNextGeneration();
		}
		else
		{
			//done; output the best to a file or something
			arena->setAI(nullptr, 1);
			arena->setAI(nullptr, 2);
			getNextGeneration();
			//save all the AIs
			for(int counter = 0; counter < stockSize; counter++)
			{
				stringstream ss;
				ss << "lastGeneration/" <<counter << ".csv";
				breedingStock[counter]->outputAIToCSV(ss.str());
			}
			//save the 3 best AIs
			int bestScores[3] = {0, 0, 0};
			for(int counter = 1; counter < stockSize; counter++)
			{
				for(int counte = 0; counte < 3; counte++)
				{
					if(runningScores[bestScores[counte]] < runningScores[counter])
					{
						int temp = bestScores[counte];
						int tem;
						bestScores[counte] = counter;
						for(int count = counte + 1; count < 3; count++)
						{
							tem = bestScores[count];
							bestScores[count] = temp;
							temp = tem;
						}
						break;
					}
				}
			}
			breedingStock[bestScores[0]]->outputAIToCSV("bestAI.csv");
			breedingStock[bestScores[1]]->outputAIToCSV("secondBestAI.csv");
			breedingStock[bestScores[2]]->outputAIToCSV("thirdBestAI.csv");
			return;
		}
	}
	beginMatch();
}

inline bool AIBreeder::advanceTeamCounters()
{
	team2 += 1;
	if(team1 == team2) //mirror matches are pointess
	{
		team2++;
	}
	if(team2 >= stockSize)
	{
		team1 += 1;
		if(team1 >= stockSize)
		{
			return true;
		}
		team2 = 0;
	}
	return false;
}

void AIBreeder::getNextGeneration()
{
	//now that we have the scores, we need to generate more AIs
	int sum = 0;
	for(int counter = 0; counter < stockSize; counter++)
	{
		sum += runningScores[counter];
	}
	GeneticAI** nextGen = new GeneticAI*[stockSize];
	GeneticAI** parents = new GeneticAI*[2];
	for(int counter = 0; counter < stockSize; counter++)
	{
		//first decide on the parents
		int father  = rand() % sum;
		for(int counte = 0; counte < stockSize; counte++)
		{
			father -= runningScores[counte];
			if(father < 0)
			{
				father = counte; //this changes the meaning of the variable
				break;
			}
		}
		int mother = rand() % (sum - runningScores[father]);
		for(int counte = 0; counte < stockSize; counte++)
		{
			if(counte == father)
			{
				continue;
			}
			mother -= runningScores[counte];
			if(mother < 0)
			{
				mother = counte; //this changes the meaning of the variable
				break;
			}
		}
		//now that the parents are decided, make a new AI
		parents[0] = breedingStock[father];
		parents[1] = breedingStock[mother];
		nextGen[counter] = new GeneticAI(parents, 2);
	}
	delete[] parents;
	for(int counter = 0; counter < stockSize; counter++)
	{
		delete breedingStock[counter];
		runningScores[counter] = 0;
	}
	delete[] breedingStock;
	breedingStock = nextGen;
	team1 = 0;
	team2 = 1;
	currentGeneration++;
}

void AIBreeder::breedGenerations(int gensToBreed)
{
	desiredGenerations += gensToBreed;
	resolveGeneration();
}

spawnPoint::spawnPoint(int _x, int _y, char _face, int _skin, int _team, StatBlock* _stats, Item* _weapon, string _name)
{
	x = _x;
	y = _y;
	face = _face;
	skin = _skin;
	team = _team;
	stats = _stats;
	weapon = _weapon;
	name = _name;
}

spawnPoint::~spawnPoint()
{
}

void AIBreeder::resolveGeneration()
{
	//round robin, both ways
	team1 = 0;
	team2 = 1;
	midGeneration = true;
	beginMatch();
}

void AIBreeder::beginMatch()
{
	//set up the board
	arena->killAllUnits();
	forEach(spawnPoint, count, spawnPoints->getFirst())
	{
		arena->enter(new FEUnit(count->first->face, count->first->skin, count->first->team, count->first->stats,
			count->first->weapon, count->first->name), count->first->x, count->first->y);
	}
	stringstream ss;
	ss << "Generation " << currentGeneration << ": " << team1 << " vs " << team2;
	log->writeLog(ss.str());
	arena->setAI(breedingStock[team1], 1);
	arena->setAI(breedingStock[team2], 2);
	arena->uUFightToTheDeath(this);
}

void AIBreeder::takeScores(int* scores)
{
	runningScores[team1] += scores[1];
	runningScores[team2] += scores[2];
	delete[] scores;
	//now prepare the next match
	if(advanceTeamCounters())
	{
		//scores complete; breed new generation
		if(desiredGenerations > currentGeneration)
		{
			getNextGeneration();
		}
		else
		{
			//done; output the best to a file or something
			arena->setAI(nullptr, 1);
			arena->setAI(nullptr, 2);
			//save the 3 best AIs
			int bestScores[3] = {0, 0, 0};
			for(int counter = 1; counter < stockSize; counter++)
			{
				for(int counte = 0; counte < 3; counte++)
				{
					if(runningScores[bestScores[counte]] < runningScores[counter])
					{
						int temp = bestScores[counte];
						int tem;
						bestScores[counte] = counter;
						for(int count = counte + 1; count < 3; count++)
						{
							tem = bestScores[count];
							bestScores[count] = temp;
							temp = tem;
						}
						break;
					}
				}
			}
			breedingStock[bestScores[0]]->outputAIToCSV("bestAI.csv");
			breedingStock[bestScores[1]]->outputAIToCSV("secondBestAI.csv");
			breedingStock[bestScores[2]]->outputAIToCSV("thirdBestAI.csv");
			return;
		}
	}
	beginMatch();
}

inline bool AIBreeder::advanceTeamCounters()
{
	team2 += 1;
	if(team1 == team2) //mirror matches are pointess
	{
		team2++;
	}
	if(team2 >= stockSize)
	{
		team1 += 1;
		if(team1 >= stockSize)
		{
			return true;
		}
		team2 = 0;
	}
	return false;
}

void AIBreeder::getNextGeneration()
{
	//now that we have the scores, we need to generate more AIs
	int sum = 0;
	for(int counter = 0; counter < stockSize; counter++)
	{
		sum += runningScores[counter];
	}
	GeneticAI** nextGen = new GeneticAI*[stockSize];
	GeneticAI** parents = new GeneticAI*[2];
	for(int counter = 0; counter < stockSize; counter++)
	{
		//first decide on the parents
		int father  = rand() % sum;
		for(int counte = 0; counte < stockSize; counte++)
		{
			father -= runningScores[counte];
			if(father < 0)
			{
				father = counte; //this changes the meaning of the variable
				break;
			}
		}
		int mother = rand() % (sum - runningScores[father]);
		for(int counte = 0; counte < stockSize; counte++)
		{
			if(counte == father)
			{
				continue;
			}
			mother -= runningScores[counte];
			if(mother < 0)
			{
				mother = counte; //this changes the meaning of the variable
				break;
			}
		}
		//now that the parents are decided, make a new AI
		parents[0] = breedingStock[father];
		parents[1] = breedingStock[mother];
		nextGen[counter] = new GeneticAI(parents, 2);
	}
	delete[] parents;
	for(int counter = 0; counter < stockSize; counter++)
	{
		delete breedingStock[counter];
		runningScores[counter] = 0;
	}
	delete[] breedingStock;
	breedingStock = nextGen;
	team1 = 0;
	team2 = 1;
	currentGeneration++;
}

void AIBreeder::breedGenerations(int gensToBreed)
{
	desiredGenerations += gensToBreed;
	resolveGeneration();
}

spawnPoint::spawnPoint(int _x, int _y, char _face, int _skin, int _team, StatBlock* _stats, Item* _weapon, string _name)
{
	x = _x;
	y = _y;
	face = _face;
	skin = _skin;
	team = _team;
	stats = _stats;
	weapon = _weapon;
	name = _name;
}

spawnPoint::~spawnPoint()
{
}