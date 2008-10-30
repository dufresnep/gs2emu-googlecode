#ifndef TSERVER_H
#define TSERVER_H

#include <vector>
#include "ICommon.h"
#include "CSettings.h"
#include "CSocket.h"
#include "CLog.h"
#include "CFileSystem.h"
#include "TPlayer.h"
#include "TServerList.h"
#include "TLevel.h"
#include "TMap.h"
#include "TNPC.h"
#include "TWeapon.h"

enum // Socket Type
{
	SOCK_PLAYER = 0,
	SOCK_SERVER = 1,
};

//class TPlayer;
//class TLevel;
class TServer
{
	public:
		TServer(CString pName);
		~TServer();

		int init();
		bool doMain();

		// Get functions.
		CSettings* getSettings()				{ return &settings; }
		std::vector<TPlayer*>* getPlayerList()	{ return &playerList; }
		std::vector<TPlayer*>* getPlayerIdList(){ return &playerIds; }
		std::vector<TNPC*>* getNPCList()		{ return &npcList; }
		std::vector<TNPC*>* getNPCIdList()		{ return &npcIds; }
		std::vector<TLevel*>* getLevelList()	{ return &levelList; }
		std::vector<TMap*>* getMapList()		{ return &mapList; }
		std::vector<TWeapon*>* getWeaponList()	{ return &weaponList; }
		std::vector<CString>* getServerFlags()	{ return &serverFlags; }
		TServerList* getServerList()			{ return &serverlist; }
		CFileSystem* getFileSystem()			{ return &filesystem; }
		CString getServerPath()					{ return serverpath; }
		CLog& getServerLog()					{ return serverlog; }
		CLog& getRCLog()						{ return rclog; }
		CString* getServerMessage()				{ return &servermessage; }
		CString getNPCServerAddr()              { return npcServerAddr; }
		unsigned int getNWTime();

		TPlayer* getPlayer(const unsigned short id) const;
		TNPC* getNPC(const unsigned int id) const;
		TLevel* getLevel(const CString& pLevel);
		TMap* getMap(const CString& name) const;
		TMap* getMap(const TLevel* pLevel) const;
		TWeapon* getWeapon(const CString& name) const;
		CString getFlag(const CString& pName) const;

		TNPC* addNPC(const CString& pImage, const CString& pScript, float pX, float pY, TLevel* pLevel, bool pLevelNPC, bool sendToPlayers = false);
		bool deleteNPC(const unsigned int pId, TLevel* pLevel = 0);
		bool deleteNPC(TNPC* npc, TLevel* pLevel = 0);
		bool addFlag(const CString& pFlag);
		bool deleteFlag(const CString& pFlag);

		// Packet sending.
		void sendPacketToAll(CString pPacket) const;
		void sendPacketToAll(CString pPacket, TPlayer *pPlayer) const;
		void sendPacketToLevel(CString pPacket, TLevel *pLevel) const;
		void sendPacketToLevel(CString pPacket, TLevel *pLevel, TPlayer *pPlayer) const;
		void sendPacketToLevel(CString pPacket, TMap* pMap, TLevel* pLevel) const;
		void sendPacketToLevel(CString pPacket, TMap* pMap, TPlayer* pPlayer, bool sendToSelf = false) const;
		void sendPacketTo(int who, CString pPacket) const;
		void sendPacketTo(int who, CString pPacket, TPlayer* pPlayer) const;

	private:
		bool doTimedEvents();
		void acceptSock(CSocket& pSocket);

		CSettings settings;
		std::vector<TPlayer*> playerIds, playerList;
		std::vector<TNPC*> npcIds, npcList;
		std::vector<TLevel*> levelList;
		std::vector<TMap*> mapList;
		std::vector<TWeapon*> weaponList;
		std::vector<CString> serverFlags;
		CSocket playerSock, serverSock;
		TServerList serverlist;
		CFileSystem filesystem;
		CString name;
		CString serverpath;
		CString servermessage;
		CString npcServerAddr;

		CLog serverlog;//("logs/serverlog.txt");
		CLog rclog;//("logs/rclog.txt");

		time_t lastTimer, lastNWTimer;
};

#endif
