#ifndef TPLAYER_H
#define TPLAYER_H

#include "CSocket.h"
#include "CString.h"
#include "TAccount.h"
#include "TLevel.h"

/*
	Enumerators
*/
enum
{
	PLI_LANGUAGE = 37,
	PLI_TRIGGER = 38,
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
	PLO_RPGWINDOW = 179,
};

class TPlayer : public TAccount
{
	public:
		// Constructor - Deconstructor
		TPlayer(CSocket *pSocket = NULL);
		~TPlayer();

		// Manage Account
		inline bool isLoggedIn();
		void sendLogin();

		// Get Properties
		TLevel* getLevel();

		// Set Properties
		void setLevel(const CString& pLevelName);
		void setNick(const CString& pNickName);

		// Prop-Manipulation
		CString getProp(int pPropId);
		void setProps(CString& pPacket, bool pForward = false);
		void sendProps(bool *pProps, int pCount);

		// Socket-Functions
		bool doMain();
		void decryptPacket(CString& pPacket);
		void sendCompress();
		void sendPacket(CString& pPacket);

		// Packet-Functions
		bool parsePacket(CString& pPacket);
		bool msgPLI_NULL(CString& pPacket);
		bool msgPLI_LOGIN(CString& pPacket);

		bool msgPLI_LANGUAGE(CString& pPacket);
		bool msgPLI_TRIGGER(CString& pPacket);

	private:
		// Socket Variables
		CSocket *playerSock;
		CString rBuffer, sBuffer;
		int iterator, key;

		// Variables
		TLevel *level;
};

inline bool TPlayer::isLoggedIn()
{
	return (type != CLIENTTYPE_AWAIT && id > 0);
}

// Packet-Functions
typedef bool (TPlayer::*TPLSock)(CString&);
void createPLFunctions();

#endif // TPLAYER_H
