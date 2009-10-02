/* GraalReborn Server
        $Id$
 (C) GraalReborn 2007 */

#ifndef HCLIST
#define HCLIST

#include <cstdlib>
#include <cstring>
#include <cstdio>
class CList
{
        void**items;
        int buffsize;
        int itemCount;

        public:
        CList(int pBuffSize = 5);
        ~CList();

        int add(void* pItem);
        void remove(int pIndex);
        void resize(int pBuffSize);
        void clear(int pBuffSize = 5);
        void* item(int pIndex);
        void replace(int pPos, void* pItem);
        int find(void* pItem);

        void* operator[](int pIndex);
        void remove(void* pItem);
        int count();
        int size();
};

inline CList::CList(int pBuffSize)
{
        buffsize = (pBuffSize > 1 ? pBuffSize : 1);
        items = (void**)malloc(buffsize * sizeof(void*));
        itemCount = 0;
}

inline void* CList::operator[](int pIndex)
{
        return item(pIndex);
}

inline void CList::remove(void* pItem)
{
        remove(find(pItem));
}

inline int CList::count()
{
        return itemCount;
}

inline int CList::size()
{
        return buffsize;
}
#endif // HCLIST

