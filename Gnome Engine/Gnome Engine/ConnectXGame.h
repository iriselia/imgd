#pragma once
#include "game.h"

#define HEIGHT 6
#define WIDTH 7
#define LENGTH 4

class ConnectXGame :	public Game
{
public:
	ConnectXGame(void);
	~ConnectXGame(void);
	void initialize(void);
};
