#ifndef TSERVERLIST_H
#define TSERVERLIST_H

#include <time.h>
#include "CString.h"
#include "CSocket.h"
#include "TPlayer.h"

/*
	Enumerators
*/
enum
{
	SVO_SETNAME,
	SVO_SETDESC,
	SVO_SETLANG,
	SVO_SETVERS,
	SVO_SETURL,
	SVO_SETIP,
	SVO_SETPORT,
	SVO_SETPLYR,
	SVO_VERIACC,	// deprecated
	SVO_VERIGUILD,
	SVO_GETFILE,
	SVO_NICKNAME,
	SVO_GETPROF,
	SVO_SETPROF,
	SVO_PLYRADD,
	SVO_PLYRREM,
	SVO_SVRPING,
	SVO_VERIACC2,
};

enum
{
	SVI_VERIACC,
	SVI_VERIGUILD,
	SVI_FILESTART,
	SVI_FILEDATA,
	SVI_FILEEND,
	SVI_VERSIONOLD,
	SVI_VERSIONCURRENT,
	SVI_PROFILE,
	SVI_ERRMSG,
	SVI_NULL4,
	SVI_NULL5,
	SVI_PING = 99
};

class TServerList
{
	public:
		// Constructor - Deconstructor
		TServerList();
		~TServerList();

		// Socket-Control Functions
		bool getConnected();
		bool main();
		bool init(const CString& pserverIp, const CString& pServerPort = "14900");
		bool connectServer();
		void sendPacket(CString& pPacket);

		// Altering Player Information
		void addPlayer(TPlayer *pPlayer);
		void remPlayer(const CString& pAccountName, int pType = CLIENTTYPE_CLIENT);
		void sendPlayers();

		// Send New Server-Info
		void setDesc(const CString& pServerDesc);
		void setIp(const CString& pServerIp);
		void setName(const CString& pServerName);
		void setPort(const CString& pServerPort);
		void setUrl(const CString& pServerUrl);
		void setVersion(const CString& pServerVersion);

		// Incoming message parsing functions
		void msgSVI_NULL(CString& pPacket);
		void msgSVI_VERIACC(CString& pPacket);
		void msgSVI_VERIGUILD(CString& pPacket);
		void msgSVI_FILESTART(CString& pPacket);
		void msgSVI_FILEDATA(CString& pPacket);
		void msgSVI_FILEEND(CString& pPacket);
		void msgSVI_VERSIONOLD(CString& pPacket);
		void msgSVI_VERSIONCURRENT(CString& pPacket);
		void msgSVI_PROFILE(CString& pPacket);
		void msgSVI_ERRMSG(CString& pPacket);
		//void msgSVI_NULL4(CString& pPacket);
		//void msgSVI_NULL5(CString& pPacket);
		void msgSVI_PING(CString& pPacket);

	protected:
		// Packet Functions
		void parsePacket(CString& pPacket);
		void sendCompress();

		bool doTimedEvents();

		// Socket Variables
		bool isConnected;
		CString rBuffer, sBuffer;
		CSocket sock;
		time_t lastData, lastPing, lastTimer;
};

// Packet-Functions
typedef void (TServerList::*TSLSock)(CString&);
void createSLFunctions();

#endif // TSERVERLIST_H
