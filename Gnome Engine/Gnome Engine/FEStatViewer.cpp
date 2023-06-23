#include "StdAfx.h"
#include "FEStatViewer.h"

#define HEIGHT 8
#define WIDTH 12

FEStatViewer::FEStatViewer(void)
{
	screen = new char[12 * 8 + 1];
	strncpy(screen, "Na:         Hp:  /      St:   Ma:   De:   Re:   Sk:   Sp:   Lk:   Cr:   Ac:   Av:   Mv:   Ra:   ", 12 * 8 + 1);
	//screen = "Na:         Hp:  /      St:   Ma:   De:   Re:   Sk:   Sp:   Lk:   Cr:   Ac:   Av:   Mv:   Ra:   ";
/*
Na:$$$$$$$$$
Hp:$$/$$____
tS:$$_Ma:$$_
De:$$_Re:$$_
Sk:$$_Sp:$$_
Lk:$$_Cr:$$_
Ac:$$$Av:$$$
Mv:$$_Ra:$$_
width: 12
Height: 8
*/
}


FEStatViewer::~FEStatViewer(void)
{
}

ColorChar FEStatViewer::getColorChar(int x, int y)
{
	ColorChar retVal = ColorChar();
	retVal.color = 7; //white on black
	retVal.glyph = screen[x + y * WIDTH];
	return retVal;
}

void FEStatViewer::setUnit(FEUnit* newUnit)
{
	string tempString = newUnit->getName();
	int limit = tempString.length() >=9 ? 9 : tempString.length();
	for(int counter = 0; counter < 8; counter++)
	{
		if(counter < limit) screen[3+counter] = tempString[counter];
		else screen[3+counter] = ' ';
	}
	StatBlock* stats = newUnit->getStats();
	int temp = newUnit->getCurrentHP();
	screen[15]=(temp/10) + 48;
	screen[16]=(temp%10) + 48;
	temp = stats->max_hp;
	screen[18]=(temp/10) + 48;
	screen[19]=(temp%10) + 48;
	temp = stats->strength;
	screen[27]=(temp/10) + 48;
	screen[28]=(temp%10) + 48;
	temp = stats->magic;
	screen[33]=(temp/10) + 48;
	screen[34]=(temp%10) + 48;
	temp = stats->defense;
	screen[39]=(temp/10) + 48;
	screen[40]=(temp%10) + 48;
	temp = stats->resist;
	screen[45]=(temp/10) + 48;
	screen[46]=(temp%10) + 48;
	temp = stats->skill;
	screen[51]=(temp/10) + 48;
	screen[52]=(temp%10) + 48;
	temp = stats->speed;
	screen[57]=(temp/10) + 48;
	screen[58]=(temp%10) + 48;
	temp = stats->luck;
	screen[63]=(temp/10) + 48;
	screen[64]=(temp%10) + 48;
	temp = newUnit->getCritChance();
	screen[69]=(temp/10) + 48;
	screen[70]=(temp%10) + 48;
	temp = newUnit->getAccuracy();
	screen[75]=(temp/100) + 48;
	screen[76]=((temp/10) % 10) + 48;
	screen[77]=(temp%10) + 48;
	temp = newUnit->getAvoid(ITEM);
	screen[81]=(temp/100) + 48;
	screen[82]=((temp/10) % 10) + 48;
	screen[83]=(temp%10) + 48;
	temp = stats->move;
	screen[87]=(temp/10) + 48;
	screen[88]=(temp%10) + 48;
	temp = newUnit->getEquipped()->max_range;
	screen[93]=(temp/10) + 48;
	screen[94]=(temp%10) + 48;
}
