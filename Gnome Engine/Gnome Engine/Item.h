#pragma once

enum WEAPON_TYPE {SWORD, AXE, LANCE, BOW, ANIMA, LIGHT, DARK, STAFF, ITEM};

class Item
{
public:
	Item(WEAPON_TYPE _type, int _rank, int might, int _accuracy, int _crit, int _min_range, int _max_range);
	~Item(void);

	WEAPON_TYPE type;
	int rank;
	int might;
	int accuracy;
	int crit;
	int min_range;
	int max_range;
};

