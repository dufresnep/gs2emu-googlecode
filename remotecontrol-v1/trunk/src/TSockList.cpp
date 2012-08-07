#include "TSockList.h"

/*
	Destructor
*/

TSockList::~TSockList()
{
	clear();
}

/*
	Functions
*/

bool TSockList::inRange(int pIndex)
{
	return (pIndex >= 0 && pIndex < (int)list.size());
}

// add
void TSockList::add(CSocket *pSocket)
{
	// check for dupes
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i] == pSocket)
			return;
	}

	// add socket
	list.push_back(pSocket);
}

// clear
void TSockList::clear()
{
	// dont delete list[0] -> listSock
	while (list.size() > 1)
		delete list[1];
}

// get
CSocket* TSockList::get(int pIndex)
{
	if (!inRange(pIndex))
		return NULL;
	return list[pIndex];
}

// remove
void TSockList::remove(CSocket *pSocket)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i] != pSocket)
			continue;

		list.erase(list.begin() + i);
	}
}

void TSockList::remove(int pIndex)
{
	if (!inRange(pIndex))
		return;

	list.erase(list.begin() + pIndex);
}

/*
	Extra-Function for Socket-Running
*/
void TSockList::run()
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i] == NULL)
			continue;
		if (list[i]->getSocket() == 0)
			continue;

		list[i]->main();
	}
}
