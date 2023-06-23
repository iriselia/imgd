//parameters of the connectX game
#include "ConnectXGame.h"

#define GAMENAME ConnectXGame

#define COLORMAP \
	init_pair(1, COLOR_RED, COLOR_BLACK);    /* player 1    */ \
	init_pair(2, COLOR_GREEN, COLOR_BLACK);   /* player 2    */ \
	init_pair(3, COLOR_YELLOW, COLOR_BLACK); /* boarder     */ \
	init_pair(4, COLOR_WHITE, COLOR_BLACK);  /* meta info   */ \
	init_pair(5, COLOR_BLACK, COLOR_BLACK);  /* blank space */ \
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK); /* red threat */ \
	init_pair(7, COLOR_CYAN, COLOR_BLACK); /* green threat */ \
	init_pair(8, COLOR_BLUE, COLOR_BLACK); /* double threat */