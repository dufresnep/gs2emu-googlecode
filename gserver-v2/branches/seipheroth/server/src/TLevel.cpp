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
short respawningTiles[10] = {
	0x1ff, 0x3ff, 0x2ac, 0x002, 0x200,
	0x022, 0x3de, 0x1a4, 0x14a, 0x674
};

/*
	TLevel: Constructor - Deconstructor
*/
TLevel::TLevel(TServer* pServer)
:
server(pServer), modTime(0), levelSpar(false)
{
	memset(levelTiles, 0, sizeof(levelTiles));

	// Baddy id 0 breaks the client.  Put a null pointer in id 0.
	levelBaddyIds.resize(1, 0);
}

TLevel::~TLevel()
{
}

/*
	TLevel: Get Crafted Packets
*/
CString TLevel::getBaddyPacket()
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	CString retVal;
	for (std::vector<TLevelBaddy *>::iterator i = levelBaddies.begin(); i != levelBaddies.end(); ++i)
	{
		TLevelBaddy* baddy = *i;
		if (baddy == 0) continue;
		if (baddy->getProp(BDPROP_MODE).readGChar() != BDMODE_DIE)
			retVal >> (char)PLO_BADDYPROPS >> (char)baddy->getId() << baddy->getProps() << "\n";
	}
	return retVal;
}

CString TLevel::getBoardPacket()
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	CString retVal;
	retVal.writeGChar(PLO_BOARDPACKET);
	retVal.write((char *)levelTiles, sizeof(levelTiles));
	retVal << "\n";
	return retVal;
}

CString TLevel::getBoardChangesPacket(time_t time)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	CString retVal;
	retVal >> (char)PLO_LEVELBOARD;
	for (std::vector<TLevelBoardChange*>::const_iterator i = levelBoardChanges.begin(); i != levelBoardChanges.end(); ++i)
	{
		TLevelBoardChange* change = *i;
		if (change->getModTime() >= time)
			retVal << change->getBoardStr();
	}
	return retVal;
}

CString TLevel::getChestPacket(TPlayer *pPlayer)
{
	if (pPlayer == 0)
		return CString();

	CString retVal;

	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	for (std::vector<TLevelChest *>::iterator i = levelChests.begin(); i != levelChests.end(); ++i)
	{
		TLevelChest *chest = *i;
		bool hasChest = pPlayer->hasChest(chest);

		retVal >> (char)PLO_LEVELCHEST >> (char)(hasChest ? 1 : 0) >> (char)chest->getX() >> (char)chest->getY();
		if (!hasChest) retVal >> (char)chest->getItemIndex() >> (char)chest->getSignIndex();
		retVal << "\n";
	}

	return retVal;
}

CString TLevel::getHorsePacket()
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	CString retVal;
	for (std::vector<TLevelHorse *>::iterator i = levelHorses.begin(); i != levelHorses.end(); ++i)
	{
		TLevelHorse *horse = *i;
		retVal >> (char)PLO_HORSEADD << horse->getHorseStr() << "\n";
	}

	return retVal;
}

CString TLevel::getLinksPacket()
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
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
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
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
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	CString retVal;
	for (std::vector<TLevelSign*>::const_iterator i = levelSigns.begin(); i != levelSigns.end(); ++i)
	{
		TLevelSign* sign = *i;
		retVal >> (char)PLO_LEVELSIGN << sign->getSignStr() << "\n";
	}
	return retVal;
}

/*
	TLevel: Level-Loading Functions
*/
bool TLevel::loadLevel(const CString& pLevelName)
{
	return (getExtension(pLevelName) == ".nw" ? loadNW(pLevelName) : loadGraal(pLevelName));
}

bool TLevel::loadGraal(const CString& pLevelName)
{
	return true;
}

bool TLevel::loadNW(const CString& pLevelName)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	CFileSystem* fileSystem = server->getFileSystem();

	// Path-To-File
	levelName = pLevelName;
	fileName = fileSystem->find(pLevelName);
	modTime = fileSystem->getModTime(pLevelName);

	// Load File
	std::vector<CString> fileData = CString::loadToken(fileName);
	if (fileData.size() == 0)
		return false;

	// Grab File Version
	fileVersion = fileData[0];

	// Parse Level
	for (std::vector<CString>::iterator i = fileData.begin(); i != fileData.end(); ++i)
	{
		// Tokenize
		std::vector<CString> curLine = i->removeAll("\r").tokenize();
		if (curLine.size() < 1)
			continue;

		// Parse Each Type
		if (curLine[0] == "BOARD")
		{
			if (curLine.size() != 6)
				continue;

			int x, y, w, layer;
			x = strtoint(curLine[1]);
			y = strtoint(curLine[2]);
			w = strtoint(curLine[3]);
			layer = strtoint(curLine[4]);

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
			if ((curLine[3] = CString(TLevelItem::getItemId(curLine[3]))) == "-1")
				continue;

			levelChests.push_back(new TLevelChest(curLine));
		}
		else if (curLine[0] == "LINK")
		{
			if (curLine.size() != 8)
				continue;
			if (fileSystem->find(curLine[1]).length() < 1)
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
				CString line = i->removeAll("\r");
				if (line == "NPCEND") break;
				code << line << "\xa7";
				++i;
			}
			//printf( "image: %s, x: %.2f, y: %.2f, code: %s\n", image.text(), x, y, code.text() );
			// Add the new NPC.
			TNPC* npc = server->addNPC(image, code, x, y, this, true, false);
			levelNPCs.push_back(npc);
		}
		else if (curLine[0] == "SIGN")
		{
			if (curLine.size() != 3)
				continue;

			int x = strtoint(curLine[1]);
			int y = strtoint(curLine[2]);

			// Grab the NPC code.
			CString text;
			++i;
			while (i != fileData.end())
			{
				CString line = i->removeAll("\r");
				if (line == "SIGNEND") break;
				text << line << "\n";
				++i;
			}

			// Add the new sign.
			levelSigns.push_back(new TLevelSign(x, y, text));
		}
		else if (curLine[0] == "BADDY")
		{
			if (curLine.size() != 4)
				continue;

			int x = strtoint(curLine[1]);
			int y = strtoint(curLine[2]);
			int type = strtoint(curLine[3]);

			// Add the baddy.
			TLevelBaddy* baddy = addBaddy((float)x, (float)y, type);
			if (baddy == 0)
				continue;

			// Load the verses.
			std::vector<CString> bverse;
			++i;
			while (i != fileData.end())
			{
				CString line = i->removeAll("\r");
				if (line == "BADDYEND") break;
				bverse.push_back(line);
				++i;
			}
			CString props;
			for (char j = 0; j < (char)bverse.size(); ++j)
				props >> (char)(BDPROP_VERSESIGHT + j) >> (char)bverse[j].length() << bverse[j];
			if (props.length() != 0) baddy->setProps(props);
		}
	}

	return true;
}

/*
	TLevel: Find Level
*/
TLevel* TLevel::findLevel(const CString& pLevelName, TServer* server)
{
	std::vector<TLevel*>* levelList = server->getLevelList();

	// Find Appropriate Level by Name
	boost::recursive_mutex::scoped_lock lock_levelList(server->m_levelList);
	for (std::vector<TLevel *>::iterator i = levelList->begin(); i != levelList->end(); )
	{
		if ((*i) == 0)
		{
			i = levelList->erase(i);
			continue;
		}

		if ((*i)->getLevelName() == pLevelName)
			return (*i);

		++i;
	}

	// Load New Level
	TLevel *level = new TLevel(server);
	if (!level->loadLevel(pLevelName))
	{
		delete level;
		return 0;
	}

	// Return Level
	levelList->push_back(level);
	return level;
}

bool TLevel::alterBoard(CString& pTileData, int pX, int pY, int pWidth, int pHeight, TPlayer* player)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	if( pX < 0 || pY < 0 || pX > 63 || pY > 63 ||
		pWidth < 1 || pHeight < 1 ||
		pX + pWidth > 64 || pY + pHeight > 64 )
		return false;

	CSettings* settings = server->getSettings();

	// Do the check for the push-pull block.
	if (pWidth == 4 && pHeight == 4 && settings->getBool("clientsidepushpull", true))
	{
		// Try to find the top-left corner tile.
		int i;
		for (i = 0; i < 16; ++i)
		{
			short stoneCheck = pTileData.readGShort();
			if (stoneCheck == 0x06E4 || stoneCheck == 0x07CE)
				break;
		}

		// Check if we found a possible push-pull block.
		if (i != 16 && i < 11)
		{
			// Go back one full short so the first readByte2() returns the top-left corner.
			pTileData.setRead(i * 2);

			int foundCount = 0;
			for (int j = 0; j < 6; ++j)
			{
				// Read a piece.
				short stoneCheck = pTileData.readGShort();

				// A valid stone will have pieces at the following j locations.
				if (j == 0 || j == 1 || j == 4 || j == 5)
				{
					switch (stoneCheck)
					{
						// red
						case 0x6E4:
						case 0x6E5:
						case 0x6F4:
						case 0x6F5:
						// blue
						case 0x7CE:
						case 0x7CF:
						case 0x7DE:
						case 0x7DF:
							foundCount++;
							break;
					}
				}
			}
			pTileData.setRead(0);

			// Check if we found a full tile.  If so, don't accept the change.
			if (foundCount == 4)
			{
				player->sendPacket(CString() >> (char)PLO_BOARDMODIFY >> (char)pX >> (char)pY >> (char)pWidth >> (char)pHeight << pTileData);
				return false;
			}
		}
	}

	// Delete any existing changes within the same region.
	for (std::vector<TLevelBoardChange*>::iterator i = levelBoardChanges.begin(); i != levelBoardChanges.end(); )
	{
		TLevelBoardChange* change = *i;
		if ((change->getX() >= pX && change->getX() + change->getWidth() <= pX + pWidth) &&
			(change->getY() >= pY && change->getY() + change->getHeight() <= pY + pHeight))
		{
			delete change;
			i = levelBoardChanges.erase(i);
		} else ++i;
	}

	// Check if the tiles should be respawned.
	// Only tiles in the respawningTiles array are allowed to respawn.
	// These are things like signs, bushes, pots, etc.
	int respawnTime = settings->getInt("respawntime", 15);
	bool doRespawn = false;
	short testTile = levelTiles[pX + (pY * 64)];
	for (int i = 0; i < 10; ++i)
		if (testTile == respawningTiles[i]) doRespawn = true;

	// Grab old tiles for the respawn.
	CString oldTiles;
	if (doRespawn)
	{
		for (int j = pY; j < pY + pHeight; ++j)
		{
			for (int i = pX; i < pX + pWidth; ++i)
				oldTiles.writeGShort(levelTiles[i + (j * 64)]);
		}
	}

	// TODO: old gserver didn't save the board change if oldTiles.length() == 0.
	// Should we do it that way still?
	levelBoardChanges.push_back(new TLevelBoardChange(pX, pY, pWidth, pHeight, pTileData, oldTiles, (doRespawn ? respawnTime : -1)));
	return true;
}

bool TLevel::addItem(float pX, float pY, char pItem)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	levelItems.push_back(new TLevelItem(pX, pY, pItem));
	return true;
}

char TLevel::removeItem(float pX, float pY)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	for (std::vector<TLevelItem*>::iterator i = levelItems.begin(); i != levelItems.end(); ++i)
	{
		TLevelItem* item = *i;
		if (item->getX() == pX && item->getY() == pY)
		{
			char itemType = item->getItem();
			levelItems.erase(i);
			return itemType;
		}
	}
	return -1;
}

bool TLevel::addHorse(CString& pImage, float pX, float pY, char pDir, char pBushes)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	levelHorses.push_back(new TLevelHorse(pImage, pX, pY, pDir, pBushes));
	return true;
}

void TLevel::removeHorse(float pX, float pY)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	for (std::vector<TLevelHorse *>::iterator i = levelHorses.begin(); i != levelHorses.end(); ++i)
	{
		TLevelHorse* horse = *i;
		if (horse->getX() == pX && horse->getY() == pY)
		{
			levelHorses.erase(i);
			return;
		}
	}
}

TLevelBaddy* TLevel::addBaddy(float pX, float pY, char pType)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);

	// Limit of 50 baddies per level.
	if (levelBaddies.size() > 50) return 0;

	// New Baddy
	TLevelBaddy* newBaddy = new TLevelBaddy(pX, pY, pType, this, server);
	levelBaddies.push_back(newBaddy);

	// Assign Baddy Id
	// Don't assign id 0.
	for (unsigned int i = 1; i < levelBaddyIds.size(); ++i)
	{
		if (levelBaddyIds[i] == 0)
		{
			levelBaddyIds[i] = newBaddy;
			newBaddy->setId((char)i);
			return newBaddy;
		}
	}

	newBaddy->setId((char)levelBaddyIds.size());
	levelBaddyIds.push_back(newBaddy);

	return newBaddy;
}

void TLevel::removeBaddy(char pId)
{
	// Don't allow us to remove id 0 or any id over 50.
	if (pId < 1 || pId > 50) return;

	boost::recursive_mutex::scoped_lock lock(m_preventChange);

	TLevelBaddy* baddy = levelBaddyIds[pId];
	delete baddy;
	levelBaddyIds[pId] = 0;
}

TLevelBaddy* TLevel::getBaddy(char id)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	if ((unsigned char)id >= levelBaddyIds.size()) return 0;
	return levelBaddyIds[id];
}

int TLevel::addPlayer(TPlayer* player)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	levelPlayerList.push_back(player);
	return levelPlayerList.size() - 1;
}

void TLevel::removePlayer(TPlayer* player)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	for (std::vector<TPlayer *>::iterator i = levelPlayerList.begin(); i != levelPlayerList.end(); )
	{
		TPlayer* search = *i;
		if (player == search)
			i = levelPlayerList.erase(i);
		else ++i;
	}
}

TPlayer* TLevel::getPlayer(unsigned int id)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	if (id >= levelPlayerList.size()) return 0;
	return levelPlayerList[id];
}

TMap* TLevel::getMap() const
{
	return server->getMap(this);
}

bool TLevel::addNPC(TNPC* npc)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	for (std::vector<TNPC*>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		TNPC* search = *i;
		if (npc == search) return false;
	}
	levelNPCs.push_back(npc);
	return true;
}

void TLevel::removeNPC(TNPC* npc)
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);
	for (std::vector<TNPC*>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); )
	{
		TNPC* search = *i;
		if (npc == search)
			i = levelNPCs.erase(i);
		else ++i;
	}
}

bool TLevel::doTimedEvents()
{
	boost::recursive_mutex::scoped_lock lock(m_preventChange);

	// Check if we should revert any board changes.
	for (std::vector<TLevelBoardChange*>::iterator i = levelBoardChanges.begin(); i != levelBoardChanges.end(); ++i)
	{
		TLevelBoardChange* change = *i;
		int respawnTimer = change->timeout.doTimeout();
		if (respawnTimer == 0)
		{
			// Put the old data back in.  DON'T DELETE THE CHANGE.
			// The client remembers board changes and if we delete the
			// change, the client won't get the new data.
			change->swapTiles();
			change->setModTime(time(0));
			server->sendPacketToLevel(CString() >> (char)PLO_BOARDMODIFY << change->getBoardStr(), this);
		}
	}

	// Check if any items have timed out.
	// This allows us to delete items that have disappeared if nobody is in the level to send
	// the PLI_ITEMDEL packet.
	for (std::vector<TLevelItem*>::iterator i = levelItems.begin(); i != levelItems.end(); )
	{
		TLevelItem* item = *i;
		int deleteTimer = item->timeout.doTimeout();
		if (deleteTimer == 0)
		{
			delete item;
			i = levelItems.erase(i);
		} else ++i;
	}

	// Check if any horses need to be deleted.
	for (std::vector<TLevelHorse *>::iterator i = levelHorses.begin(); i != levelHorses.end(); )
	{
		TLevelHorse* horse = *i;
		int deleteTimer = horse->timeout.doTimeout();
		if (deleteTimer == 0)
		{
			server->sendPacketToLevel(CString() >> (char)PLO_HORSEDEL >> (char)(horse->getX() * 2) >> (char)(horse->getY() * 2), this);
			delete horse;
			i = levelHorses.erase(i);
		}
		else ++i;
	}

	// Check if any baddies need to be respawned.
	for (std::vector<TLevelBaddy *>::iterator i = levelBaddies.begin(); i != levelBaddies.end(); ++i)
	{
		TLevelBaddy* baddy = *i;
		int respawnTimer = baddy->timeout.doTimeout();
		if (respawnTimer == 0)
		{
			baddy->reset();
			server->sendPacketToLevel(CString() >> (char)PLO_BADDYPROPS >> (char)baddy->getId() << baddy->getProps(), this);
		}
	}
	return true;
}
