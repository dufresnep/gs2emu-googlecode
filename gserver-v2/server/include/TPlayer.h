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
#include "TWeapon.h"
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
	PLI_BOMBADD			= 4,
	PLI_BOMBDEL			= 5,
	PLI_TOALL			= 6,
	PLI_HORSEADD		= 7,
	PLI_HORSEDEL		= 8,
	PLI_ARROWADD		= 9,
	PLI_FIRESPY			= 10,
	PLI_THROWCARRIED	= 11,
	PLI_ITEMADD			= 12,
	PLI_ITEMDEL			= 13,
	PLI_CLAIMPKER		= 14,
	PLI_BADDYPROPS		= 15,
	PLI_BADDYHURT		= 16,
	PLI_BADDYADD		= 17,

	PLI_OPENCHEST		= 20,
	PLI_WANTFILE		= 23,
	PLI_SHOWIMG			= 24,

	PLI_NPCWEAPONDEL	= 29,
	PLI_FORCELEVELWARP	= 30,
	PLI_PACKETCOUNT		= 31,
	PLI_ITEMTAKE		= 32,
	PLI_WEAPONADD		= 33,
	PLI_UPDATEFILE		= 34,
	PLI_ADJACENTLEVEL	= 35,
	PLI_LANGUAGE		= 37,
	PLI_TRIGGERACTION	= 38,
	PLI_MAPINFO			= 39,
	PLI_UNKNOWN46		= 46,		// Always is 1.  Might be a player count for the gmap level.
};

enum
{
	PLO_LEVELLINK		= 1,
	PLO_BADDYPROPS		= 2,
	PLO_NPCPROPS		= 3,
	PLO_LEVELCHEST		= 4,
	PLO_LEVELSIGN		= 5,
	PLO_LEVELNAME		= 6,
	PLO_BOARDMODIFY		= 7,
	PLO_OTHERPLPROPS	= 8,
	PLO_PLAYERPROPS		= 9,
	PLO_ISLEADER		= 10,
	PLO_BOMBADD			= 11,
	PLO_BOMBDEL			= 12,
	PLO_TOALL			= 13,
	PLO_PLAYERWARP		= 14,
	PLO_WARPFAILED		= 15,
	PLO_DISCMESSAGE		= 16,
	PLO_HORSEADD		= 17,
	PLO_HORSEDEL		= 18,
	PLO_ARROWADD		= 19,
	PLO_FIRESPY			= 20,
	PLO_THROWCARRIED	= 21,
	PLO_ITEMADD			= 22,
	PLO_ITEMDEL			= 23,

	PLO_SIGNATURE		= 25,
	PLO_BADDYHURT		= 27,
	PLO_FLAGSET			= 28,
	PLO_FILESENDFAILED	= 30,
	PLO_SHOWIMG			= 32,
	PLO_NPCWEAPONADD	= 33,
	PLO_NPCWEAPONDEL	= 34,
	PLO_LEVELMODTIME	= 39,
	PLO_DEFAULTWEAPON	= 43,
	PLO_LISTPROCESSES	= 44,	// Requests a list of running processes.
	PLO_STAFFGUILDS		= 47,
	PLO_TRIGGERACTION	= 48,
	PLO_EMPTY49			= 49,	// Held 5 bytes then the gmap.  Might be modTime + gmap.
	PLO_ADDPLAYER		= 55,
	PLO_DELPLAYER		= 56,
	PLO_LARGEFILESTART	= 68,
	PLO_LARGEFILEEND	= 69,
	PLO_EMPTY73			= 73,
	PLO_RCMESSAGE		= 74,
	PLO_RAWDATA			= 100,
	PLO_BOARDPACKET		= 101,
	PLO_FILE			= 102,
	PLO_EMPTY156		= 156,	// Held the gmap. {156}{gmap name}
	PLO_GHOSTICON		= 174,	// Pass 1 to enable the ghost icon
	PLO_RPGWINDOW		= 179,
	PLO_STATUSLIST		= 180,
};

enum
{
	PLFLAG_NOTOALL		= 0x04,
};

enum
{
	PLSTATUS_PAUSED		= 0x01,
	PLSTATUS_MALE		= 0x04,
	PLSTATUS_DEAD		= 0x08,
	PLSTATUS_HASSPIN	= 0x40,
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
		void setProps(CString& pPacket, bool pForward = false, bool pForwardToSelf = false);
		void sendProps(bool *pProps, int pCount);
		CString getProps(bool *pProps, int pCount);

		// Socket-Functions
		bool doMain();
		void sendCompress();
		void sendPacket(CString pPacket);

		// Misc functions.
		bool doTimedEvents();

		// Packet-Functions
		bool msgPLI_NULL(CString& pPacket);
		bool msgPLI_LOGIN(CString& pPacket);

		bool msgPLI_LEVELWARP(CString& pPacket);
		bool msgPLI_BOARDMODIFY(CString& pPacket);
		bool msgPLI_PLAYERPROPS(CString& pPacket);
		bool msgPLI_NPCPROPS(CString& pPacket);
		bool msgPLI_BOMBADD(CString& pPacket);
		bool msgPLI_BOMBDEL(CString& pPacket);
		bool msgPLI_TOALL(CString& pPacket);
		bool msgPLI_HORSEADD(CString& pPacket);
		bool msgPLI_HORSEDEL(CString& pPacket);
		bool msgPLI_ARROWADD(CString& pPacket);
		bool msgPLI_FIRESPY(CString& pPacket);
		bool msgPLI_THROWCARRIED(CString& pPacket);
		bool msgPLI_ITEMADD(CString& pPacket);
		bool msgPLI_ITEMDEL(CString& pPacket);
		bool msgPLI_CLAIMPKER(CString& pPacket);
		bool msgPLI_BADDYPROPS(CString& pPacket);
		bool msgPLI_BADDYHURT(CString& pPacket);
		bool msgPLI_BADDYADD(CString& pPacket);

		bool msgPLI_OPENCHEST(CString& pPacket);

		bool msgPLI_WANTFILE(CString& pPacket);
		bool msgPLI_SHOWIMG(CString& pPacket);
		bool msgPLI_NPCWEAPONDEL(CString& pPacket);
		bool msgPLI_WEAPONADD(CString& pPacket);
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
		time_t lastData, lastMovement, lastChat, lastMessage, lastSave;
		TServer* server;
		std::vector<SCachedLevel*> cachedLevels;
		bool allowBomb;
		bool hadBomb;
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
