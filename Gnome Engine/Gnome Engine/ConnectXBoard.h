#pragma once
#include "map.h"
#include "CycleManager.h"
#include "Board.h"
#include "LogManager.h"
#include "Minimax.h"
#include "stopWatch.h"

class ConnectXBoard :	public Map
{
public:
	ConnectXBoard(int width, int height, int length);
	~ConnectXBoard(void);
	void takeInput(char in);
	void step(void);
	ColorChar getColorChar(int x, int y); //returns the character and color at this point
private:
	int height;
	int width;
	int length; //of winning streak
	bool turn; //true if player 1, false if player 2
	Board* board;
	CycleManager* cycle;
	LogManager* log;
	bool popAllowed; //true if players are allowed to pop pieces
	bool popMode; //true if the next move is set to be a pop
	int flashCountDown; //counts down from 30 to animate the flashing poppable tokens
	Movetree* thoughts;
	stopWatch* timer;
	Board* mentalBoard;
};

