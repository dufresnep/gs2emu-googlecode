#ifndef TPLAYER_H
#define TPLAYER_H

#include <time.h>
#include <map>
#include <vector>
#include "IEnums.h"
#include "CFileQueue.h"
#include "TAccount.h"
#include "CEncryption.h"
#include "CSocket.h"

class TLevel;
class TServer;
class TMap;
class TWeapon;
//class CFileQueue;

struct SCachedLevel
{
	SCachedLevel(TLevel* pLevel, time_t pModTime) : level(pLevel), modTime(pModTime) { }
	TLevel* level;
	time_t modTime;
};

class TPlayer : public TAccount, public CSocketStub
{
	public:
		// Required by CSocketStub.
		bool onRecv();
		bool onSend();
		bool onRegister()			{ return true; }
		void onUnregister();
		SOCKET getSocketHandle()	{ return playerSock->getHandle(); }
		bool canRecv();
		bool canSend();

		// Constructor - Deconstructor
		TPlayer(TServer* pServer, CSocket* pSocket, int pId);
		~TPlayer();
		void operator()();

		// Manage Account
		inline bool isLoggedIn() const;
		bool sendLogin();

		// Get Properties
		CSocket* getSocket()	{ return playerSock; }
		TLevel* getLevel()		{ return level; }
		TMap* getMap()			{ return pmap; }
		CString getGroup()		{ return levelGroup; }
		int getId() const;
		int getType() const;
		time_t getLastData() const		{ return lastData; }
		CString getGuild() const		{ return guild; }
		int getVersion() const			{ return versionID; }
		CString getVersionStr() const	{ return version; }
		bool isUsingFileBrowser() const	{ return isFtp; }

		// Set Properties
		void setChat(const CString& pChat);
		void setNick(const CString& pNickName, bool force = false);
		void setId(int pId);
		void setLoaded(bool loaded)		{ this->loaded = loaded; }
		void setGroup(CString group)	{ levelGroup = group; }
		void setFlag(const CString& pFlagName, const CString& pFlagValue, bool sendToPlayer = false, bool sendToNPCServer = false);
		void setMap(TMap* map)			{ pmap = map; }

		// Level manipulation
		bool warp(const CString& pLevelName, float pX, float pY, time_t modTime = 0);
		bool setLevel(const CString& pLevelName, time_t modTime = 0);
		bool sendLevel(TLevel* pLevel, time_t modTime, bool fromAdjacent = false);
		bool sendLevel141(TLevel* pLevel, time_t modTime, bool fromAdjacent = false);
		bool leaveLevel(bool resetCache = false);
		time_t getCachedLevelModTime(const TLevel* level) const;
		void resetLevelCache(const TLevel* level);

		// Prop-Manipulation
		CString getProp(int pPropId);
		CString getProps(const bool *pProps, int pCount);
		CString getPropsRC();
		void setProps(CString& pPacket, bool pForward = false, bool pForwardToSelf = false, TPlayer *rc = 0);
		void sendProps(const bool *pProps, int pCount);
		void setPropsRC(CString& pPacket, TPlayer* rc);

		// Socket-Functions
		bool doMain();
		void sendPacket(CString pPacket, bool appendNL = true);
		bool sendFile(const CString& pFile);
		bool sendFile(const CString& pPath, const CString& pFile);

		// Misc functions.
		bool doTimedEvents();
		void disconnect();
		bool processChat(CString pChat);
		bool isAdminIp();
		bool isStaff();
		bool isRC()				{ return (type & PLTYPE_ANYRC) ? true : false; }
		bool isNPCServer()		{ return (type & PLTYPE_NPCSERVER) ? true : false; }
		bool isClient()			{ return (type & PLTYPE_ANYCLIENT) ? true : false; }
		bool isLoaded()			{ return loaded; }
		bool addWeapon(int defaultWeapon);
		bool addWeapon(const CString& name);
		bool addWeapon(TWeapon* weapon);
		bool deleteWeapon(int defaultWeapon);
		bool deleteWeapon(const CString& name);
		bool deleteWeapon(TWeapon* weapon);

		// NPC-Server Functionality
		void sendNCAddr();
		void sendNC_Maps();
		void sendNC_Levels();
		void sendNC_Weapons();
		void sendNC_GMapList();

		// Packet-Functions
		static bool created;
		static void createFunctions();

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
		bool msgPLI_FLAGSET(CString& pPacket);
		bool msgPLI_FLAGDEL(CString& pPacket);
		bool msgPLI_OPENCHEST(CString& pPacket);
		bool msgPLI_PUTNPC(CString& pPacket);
		bool msgPLI_NPCDEL(CString& pPacket);
		bool msgPLI_WANTFILE(CString& pPacket);
		bool msgPLI_SHOWIMG(CString& pPacket);
		// PLI_UNKNOWN25
		bool msgPLI_HURTPLAYER(CString& pPacket);
		bool msgPLI_EXPLOSION(CString& pPacket);
		bool msgPLI_PRIVATEMESSAGE(CString& pPacket);
		bool msgPLI_NPCWEAPONDEL(CString& pPacket);
		bool msgPLI_PACKETCOUNT(CString& pPacket);
		bool msgPLI_WEAPONADD(CString& pPacket);
		bool msgPLI_UPDATEFILE(CString& pPacket);
		bool msgPLI_ADJACENTLEVEL(CString& pPacket);
		bool msgPLI_HITOBJECTS(CString& pPacket);
		bool msgPLI_LANGUAGE(CString& pPacket);
		bool msgPLI_TRIGGERACTION(CString& pPacket);
		bool msgPLI_MAPINFO(CString& pPacket);
		bool msgPLI_SHOOT(CString& pPacket);
		bool msgPLI_SERVERWARP(CString& pPacket);
		bool msgPLI_PROCESSLIST(CString& pPacket);
		bool msgPLI_UNKNOWN46(CString& pPacket);
		bool msgPLI_UNKNOWN47(CString& pPacket);
		bool msgPLI_RAWDATA(CString& pPacket);

		bool msgPLI_RC_SERVEROPTIONSGET(CString& pPacket);
		bool msgPLI_RC_SERVEROPTIONSSET(CString& pPacket);
		bool msgPLI_RC_FOLDERCONFIGGET(CString& pPacket);
		bool msgPLI_RC_FOLDERCONFIGSET(CString& pPacket);
		bool msgPLI_RC_RESPAWNSET(CString& pPacket);
		bool msgPLI_RC_HORSELIFESET(CString& pPacket);
		bool msgPLI_RC_APINCREMENTSET(CString& pPacket);
		bool msgPLI_RC_BADDYRESPAWNSET(CString& pPacket);
		bool msgPLI_RC_PLAYERPROPSGET(CString& pPacket);
		bool msgPLI_RC_PLAYERPROPSSET(CString& pPacket);
		bool msgPLI_RC_DISCONNECTPLAYER(CString& pPacket);
		bool msgPLI_RC_UPDATELEVELS(CString& pPacket);
		bool msgPLI_RC_ADMINMESSAGE(CString& pPacket);
		bool msgPLI_RC_PRIVADMINMESSAGE(CString& pPacket);
		bool msgPLI_RC_LISTRCS(CString& pPacket);
		bool msgPLI_RC_DISCONNECTRC(CString& pPacket);
		bool msgPLI_RC_APPLYREASON(CString& pPacket);
		bool msgPLI_RC_SERVERFLAGSGET(CString& pPacket);
		bool msgPLI_RC_SERVERFLAGSSET(CString& pPacket);
		bool msgPLI_RC_ACCOUNTADD(CString& pPacket);
		bool msgPLI_RC_ACCOUNTDEL(CString& pPacket);
		bool msgPLI_RC_ACCOUNTLISTGET(CString& pPacket);
		bool msgPLI_RC_PLAYERPROPSGET2(CString& pPacket);
		bool msgPLI_RC_PLAYERPROPSGET3(CString& pPacket);
		bool msgPLI_RC_PLAYERPROPSRESET(CString& pPacket);
		bool msgPLI_RC_PLAYERPROPSSET2(CString& pPacket);
		bool msgPLI_RC_ACCOUNTGET(CString& pPacket);
		bool msgPLI_RC_ACCOUNTSET(CString& pPacket);
		bool msgPLI_RC_CHAT(CString& pPacket);
		bool msgPLI_PROFILEGET(CString& pPacket);
		bool msgPLI_PROFILESET(CString& pPacket);
		bool msgPLI_RC_WARPPLAYER(CString& pPacket);
		bool msgPLI_RC_PLAYERRIGHTSGET(CString& pPacket);
		bool msgPLI_RC_PLAYERRIGHTSSET(CString& pPacket);
		bool msgPLI_RC_PLAYERCOMMENTSGET(CString& pPacket);
		bool msgPLI_RC_PLAYERCOMMENTSSET(CString& pPacket);
		bool msgPLI_RC_PLAYERBANGET(CString& pPacket);
		bool msgPLI_RC_PLAYERBANSET(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_START(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_CD(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_END(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_DOWN(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_UP(CString& pPacket);
		bool msgPLI_NPCSERVERQUERY(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_MOVE(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_DELETE(CString& pPacket);
		bool msgPLI_RC_FILEBROWSER_RENAME(CString& pPacket);
		bool msgPLI_RC_LARGEFILESTART(CString& pPacket);
		bool msgPLI_RC_LARGEFILEEND(CString& pPacket);
		bool msgPLI_RC_FOLDERDELETE(CString& pPacket);

		bool msgPLI_REQUESTTEXT(CString& pPacket);
		bool msgPLI_SENDTEXT(CString& pPacket);

		bool msgPLI_UNKNOWN157(CString& pPacket);
		bool msgPLI_UPDATESCRIPT(CString& pPacket);

		bool msgPLI_NC_QUERY(CString& pPacket);

	private:
		// Login functions.
		bool sendLoginClient();
		bool sendLoginRC();
		bool sendLoginNPCServer();

		// Packet functions.
		bool parsePacket(CString& pPacket);
		void decryptPacket(CString& pPacket);

		// Collision detection stuff.
		bool testSign();

		// Misc.
		void dropItemsOnDeath();

		// Socket Variables
		CSocket *playerSock;
		CString rBuffer;

		// Encryption
		unsigned char key;
		CEncryption in_codec;

		// Variables
		CString version, os;
		int codepage;
		TLevel *level;
		int id, type, versionID;
		time_t lastData, lastMovement, lastChat, lastNick, lastMessage, lastSave, last1m;
		std::vector<SCachedLevel*> cachedLevels;
		std::map<CString, CString> rcLargeFiles;
		std::map<CString, TLevel*> spLevels;
		bool allowBomb, allowBow;
		TMap* pmap;
		unsigned int carryNpcId;
		bool carryNpcThrown;
		CString guild;
		bool loaded;
		bool nextIsRaw;
		int rawPacketSize;
		bool isFtp;
		bool grMovementUpdated;
		CString grMovementPackets;
		CString npcserverPort;
		int packetCount;
		bool firstLevel;
		CString levelGroup;
		int invalidPackets;

		CString grExecParameterList;

		// File queue.
		CFileQueue fileQueue;
};

inline bool TPlayer::isLoggedIn() const
{
	return (type != PLTYPE_AWAIT && id > 0);
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

#endif // TPLAYER_H
