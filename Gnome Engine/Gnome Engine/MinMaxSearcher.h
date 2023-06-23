#pragma once

#include "Board.h"

struct action
{
	int column;
	bool isPop;
	bool isMyMove;
};

struct evaledMove
{
	action myMove;
	int value;
};

class MinMaxSearcher
{
public:
	MinMaxSearcher(int width, int height, int length);
	~MinMaxSearcher(void);
	action findBestMove(int depth);
	void makeMove(action in);
	void makeMove(int column, bool isMyMove, bool isPop);
private:
	evaledMove findMaxMove(Board* in, int depth, bool isMyTurn);
	evaledMove findMinMove(Board* in, int depth, bool isMyTurn);
	Board* rootBoard;
	Board** childBoards;
	int width;
};

