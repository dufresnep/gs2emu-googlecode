#include "main.h"
#include "TLevel.h"
#include "TPlayer.h"

/*
	Global Variables
*/
CString base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
std::vector<TLevel *> levelList;

/*
	TLevel: Constructor - Deconstructor
*/
TLevel::TLevel()
{
	memset(levelTiles, 0, 8192/*sizeof(levelTiles)*/);
}

TLevel::~TLevel()
{
}

/*
	TLevel: Get Crafted Packets
*/
CString TLevel::getBaddyPacket()
{
	return CString();
}

CString TLevel::getBoardPacket()
{
	CString retVal;
	retVal.writeGChar(PLO_BOARDPACKET);
	retVal.write((char *)levelTiles, sizeof(levelTiles));
	return retVal;
}

CString TLevel::getChestPacket(TPlayer *pPlayer)
{
	if (pPlayer == NULL)
		return CString();

	CString retVal;

	for (std::vector<TLevelChest *>::iterator i = levelChests.begin(); i != levelChests.end(); ++i)
	{
		TLevelChest *chest = *i;
		bool hasChest = pPlayer->hasChest(chest);

		retVal >> (char)PLO_LEVELCHEST >> (char)hasChest >> (char)chest->getX() >> (char)chest->getY();
		if (hasChest)
			retVal >> (char)chest->getItemIndex() >> (char)chest->getSignIndex();
	}

	return retVal;
}

CString TLevel::getHorsePacket()
{
	CString retVal;
	for (std::vector<TLevelHorse *>::iterator i = levelHorses.begin(); i != levelHorses.end(); ++i)
	{
		TLevelHorse *horse = *i;
		retVal >> (char)PLO_HORSEADD >> (char)horse->getX() >> (char)horse->getY() << horse->getImage();
	}

	return retVal;
}

CString TLevel::getLinksPacket()
{
	CString retVal;
	for (std::vector<TLevelLink *>::iterator i = levelLinks.begin(); i != levelLinks.end(); ++i)
	{
		TLevelLink *link = *i;
		retVal >> (char)PLO_LEVELLINK << link->getLinkStr();
	}

	return retVal;
}

CString TLevel::getNpcsPacket()
{
	return CString();
}

CString TLevel::getSignsPacket()
{
	return CString();
}
/*
	TLevel: Level-Loading Functions
*/
bool TLevel::loadLevel(const CString& pFileName)
{
	return (getExtension(pFileName) == ".nw" ? loadNW(pFileName) : loadGraal(pFileName));
}

bool TLevel::loadGraal(const CString& pFileName)
{
	return true;
}

bool TLevel::loadNW(const CString& pFileName)
{
	// Path-To-File
	fileName = getDataFile(pFileName);
	int l[2] = {pFileName.findl('/')+1, pFileName.findl('\\')+1};
	int ll = max(l[0], l[1]);
	levelName = pFileName.subString(ll);

	// Load File
	std::vector<CString> fileData = CString::loadToken(fileName);
	if (fileData.size() < 1)
		return false;

	// Grab File Version
	fileVersion = fileData[0];

	// Parse Level
	for (std::vector<CString>::iterator i = fileData.begin(); i != fileData.end(); ++i)
	{
		// Tokenize
		std::vector<CString> curLine = i->tokenize();
		if (curLine.size() < 1)
			continue;

		// Parse Each Type
		if (curLine[0] == "BOARD")
		{
			if (curLine.size() != 6)
				continue;

			int x, y, w;
			x = strtoint(curLine[1]);
			y = strtoint(curLine[2]);
			w = strtoint(curLine[3]);

			if (!inrange(x, 0, 64) || !inrange(y, 0, 64) || w <= 0 || x + w > 64)
				continue;

			if (curLine[5].length() >= w*2)
			{
				for(int ii = x; ii < x + w; ii++)
				{
					char left = curLine[5].readChar();
					char top = curLine[5].readChar();
					short tile = base64.find(left) << 6;
					tile += base64.find(top);
					levelTiles[ii + y*64] = tile;
				}
			}
		}
		else if (curLine[0] == "CHEST")
		{
			if (curLine.size() != 5)
				continue;
			if ((curLine[3] = findItemId(curLine[3])) == -1)
				continue;

			levelChests.push_back(new TLevelChest(curLine));
		}
		else if (curLine[0] == "LINK")
		{
			if (curLine.size() != 8)
				continue;
			if (getDataFile(curLine[1]).length() < 1)
				continue;

			levelLinks.push_back(new TLevelLink(curLine));
		}
	}

	return true;
}

/*
	TLevel: Find Level
*/
TLevel * TLevel::findLevel(const CString& pLevelName)
{
	// Find Appropriate Level by Name
	for (std::vector<TLevel *>::iterator i = levelList.begin(); i != levelList.end(); ++i)
	{
		if ((*i) == NULL)
		{
			i = levelList.erase(i);
			continue;
		}

		if ((*i)->getLevelName() == pLevelName)
			return (*i);
	}

	// Load New Level
	TLevel *level = new TLevel();
	if (!level->loadLevel(pLevelName))
	{
		delete level;
		return NULL;
	}

	// Return Level
	levelList.push_back(level);
	return level;
}
