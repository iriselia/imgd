#pragma once
#include "creature.h"
#include <string>
#include <vector>
#include "FEConsole.h"

using namespace std;

#define NO_PLAYER 0
#define RED_PLAYER 1
#define BLUE_PLAYER 2

struct Proficiency
{
	int sword;
	int axe;
	int lance;
	int anima;
	int light;
	int dark;
	int staff;

	Proficiency(){}
	Proficiency(int _sword, int _axe, int _lance, int _anima, int _light, int _dark, int _staff)
	{
		sword = _sword;
		axe = _axe;
		lance = _lance;
		anima = _anima;
		light = _light;
		dark = _dark;
		staff = _staff;
	}
};

struct StatBlock
{
	int max_hp;
	int strength;
	int magic;
	int defense;
	int resist;
	int skill;
	int speed;
	int luck;
	int move;
	Proficiency proficiencies;

	StatBlock(){}
	StatBlock( int _hp, int _strength, int _magic, int _defense, int _resist, int _skill, int _speed, int _luck, int _move, Proficiency _proficiencies)
	{
		max_hp = _hp;
		strength = _strength;
		magic = _magic;
		defense = _defense;
		resist = _resist;
		skill = _skill;
		speed = _speed;
		luck = _luck;
		move = _move;
		proficiencies = _proficiencies;
	}
};

using namespace std;

class FEUnit :	public Creature
{
public:
	FEUnit(char _face, int _skin, int _team, StatBlock* _stats, Item* _weapon, string _name);
	~FEUnit(void);
	bool attack(FEUnit* enemy, bool counter, FEConsole* log);
	virtual ColorChar getColorChar();
	int getPlayer();
	bool getIsActive();
	void activate(); //make active
	void deactivate(); //make inactive
	string getName();
	//stat getters
	StatBlock* getStats();
	int getDamageReduction(WEAPON_TYPE);
	int getAvoid(WEAPON_TYPE);
	int getMight(); //given weapon type, strength or magic
	int getAccuracy(); //given skill, luck, and weapon accuracy
	int getCritChance(); //given skill and weapon
	int getCurrentHP();
	void setPatrolState(int _state);
	int getPatrolState();
	void setPatrolPath(Cell** _patrolPath, int _patrolPoints);
	Cell** getPatrolPath();
	int getPatrolPoints();

	void giveItem(Item* _item);
	Pile* getInventory();
	Item* getEquipped();

	bool inRange(int distance);

	bool modifyHP(int change); //returns true if unit is destroyed, otherwise false
	
	FEUnit* clone();
private:
	StatBlock* stats;
	Item* weapon;
	int currentHP;

	bool isActive; //if this unit has moved yet this turn; true means it can still move
	int player; //the number of the player who owns this unit
	string name; //like "Sir Fred" or whatever this individual guy is called

	int patrolState;
	int patrolPoints;
	Cell** patrolPath;
	
//friend class FEBattleField;
};
