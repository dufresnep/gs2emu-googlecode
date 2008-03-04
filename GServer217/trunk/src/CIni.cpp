/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#include "CIni.h"
#include "CBuffer.h"

bool CIni::load(char* pFileName)
{
	//Open file
	strcpy(fileName, pFileName);
	FILE* fileHandle = fopen(pFileName, "r");
	if (fileHandle == NULL)
		return false;

	//Interpret data
	sSection* section = NULL;
	sKey* key = NULL;
	char buffer[100000];

	//Loop until we read all the lines
	while (fgets(buffer, sizeof(buffer), fileHandle) != NULL)
	{
		char* line = trim(buffer);
		int lineLength = (int)strlen(line);
		//Ignore blank lines
		if (lineLength <= 0)
			continue;
		//Ignore comments
		if (line[0] == '#' || line[0] == ';' || line[0] == '/')
			continue;
		//Start of new section
		if (line[0] == '[')
		{
			line++;
			section = new sSection;
			//search for ] symbol
			int sectionLen = (int)strcspn(line, "]");
			//If no ']' symbol then ignore line
			if(sectionLen == lineLength-1)
				continue;
			//Copy section name to the structure
			strcpy(section->name, line);
			//Remove the ']' character
			section->name[sectionLen] = '\0';
			//Add section to linked list
			sections.add(section);
			continue;
		}

		//If no comment and no section name then this line might be a key..
		//Ignore line if we arnt yet within a section
		if (section == NULL)
			continue;
		//search for '=' symbol
		int keyLen = (int)strcspn(line, "=");
		//Ignore line if there is no '=' symbol
		if (keyLen == lineLength)
			continue;
		key = new sKey;
		//put a null terminator in the place of the '=' symbol (to split the keyname and key value)
		line[keyLen] = '\0';
		//Extract the key name and copy it to the name variable in the key object
		strcpy(key->name, trim(line));
		//Make the line pointer go just after the '=' symbol so we can read the key value
		line += keyLen + 1;
		line = trim(line);
		key->value = new char[strlen(line)+1];
		strcpy(key->value, line);
		section->keys.add(key);
	}
	fclose(fileHandle);
	return true;
}

bool CIni::sectionExists(char* pSection)
{
	if (getSection(pSection) == NULL)
		return false;
	return true;
}

bool CIni::keyExists(char* pSection, char* pKey)
{
	if (getKey(pSection, pKey) == NULL)
		return false;
	return true;
}

char* CIni::readString(char* pSection, char* pKey, char* pDefault)
{
	sKey* key = getKey(pSection, pKey);
	if (key == NULL)
		return pDefault;
	return key->value;
}

double CIni::readReal(char* pSection, char* pKey, double pDefault)
{
	sKey* key = getKey(pSection, pKey);
	if (key == NULL)
		return pDefault;
	return atof(key->value);
}

void CIni::writeString(char* pSection, char* pKey, char* pValue)
{
	sSection* section = getSection(pSection);
	sKey* key = getKey(pSection, pKey);
	//If section doesnt exist than create it
	if (section == NULL)
	{
		section = new sSection;
		strcpy(section->name, pSection);
		sections.add(section);
	}
	if (key == NULL) //if key doesnt exist then create it
	{
		key = new sKey;
		strcpy(key->name, pKey);
		section->keys.add(key);
	} else delete key->value;   //if key exists than delete original value
	int valueLen = (int)strlen(pValue);
	key->value = new char[valueLen+1];
	strcpy(key->value, pValue);
}

void CIni::writeReal(char* pSection, char* pKey, double pValue)
{
	char buffer[20];
	//if whole number than print without floating point
	if (pValue == (int)pValue)
		sprintf(buffer, "%i", (int)pValue);
	else sprintf(buffer, "%f", pValue);
	writeString(pSection, pKey, buffer);
}

void CIni::keyDelete(char* pSection, char* pKey)
{
	sSection* section = getSection(pSection);
	if (section == NULL)
		return;
	for (int i = 0; i < section->keys.count(); i++)
	{
		sKey* key = (sKey*)section->keys.item(i);
		if (stricmp(key->name, pKey) == 0)
		{
			keyDelete(section, key);
			return;
		}
	}
}

void CIni::sectionDelete(char* pSection)
{

	for (int i = 0; i < sections.count(); i++)
	{
		sSection* section = (sSection*)sections.item(i);
		if (stricmp(section->name, pSection) == 0)
		{
			sections.remove(i);
			keyDeleteAll(section);
			delete section;
			return;
		}
	}

}

CIni::~CIni()
{
	dispose();
}

void CIni::dispose()
{
	sectionDeleteAll();
}

void CIni::save(char* pFileName)
{
	//Open output file
	FILE* fileHandle = fopen(pFileName, "wb");
	//If failed to open output then exit save function
	if (fileHandle == NULL)
		return;

	for (int i = 0; i < sections.count(); i++)
	{
		sSection* section = (sSection*)sections.item(i);
		fprintf(fileHandle, "[%s]\r\n", section->name);

		for (int ii = 0; ii < section->keys.count(); ii++)
		{
			sKey* key = (sKey*)section->keys.item(ii);
			fprintf(fileHandle, "%s=%s\r\n", key->name, key->value);
		}
		fprintf(fileHandle, "\r\n");
	}
	fclose(fileHandle);
}

void CIni::sectionDeleteAll()
{
	for(int i = 0; i < sections.count(); i++)
	{
		sSection* section = (sSection*)sections.item(i);
		keyDeleteAll(section);
		delete section;
	}
	sections.clear();
}

//privates
void CIni::keyDelete(sSection* pSection, sKey* pKey)
{
	delete pKey->value;
	delete pKey;
	pSection->keys.remove(pKey);
}

void CIni::keyDeleteAll(sSection* pSection)
{
	if(pSection == NULL)
		return;

	for(int i = 0; i < pSection->keys.count(); i++)
	{
		sKey* key = (sKey*)pSection->keys.item(i);
		delete key->value;
		delete key;
	}
	pSection->keys.clear();
}

sKey* CIni::getKey(char* pSection, char* pKey)
{
	sSection* section = getSection(pSection);
	if (section == NULL)
		return NULL;

	for (int i = 0; i < section->keys.count(); i++)
	{
		sKey* key = (sKey*)section->keys.item(i);
		if (stricmp(key->name, pKey) == 0)
			return key;

	}
	return NULL;
}

sSection* CIni::getSection(char* pSection)
{

	for (int i = 0; i < sections.count(); i++)
	{
		sSection* section = (sSection*)sections.item(i);
		if (stricmp(section->name, pSection) == 0)
			return section;
	}
	return NULL;
}

//quick trim function
char* CIni::trim(char* pString)
{
	char* retVal = pString;
	int len, len2 = (int)strlen(pString);
	len = len2;
	for (int i = 0; i < len; i++)
	{
		if (*retVal <= ' ')
			retVal++;
		else break;
		len2--;
	}
	for (int i = len2-1; i >= 0; i--)
	{
		if (retVal[i] <= ' ')
			retVal[i] = '\0';
		else break;
	}
	return retVal;
}
