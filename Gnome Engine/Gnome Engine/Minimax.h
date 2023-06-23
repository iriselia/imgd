#pragma once

#include "Board.h"
#include "stopWatch.h"

//replace width with reference to relevant header file later

struct Move{int column; bool isPop;};

class Movetree
{


public:
	//First, make the movetree
	//a duplicate board, n int indicting how deep the board should iterate (if another program is calling, it should be zero), a boolean stating whether it is the player's turn, the width of the board, a stopwatch to be started elsewhere, and the time limit in seconds
	Movetree(Board* fakeboard, int nextdepth, bool whichplayer, int newwidth, stopWatch *checkifdone, int timelimit); //creates an initialized board

	//Then, call this to get the next move
	Move* Minimax(); //repeatedly calls the iterative depth search and keeps track and returns the best answer

	//Then, use this to update the movetree with the opponent's move, uses a duplicate board
	void UpdateMovetree(Board* backup); //replaces the pointer to the current Movetree with one further along in the game


	~Movetree(); //deletes the Movetree, the array of potential moves, and the board it uses
	int Evalhelper(int currentminmax); //does iterative depth search for optimal play
	int getWidth();
	int getDepthToAdd(); //returns how deep the tree goes, used primarily for testing purposes


private:
	Movetree* branch; //stores potential moves
	stopWatch *timer; //timer used to keep track of time left until move must be submittd
	Board* storedboard; //board used to keep track of current/future moves
	int statevalue; //value of board at current depth; previous iterations kept track but it is now only stored at the lowest depth, can probably delete
	int depthtoadd; //check how far to iteratively search
	bool getmax; //keeps track of who's turn it is, and whether to search for the best or worst move
	bool timesup; //keeps track of whether to break the while loop in minimax
	int width; //the width of the board
	int timeleft; //time per move in seconds
};