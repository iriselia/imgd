#pragma once
#include "dungeon.h"
#include "FEUnit.h"
#include "FEStatViewer.h"
#include "FEAIInterface.h"
#include "FEConsole.h"

class FEAIInterface;
class AIBreeder;

class FEBattleField :	public Dungeon
{
public:
	FEBattleField(int numberOfPlayers, int height, int width, FEStatViewer* statWindow, FEConsole* log);
	~FEBattleField(void);
	void step();
	void takeInput(char in);
	ColorChar getColorChar(int x, int y);
	bool enter(Creature* newCreature, int x, int y);
	void exit(int x, int y);
	void setAI(FEAIInterface* newAI, int faction);
	LinkedList<FEUnit>* FEBattleField::getPlayerUnits(int player);
	int FEBattleField::getNumPlayers();
	int InitTerrain(int map[], int x, int y);
	bool* getValidFinalPositions(FEUnit* unitToMove);
	bool* getValidAttackPositions(FEUnit* unitToMove);
	Cell** getValidEnemiesToAttack(bool* attackPositions);
	FEBattleField* clone();//makes a duplicate of this battlefield with new units and all; probably not worth using but you guys want it
	static int getDistance(int startX, int startY, int endX, int endY);
	LinkedList<FEUnit>* getPossibleAttackTargets(int x, int y, int player, Item* weapon);
	void killAllUnits(); //leaves rocks in place
	void uUFightToTheDeath(AIBreeder* scoreWanter); //fights a battle returns an array of the scores of the teams
	int getCurrentTurn(); //this probably won't be called much
private:
	FEUnit* activeUnit; //this unit is selected
	int cursorX;
	int cursorY;
	//int moveCounter;
	int flashCounter;
	int currentTurn; //whose turn it is
	int numPlayers;
	//int unitsOnField;
	LinkedList<FEUnit>** unitCounts; //for determining when the game is over
	bool attackMode; //true when done moving but about to attack
	void finishMoving(); //to be called when a unit becomes inactive
	FEStatViewer* statWindow;
	FEAIInterface** factionAIs;
	LinkedList<FEUnit>* unitsToMove;
	bool canMove(FEUnit* movingUnit, int x, int y);
	bool canAttack(FEUnit* attackingUnit, int x, int y);
	bool canAttackSpace(FEUnit* attackingUnit, int x, int y);
	FEConsole* attacklog;
	int turnCounter;
	void endMatch(); //this will be used to reset the map for the next round
	AIBreeder* scoreKeeper;
	bool matchOver;
};