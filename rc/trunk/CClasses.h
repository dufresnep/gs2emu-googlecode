#ifndef CCLASSES_H
#define CCLASSES_H

#include "CBuffer.h"
#include "CPacket.h"
#include "CRCSock.h"

class CRCChat;

class CPlayer
{
	public:
		CPlayer(CRCChat *tab);
		~CPlayer();
		
		void setProps(CPacket &pProps);
		
		CRCChat *tab;
		
		bool banned, loadOnly;
		char colors[5];
		CString account, adminIp, banhistory, banreason1, banreason2, bodyimg, bowImage, chatMsg, comments, folderRights, gAni, ganiAttribs[5], headimg, horseimg, language, level, nickname, playerWorld, shieldImage, swordImage;
		float maxPower, power, x, y;
		int additionalFlags, adminRights, adminLevel, ap, apCounter, banTime, bombCount, bombPower, carrySprite, darts, deaths, failAttempts, glovePower, horseBushes, id, kills, lastIp, magicPoints, noMovement, onlineSecs, packCount, rating, rubins, shieldPower, sprite, status, statusMsg, swordPower, type, udpPort;
		CStringList adminFolders, myChests, myWeapons, myFlags;
		QStringList banList;
		
	private:
};

#endif