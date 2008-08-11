#ifndef TLEVEL_H
#define TLEVEL_H

#include <vector>
#include <map>
#include "ICommon.h"
#include "TServer.h"
#include "TLevelBaddy.h"
#include "TLevelBoardChange.h"
#include "TLevelChest.h"
#include "TLevelHorse.h"
#include "TLevelItem.h"
#include "TLevelLink.h"
#include "TLevelSign.h"

class TPlayer;
class TNPC;

class TLevel
{
	public:
		TLevel(TServer* pServer);
		~TLevel();

		// get crafted packets
		CString getBaddyPacket();
		CString getBoardPacket();
		CString getBoardChangesPacket();
		CString getChestPacket(TPlayer *pPlayer);
		CString getHorsePacket();
		CString getLinksPacket();
		CString getNpcsPacket(time_t time);
		CString getSignsPacket();
		inline CString getLevelName();

		// level-loading functions
		bool loadLevel(const CString& pLevelName);
		bool loadGraal(const CString& pLevelName);
		bool loadNW(const CString& pLevelName);

		// find level
		static TLevel * findLevel(const CString& pLevelName, TServer* server);

		// get functions
		short* getTiles();
		time_t getModTime()		{ return modTime; }

		// other functions
		bool alterBoard(CString& pTileData, int pX, int pY, int pWidth, int pHeight, TPlayer* player);
		bool doTimedEvents();

	private:
		TServer* server;
		time_t modTime;
		bool levelSpar;
		short levelTiles[4096];
		CString fileName, fileVersion, levelName;
		std::vector<TLevelBaddy *> levelBaddys;
		std::vector<TLevelBoardChange *> levelBoardChanges;
		std::vector<TLevelChest *> levelChests;
		std::vector<TLevelHorse *> levelHorses;
		std::vector<TLevelItem *> levelItems;
		std::vector<TLevelLink *> levelLinks;
		std::vector<TLevelSign *> levelSigns;
		std::vector<TNPC *> levelNPCs;
};


inline CString TLevel::getLevelName()
{
	return levelName;
}

inline short* TLevel::getTiles()
{
	return levelTiles;
}

#endif // TLEVEL_H
