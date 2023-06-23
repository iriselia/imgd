#include "StdAfx.h"
#include "FEConsole.h"
#include <string>

using namespace std;

FEConsole::FEConsole(int width, int height)
{
	outputHeight = height;
	outputWidth = width;
	size = height * width;
	outputStack = new char*[height];
	for(int counter = 0; counter < height; counter++)
	{
		outputStack[counter] = new char[width];
		for(int counte = 0; counte < width; counte++)
		{
			outputStack[counter][counte] = ' ';
		}
	}
	nextLine = 0;
}


FEConsole::~FEConsole(void)
{
}

ColorChar FEConsole::getColorChar(int x, int y)
{
	ColorChar retVal = ColorChar();
	retVal.color = 7; //white on black
	retVal.glyph = outputStack[(y + nextLine) % outputHeight][x];
	return retVal;
}

void FEConsole::sendMessage(string message)
{
	for(int counter = 0; counter < outputWidth; counter++)
	{
		if(counter < message.length())
		{
			outputStack[nextLine][counter] = message[counter];
		}
		else
		{
			outputStack[nextLine][counter] = ' ';
		}
	}
	nextLine = (nextLine + 1) % outputHeight;
}