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
#include "TNPC.h"
#include "TWeapon.h"

enum // Socket Type
{
	SOCK_PLAYER,
	SOCK_SERVER,
};

//class TPlayer;
//class TLevel;
class TServer
{
	public:
		TServer(CString pName);

		int init();
		bool doMain();

		// Get functions.
		CSettings& getSettings()				{ return settings; }
		std::vector<TPlayer*>& getPlayerList()	{ return playerList; }
		std::vector<TNPC*>& getNPCList()		{ return npcList; }
		std::vector<TNPC*>& getNPCIdList()		{ return npcIds; }
		std::vector<TLevel*>& getLevelList()	{ return levelList; }
		std::vector<TWeapon*>& getWeaponList()	{ return weaponList; }
		TServerList& getServerList()			{ return serverlist; }
		CFileSystem& getFileSystem()			{ return filesystem; }
		CString getServerPath()					{ return serverpath; }
		CLog& getServerLog()					{ return serverlog; }
		CLog& getRCLog()						{ return rclog; }
		
		TWeapon* getWeapon(const CString& name);

		TNPC* addNewNPC(const CString& pImage, const CString& pScript, float pX, float pY, TLevel* pLevel, bool pLevelNPC = true);

		// Packet sending.
		void sendPacketToAll(CString pPacket) const;
		void sendPacketToAll(CString pPacket, TPlayer *pPlayer) const;
		void sendPacketToLevel(CString pPacket, TLevel *pLevel) const;
		void sendPacketToLevel(CString pPacket, TLevel *pLevel, TPlayer *pPlayer) const;
		void sendPacketTo(int who, CString pPacket) const;
		void sendPacketTo(int who, CString pPacket, TPlayer* pPlayer) const;


	private:
		bool doTimedEvents();
		void acceptSock(CSocket& pSocket);

		CSettings settings;
		std::vector<TPlayer*> playerIds, playerList;
		std::vector<TNPC*> npcIds, npcList;
		std::vector<TLevel*> levelList;
		std::vector<TWeapon*> weaponList;
		CSocket playerSock, serverSock;
		TServerList serverlist;
		CFileSystem filesystem;
		CString name;
		CString serverpath;

		CLog serverlog;//("logs/serverlog.txt");
		CLog rclog;//("logs/rclog.txt");

		time_t lastTimer;
};

#endif
