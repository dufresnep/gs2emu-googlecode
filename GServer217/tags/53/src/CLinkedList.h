// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef HCLINKEDLIST
#define HCLINKEDLIST

typedef struct SLinkedItem
{
    void* value;
    SLinkedItem* nextItem;
} sLinkedItem;

class CLinkedList
{
    SLinkedItem* firstItem;
    SLinkedItem* endItem;
    int itemCount;
    public:

    CLinkedList();
    ~CLinkedList();
    int add(void* pItem);
    void remove(void* pItem);
    void clear();
    void* item(int pIndex);
    void replace(int pIndex, void* pItem);

    inline void* operator[](int pIndex){
        return item(pIndex);
    }
    inline void remove(int pIndex){
        remove(item(pIndex));
    }
    inline int count(){
        return itemCount;
    }

};

#endif // HCLINKEDLIST

