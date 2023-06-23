#pragma once

struct ColorChar //not sure if there's a better place to put this struct
{
	int color;
	char glyph;
};

class Map
{
public:
	Map(void);
	virtual ~Map(void);
	virtual void takeInput(char in); //a map will only get input if it asks for it; remember to unregister when you're done
	virtual void step(void); //advance the map a turn; the logic is handled by the subclass; maybe move some functionality into a superclass?
	virtual ColorChar getColorChar(int x, int y); //returns the character and color at this point
};

