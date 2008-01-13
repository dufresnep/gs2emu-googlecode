// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef CACCOUNTH
#define CACCOUNTH
#include "CString.h"
#include "CStringList.h"

class CAccount
{
    public:
		CAccount();

		bool banned, ftpOn, loadOnly;
		CString accountName, adminIp, banReason, bodyImage, comments, gAni, headImage, horseImage, language, lastFolder, levelName, myAttr[30], nickName, shieldImage, swordImage;
		char colors[5];
		int adminRights, ap, apCounter, bombs, bombPower, darts, deaths, glovePower, horseBushes, kills, lastIp, magicPoints, onlineSecs, rubins, shieldPower, sprite, status, swordPower, type;
		float x, y, z, power, maxPower;
		CStringList myChests, myFlags, myFolders, myWeapons;

		// Load Graal Account
		bool loadDBAccount(CString pAccount);
		bool loadWorldPropsIni();
		void saveAccount(bool pAttributes = false);
};
#endif
