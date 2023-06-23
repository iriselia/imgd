#pragma once

//this is a class that sets up a game; this is pretty much completely virtual since it will be redefined for each game

class Game
{
public:
	Game(void);
	virtual ~Game(void);
	virtual void initialize(void); //set up the initial gamestate
};

