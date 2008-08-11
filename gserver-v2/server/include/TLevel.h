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
		TLevel();
		~TLevel();

		// get crafted packets
		CString getBaddyPacket();
		CString getBoardPacket();
		CString getChestPacket(TPlayer *pPlayer);
		CString getHorsePacket();
		CString getLinksPacket();
		CString getNpcsPacket(time_t time);
		CString getSignsPacket();
		inline CString getLevelName();

		// level-loading functions
		bool loadLevel(const CString& pLevelName, TServer* server);
		bool loadGraal(const CString& pLevelName, TServer* server);
		bool loadNW(const CString& pLevelName, TServer* server);

		// find level
		static TLevel * findLevel(const CString& pLevelName, TServer* server);

		// get functions
		short* getTiles();
		time_t getModTime()		{ return modTime; }

		// other functions
		bool alterBoard(CString& pTileData, int pX, int pY, int pWidth, int pHeight, TPlayer* player, TServer* server);
		bool doTimedEvents();

	private:
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
