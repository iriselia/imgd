//parameters of the connectX game
#include "RogueGame.h"

#define GAMENAME RogueGame

//initiallize all the pairs
/*
        COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7
*/
#define COLORMAP \
	for(int counter = 0; counter < 8; counter++) \
	{ \
		for(int counte = 0; counte < 8; counte++) \
		{ \
			init_pair(counter * 8 + counte, counte, counter); \
		} \
	}