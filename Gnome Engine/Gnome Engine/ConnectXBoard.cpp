#include "StdAfx.h"
#include "ConnectXBoard.h"
#include "curses.h"

ConnectXBoard::ConnectXBoard(int width, int height, int length) : width(width), height(height), length(length)
{
	turn = true; //true if player 1, false if player 2
	board = new Board(width, height, length);
	mentalBoard = board->duplicate();
	cycle = CycleManager::getInstance();
	log = LogManager::getInstance();
	popAllowed = true; //maybe replace this with conditional compilation from the GameInfo
	popMode = false;
	flashCountDown = 30;
	timer = new stopWatch();
	thoughts = new Movetree(mentalBoard->duplicate(), 0, true, width, timer, 4);
}


ConnectXBoard::~ConnectXBoard(void)
{
	delete board;
	delete timer;
}

void ConnectXBoard::takeInput(char in)
{
	if(in > 96 && in <= 96 + width)
	{
		bool moveHappened = false;
		int x = in - 97;
		if(board->isValidMove(x, turn, popMode))
		{
			board->makeMove(x, turn, popMode);
			if(board->won || board->lost)
			{
				cycle->endGame();
			}
			//advance the turn
			turn = !turn;
		}
	}
	else if(in == 'p' && popAllowed) //as written, if width is at least 16, popping will not be possible since 'p' will drop a token in the 16th column instead
	{								//this is acceptable because 16 is out of the expected range of width
		popMode = !popMode;
	}
	//log->writeLog("ConnectXBoard:Evaluated at " + board->evaluate() + "");
}

void ConnectXBoard::step(void)
{
	if(flashCountDown > 1)
	{
		flashCountDown -= 1;
	}
	else
	{
		flashCountDown = 30;
	}
}

ColorChar ConnectXBoard::getColorChar(int x, int y)
{
	//boarder of +s, -s and |s
	//inner squares are @s of color based on owner
	ColorChar mark = ColorChar();
	if((x == 0 || x == width + 1) && (y == 0 || y == height + 1))
	{
		if(turn)
			mark.color = MYPIECE;
		else
			mark.color = OPPONENTSPIECE;
		mark.glyph = '+';
	}
	else if ((x == 0 || x == width + 1) && (y > 0 && y <= height))
	{
		mark.color = 3;
		mark.glyph = '|';
	}
	else if ((x > 0 && x <= width) && (y == 0 || y == height + 1))
	{
		mark.color = 3;
		mark.glyph = '-';
	}
	else if (x == -1 && y > 0 && y <= height)
	{
		mark.color = 4;
		mark.glyph = 49 - y + height;
	}
	else if (x >= 1 && x <= width && y == height + 2)
	{
		mark.color = 4;
		mark.glyph = 96 + x;
	}
	else if (x >= 1 && x <= width && y >= 1 && y < height)
	{
		//look up slot on board and display it
		token state = board->boardState[x - 1 + width * y - width];
		mark.color = state;
		if(state > 5)
			mark.glyph = 't';
		else
			mark.glyph = '@';
	}
	else if (x >= 1 && x <= width && y >= 1 && y == height)
	{
		//bottom row, so might be flashing
		token state = board->boardState[x - 1 + width * y - width];
		mark.color = state;
		if(state > 5)
			mark.glyph = 't';
		else
			mark.glyph = '@';
		if(popMode && flashCountDown < 15 && ((turn && mark.color == MYPIECE) || (!turn && mark.color == OPPONENTSPIECE)))
			mark.color = 4;
	}
	else
	{
		mark.color = EMPTY;
		mark.glyph = ' ';
	}
	return mark;
}