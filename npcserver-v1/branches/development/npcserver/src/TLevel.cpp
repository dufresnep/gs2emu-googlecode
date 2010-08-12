
#include "ICommon.h"


#include "TNPCServer.h"
#include "TLevel.h"
#include "TPlayer.h"
#include "TNPC.h"

/*
	Global Variables
*/
CString base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
short respawningTiles[] = {
	0x1ff, 0x3ff, 0x2ac, 0x002, 0x200,
	0x022, 0x3de, 0x1a4, 0x14a, 0x674,
};

/*
	TLevel: Constructor - Deconstructor
*/
TLevel::TLevel(TNPCServer* pServer,CString pLevelName)
:
server(pServer), modTime(0), levelSpar(false), levelSingleplayer(false), levelGroup(false)
{
	memset(levelTiles, 0, sizeof(levelTiles));
actualLevelName = levelName = pLevelName;

	
}

TLevel::~TLevel()
{
	// Delete NPCs.
	{
		/*// Get some pointers.
		std::vector<TNPC*>* npcList = server->getNPCList();
		std::vector<TNPC*>* npcIds = server->getNPCIdList();

		// Remove every NPC in the level.
		if (npcList->size() != 0 && npcIds->size() != 0)
		{
			for (std::vector<TNPC*>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
				server->deleteNPC(*i, this, false);
			levelNPCs.clear();
		}*/
	}

	// Delete items.
	for (std::vector<TLevelItem*>::iterator i = levelItems.begin(); i != levelItems.end(); ++i)
	{
		TLevelItem* item = *i;
		CString packet = CString() >> (char)PLO_ITEMDEL >> (char)(item->getX() * 2) >> (char)(item->getY() * 2);
		for (std::vector<TPlayer*>::iterator j = levelPlayerList.begin(); j != levelPlayerList.end(); ++j)
		delete item;
	}
	levelItems.clear();

	// Delete board changes.
	for (std::vector<TLevelBoardChange*>::iterator i = levelBoardChanges.begin(); i != levelBoardChanges.end(); ++i) delete *i;
	levelBoardChanges.clear();
}

/*
	TLevel: Get Crafted Packets
*/


CString TLevel::getBoardPacket()
{
	CString retVal;
	retVal.writeGChar(PLO_BOARDPACKET);
	retVal.write((char *)levelTiles, sizeof(levelTiles));
	retVal << "\n";
	return retVal;
}

CString TLevel::getBoardChangesPacket(time_t time)
{
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

CString TLevel::getBoardChangesPacket2(time_t time)
{
	CString retVal;
	retVal >> (char)PLO_BOARDMODIFY;
	for (std::vector<TLevelBoardChange*>::const_iterator i = levelBoardChanges.begin(); i != levelBoardChanges.end(); ++i)
	{
		TLevelBoardChange* change = *i;
		if (change->getModTime() >= time)
			retVal << change->getBoardStr();
	}
	return retVal;
}

TLevel* TLevel::findLevel(const CString& pLevelName, TNPCServer* server)
{
	std::vector<TLevel*>* levelList = server->getLevelList();

	if (levelList->empty() == 0) return 0;

	// Find Appropriate Level by Name
	for (std::vector<TLevel *>::iterator i = levelList->begin(); i != levelList->end(); )
	{
		if ((*i) == 0)
		{
			i = levelList->erase(i);
			continue;
		}

		if ((*i)->getLevelName().toLower() == pLevelName.toLower())
			return (*i);

		++i;
	}
	return 0;
}


bool TLevel::alterBoard(CString& pTileData, int pX, int pY, int pWidth, int pHeight, TPlayer* player)
{
	if (pX < 0 || pY < 0 || pX > 63 || pY > 63 ||
		pWidth < 1 || pHeight < 1 ||
		pX + pWidth > 64 || pY + pHeight > 64)
		return false;


	// Do the check for the push-pull block.
	if (pWidth == 4 && pHeight == 4)
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
				//server->sendToGserver(CString() >> (char)PLO_BOARDMODIFY >> (char)pX >> (char)pY >> (char)pWidth >> (char)pHeight << pTileData);
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
	int respawnTime = 15;
	bool doRespawn = false;
	short testTile = levelTiles[pX + (pY * 64)];
	int tileCount = sizeof(respawningTiles) / sizeof(short);
	for (int i = 0; i < tileCount; ++i)
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
void TLevel::callSpecificNPC(float x, float y,CString function,TPlayer * player,CString params)
{
	for (std::vector<TNPC *>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		TNPC * pNPC = *i;
		if (pNPC == 0) continue;
 
		if ((pNPC->getX() == x && pNPC->getY() == y))
		{
			pNPC->callFunction(function,player,params);
		}
	}
}


void TLevel::callNPCS(CString function,TPlayer * player,CString params)
	{

	if (levelNPCs.empty()) return;

	for (std::vector<TNPC *>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		TNPC * pNPC = *i;
		if (pNPC == 0) continue;
		
		pNPC->callFunction(function,player,params);
	}
}

bool TLevel::addItem(float pX, float pY, char pItem)
{
	levelItems.push_back(new TLevelItem(pX, pY, pItem));
	return true;
}

char TLevel::removeItem(float pX, float pY)
{
	for (std::vector<TLevelItem*>::iterator i = levelItems.begin(); i != levelItems.end(); ++i)
	{
		TLevelItem* item = *i;
		if (item->getX() == pX && item->getY() == pY)
		{
			char itemType = item->getItem();
			delete item;
			levelItems.erase(i);
			return itemType;
		}
	}
	return -1;
}

void TLevel::clearLevel()
{
	//remove all npcs
	for (std::vector<TNPC*>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		TNPC* npc = *i;
		if (npc == 0) continue;
		server->deleteNPC(npc);
	}
	levelNPCs.clear();


	//remove links


	//remove signs


	//remove items

}


int TLevel::addPlayer(TPlayer* player)
{
	for (std::vector<TPlayer *>::iterator i = levelPlayerList.begin(); i != levelPlayerList.end(); ++i)
	{
		TPlayer* search = *i;
		if (player == search) return levelPlayerList.size() - 1;
	}

	levelPlayerList.push_back(player);

	callNPCS("onPlayerenters",player,"");

	return levelPlayerList.size() - 1;
}

void TLevel::removePlayer(TPlayer* player)
{
	if (levelPlayerList.empty()) return;

	for (std::vector<TPlayer *>::iterator i = levelPlayerList.begin(); i != levelPlayerList.end();)
	{
		TPlayer* search = *i;
		if (player == search)
			i = levelPlayerList.erase(i);
		else ++i;

	}
}

TPlayer* TLevel::getPlayer(unsigned int id)
{
	if (id >= levelPlayerList.size()) return 0;
	return levelPlayerList[id];
}



bool TLevel::addNPC(TNPC* npc)
{
	for (std::vector<TNPC*>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		TNPC* search = *i;
		if (npc == search) return false;
	}
	levelNPCs.push_back(npc);

	/* Deprecated code? Already called in TLevel::addPlayer(TPlayer* player) so this ends up doubling the count of players o_O
	for (std::vector<TPlayer *>::iterator i = levelPlayerList.begin(); i != levelPlayerList.end(); ++i)
	{
		TPlayer* player = *i;
		
		npc->callFunction("onPlayerenters",player,"");
		
	}*/

	return true;
}

void TLevel::removeNPC(TNPC* npc)
{
	for (std::vector<TNPC*>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); )
	{
		TNPC* search = *i;
		if (npc == search)
			i = levelNPCs.erase(i);
		else ++i;
	}
}

void TLevel::DoTimedEvents()
{
	//CPU HOGGER
	if (levelNPCs.empty()) return;

	for (std::vector<TNPC *>::iterator i = levelNPCs.begin(); i != levelNPCs.end(); ++i)
	{
		if (*i == 0) continue;
		if ((*i)->getServerScript().isEmpty()) continue;

		//npc collision (primitive)
		for (std::vector<TPlayer *>::iterator p = levelPlayerList.begin(); p != levelPlayerList.end(); ++p)
		{
			float nWidth = (*i)->width/16;
			float nHeight = (*i)->height/16;

			if (nHeight <= 0 || nWidth <= 0) continue;

			TPlayer* player = *p;
			
			float pX = player->mX;
			float pY = player->mY;

			float nX = (*i)->getX();
			float nY = (*i)->getY();

			float left1, left2;
			float right1, right2;
			float top1, top2;
			float bottom1, bottom2;

			left1 = pX+1;
			left2 = nX;
			right1 = pX + 2.4f;
			right2 = nX + (nWidth);
			top1 = pY+1.1f;
			top2 = nY;
			bottom1 = pY + 2.9f;
			bottom2 = nY + (nHeight);

			if (player->mPlayerDir == 0) top1 -= .1f;
			if (player->mPlayerDir == 1) left1 -= 1;
			if (player->mPlayerDir == 2) bottom1 += .1f;
			if (player->mPlayerDir == 3) right1 += .1f;

			if (bottom1 < top2) 
			{
				player->touchingnpcs[(*i)->getId()] = false;
				continue;
			}
			if (top1 > bottom2)
			{
				player->touchingnpcs[(*i)->getId()] = false;
				continue;
			}

			if (right1 < left2) 
			{
				player->touchingnpcs[(*i)->getId()] = false;
				continue;
			}
			if (left1 > right2)
			{
				player->touchingnpcs[(*i)->getId()] = false;
				continue;
			}

			if (player->touchingnpcs[(*i)->getId()] == true) continue;

			player->touchingnpcs[(*i)->getId()] = true;
			(*i)->callFunction("onPlayertouchsme",player,"");		
		}

		//TIMEOUTS
		if ((*i)->getTimer() > 0) (*i)->decTimer();

		if ((*i)->getTimer() <= 0 && (*i)->getTimer() > -1)
		{
			//Trigger everyone in the level - timeout
			//for (std::vector<TPlayer *>::iterator p = levelPlayerList.begin(); p != levelPlayerList.end();++p)
			//{
			//	if (*p == 0) continue;
			(*i)->setTimer(-1);
			(*i)->callFunction("onTimeout",NULL,"");
			//}
			
		}
	}
}



