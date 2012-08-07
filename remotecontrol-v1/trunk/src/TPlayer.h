#ifndef TPLAYER_H
#define TPLAYER_H

#include <QTreeWidgetItem>
#include <vector>
#include "CBuffer.h"
#include "CPacket.h"

class TPlayer
{
	public:
		TPlayer(int Id = -1);
		~TPlayer();

		// Functions
		QTreeWidgetItem* getItem();
		void updateItem();

		CPacket getProp(int pProp);
		void setProps(CPacket& pPacket);

		// Account-Attributes
		bool isBanned, isLoadOnly;
		CBuffer acctBan, acctComments, acctIp, acctName, acctNick;
		int acctRights;
		std::vector<CBuffer> acctFolders;

		// Player-Attributes
		CBuffer plyrAttr[30], plyrBody, plyrGani, plyrHead, plyrHorse, plyrIp, plyrLang, plyrLevel, plyrShield, plyrSword;
		char plyrColors[5];
		int plyrAp, plyrBombPower, plyrBombs, plyrDarts, plyrDeaths, plyrGlovePower, plyrGralats, plyrId, plyrKills, plyrMagic, plyrOnline, plyrShieldPower, plyrStatus, plyrSwordPower;
		float plyrMaxPower, plyrPower, plyrRating, plyrDev, plyrX, plyrY, plyrZ;
		std::vector<CBuffer> plyrChests, plyrFlags, plyrWeapons;

	private:
		QTreeWidgetItem *playerItem;
};

#endif // TPLAYER_H

/*
bool sendRcProps[propscount] =
{
	false, true,  true,  true,  true,  true,  // 0-5
	true,  true,  true,  true,  true,  true,  // 6-11
	false, true,  false, true,  true,  false, // 12-17
	true,  false, true,  true,  true,  true,  // 18-23 (19 Status)
	false, false, true,  true,  true,  true,  // 24-29
	true,  false, true,  false, false, true,  // 30-35
	true,  false, false, false, false, false, // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false  // 48-53
};
*/
