#ifndef TACCOUNT_H
#define TACCOUNT_H

#include "ICommon.h"
#include "CString.h"
#include "TServer.h"
#include "TLevel.h"
#include "TLevelChest.h"

enum
{
	PLPERM_WARPTO				= 0x00001,
	PLPERM_WARPTOPLAYER			= 0x00002,
	PLPERM_SUMMON				= 0x00004,
	PLPERM_UPDATELEVEL			= 0x00008,
	PLPERM_DISCONNECT			= 0x00010,
	PLPERM_VIEWATTRIBUTES		= 0x00020,
	PLPERM_SETATTRIBUTES		= 0x00040,
	PLPERM_SETSELFATTRIBUTES	= 0x00080,
	PLPERM_RESETATTRIBUTES		= 0x00100,
	PLPERM_ADMINMSG				= 0x00200,
	PLPERM_SETRIGHTS			= 0x00400,
	PLPERM_BAN					= 0x00800,
	PLPERM_SETCOMMENTS			= 0x01000,
	PLPERM_INVISIBLE			= 0x02000,
	PLPERM_MODIFYSTAFFACCOUNT	= 0x04000,
	PLPERM_SETSERVERFLAGS		= 0x08000,
	PLPERM_SETSERVEROPTIONS		= 0x10000,
	PLPERM_SETFOLDEROPTIONS		= 0x20000,
	PLPERM_SETFOLDERRIGHTS		= 0x40000,
	PLPERM_NPCCONTROL			= 0x80000,
};

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
	PLPROP_GANI				= 10,	// PLPROP_BOWGIF in pre-2.x
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
	PLPROP_IPADDR			= 30,
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
	PLPROP_OSTYPE			= 75,	// 2.19+
	PLPROP_TEXTCODEPAGE		= 76,	// 2.19+
	PLPROP_UNKNOWN77		= 77,
	PLPROP_X2				= 78,
	PLPROP_Y2				= 79,
	PLPROP_Z2				= 80,
	PLPROP_UNKNOWN81		= 81,

	// In Graal v5, where players have the Graal######## accounts, this is their chosen account alias (community name.)
	PLPROP_COMMUNITYNAME	= 82,
};
#define propscount	83

class TServer;

class TAccount
{
	public:
		// Constructor - Deconstructor
		TAccount(TServer* pServer);
		~TAccount();

		static bool meetsConditions(CString fileName, CString conditions);

		// Load/Save Account
		void reset();
		bool loadAccount(const CString& pAccount);
		bool saveAccount();

		// Attribute-Managing
		bool hasChest(const TLevelChest *pChest, const CString& pLevel = "");
		bool hasWeapon(const CString& pWeapon);

		// get functions
		float getX() const				{ return x; }
		float getY() const				{ return y; }
		CString getAccountName() const	{ return accountName; }
		CString getNickname() const		{ return nickName; }
		bool getBanned() const			{ return isBanned; }
		CString getBanReason() const	{ return banReason; }
		CString getBanLength() const	{ return banLength; }
		bool getLoadOnly() const		{ return isLoadOnly; }
		CString getEmail() const		{ return email; }
		int getAdminRights() const		{ return adminRights; }
		CString getAdminIp() const		{ return adminIp; }
		CString getIpStr() const		{ return accountIpStr; }
		CString getComments() const		{ return accountComments; }
		std::vector<CString>* getFolderList()	{ return &folderList; }

		// set functions
		void setLastSparTime(time_t newTime)		{ lastSparTime = newTime; }
		void setApCounter(int newTime)				{ apCounter = newTime; }
		void setKills(int newKills)					{ kills = newKills; }
		void setRating(int newRate, int newDeviate)	{ rating = (float)newRate; deviation = (float)newDeviate; }
		void setAccountName(CString account)		{ accountName = account; }
		void setBanned(bool banned)					{ isBanned = banned; }
		void setBanReason(CString reason)			{ banReason = reason; }
		void setBanLength(CString length)			{ banLength = length; }
		void setLoadOnly(bool loadOnly)				{ isLoadOnly = loadOnly; }
		void setEmail(CString email)				{ this->email = email; }
		void setAdminRights(int rights)				{ adminRights = rights; }
		void setAdminIp(CString ip)					{ adminIp = ip; }
		void setComments(CString comments)			{ accountComments = comments; }

		bool hasRight(int mask)		{ return ((adminRights & mask) && accountIpStr.match(adminIp)); }

	protected:
		TServer* server;

		// Player-Account
		bool isBanned, isLoadOnly;
		CString adminIp, accountComments, accountName, communityName, banReason, banLength, lastFolder, email;
		CString accountIpStr;
		int accountIp, adminRights;

		// Player-Attributes
		CString attrList[30], bodyImg, chatMsg, headImg, horseImg, gani, bowImage, language;
		CString levelName, nickName, shieldImg, swordImg;
		float deviation, oldDeviation, power, rating, x, y, z;
		int x2, y2, z2, gmaplevelx, gmaplevely;
		int additionalFlags, ap, apCounter, arrowc, bombc, bombPower, carrySprite;
		unsigned char colors[5];
		int deaths, glovePower, gralatc, horsec, kills, mp, maxPower;
		int onlineTime, shieldPower, sprite, status, swordPower, udpport;
		time_t lastSparTime;
		unsigned char statusMsg;
		std::vector<CString> chestList, flagList, folderList, weaponList;
};

#endif // TACCOUNT_H
