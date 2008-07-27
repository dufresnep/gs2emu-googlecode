#ifndef TPLAYER_H
#define TPLAYER_H

#include <time.h>
#include "CString.h"
#include "TAccount.h"
#include "TLevel.h"
#include "CSocket.h"
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
	PLI_WANTFILE		= 23,
	PLI_NPCWEAPONIMG	= 24,
	PLI_UPDATEFILE		= 34,
	PLI_LANGUAGE		= 37,
	PLI_TRIGGERACTION	= 38,
	PLI_MAPINFO			= 39,
	PLI_UNKNOWN46		= 46,
};

enum
{
	PLO_LEVELLINK = 1,
	PLO_LEVELCHEST = 4,
	PLO_LEVELNAME = 6,
	PLO_OTHERPLPROPS = 8,
	PLO_PLAYERPROPS = 9,
	PLO_PLAYERWARP = 14,
	PLO_WARPFAILED = 15,
	PLO_DISCMESSAGE = 16,
	PLO_HORSEADD = 17,
	PLO_HORSEDEL = 18,
	PLO_SIGNATURE = 25,
	PLO_DELNPCWEAPON = 34,
	PLO_LEVELMODTIME = 39,
	PLO_TRIGGERACTION = 48,
	PLO_ADDPLAYER = 55,
	PLO_DELPLAYER = 56,
	PLO_RCMESSAGE = 74,
	PLO_BOARDPACKETSIZE = 100,
	PLO_BOARDPACKET = 101,
	PLO_RPGWINDOW = 179,
};

enum
{
	CLIENTTYPE_CLIENT,
	CLIENTTYPE_RC,
	CLIENTTYPE_AWAIT,
	CLIENTTYPE_CLIENT22 = 5,
};

class TPlayer : public TAccount
{
	public:
		// Constructor - Deconstructor
		TPlayer(CSocket *pSocket = 0);
		~TPlayer();

		// Manage Account
		inline bool isLoggedIn();

		// Get Properties
		TLevel* getLevel();
		int getId();
		int getType();

		// Set Properties
		void setLevel(const CString& pLevelName);
		void setNick(const CString& pNickName);
		void setId(int pId);

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
		bool msgPLI_WANTFILE(CString& pPacket);
		bool msgPLI_NPCWEAPONIMG(CString& pPacket);
		bool msgPLI_UPDATEFILE(CString& pPacket);
		bool msgPLI_LANGUAGE(CString& pPacket);
		bool msgPLI_TRIGGERACTION(CString& pPacket);
		bool msgPLI_MAPINFO(CString& pPacket);
		bool msgPLI_UNKNOWN46(CString& pPacket);

	private:
		// Login functions.
		void sendLogin();
		void sendLoginClient();
		void sendLoginRC();

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
		CString version;
		TLevel *level;
		int id, type;
		time_t lastData, lastMovement, lastChat, lastMessage;
		time_t lastTimer;
};

inline bool TPlayer::isLoggedIn()
{
	return (type != CLIENTTYPE_AWAIT && id > 0);
}

inline int TPlayer::getId()
{
	return id;
}

inline int TPlayer::getType()
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
