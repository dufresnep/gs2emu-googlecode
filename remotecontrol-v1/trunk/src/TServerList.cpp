#include "main.h"
#include "TServerList.h"

/*
	Destructor
*/

TServerList::~TServerList()
{
	clear();
}

/*
	Functions
*/

bool TServerList::inRange(int pIndex)
{
	return (pIndex >= 0 && pIndex < (int)list.size());
}

// add
void TServerList::add(TServer *pServer)
{
	// check for dupes
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i] == pServer)
			return;
	}

	// add server
	list.push_back(pServer);
}

// clear
void TServerList::clear()
{
	for (unsigned int i = 0; i < list.size(); i++)
		delete list[i];
	list.clear();
}

// get
TServer* TServerList::get(int pIndex)
{
	if (!inRange(pIndex))
		return NULL;
	return list[pIndex];
}

// remove
void TServerList::remove(TServer *pServer)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i] != pServer)
			continue;

		list.erase(list.begin() + i);
	}
}

void TServerList::remove(int pIndex)
{
	if (!inRange(pIndex))
		return;

	list.erase(list.begin() + pIndex);
}
