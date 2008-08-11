#include "ICommon.h"
#include "main.h"
#include "CFileSystem.h"
#include "TServer.h"
#include "TLevel.h"
#include "TPlayer.h"
#include "TNPC.h"

/*
	Global Variables
*/
CString base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
std::vector<TLevel *> levelList;

/*
	TLevel: Constructor - Deconstructor
*/
TLevel::TLevel()
:
modTime(0), levelSpar(false)
{
	memset(levelTiles, 0, sizeof(levelTiles));
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
	if (pPlayer == 0)
		return CString();

	CString retVal;

	for (std::vector<TLevelChest *>::iterator i = levelChests.begin(); i != levelChests.end(); ++i)
	{
		TLevelChest *chest = *i;
		bool hasChest = pPlayer->hasChest(chest);

		retVal >> (char)PLO_LEVELCHEST >> (char)(hasChest ? 1 : 0) >> (char)chest->getX() >> (char)chest->getY();
		if (!hasChest)
			retVal >> (char)chest->getItemIndex() >> (char)chest->getSignIndex() << "\n";
		else
			retVal << "\n";
	}

	return retVal;
}

CString TLevel::getHorsePacket()
{
	CString retVal;
	for (std::vector<TLevelHorse *>::iterator i = levelHorses.begin(); i != levelHorses.end(); ++i)
	{
		TLevelHorse *horse = *i;
		retVal >> (char)PLO_HORSEADD >> (char)horse->getX() >> (char)horse->getY() << horse->getImage() << "\n";
	}

	return retVal;
}

CString TLevel::getLinksPacket()
{
	CString retVal;
	for (std::vector<TLevelLink *>::iterator i = levelLinks.begin(); i != levelLinks.end(); ++i)
	{
		TLevelLink *link = *i;
		retVal >> (char)PLO_LEVELLINK << link->getLinkStr() << "\n";
	}

	return retVal;
}

CString TLevel::getNpcsPacket(time_t time)
{
	CString retVal;
	for (std::vector<TNPC *>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		TNPC* npc = *i;
		retVal >> (char)PLO_NPCPROPS >> (int)npc->getId() << npc->getProps(time) << "\n";
	}
	return retVal;
}

CString TLevel::getSignsPacket()
{
	return CString();
}
/*
	TLevel: Level-Loading Functions
*/
bool TLevel::loadLevel(const CString& pLevelName, TServer* server)
{
	return (getExtension(pLevelName) == ".nw" ? loadNW(pLevelName, server) : loadGraal(pLevelName, server));
}

bool TLevel::loadGraal(const CString& pLevelName, TServer* server)
{
	return true;
}

bool TLevel::loadNW(const CString& pLevelName, TServer* server)
{
	CFileSystem fileSystem = server->getFileSystem();

	// Path-To-File
	levelName = pLevelName;
	fileName = fileSystem.find(pLevelName);
	modTime = fileSystem.getModTime(pLevelName);

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

		// Get rid of \r
		if (curLine[curLine.size() - 1][curLine[curLine.size() - 1].length() - 1] == '\r')
			curLine[curLine.size() - 1].removeI(curLine[curLine.size() - 1].length() - 1, 1);

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
			if ((curLine[3] = CString(findItemId(curLine[3]))) == "-1")
				continue;

			levelChests.push_back(new TLevelChest(curLine));
		}
		else if (curLine[0] == "LINK")
		{
			if (curLine.size() != 8)
				continue;
			if (fileSystem.find(curLine[1]).length() < 1)
				continue;

			levelLinks.push_back(new TLevelLink(curLine));
		}
		else if (curLine[0] == "NPC")
		{
			if (curLine.size() != 4)
				continue;

			// Grab the image properties.
			CString image = curLine[1];
			float x = (float)strtofloat(curLine[2]);
			float y = (float)strtofloat(curLine[3]);

			// Grab the NPC code.
			CString code;
			++i;
			while (i != fileData.end())
			{
				CString line = *i;
				if (line[line.length() - 1] == '\r') line.removeI(line.length() - 1, 1);
				if (line == "NPCEND") break;
				code << line << "\xa7";
				++i;
			}
			//printf( "image: %s, x: %.2f, y: %.2f, code: %s\n", image.text(), x, y, code.text() );
			// Add the new NPC.
			TNPC* npc = server->addNewNPC(image, code, x, y, this, true);
			levelNPCs.push_back(npc);
		}
	}

	return true;
}

/*
	TLevel: Find Level
*/
TLevel * TLevel::findLevel(const CString& pLevelName, TServer* server)
{
	// Find Appropriate Level by Name
	for (std::vector<TLevel *>::iterator i = levelList.begin(); i != levelList.end(); )
	{
		if ((*i) == 0)
		{
			i = levelList.erase(i);
			continue;
		}

		if ((*i)->getLevelName() == pLevelName)
			return (*i);

		++i;
	}

	// Load New Level
	TLevel *level = new TLevel();
	if (!level->loadLevel(pLevelName, server))
	{
		delete level;
		return 0;
	}

	// Return Level
	levelList.push_back(level);
	return level;
}
