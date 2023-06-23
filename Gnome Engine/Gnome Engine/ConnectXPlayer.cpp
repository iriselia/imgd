#include "StdAfx.h"
#include "ConnectXPlayer.h"
#include "Minimax.h"
#include "stopWatch.h"
#include <iostream>

using namespace std;

ConnectXPlayer::ConnectXPlayer(void)
{
}


ConnectXPlayer::~ConnectXPlayer(void)
{
}

void ConnectXPlayer::main()
{
	//declare name:
	stopWatch *startTiming = new stopWatch(); //needed this to be accessible to the main method
	cout << "IF ConnectX Player" << endl;
	int height, width, length, turn, timeLimit;
	int column, isPop;
	cin >> height >> width >> length >> turn >> timeLimit;
	Board* toupdate = new Board(width, height, length); //also need this accessible
	Movetree* thoughts = new Movetree(toupdate->duplicate(), 0, turn == 1, width, startTiming, timeLimit); //guessing at a bunch of these parameters
	Move myMove;
	if(turn == 1)
	{
		//take first turn without provokation
		column = (width + 1) / 2; //this is an inelegant way to do that
		isPop = 1;
		
		//deleted references to move struct, only had the struct to make return statements easier


		toupdate->makeMove(column, true, isPop); //update our move tree
		cout << column << ' ' << isPop << endl;
	}
	while (true) //built in infinite loop = good design practice
	{
		cin >> column >> isPop;
		startTiming->startCounting(); //need this immediately
		if(column < 0)
			break;

		toupdate->makeMove(column, false, isPop);
		thoughts->UpdateMovetree(toupdate->duplicate());
		myMove = *thoughts->Minimax();
		column = myMove.column;
		if(myMove.isPop)
			isPop = 0;
		else
			isPop = 1;
		toupdate->makeMove(column, true, isPop);
		cout << column << ' ' << isPop << endl;
	}
}