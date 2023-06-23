#include "StdAfx.h"
#include "Board.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Board::Board(int width, int height, int length) : width(width), height(height), length(length)
{
	size = width * height;
	boardState = new token[size];
	fill(boardState, &boardState[size], EMPTY);
	stacks = new int[width];
	fill(stacks, &stacks[width], 0);
	won = false;
	lost = false;
}

Board::~Board(void)
{
	delete [] boardState;
	delete [] stacks;
}

Board::Board(istream* boardCSV)
{
	if(!boardCSV)
	{
		//invalid stream
		cout << "Invalid stream!";
	}
	height = 6; //it's kind of sloppy hardcoding this, but it will serve
	width = 7;
	size = 42;
	boardState = new token[size];
	token* currentChar = boardState;
	string line;
	while(getline(*boardCSV, line))
    {
        stringstream lineStream(line);
        string cell;
        while(getline(lineStream, cell, ','))
        {
			char temp = cell.front();
			if(temp = '0')
				*(currentChar++) = EMPTY;
			else if(temp = '1')
				*(currentChar++) = MYPIECE;
			else
				*(currentChar++) = OPPONENTSPIECE;
        }
    }
	stacks = new int[width];
	won = false;
	lost = false;
	recalculateThreats();
}

bool Board::isValidMove(int column, bool player, bool isPop)
{
	if(isPop)
	{
		if(player)
		{
			return boardState[size - width + column] == MYPIECE;
		}
		else
		{
			return boardState[size - width + column] == OPPONENTSPIECE;
		}
	}
	else
	{
		return stacks[column] < height;
	}
}

void Board::makeMove(int column, bool player, bool isPop)
{
	if(!isPop)
	{
		int justPlaced = size - width + column - width * stacks[column]++;
		token placedPiece, conversePiece, potencialThreat, converseThreat;
		if(player)
		{
			placedPiece = MYPIECE;
			conversePiece = OPPONENTSPIECE;
			potencialThreat = MYTHREAT;
			converseThreat = OPPONENTSTHREAT;
		}
		else
		{
			placedPiece = OPPONENTSPIECE;
			conversePiece = MYPIECE;
			potencialThreat = OPPONENTSTHREAT;
			converseThreat = MYTHREAT;
		}
		boardState[justPlaced] = placedPiece;
		//check if that was a winning move
		//down
		bool gameOver = false;
		bool maybeGameOver = false;
		if(stacks[column] >= length)
		{
			maybeGameOver = true;
			for(int counter = 1; counter < length; counter++)
			{
				if(boardState[justPlaced + width * counter] != placedPiece)
				{
					maybeGameOver = false;
					break;
				}
			}
			if(maybeGameOver)
				gameOver = true;
		}
		//right side
		int leftBound = max(1-length, -column);
		int rightBound = min(0, width-column-length);
		for(int counter = leftBound; counter <= rightBound && !gameOver; counter++)
		{
			maybeGameOver = true;
			//horazontal
			for(int counte = 0; counte < length; counte++)
			{
				if(boardState[justPlaced + counter + counte] != placedPiece)
				{
					maybeGameOver = false;
				}
			}
			if(maybeGameOver)
				gameOver = true;
		}
		//decending
		leftBound = max(leftBound, stacks[column] - height);
		rightBound = min(rightBound, stacks[column] - length);
		for(int counter = leftBound; counter <= rightBound && !gameOver; counter++)
		{
			maybeGameOver = true;
			for(int counte = 0; counte < length; counte++)
			{
				if(boardState[justPlaced + (counter + counte) * (width + 1)] != placedPiece)
				{
					maybeGameOver = false;
					break;
				}
			}
			if(maybeGameOver)
				gameOver = true;
		}
		//ascending
		leftBound = max(max(1-length, -column), 1-stacks[column]);
		rightBound = min(min(0, width-column-length),height-stacks[column]-length+1);
		for(int counter = leftBound; counter <= rightBound && !gameOver; counter++)
		{
			maybeGameOver = true;
			for(int counte = 0; counte < length; counte++)
			{
				if(boardState[justPlaced + (counter + counte) * (1 - width)] != placedPiece)
				{
					maybeGameOver = false;
					break;
				}
			}
			if(maybeGameOver)
				gameOver = true;
		}
		if(gameOver)
		{
			if(player)
				won = true;
			else
				lost = true;
		}
		//check for new potecial threats
		//vertical
		if(stacks[column] >= length - 1 && stacks[column] < height)
		{
			bool threatening = true;
			for(int counter = 1; counter < length - 1; counter++)
			{
				if(boardState[justPlaced + counter * width] != placedPiece)
				{
					threatening = false;
					break;
				}
			}
			if(threatening)
			{
				if(boardState[justPlaced - width] == converseThreat || boardState[justPlaced - width] == DOUBLETHREAT)
					boardState[justPlaced - width] = DOUBLETHREAT;
				else
					boardState[justPlaced - width] = potencialThreat;
			}
		}
		//horizontal
		leftBound = max(-column, 1 - length);
		rightBound = min(0, width - column - length);
		for(int count = leftBound; count <= rightBound; count++)
		{
			int threatSpace = -1;
			for(int coun = count; coun < count + length; coun++)
			{
				if(boardState[justPlaced + coun] == conversePiece)
				{
					threatSpace = -1;
					break;
				}
				else if (boardState[justPlaced + coun] != placedPiece)
				{
					if(threatSpace == -1)
					{
						threatSpace = justPlaced + coun;
					}
					else
					{
						threatSpace = -1;
						break;
					}
				}
			}
			if(threatSpace != -1)
			{
				if(boardState[threatSpace] == converseThreat)
					boardState[threatSpace] = DOUBLETHREAT;
				else
					boardState[threatSpace] = potencialThreat;
			}
		}
		//ascending
		leftBound = max(leftBound, 1 - stacks[column]);
		rightBound = min(rightBound, height - stacks[column]);
		for(int count = leftBound; count <= rightBound; count++)
		{
			int threatSpace = -1;
			for(int coun = count; coun < count + length; coun++)
			{
				if(boardState[justPlaced + coun * (1 - width)] == conversePiece)
				{
					threatSpace = -1;
					break;
				}
				else if (boardState[justPlaced + coun * (1 - width)] != placedPiece)
				{
					if(threatSpace == -1)
					{
						threatSpace = justPlaced + coun * (1 - width);
					}
					else
					{
						threatSpace = -1;
						break;
					}
				}
			}
			if(threatSpace != -1)
			{
				if(boardState[threatSpace] == converseThreat)
					boardState[threatSpace] = DOUBLETHREAT;
				else
					boardState[threatSpace] = potencialThreat;
			}
		}
		//decending
		leftBound = max(max(-column, 1 - length), stacks[column] - height);
		rightBound = min(min(0, width - column - length), stacks[column] - length);
		for(int count = leftBound; count <= rightBound; count++)
		{
			int threatSpace = -1;
			for(int coun = count; coun < count + length; coun++)
			{
				if(boardState[justPlaced + coun * (1 + width)] == conversePiece)
				{
					threatSpace = -1;
					break;
				}
				else if (boardState[justPlaced + coun * (1 + width)] != placedPiece)
				{
					if(threatSpace == -1)
					{
						threatSpace = justPlaced + coun * (1 + width);
					}
					else
					{
						threatSpace = -1;
						break;
					}
				}
			}
			if(threatSpace != -1)
			{
				if(boardState[threatSpace] == converseThreat)
					boardState[threatSpace] = DOUBLETHREAT;
				else
					boardState[threatSpace] = potencialThreat;
			}
		}
	}
	else
	{
		//piece was remove, the cunning bastard!
		stacks[column]--;
		for(int counter = 0; counter < stacks[column]; counter++)
		{
			boardState[size - width + column - width * counter] = boardState[size - 2 * width + column - width * counter];
		}
		boardState[size + column - width * stacks[column] - width] = EMPTY;
		//check if the game just ended
		//horazontally
		int leftBound = max(1+column-length, 0);
		int rightBound = min(column, width-length);
		for(int counter = leftBound; counter <= rightBound; counter++) //x
		{
			for(int counte = 0; counte < height; counte++) //y
			{
				bool maybeIWin = true;
				bool maybeILose = true;
				for(int count = 0; count < length; count++)
				{
					if(boardState[counter + counte * width + count] == MYPIECE)
					{
						maybeILose = false;
					}
					else if(boardState[counter + counte * width + count] == OPPONENTSPIECE)
					{
						maybeIWin = false;
					}
					else
					{
						maybeILose = false;
						maybeIWin = false;
						break;
					}
				}
				if(maybeIWin)
					won = true;
				if(maybeILose)
					lost = true;
			}
		}
		int topBound = length - 1;
		int bottomBound = height - length;
		//descending
		for(int counter = leftBound; counter <= rightBound; counter++) //x
		{
			for(int counte = 0; counte <= bottomBound; counte++) //y
			{
				bool maybeIWin = true;
				bool maybeILose = true;
				for(int count = 0; count < length; count++)
				{
					if(boardState[counter + counte * width + count * (width + 1)] == MYPIECE)
					{
						maybeILose = false;
					}
					else if(boardState[counter + counte * width + count * (width + 1)] == OPPONENTSPIECE)
					{
						maybeIWin = false;
					}
					else
					{
						maybeILose = false;
						maybeIWin = false;
						break;
					}
				}
				if(maybeIWin)
					won = true;
				if(maybeILose)
					lost = true;
			}
		}
		//ascending
		for(int counter = leftBound; counter <= rightBound; counter++) //x
		{
			for(int counte = topBound; counte < height; counte++) //y
			{
				bool maybeIWin = true;
				bool maybeILose = true;
				for(int count = 0; count < length; count++)
				{
					if(boardState[counter + counte * width + count * (1 - width)] == MYPIECE)
					{
						maybeILose = false;
					}
					else if(boardState[counter + counte * width + count * (1 - width)] == OPPONENTSPIECE)
					{
						maybeIWin = false;
					}
					else
					{
						maybeILose = false;
						maybeIWin = false;
						break;
					}
				}
				if(maybeIWin)
					won = true;
				if(maybeILose)
					lost = true;
			}
		}
		if(won || lost)
		{
			//anything else is irrelevent: might as well return
				return;
		}
		//if a row just dropped, recalculate all potencial threats in range
		//might as well just recalculate all threats since edge threats would have to look at the far reaches of the board anyway.
		//first clear the board of threats since their causes may have moved
		for(int counter = 0; counter < width; counter++) //x
		{
			for(int counte = 0; counte < height - stacks[counter]; counte++) //y
			{
				boardState[width * counte + counter] = EMPTY;
			}
		}
		//now check the vertical threats in each row
		for(int counter = 0; counter < width; counter++) //x
		{
			if(stacks[counter] >= length - 1 && stacks[counter] < height)
			{
				bool isThreat = true;
				token streak = boardState[counter + width * (height - stacks[counter])];
				for(int counte = 1; counte < length; counte++) //y
				{
					if(boardState[counter + width * (height - stacks[counter] + counte - 1)] != streak)
					{
						isThreat = false;
						break;
					}
				}
				if(isThreat)
				{
					if(streak == MYPIECE)
					{
						boardState[counter + size - width * stacks[counter] - width] = MYTHREAT;
					}
					else if(streak == OPPONENTSPIECE) //this should always be true if it gets here, but just in case
					{
						boardState[counter + size - width * stacks[counter] - width] = OPPONENTSTHREAT;
					}
				}
			}
		}
		//now check vertical
		for(int counter = 0; counter <= width - length; counter++) //x
		{
			for(int counte = 0; counte < height; counte++) //y
			{
				int emptySlot = -1;
				bool maybeMe = true;
				bool maybeOp = true;
				for(int count = 0; count < length; count++) //slot along row
				{
					if(boardState[counter + width * counte + count] == MYPIECE)
					{
						maybeOp = false;
					}
					else if(boardState[counter + width * counte + count] == OPPONENTSPIECE)
					{
						maybeMe = false;
					}
					else if(emptySlot == -1) //empty space
					{
						emptySlot = count;
					}
					else
					{
						emptySlot = -1;
						break;
					}
				}
				if(emptySlot != -1)
				{
					if(maybeOp)
					{
						if(boardState[counter + width * counte + emptySlot] == MYTHREAT)
						{
							boardState[counter + width * counte + emptySlot] = DOUBLETHREAT;
						}
						else
						{
							boardState[counter + width * counte + emptySlot] = OPPONENTSTHREAT;
						}
					}
					else if(maybeMe)
					{
						if(boardState[counter + width * counte + emptySlot] == OPPONENTSTHREAT)
						{
							boardState[counter + width * counte + emptySlot] = DOUBLETHREAT;
						}
						else
						{
							boardState[counter + width * counte + emptySlot] = MYTHREAT;
						}
					}
				}
			}
		}
		//check descending
		for(int counter = 0; counter <= width - length; counter++) //x
		{
			for(int counte = 0; counte <= height - length; counte++) //y
			{
				int emptySlot = -1;
				bool maybeMe = true;
				bool maybeOp = true;
				for(int count = 0; count < length; count++) //slot along row
				{
					if(boardState[counter + width * (counte + count) + count] == MYPIECE)
					{
						maybeOp = false;
					}
					else if(boardState[counter + width * (counte + count) + count] == OPPONENTSPIECE)
					{
						maybeMe = false;
					}
					else if(emptySlot == -1) //empty space
					{
						emptySlot = count;
					}
					else
					{
						emptySlot = -1;
						break;
					}
				}
				if(emptySlot != -1)
				{
					if(maybeOp)
					{
						if(boardState[counter + width * (counte + emptySlot) + emptySlot] == MYTHREAT)
						{
							boardState[counter + width * (counte + emptySlot) + emptySlot] = DOUBLETHREAT;
						}
						else
						{
							boardState[counter + width * (counte + emptySlot) + emptySlot] = OPPONENTSTHREAT;
						}
					}
					else if(maybeMe)
					{
						if(boardState[counter + width * (counte + emptySlot) + emptySlot] == OPPONENTSTHREAT)
						{
							boardState[counter + width * (counte + emptySlot) + emptySlot] = DOUBLETHREAT;
						}
						else
						{
							boardState[counter + width * (counte + emptySlot) + emptySlot] = MYTHREAT;
						}
					}
				}
			}
		}
		//check ascending
		for(int counter = 0; counter <= width - length; counter++) //x
		{
			for(int counte = length - 1; counte < height; counte++) //y
			{
				int emptySlot = -1;
				bool maybeMe = true;
				bool maybeOp = true;
				for(int count = 0; count < length; count++) //slot along row
				{
					if(boardState[counter + width * (counte - count) + count] == MYPIECE)
					{
						maybeOp = false;
					}
					else if(boardState[counter + width * (counte - count) + count] == OPPONENTSPIECE)
					{
						maybeMe = false;
					}
					else if(emptySlot == -1) //empty space
					{
						emptySlot = count;
					}
					else
					{
						emptySlot = -1;
						break;
					}
				}
				if(emptySlot != -1)
				{
					if(maybeOp)
					{
						if(boardState[counter + width * (counte - emptySlot) + emptySlot] == MYTHREAT)
						{
							boardState[counter + width * (counte - emptySlot) + emptySlot] = DOUBLETHREAT;
						}
						else
						{
							boardState[counter + width * (counte - emptySlot) + emptySlot] = OPPONENTSTHREAT;
						}
					}
					else if(maybeMe)
					{
						if(boardState[counter + width * (counte - emptySlot) + emptySlot] == OPPONENTSTHREAT)
						{
							boardState[counter + width * (counte - emptySlot) + emptySlot] = DOUBLETHREAT;
						}
						else
						{
							boardState[counter + width * (counte - emptySlot) + emptySlot] = MYTHREAT;
						}
					}
				}
			}
		}
	}
}

#ifdef EVAL1

int Board::evaluate()
{
	//this algorithm will need a lot of refining; consider this just a placeholder; not that refining ever happened
	if(won && lost)
		return 0;
	else if(won)
		return 1000;
	else if(lost)
		return -1000;
	int netThreats = 0;
	for(int counter = 0; counter < size; counter++)
	{
		if(boardState[counter] == MYTHREAT)
			netThreats += 10;
		else if(boardState[counter] == OPPONENTSTHREAT)
			netThreats -= 10;
	}
	for(int counter = 0; counter < width; counter++) //x
	{
		for(int counte = height - stacks[counter] - 1; counte > 0; counte--)
		{
			if(boardState[counte * width + counter] == MYTHREAT && (boardState[counte * width + counter - width] == MYTHREAT || boardState[counte * width + counter - width] == DOUBLETHREAT))
			{
				netThreats += 100 * (height + stacks[counter] - counte); //lower is better, but high is still pretty darn good
				break;
			}
			else if(boardState[counte * width + counter] == OPPONENTSTHREAT && (boardState[counte * width + counter - width] == OPPONENTSTHREAT || boardState[counte * width + counter - width] == DOUBLETHREAT))
			{
				netThreats -= 100 * (height + stacks[counter] - counte);
				break;
			}
		}
	}

	//middle columns are more powerful
	for(int counter = length - 1; counter <= width - length; length++)
	{
		for( int counte = 0; counte < height; counte++)
		{
			if(boardState[counte * width + counter] == MYPIECE)
				netThreats++;
			else if(boardState[counte * width + counter] == OPPONENTSPIECE)
				netThreats--;
		}
	}

	//bottom row is still pretty good
	for(int counter = 0; counter < width; counter++)
	{
		if(boardState[size - width + counter] == MYPIECE)
			netThreats++;
		else if(boardState[size - width + counter] == OPPONENTSPIECE)
			netThreats--;
	}

	return netThreats;
}

#endif

#ifdef EVAL2

int Board::evaluate()
{
	//this algorithm will need a lot of refining; consider this just a placeholder
	if(won && lost)
		return 0;
	else if(won)
		return 1000;
	else if(lost)
		return -1000;
	int netThreats = 0;
	for(int counter = 0; counter < size; counter++)
	{
		if(boardState[counter] == MYTHREAT)
			netThreats += 10;
		else if(boardState[counter] == OPPONENTSTHREAT)
			netThreats -= 10;
	}
	for(int counter = 0; counter < width; counter++) //x
	{
		for(int counte = height - stacks[counter] - 1; counte > 0; counte--)
		{
			if(boardState[counte * width + counter] == MYTHREAT && (boardState[counte * width + counter - width] == MYTHREAT || boardState[counte * width + counter - width] == DOUBLETHREAT))
			{
				netThreats += 100 * (height + stacks[counter] - counte); //lower is better, but high is still pretty darn good
				break;
			}
			else if(boardState[counte * width + counter] == OPPONENTSTHREAT && (boardState[counte * width + counter - width] == OPPONENTSTHREAT || boardState[counte * width + counter - width] == DOUBLETHREAT))
			{
				netThreats -= 100 * (height + stacks[counter] - counte);
				break;
			}
		}
	}

	//middle columns are more powerful
	for(int counter = length - 1; counter <= width - length; length++)
	{
		for( int counte = 0; counte < height; counte++)
		{
			if(boardState[counte * width + counter] == MYPIECE)
				netThreats += 10;
			else if(boardState[counte * width + counter] == OPPONENTSPIECE)
				netThreats -= 10;
		}
	}

	return netThreats;
}

#endif

Board* Board::duplicate()
{
	Board* imprint = new Board(width, height, length);
	for(int counter = 0; counter < size; counter++)
	{
		imprint->boardState[counter] = boardState[counter];
	}
	for(int counter = 0; counter < width; counter++)
	{
		imprint->stacks[counter] = stacks[counter];
	}
	imprint->won = won;
	imprint->lost = lost;
	return imprint;
}

void Board::recalculateThreats()
{
	//first clear away any listed threats
	for(int counter = 0; counter < size; counter++)
	{
		if(boardState[counter] != MYPIECE && boardState[counter] != OPPONENTSPIECE)
			boardState[counter] = EMPTY;
	}

	//check vertical threats
	for(int counter = 0; counter < width; counter++) //x
	{
		if(stacks[counter] < height && stacks[counter] >= length - 1)
		{
			bool maybeOpThreat = true;
			bool maybeMyThreat = true;
			for(int counte = 1; counte < length; counte++)
			{
				if(boardState[counter + size - (stacks[counter] - counte) * width] = MYPIECE)
				{
					maybeOpThreat = false;
				}
				else if(boardState[counter + size - (stacks[counter] - counte) * width] = OPPONENTSPIECE)
				{
					maybeMyThreat = false;
				}
			}
			if(maybeMyThreat && maybeOpThreat)
			{
				boardState[counter + size - stacks[counter] * width] = DOUBLETHREAT;
			}
			else if(maybeMyThreat)
			{
				if(boardState[counter + size - stacks[counter] * width] == OPPONENTSTHREAT)
					boardState[counter + size - stacks[counter] * width] = DOUBLETHREAT;
				else
					boardState[counter + size - stacks[counter] * width] = MYTHREAT;
			}
			else if(maybeOpThreat)
			{
				if(boardState[counter + size - stacks[counter] * width] == MYTHREAT)
					boardState[counter + size - stacks[counter] * width] = DOUBLETHREAT;
				else
					boardState[counter + size - stacks[counter] * width] = OPPONENTSTHREAT;
			}
		}
	}

	//now consider each line of length in turn
	for(int slope = -1; slope <= 1; slope++)
	{
		for(int counter = (slope < 0 ? length - 1 : 0); counter < (slope > 0 ? height - length + 1 : height); counter++) //y
		{
			for(int counte = 0; counte <= width - length; counte++) //x
			{
				bool maybeMyThreat = true;
				bool maybeOpThreat = true;
				int hole = -1;
				for(int count = 0; count < length; count++)
				{
					if(boardState[counter * width + counte + count * (1 + width * slope)] == MYPIECE)
					{
						maybeOpThreat = false;
					}
					else if(boardState[counter * width + counte + count * (1 + width * slope)] == MYPIECE)
					{
						maybeMyThreat = false;
					}
					else if(hole = -1)
					{
						hole = count;
					}
					else
					{
						hole = -1;
						break;
					}
				}
				//only one empty space in this set
				if(hole >= 0)
				{
					if(maybeMyThreat && maybeOpThreat)
					{
						boardState[counter * width + counte + hole * (1 + width * slope)] = DOUBLETHREAT;
					}
					else if(maybeMyThreat)
					{
						if(boardState[counter * width + counte + hole * (1 + width * slope)] == OPPONENTSTHREAT)
							boardState[counter * width + counte + hole * (1 + width * slope)] = DOUBLETHREAT;
						else
							boardState[counter * width + counte + hole * (1 + width * slope)] = MYTHREAT;
					}
					else if(maybeOpThreat)
					{
						if(boardState[counter * width + counte + hole * (1 + width * slope)] == MYTHREAT)
							boardState[counter * width + counte + hole * (1 + width * slope)] = DOUBLETHREAT;
						else
							boardState[counter * width + counte + hole * (1 + width * slope)] = OPPONENTSTHREAT;
					}
				}
			}
		}
	}
}