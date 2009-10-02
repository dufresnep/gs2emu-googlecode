#include "CList.h"
#include <stdio.h>

int CList::add(void* pItem)
{
	if(itemCount >= buffsize)
	{
		buffsize += 10;
		items = (void**)realloc(items, buffsize*sizeof(void*));
	}
	items[itemCount] = pItem;
	itemCount++;
	return itemCount-1;
}

CList::~CList()
{
    free(items);
}

void CList::remove(int pIndex)
{
	if(pIndex < 0 || pIndex >= itemCount)
        return;
	if(itemCount > 1)
	{
		memmove(&items[pIndex], &items[pIndex+1], (itemCount-pIndex-1)*sizeof(void*));
		itemCount--;
	} else clear();
}

void CList::replace(int pos, void*item)
{
	if(pos >= 0 && pos < itemCount)
		items[pos] = item;

}

void CList::clear(int pBuffSize)
{
	free(items);
	buffsize = (pBuffSize > 1 ? pBuffSize : 1);
	items = (void**)malloc(buffsize * sizeof(void*));
	itemCount = 0;
}

void CList::resize(int pBuffSize)
{
	if (itemCount > pBuffSize)
		return;

	buffsize = (pBuffSize > 1 ? pBuffSize : 1);
	items = (void**)realloc(items, buffsize*sizeof(void*));
}

int CList::find(void* pItem)
{
    for(int i = 0; i < itemCount; i++)
	{
		if(items[i] == pItem)
			return i;
	}
	return -1;
}

void* CList::item(int i)
{
	if(i >= 0 && i < itemCount)
        return items[i];
	return NULL;

}
