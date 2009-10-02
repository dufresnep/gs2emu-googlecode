/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

/*
	This INI manager class was coded by 39ster (39ster@gmail.com)
	Use in your applications with or without credit (credit appreciated)
*/

#ifndef HINIREADER
#define HINIREADER
//Include some standard C libraries
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "CList.h"
#include "CBuffer.h"

typedef struct
{
	char name[50];
	char* value;
} sKey;

typedef struct
{
	char name[50];
	CList keys;
} sSection;

class CIni
{
	private:
	char fileName[60];
	CList sections;

	//private functions
	sSection* getSection(char* pSection);
	sKey* getKey(char* pSection, char* pKey);
	char* trim(char* pString);
	void keyDelete(sSection* pSection, sKey* pKey);
	void keyDeleteAll(sSection* pSection);

	public:
	bool load(char* pFileName); //Open and parse an INI file
	void save(char* pFileName); //Save the sections, keys and values to a file
	~CIni();

	//Reading
	char* readString(char* pSection, char* pKey, char* pDefault);
	double readReal(char* pSection, char* pKey, double pDefault);

	//Writing
	void writeString(char* pSection, char* pKey, char* pValue);
	void writeReal(char* pSection, char* pKey, double pValue);

	//Conditions
	bool keyExists(char* pSection, char* pKey);
	bool sectionExists(char* pSection);

	//Deletion
	void keyDelete(char* pSection, char* pKey);
	void sectionDelete(char* pSection);
	void sectionDeleteAll();
	inline void keyDeleteAll(char* pSection)
	{
		keyDeleteAll(getSection(pSection));
	}
	void dispose();
};

#endif // HINIREADER
