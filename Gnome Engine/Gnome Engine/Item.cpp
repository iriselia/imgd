#include "StdAfx.h"
#include "Item.h"


Item::Item(WEAPON_TYPE _type, int _rank, int _might, int _accuracy, int _crit, int _min_range, int _max_range)
{
	type = _type;
	rank = _rank;
	might = _might;
	accuracy = _accuracy;
	crit = _crit;
	min_range = _min_range;
	max_range = _max_range;
}


Item::~Item(void)
{
}
