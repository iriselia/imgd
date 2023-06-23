#pragma once
#include "GeneticAI.h"
#include "FEUnit.h"
#include <string>
#include "LogManager.h"

class spawnPoint //this was a struct but I wanted a constructor
{
public:
	spawnPoint(int _x, int _y, char _face, int _skin, int _team, StatBlock* _stats, Item* _weapon, string _name);
	~spawnPoint();
	int x;
	int y;
	char face;
	int skin;
	int team;
	StatBlock* stats;
	Item* weapon;
	string name;
};

class AIBreeder
{
public:
	AIBreeder(int _stockSize, StatBlock** _idStats, int _numBlocks, FEBattleField* arena, LinkedList<spawnPoint>* _spawnPoints);
	AIBreeder(int _stockSize, StatBlock** _idStats, int _numBlocks, FEBattleField* arena, LinkedList<spawnPoint>* _spawnPoints, string loadDirectory);
	~AIBreeder(void);
	void resolveGeneration(); //have all existing ais fight and create new ones based on the results
	void takeScores(int* scores);
	void breedGenerations(int generations);
private:
	int currentGeneration;
	int stockSize;
	int numBlocks;
	GeneticAI** breedingStock;
	int* runningScores;
	StatBlock** idStats;
	FEBattleField* arena;
	LinkedList<spawnPoint>* spawnPoints;
	bool midGeneration;
	int team1;
	int team2;
	void beginMatch();
	bool advanceTeamCounters(); //true indicates done
	void getNextGeneration();
	int desiredGenerations;
	LogManager* log;

//debug
	friend class FEBattleField;
};

