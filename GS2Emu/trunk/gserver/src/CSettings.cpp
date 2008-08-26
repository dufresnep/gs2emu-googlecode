#include "CSettings.h"

/*
	Constructor - Deconstructor
*/
CSettings::CSettings(const CString& pStr, const CString& pSeperator)
{
	strSep = pSeperator;
	opened = loadFile(pStr);
}

CSettings::~CSettings()
{
	clear();
}

/*
	File-Loading Functions
*/
bool CSettings::loadFile(const CString& pStr)
{
	// definitions
	CString fileData;

	// Clear Keys
	clear();

	// Load File
	if (!fileData.load(pStr))
		return false;

	// Parse Data
	strList = fileData.tokenize("\n");
	for (unsigned int i = 0; i < strList.size(); i++)
	{
		// Skip Comments
		if (strList[i][0] == '#' || strList[i].find(strSep) == -1)
			continue;

		// Tokenize Line && Trim && Lowercase Key Name
		std::vector<CString> line = strList[i].tokenize(strSep);
		line[0].toLowerI();
		for (unsigned int j = 0; j < line.size(); j++)
			line[j].trimI();

		// Create Key
		keys.push_back(new CKey(line[0], line[1]));
		/*
		CKey *key;
		if ((key = getKey(line[0])) == NULL)
			keys.push_back(new CKey(line[0], line[1]));
		else
			key->value << "," << line[1];
		*/
	}

	return true;
}

void CSettings::clear()
{
	// Clear Keys
	for (unsigned int i = 0; i < keys.size(); i++)
		delete keys[i];
	keys.clear();
}

/*
	Get Settings
*/
CKey * CSettings::getKey(CString pStr)
{
	// Lowercase Name
	pStr.toLowerI();

	// Iterate key List
	for (std::vector<CKey *>::iterator i = keys.begin(); i != keys.end(); ++i)
	{
		if ((*i)->name == pStr)
			return *i;
	}

	// None :(
	return NULL;
}

bool CSettings::getBool(const CString& pStr, bool pDefault)
{
	CKey *key = getKey(pStr);
	return (key == NULL ? pDefault : (key->value == "true" || key->value == "1"));
}

float CSettings::getFloat(const CString& pStr, float pDefault)
{
	CKey *key = getKey(pStr);
	return (key == NULL ? pDefault : (float)strtofloat(key->value));
}

int CSettings::getInt(const CString& pStr, int pDefault)
{
	CKey *key = getKey(pStr);
	return (key == NULL ? pDefault : strtoint(key->value));
}

const CString& CSettings::getStr(const CString& pStr, const CString& pDefault)
{
	CKey *key = getKey(pStr);
	return (key == NULL ? pDefault : key->value);
}

const CString& CSettings::operator[](int pIndex)
{
	return strList[pIndex].trimI();
}

std::vector<CString> CSettings::getFile()
{
	std::vector<CString> newStrList;
	std::copy(strList.begin(), strList.end(), newStrList.begin());
	return newStrList;
}