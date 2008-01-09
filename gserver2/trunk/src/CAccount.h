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
	CString accountName, comments, adminIp, folderRights;
	int adminRights, adminLevel;
	bool banned, loadOnly;
	//Props
	CString nickName, levelName, headImage, bodyImage, swordImage, shieldImage, horseImage;
	float x, y, z, power, maxPower;
	int rubins, darts, bombCount, glovePower, swordPower, shieldPower, sprite, status, horseBushes, magicPoints, kills, deaths, onlineSecs, lastIp, ap, apCounter;
	char colors[5];
	CStringList myWeapons, myChests, myFlags;

	//Load account only
	bool loadDBAccount(CString& pAccount);
	//Load account and playerworld stats
	bool loadWorldProps(CString& pAccount);
	bool loadWorldPropsIni();
	void saveAccount();
	void saveWorldProps();
};
#endif
