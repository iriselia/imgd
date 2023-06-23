#pragma once
#include "creature.h"
class RandomWalker :	public Creature
{
public:
	RandomWalker(void);
	~RandomWalker(void);
	void step();
private:
	int counter;
};

