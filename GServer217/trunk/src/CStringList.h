/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#ifndef HCSTRINGLIST
#define HCSTRINGLIST

//Requires CBuffer, CList

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "CList.h"
#include "CBuffer.h"

class CStringList:
	public CList
{
	public:
	~CStringList();

	int add(const char* pString);
	int add(CString& pString);
	void remove(int pIndex);
	void remove(const char* pString);
	void remove(CString& pString);
	int find(const char* pString);
	int find(CString& pString);
	int findI(const char* pString);
	int findI(CString& pString);
	void clear();
	CString& item(int pIndex);
	CString& operator[](int pPos);
	void replace(int pIndex, const char* pString);
	void replace(const char* pOldString, const char* pNewString);
	void replace(CString& pOldString, CString& pNewString);
	bool load(const char* pFileName);
	void load(const char* pInput, const char* pSep);
	void load(const char** pStrings, int pCount);
	void save(const char* pFileName);
	CString join(char* pSep);

};

inline CString& CStringList::operator[](int pPos)
{
	return item(pPos);
}

inline void CStringList::remove(const char* pString)
{
	remove(find(pString));
}

inline void CStringList::remove(CString& pString)
{
	remove(pString.text());
}

inline void CStringList::replace(const char* pOldString, const char* pNewString)
{
	replace(find(pOldString), pNewString);
}

inline int CStringList::find(CString& pString)
{
	return find(pString.text());
}

inline int CStringList::findI(CString& pString)
{
	return findI(pString.text());
}
inline void CStringList::replace(CString& pOldString, CString& pNewString)
{
	replace(pOldString.text(), pNewString.text());
}

#endif // HCSTRINGLIST

