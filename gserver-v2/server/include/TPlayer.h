#ifndef TPLAYER_H
#define TPLAYER_H

#include <time.h>
#include <vector>
#include "ICommon.h"
#include "IUtil.h"
#include "CSocket.h"
#include "TServer.h"
#include "TAccount.h"
#include "TLevel.h"
#include "codec.h"

/*
	Enumerators
*/
// PLI_UNKNOWN## are packets sent by the client whose
// purpose is unknown.
enum
{
	PLI_LEVELWARP		= 0,
	PLI_BOARDMODIFY		= 1,
	PLI_PLAYERPROPS		= 2,
	PLI_NPCPROPS		= 3,
	PLI_WANTFILE		= 23,
	PLI_NPCWEAPONIMG	= 24,
	PLI_FORCELEVELWARP	= 30,
	PLI_UPDATEFILE		= 34,
	PLI_ADJACENTLEVEL	= 35,
	PLI_LANGUAGE		= 37,
	PLI_TRIGGERACTION	= 38,
	PLI_MAPINFO			= 39,
	PLI_UNKNOWN46		= 46,		// Always is 1.  Might be a player count for the gmap level.
};

enum
{
	PLO_LEVELLINK = 1,
	PLO_NPCPROPS = 3,
	PLO_LEVELCHEST = 4,
	PLO_LEVELNAME = 6,
	PLO_OTHERPLPROPS = 8,
	PLO_PLAYERPROPS = 9,
	PLO_ISLEADER = 10,
	PLO_ADDITEM = 12,
	PLO_PLAYERWARP = 14,
	PLO_WARPFAILED = 15,
	PLO_DISCMESSAGE = 16,
	PLO_HORSEADD = 17,
	PLO_HORSEDEL = 18,
	PLO_SIGNATURE = 25,
	PLO_FILESENDFAILED = 30,
	PLO_DELNPCWEAPON = 34,
	PLO_LEVELMODTIME = 39,
	PLO_TRIGGERACTION = 48,
	PLO_EMPTY49 = 49,			// Causes level to freeze with "Loading" for some reason.  Might be server-side setlevel.
	PLO_ADDPLAYER = 55,
	PLO_DELPLAYER = 56,
	PLO_RCMESSAGE = 74,
	PLO_RAWDATA = 100,
	PLO_BOARDPACKET = 101,
	PLO_FILE = 102,
	PLO_RPGWINDOW = 179,
};

struct SCachedLevel
{
	SCachedLevel(TLevel* pLevel, time_t pModTime) : level(pLevel), modTime(pModTime) { }
	TLevel* level;
	time_t modTime;
};

class TPlayer : public TAccount
{
	public:
		// Constructor - Deconstructor
		TPlayer(TServer* pServer, CSocket *pSocket = 0);
		~TPlayer();

		// Manage Account
		inline bool isLoggedIn();

		// Get Properties
		TLevel* getLevel()		{ return level; }
		int getId() const;
		int getType() const;

		// Set Properties
		void setNick(const CString& pNickName);
		void setId(int pId);

		// Level manipulation
		bool setLevel(const CString& pLevelName, float x, float y, time_t modTime = 0);
		time_t getCachedLevelModTime(const TLevel* level) const;

		// Prop-Manipulation
		CString getProp(int pPropId);
		void setProps(CString& pPacket, bool pForward = false);
		void sendProps(bool *pProps, int pCount);
		CString getProps(bool *pProps, int pCount);

		// Socket-Functions
		bool doMain();
		void sendCompress();
		void sendPacket(CString pPacket);

		// Packet-Functions
		bool msgPLI_NULL(CString& pPacket);
		bool msgPLI_LOGIN(CString& pPacket);

		bool msgPLI_LEVELWARP(CString& pPacket);
		bool msgPLI_BOARDMODIFY(CString& pPacket);
		bool msgPLI_PLAYERPROPS(CString& pPacket);
		bool msgPLI_NPCPROPS(CString& pPacket);
		bool msgPLI_WANTFILE(CString& pPacket);
		bool msgPLI_NPCWEAPONIMG(CString& pPacket);
		bool msgPLI_UPDATEFILE(CString& pPacket);
		bool msgPLI_ADJACENTLEVEL(CString& pPacket);
		bool msgPLI_LANGUAGE(CString& pPacket);
		bool msgPLI_TRIGGERACTION(CString& pPacket);
		bool msgPLI_MAPINFO(CString& pPacket);
		bool msgPLI_UNKNOWN46(CString& pPacket);

	private:
		// Login functions.
		bool sendLogin();
		bool sendLoginClient();
		bool sendLoginRC();

		// Packet functions.
		bool parsePacket(CString& pPacket);
		void decryptPacket(CString& pPacket);

		// Misc functions.
		bool doTimedEvents();

		// Socket Variables
		CSocket *playerSock;
		CString rBuffer, sBuffer;

		// Pre 2.2 encryption.
		int iterator;
		unsigned char key;

		// Post 2.2 encryption.
		bool PLE_POST22;
		codec in_codec;
		codec out_codec;

		// Variables
		CString version, os;
		int codepage;
		TLevel *level;
		int id, type;
		time_t lastData, lastMovement, lastChat, lastMessage;
		time_t lastTimer;
		TServer* server;
		std::vector<SCachedLevel*> cachedLevels;
};

inline bool TPlayer::isLoggedIn()
{
	return (type != CLIENTTYPE_AWAIT && id > 0);
}

inline int TPlayer::getId() const
{
	return id;
}

inline int TPlayer::getType() const
{
	return type;
}

inline void TPlayer::setId(int pId)
{
	id = pId;
}

// Packet-Functions
typedef bool (TPlayer::*TPLSock)(CString&);
void createPLFunctions();

#endif // TPLAYER_H
