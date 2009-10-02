// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CMap.h"
#include "main.h"

CList CMap::mapList;

CMap* CMap::openMap(CString& pMapName)
{
	bool isTxt = false;
	if(getFileExtension(pMapName) == ".txt")
		isTxt = true;
	else if(!getFileExtension(pMapName).length())
		pMapName += ".gmap";

	for(int i = 0; i < mapList.count(); i++)
	{
		CMap* map = (CMap*)mapList[i];
		if(map->fileName == pMapName)
			return map;
	}

	CMap* map = new CMap;
	if(isTxt)
	{
		if(!map->loadTXT(pMapName))
		{
			delete map;
			return NULL;
		}
	} else if(!map->loadGMap(pMapName))
	{
		delete map;
		return NULL;
	}
	mapList.add(map);
	return map;
}

bool CMap::loadTXT(CString& pFileName)
{
	CStringList mapData;
	char* dataFile = getDataFile(pFileName.text());
	if(!strlen(dataFile))
		return false;
	if(!mapData.load(dataFile))
		return false;

	fileName = pFileName;
	height = 0;
    for(int i = 0; i < mapData.count(); i++)
    {
        if(mapData[i].length())
        {
            parseLevelNames(mapData[i]);
            height++;
        }
    }

	return true;
}

bool CMap::loadGMap(CString& pFileName)
{
	CStringList levelData;
	char* dataFile = getDataFile(pFileName.text());
	if(!strlen(dataFile))
		return false;

	if(!levelData.load(dataFile))
		return false;
	fileName = pFileName;
	bool loadFull = false;
	for(int i = 0; i < levelData.count(); i ++)
	{
		CStringList words;
		words.load(levelData[i].text(), " ");
		if(words[0] == "WIDTH")
		{
			width = atoi(words[1].text());
		} else if(words[0] == "HEIGHT")
		{
			height = atoi(words[1].text());
		} else if(words[0] == "LEVELNAMES")
		{
		    height = 0;
			for(i++; i < levelData.count() && levelData[i] != "LEVELNAMESEND"; i++)
			{
			    if(levelData[i].length())
			    {
                    parseLevelNames(levelData[i]);
                    height++;
			    }
			}
		} else if(words[0] == "LOADFULLMAP")
			loadFull = true;

	}

	if(loadFull)
	{
		for(int i = 0; i < levelList.count(); i++)
            CLevel::openMap(levelList[i]);
	}
	return true;
}

void CMap::parseLevelNames(CString& pLevelNames)
{
    CStringList names;
    names.load(pLevelNames.text(), ",");
    width = 0;
    for(int i = 0; i < names.count(); i++)
    {
        levelList.add(names[i].removeAll("\"").trim());
        width++;
    }

}
int CMap::getLevelpos(CString& pLevelName)
{
	return levelList.findI(pLevelName);
}

bool CMap::nextTo(int pPos1, int pPos2)
{
	if(pPos1 < 0 || pPos2 < 0)
		return false;

	if(pPos2 >= pPos1-width-1 && pPos2 <= pPos1-width+1)
		return true;

	if(pPos2 >= pPos1-1 && pPos2 <= pPos1+1)
		return true;

	if(pPos2 >= pPos1+width-1 && pPos2 <= pPos1+width+1)
		return true;

	return false;

}

