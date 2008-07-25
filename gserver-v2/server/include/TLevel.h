#ifndef TLEVEL_H
#define TLEVEL_H

#include <vector>
#include "CString.h"
#include "TLevelBaddy.h"
#include "TLevelChest.h"
#include "TLevelHorse.h"
#include "TLevelItem.h"
#include "TLevelLink.h"

class TPlayer;

class TLevel
{
	public:
		TLevel();
		~TLevel();

/*
		CString getEntityPacket(); // baddy-horse-items
		CString getLevelPacket();  // chests-links-tiles
		CString getNpcsPacket();   // npcs
*/
		// get crafted packets
		CString getBaddyPacket();
		CString getBoardPacket();
		CString getChestPacket(TPlayer *pPlayer);
		CString getHorsePacket();
		CString getLinksPacket();
		CString getNpcsPacket();
		CString getSignsPacket();
		inline CString getLevelName();

		// level-loading functions
		bool loadLevel(const CString& pFileName);
		bool loadGraal(const CString& pFileName);
		bool loadNW(const CString& pFileName);

		// find level
		static TLevel * findLevel(const CString& pLevelName);

	private:
		bool levelSpar;
		short levelTiles[4096];
		CString fileName, fileVersion, levelName;
		std::vector<TLevelBaddy *> levelBaddys;
		std::vector<TLevelChest *> levelChests;
		std::vector<TLevelHorse *> levelHorses;
		std::vector<TLevelItem *> levelItems;
		std::vector<TLevelLink *> levelLinks;
};


inline CString TLevel::getLevelName()
{
	return levelName;
}


#endif // TLEVEL_H
