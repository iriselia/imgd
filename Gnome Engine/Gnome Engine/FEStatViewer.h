#pragma once
#include "map.h"
#include "FEUnit.h"

/*
Na:$$$$$$$$$
Hp:$$/$$____
St:$$_Ma:$$_
De:$$_Re:$$_
Sk:$$_Sp:$$_
Lk:$$_Cr:$$_
Ac:$$$Av:$$$
width: 12
Height: 7
*/

class FEStatViewer :	public Map
{
public:
	FEStatViewer(void);
	~FEStatViewer(void);
	virtual ColorChar getColorChar(int x, int y);
	void setUnit(FEUnit* newUnit);
private:
	char* screen;
};