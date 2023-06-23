#pragma once
#include "creature.h"
class NumWalker :	public Creature
{
public:
	NumWalker(void);
	~NumWalker(void);
	void step();
	void takeInput(char in);

private:
	int counter;
};

