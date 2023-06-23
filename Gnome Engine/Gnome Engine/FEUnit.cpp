#include "StdAfx.h"
#include "FEUnit.h"
#include <stdlib.h>

FEUnit::FEUnit(char _glyph, int _color, int _player, StatBlock* _stats, Item* _weapon, string _name) : Creature(_glyph, _color)
{
	player = _player;
	stats = _stats;
	weapon = _weapon;
	name = _name;
	currentHP = stats->max_hp;
	patrolState = -1;
}


FEUnit::~FEUnit(void)
{
}

bool FEUnit::attack(FEUnit* enemy, bool counter, FEConsole* log)
{
	//assume range is checked elsewhere
	int enemy_dr = enemy->getDamageReduction(weapon->type);
	int enemy_avoid = enemy->getAvoid(weapon->type);
	int base_damage = getMight();
	int base_accuracy = getAccuracy();
	bool enemy_killed = false;
	string logMessage;
	if(enemy_dr >= base_damage)
		return enemy_killed; //attack has no effect
	/*if(enemy_avoid >= base_accuracy)
		return enemy_killed; //never hit*/
	//roll to hit
	log->sendMessage(name + " attacks " + enemy->getName());
	int toHitRoll =  ( (rand() % 100) + (rand() % 100) ) / 2; //using dynamic hit
	if(toHitRoll <= (base_accuracy - enemy_avoid))
	{
		//hit
		int total_damage = base_damage - enemy_dr;
		int crit_chance = getCritChance() - enemy->getStats()->luck;
		if( ( rand() % 100) <= crit_chance )
		{
			total_damage *= 3;
			log->sendMessage("Critical hit!");
		}
		else
		{
			log->sendMessage("Hit!");
		}
		enemy_killed = enemy->modifyHP(-total_damage);
		if(enemy_killed)
		{
			log->sendMessage(enemy->getName() + " is slain!");
			delete enemy;
		}
		else
		{
			bool killed_by_counter = false;
			if (!counter) killed_by_counter = enemy->attack(this, true, log);
			if(!killed_by_counter && stats->speed >= enemy->getStats()->speed + 4) //double attack for high speed
			{
				log->sendMessage(name + " strikes again!");
				toHitRoll = ( (rand() % 100) + (rand() % 100) ) / 2; //using dynamic hit
				if(toHitRoll <= (base_accuracy - enemy_avoid))
				{
					int total_damage = base_damage - enemy_dr;
					int crit_chance = getCritChance() - enemy->getStats()->luck;
					if( ( rand() % 100) <= crit_chance )
					{
						total_damage *= 3;
						log->sendMessage("Critical hit!");
					}
					else
					{
						log->sendMessage("Hit!");
					}
					enemy_killed = enemy->modifyHP(-total_damage);
					if(enemy_killed)
					{
						log->sendMessage(enemy->getName() + " is slain!");
						delete enemy;
					}
				}
			}
		}
	}
	else
	{
		log->sendMessage("Miss!");
	}
	return enemy_killed;
}

int FEUnit::getPlayer()
{
	return player;
}

bool FEUnit::getIsActive()
{
	return isActive;
}

ColorChar FEUnit::getColorChar()
{
	ColorChar retVal = Creature::getColorChar();
	if(!isActive)
	{
		retVal.color = 0; //black when inactive
	}
	return retVal;
}

void FEUnit::activate()
{
	isActive = true;
}

void FEUnit::deactivate()
{
	isActive = false;
}

StatBlock* FEUnit::getStats()
{
	return stats;
}

string FEUnit::getName()
{
	return name;
}

int FEUnit::getDamageReduction(WEAPON_TYPE against)
{
	int dr = 0;
	switch(against)
	{
	case SWORD:
		dr = stats->defense;
		if(weapon->type == AXE) dr += 1;
		else if(weapon->type == LANCE) dr -= 1;
		break;
	case AXE:
		dr = stats->defense;
		if(weapon->type == LANCE) dr += 1;
		else if(weapon->type == SWORD) dr -= 1;
		break;
	case LANCE:
		dr = stats->defense;
		if(weapon->type == SWORD) dr += 1;
		else if(weapon->type == AXE) dr -= 1;
		break;
	case BOW:
		dr = stats->defense;
		break;
	case ANIMA:
		dr = stats->resist;
		if(weapon->type == LIGHT) dr += 1;
		else if(weapon->type == DARK) dr -= 1;
		break;
	case LIGHT:
		dr = stats->resist;
		if(weapon->type == DARK) dr += 1;
		else if(weapon->type == ANIMA) dr -= 1;
		break;
	case DARK:
		dr = stats->resist;
		if(weapon->type == ANIMA) dr += 1;
		else if(weapon->type == LIGHT) dr -= 1;
		break;
	case STAFF:
		dr = 0;
		break;
	}
	return dr;
}

int FEUnit::getAvoid(WEAPON_TYPE against)
{
	int avoid = stats->luck + ( 2 * stats->speed);
	switch(against)
	{
	case SWORD:
		if(weapon->type == AXE) avoid += 15;
		else if(weapon->type == LANCE) avoid -= 15;
		break;
	case AXE:
		if(weapon->type == LANCE) avoid += 15;
		else if(weapon->type == SWORD) avoid -= 15;
		break;
	case LANCE:
		if(weapon->type == SWORD) avoid += 15;
		else if(weapon->type == AXE) avoid -= 15;
		break;
	case BOW:
		break;
	case ANIMA:
		if(weapon->type == LIGHT) avoid += 15;
		else if(weapon->type == DARK) avoid -= 15;
		break;
	case LIGHT:
		if(weapon->type == DARK) avoid += 15;
		else if(weapon->type == ANIMA) avoid -= 15;
		break;
	case DARK:
		if(weapon->type == ANIMA) avoid += 15;
		else if(weapon->type == LIGHT) avoid -= 15;
		break;
	case STAFF:
		break;
	}
	return avoid;
}

int FEUnit::getMight()
{
	int base_damage = weapon->might;
	if(weapon->type == SWORD || weapon->type == LANCE || weapon->type == AXE || weapon->type == BOW) base_damage += stats->strength;
	else if(weapon->type == ANIMA || weapon->type == LIGHT || weapon->type == DARK || weapon->type == STAFF) base_damage += stats->magic;
	return base_damage;
}

int FEUnit::getAccuracy()
{
	return stats->luck + (2 * stats->skill) + weapon->accuracy;
}

int FEUnit::getCritChance()
{
	return (stats->skill /2) + weapon->crit;
}

bool FEUnit::modifyHP(int change)
{
	currentHP += change;
	if(currentHP >= stats->max_hp) currentHP = stats->max_hp;
	if(currentHP <= 0) return true;
	else return false;
}

int FEUnit::getCurrentHP()
{
	return currentHP;
}

bool FEUnit::inRange(int distance)
{
	return distance >= weapon->min_range && distance <= weapon->max_range;
}

Item* FEUnit::getEquipped()
{
	return weapon;
}

FEUnit* FEUnit::clone()
{
	FEUnit* retVal = new FEUnit(face, skin, player, stats, weapon, name);
	retVal->isActive = isActive;
	retVal->currentHP = currentHP;
	return retVal;
}

void FEUnit::setPatrolState(int _state) {
	patrolState = _state;
}

int FEUnit::getPatrolState() {
	return patrolState;
}
void FEUnit::setPatrolPath(Cell** _patrolPath, int _patrolPoints) {
	patrolPath = _patrolPath;
	patrolState = 0;
	patrolPoints = _patrolPoints - 1;
}

Cell** FEUnit::getPatrolPath() {
	return patrolPath;
}