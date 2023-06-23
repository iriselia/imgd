#pragma once
#include "map.h"
#include <string>

using namespace std;

class FEConsole :	public Map
{
public:
	FEConsole(int height, int width);
	~FEConsole(void);
	ColorChar getColorChar(int x, int y);
	void sendMessage(string message); //screw it; c style strings are fine
private:
	char** outputStack;
	int outputHeight;
	int outputWidth;
	int size;
	int nextLine;
};

