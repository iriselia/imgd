#pragma once

#include <iostream>

using namespace std;

#define EVAL2

enum token {MYPIECE = 1, OPPONENTSPIECE = 2, MYTHREAT = 6, OPPONENTSTHREAT = 7, DOUBLETHREAT = 8, EMPTY = 5};

class Board
{

friend class ConnectXBoard;

public:
	Board(int width, int height, int length); //maybe make these global or at least static
	Board(istream* boardCSV);
	~Board();
	bool isValidMove(int column, bool player, bool isPop); //player = true means it's our move, else opponent's move
	void makeMove(int column, bool player, bool isPop); //array style indexing on columns: leftmost is index 0
	int evaluate(); //bigger number indicates stronger board state
	Board* duplicate(); //returns a new board with the same state as this one; rememeber to delete it when you're done
	void recalculateThreats();
private:
	int width; //width of board
	int height; //height of board
	int size; //number of spaces in the board
	int length; //lenght of winning streak
	token* boardState; //map of board
	int* stacks; //skyline of board
	bool won;
	bool lost;
};

