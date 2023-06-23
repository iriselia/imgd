#pragma once

#include "LinkedList.h"
#include "Item.h"

class Pile : public LinkedList<Item>
{
public:
	Pile(void);
	~Pile(void);
};

