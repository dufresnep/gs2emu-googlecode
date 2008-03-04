/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#include "CStringList.h"


CStringList::~CStringList()
{
	for(int i = 0; i < count(); i++)
		delete (CString*)CList::item(i);
}

int CStringList::add(const char* pStr)
{
	CString* newString;
	if(pStr == NULL)
		newString = new CString("");
	else newString = new CString(pStr);
	return CList::add(newString);
}

int CStringList::add(CString& pString)
{
	CString* newString = new CString();
	newString->writeBuffer(pString);
	return CList::add(newString);
}

void CStringList::remove(int pIndex)
{
	CString* str = (CString*)CList::item(pIndex);
	if(str == 0)
		return;
	delete str;
	CList::remove(pIndex);
}


void CStringList::clear()
{
	if(count() <= 0)
		return;

	for(int i = 0; i < count(); i++)
	{
		CString* str = (CString*)CList::item(i);
		delete str;
	}

	CList::clear();
}

CString& CStringList::item(int pIndex)
{
	return *(CString*)CList::item(pIndex);
}

void CStringList::replace(int pIndex, const char* pString)
{
	CString* str = (CString*)CList::item(pIndex);
	if(str == 0)
		return;
	delete str;
	str = new CString(pString);
	CList::replace(pIndex, str);
}

bool CStringList::load(const char* pFileName)
{
	char line[100000];
	clear();
	FILE* file = fopen(pFileName, "r");
	if(file == 0)
		return false;
	//Read each line and add it to list
	while(fgets(line, sizeof(line), file) != 0)
	{
		//Trim end of line (spaces allowed)
		for(int i = (int)strlen(line)-1; i >= 0; i--)
		{
			if(line[i] < ' ')
				line[i] = '\0';
			else break;
		}
		add(line);
	}
	fclose(file);
	return true;
}

void CStringList::load(const char* pInput, const char* pSep)
{
	CString newString;
	clear();
	newString = pInput;
	char* line = strtok(newString.text(), pSep);
	while(line != NULL)
	{
		add(line);
		line = strtok(NULL, pSep);
	}
}

void CStringList::load(const char** pStrings, int pCount)
{
	clear();
	for(int i = 0; i < pCount; i++)
		add(pStrings[i]);
}

int CStringList::find(const char* pString)
{
	for(int i = 0; i < count(); i++)
	{
		if(item(i) == pString)
			return i;
	}
	return -1;
}

int CStringList::findI(const char* pString)
{
	for(int i = 0; i < count(); i++)
	{
		if(stricmp(item(i).text(), pString) == 0)
			return i;
	}
	return -1;
}

void CStringList::save(const char* pFileName)
{
	FILE* file = fopen(pFileName, "wb");
	for(int i = 0; i < count(); i++)
		fprintf(file, "%s\r\n", item(i).text());
	fclose(file);
}

CString CStringList::join(char* pSep)
{
	CString retVal;
	for(int i = 0; i < count()-1; i++)
		retVal << item(i).escapeStr() << pSep;
	if(count())
		retVal << item(count()-1).escapeStr();
	return retVal;
}
