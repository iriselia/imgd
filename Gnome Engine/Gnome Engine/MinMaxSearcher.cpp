#include "StdAfx.h"
#include "MinMaxSearcher.h"
#include <algorithm>

using namespace std;

MinMaxSearcher::MinMaxSearcher(int width, int height, int length): width(width)
{
	rootBoard = new Board(width, height, length);
}


MinMaxSearcher::~MinMaxSearcher(void)
{
	delete rootBoard;
}

action MinMaxSearcher::findBestMove(int depth)
{
	return findMaxMove(rootBoard, depth, true).myMove;
}

void MinMaxSearcher::makeMove(int column, bool isMyMove, bool isPop)
{
	rootBoard->makeMove(column, isMyMove, isPop);
}

void MinMaxSearcher::makeMove(action in)
{
	rootBoard->makeMove(in.column, in.isMyMove, in.isPop);
}

evaledMove MinMaxSearcher::findMaxMove(Board* in, int depth, bool isMyTurn)
{
	//int leakTester = 0;
	Board** childBoards = new Board*[width * 2];
	for(int counter = 0; counter < width * 2; counter++)
	{
		childBoards[counter] = nullptr;
	}
	for(int counter = 0; counter < width; counter++)
	{
		if(rootBoard->isValidMove(counter, isMyTurn, false))
		{
			childBoards[counter] = rootBoard->duplicate();
			childBoards[counter]->makeMove(counter, isMyTurn, false);
			//leakTester++;
		}
		if(rootBoard->isValidMove(counter, isMyTurn, true))
		{
			childBoards[counter + width] = rootBoard->duplicate();
			childBoards[counter + width]->makeMove(counter, isMyTurn, true);
			//leakTester++;
		}
	}
	int bestMove = width * 2;
	int maxValue = -1001;
	evaledMove returnMove;
	if(depth > 0)
	{
		for(int counter = 0; counter < width * 2; counter++)
		{
			if(childBoards[counter] != nullptr)
			{
				evaledMove temp = findMinMove(childBoards[counter], depth - 1, !isMyTurn);
				if(temp.value > maxValue)
				{
					bestMove = counter;
					maxValue = temp.value;
				}
				delete childBoards[counter];
				//leakTester--;
			}
		}
		returnMove.myMove.column = bestMove % width;
		returnMove.myMove.isMyMove = true;
		returnMove.myMove.isPop = bestMove >= width;
		returnMove.value = maxValue;
	}
	else
	{
		//don't go any deeper
		for(int counter = 0; counter < width * 2; counter++)
		{
			if(childBoards[counter] != nullptr)
			{
				int temp = childBoards[counter]->evaluate();
				if(temp > maxValue)
				{
					bestMove = counter;
					maxValue = temp;
				}
				delete childBoards[counter];
				//leakTester--;
			}
		}
		returnMove.myMove.column = bestMove % width;
		returnMove.myMove.isMyMove = true;
		returnMove.myMove.isPop = bestMove >= width;
		returnMove.value = maxValue;
	}
	delete [] childBoards;
	return returnMove;
}

evaledMove MinMaxSearcher::findMinMove(Board* in, int depth, bool isMyTurn)
{
	//int leakTester = 0;
	Board** childBoards = new Board*[width * 2];
	//initialize all possible next boards to null
	for(int counter = 0; counter < width * 2; counter++)
	{
		childBoards[counter] = nullptr;
	}
	//generate all possible moves
	for(int counter = 0; counter < width; counter++)
	{
		if(rootBoard->isValidMove(counter, isMyTurn, false))
		{
			childBoards[counter] = rootBoard->duplicate();
			childBoards[counter]->makeMove(counter, isMyTurn, false);
			//leakTester++;
		}
		if(rootBoard->isValidMove(counter, isMyTurn, true))
		{
			childBoards[counter + width] = rootBoard->duplicate();
			childBoards[counter + width]->makeMove(counter, isMyTurn, true);
			//leakTester++;
		}
	}
	int bestMove = width * 2; //this should be overwritten
	int minValue = 1001;
	evaledMove returnMove;
	//this is for when we need to go deeper
	if(depth > 0)
	{
		for(int counter = 0; counter < width * 2; counter++)
		{
			if(childBoards[counter] != nullptr)
			{
				evaledMove temp = findMaxMove(childBoards[counter], depth - 1, !isMyTurn);
				if(temp.value < minValue)
				{
					bestMove = counter;
					minValue = temp.value;
				}
				delete childBoards[counter];
				//leakTester--;
			}
		}
		returnMove.myMove.column = bestMove % width;
		returnMove.myMove.isMyMove = true;
		returnMove.myMove.isPop = bestMove >= width;
		returnMove.value = minValue;
	}
	else
	{
		//don't go any deeper
		for(int counter = 0; counter < width * 2; counter++)
		{
			if(childBoards[counter] != nullptr)
			{
				int temp = childBoards[counter]->evaluate();
				if(temp < minValue)
				{
					bestMove = counter;
					minValue = temp;
				}
				delete childBoards[counter];
				//leakTester--;
			}
		}
		returnMove.myMove.column = bestMove % width;
		returnMove.myMove.isMyMove = true;
		returnMove.myMove.isPop = bestMove >= width;
		returnMove.value = minValue;
	}
	return returnMove;
}