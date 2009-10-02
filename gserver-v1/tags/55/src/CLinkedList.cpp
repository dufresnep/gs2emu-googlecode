// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CLinkedList.h"

CLinkedList::CLinkedList()
{
	itemCount = 0;
}

CLinkedList::~CLinkedList()
{
	clear();
}

int CLinkedList::add(void* pItem)
{
	SLinkedItem* item = new SLinkedItem;
	item->value = pItem;

	itemCount++;
	if(itemCount == 1)
		firstItem = endItem = item;
	else
	{
		endItem->nextItem = item;
		endItem = item;
	}
	return itemCount-1;
}

void CLinkedList::remove(void* pItem)
{
	SLinkedItem* item = firstItem;
	SLinkedItem* previousItem = 0;
	for(int i = 0; i < itemCount; i++)
	{
		if(item->value == pItem)
		{
			if(item == firstItem)
				firstItem = item->nextItem;
			else previousItem->nextItem = item->nextItem;
			if(item == endItem)
				endItem = previousItem;
			delete item;
			itemCount--;
			return;
		}
		previousItem = item;
		item = item->nextItem;
	}
}

void CLinkedList::clear()
{
	SLinkedItem* item = firstItem;
	while(itemCount > 0)
	{
		SLinkedItem* nextItem = item->nextItem;
		delete item;
		item = nextItem;
		itemCount--;
	}
}

void* CLinkedList::item(int pIndex)
{
	SLinkedItem* item = firstItem;
	for(int i = 0; i < itemCount; i++)
	{
		if(i == pIndex)
			return item->value;
		item = item->nextItem;
	}
	return 0;
}

void CLinkedList::replace(int pIndex, void* pItem)
{
	SLinkedItem* item = firstItem;
	for(int i = 0; i <itemCount; i++)
	{
		if(i == pIndex)
		{
			item->value = pItem;
			return;
		} else item = item->nextItem;
	}
}
