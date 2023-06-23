#include "StdAfx.h"
/*
#include "LinkedList.h"


template<class Content> LinkedList<Content>::LinkedList(void)
{
	first = nullptr;
}


template<class Content> LinkedList<Content>::~LinkedList(void)
{
	Link<Content>* last;
	for(Link<type>* ptr = start; ptr != nullptr; ptr)
	{
		last = ptr->next;
		free(ptr);
		ptr = last;
	}
}

template<class Content> void LinkedList<Content>::insert(Content* obj)
{
	new link<Content> in;
	in->first = obj;
	in->rest = first;
	first = in->first;
}

template<class Content> void LinkedList<Content>::remove(Content* obj)
{
	link<Content>** last = &first;
	forEach(Content, ptr, first)
	{
		if(ptr->first == obj)
		{
			*last = ptr->next;
			free ptr;
			break;
		}
		else
		{
			last = &(ptr->next);
		}
	}
}

template<class Content> void LinkedList<Content>::freeAll(void)
{
	Link<Content>* last;
	for(Link<type>* ptr = start; ptr != nullptr; ptr)
	{
		free(ptr->first);
		last = ptr->next;
		free(ptr);
		ptr = last;
	}
}
*/