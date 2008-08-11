#ifndef TACCOUNT_H
#define TACCOUNT_H

#include "CString.h"
#include "TServer.h"
#include "TLevel.h"
#include "TLevelChest.h"

enum
{
	PLPROP_NICKNAME			= 0,
	PLPROP_MAXPOWER			= 1,
	PLPROP_CURPOWER			= 2,
	PLPROP_RUPEESCOUNT		= 3,
	PLPROP_ARROWSCOUNT		= 4,
	PLPROP_BOMBSCOUNT		= 5,
	PLPROP_GLOVEPOWER		= 6,
	PLPROP_BOMBPOWER		= 7,
	PLPROP_SWORDPOWER		= 8,
	PLPROP_SHIELDPOWER		= 9,
	PLPROP_GANI				= 10,
	PLPROP_HEADGIF			= 11,
	PLPROP_CURCHAT			= 12,
	PLPROP_COLORS			= 13,
	PLPROP_ID				= 14,
	PLPROP_X				= 15,
	PLPROP_Y				= 16,
	PLPROP_SPRITE			= 17,
	PLPROP_STATUS			= 18,
	PLPROP_CARRYSPRITE		= 19,
	PLPROP_CURLEVEL			= 20,
	PLPROP_HORSEGIF			= 21,
	PLPROP_HORSEBUSHES		= 22,
	PLPROP_EFFECTCOLORS		= 23,
	PLPROP_CARRYNPC			= 24,
	PLPROP_APCOUNTER		= 25,
	PLPROP_MAGICPOINTS		= 26,
	PLPROP_KILLSCOUNT		= 27,
	PLPROP_DEATHSCOUNT		= 28,
	PLPROP_ONLINESECS		= 29,
	PLPROP_LASTIP			= 30,
	PLPROP_UDPPORT			= 31,
	PLPROP_ALIGNMENT		= 32,
	PLPROP_ADDITFLAGS		= 33,
	PLPROP_ACCOUNTNAME		= 34,
	PLPROP_BODYIMG			= 35,
	PLPROP_RATING			= 36,
	PLPROP_GATTRIB1			= 37,
	PLPROP_GATTRIB2			= 38,
	PLPROP_GATTRIB3			= 39,
	PLPROP_GATTRIB4			= 40,
	PLPROP_GATTRIB5			= 41,
	PLPROP_UNKNOWN42		= 42,
	PLPROP_GMAPLEVELX		= 43,
	PLPROP_GMAPLEVELY		= 44,
	PLPROP_Z				= 45,
	PLPROP_GATTRIB6			= 46,
	PLPROP_GATTRIB7			= 47,
	PLPROP_GATTRIB8			= 48,
	PLPROP_GATTRIB9			= 49,
	PLPROP_JOINLEAVELVL		= 50,
	PLPROP_PCONNECTED		= 51,
	PLPROP_PLANGUAGE		= 52,
	PLPROP_PSTATUSMSG		= 53,
	PLPROP_GATTRIB10		= 54,
	PLPROP_GATTRIB11		= 55,
	PLPROP_GATTRIB12		= 56,
	PLPROP_GATTRIB13		= 57,
	PLPROP_GATTRIB14		= 58,
	PLPROP_GATTRIB15		= 59,
	PLPROP_GATTRIB16		= 60,
	PLPROP_GATTRIB17		= 61,
	PLPROP_GATTRIB18		= 62,
	PLPROP_GATTRIB19		= 63,
	PLPROP_GATTRIB20		= 64,
	PLPROP_GATTRIB21		= 65,
	PLPROP_GATTRIB22		= 66,
	PLPROP_GATTRIB23		= 67,
	PLPROP_GATTRIB24		= 68,
	PLPROP_GATTRIB25		= 69,
	PLPROP_GATTRIB26		= 70,
	PLPROP_GATTRIB27		= 71,
	PLPROP_GATTRIB28		= 72,
	PLPROP_GATTRIB29		= 73,
	PLPROP_GATTRIB30		= 74,
	PLPROP_OSTYPE			= 75,	// 2.2+
	PLPROP_TEXTCODEPAGE		= 76,	// 2.2+
	PLPROP_UNKNOWN77		= 77,
	PLPROP_GMAPX			= 78,
	PLPROP_GMAPY			= 79,
};
#define propscount	80

class TServer;

class TAccount
{
	public:
		// Constructor - Deconstructor
		TAccount(TServer* pServer, const CString& pAccount = "defaultaccount");
		~TAccount();

		// Load/Save Account
		bool loadAccount(const CString& pAccount);
		bool saveAccount(bool pOnlyAccount = false);

		// Attribute-Managing
		bool hasChest(const TLevelChest *pChest, const CString& pLevel = "");
		bool hasWeapon(const CString& pWeapon);

	protected:
		TServer* server;

		// Player-Account
		bool isBanned, isFtp, isLoadOnly;
		CString adminIp, accountComments, accountName, banReason, lastFolder;
		int accountIp, adminRights;

		// Player-Attributes
		CString attrList[30], bodyImg, chatMsg, headImg, horseImg, gAni, language;
		CString levelName, nickName, shieldImg, swordImg;
		float deviation, oldDeviation, power, rating, x, y, z;
		int gmapx, gmapy, gmaplevelx, gmaplevely;
		int additionalFlags, ap, apCounter, arrowc, bombc, bombPower, carrySprite;
		unsigned char colors[5];
		int deaths, glovePower, gralatc, horsec, kills, mp, maxPower;
		int onlineTime, shieldPower, sprite, status, swordPower, udpport;
		time_t lastSparTime;
		unsigned char statusMsg;
		std::vector<CString> chestList, flagList, folderList, weaponList;
};

#endif // TACCOUNT_H