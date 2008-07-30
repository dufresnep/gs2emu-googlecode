#ifndef TSERVER_H
#define TSERVER_H

#include <vector>
#include "ICommon.h"
#include "CSettings.h"
#include "CSocket.h"
#include "CLog.h"
#include "TPlayer.h"
#include "TServerList.h"
#include "TLevel.h"

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
		TServerList& getServerList()			{ return serverlist; }
		CString getServerPath()					{ return serverpath; }
		CLog& getServerLog()					{ return serverlog; }
		CLog& getRCLog()						{ return rclog; }

		// Packet sending.
		void sendPacketToAll(CString pPacket) const;
		void sendPacketToAll(CString pPacket, TPlayer *pPlayer) const;
		void sendPacketToLevel(CString pPacket, TLevel *pLevel) const;
		void sendPacketToLevel(CString pPacket, TLevel *pLevel, TPlayer *pPlayer) const;
		void sendPacketTo(int who, CString pPacket) const;
		void sendPacketTo(int who, CString pPacket, TPlayer* pPlayer) const;


	private:
		void acceptSock(CSocket& pSocket);

		CSettings settings;
		std::vector<TPlayer *> playerIds, playerList;
		CSocket playerSock, serverSock;
		TServerList serverlist;
		CString name;
		CString serverpath;

		CLog serverlog;//("logs/serverlog.txt");
		CLog rclog;//("logs/rclog.txt");
};

#endif
