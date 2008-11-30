#ifndef TSERVER_H
#define TSERVER_H

#include <time.h>

enum
{
	SVI_SETNAME		= 0,
	SVI_SETDESC		= 1,
	SVI_SETLANG		= 2,
	SVI_SETVERS		= 3,
	SVI_SETURL		= 4,
	SVI_SETIP		= 5,
	SVI_SETPORT		= 6,
	SVI_SETPLYR		= 7,
	SVI_VERIACC		= 8,
	SVI_VERIGLD		= 9,
	SVI_GETFILE		= 10,
	SVI_NICKNAME	= 11,
	SVI_GETPROF		= 12,
	SVI_SETPROF		= 13,
	SVI_PLYRADD		= 14,
	SVI_PLYRREM		= 15,
	SVI_SVRPING		= 16,
	SVI_VERIACC2	= 17,
};

enum
{
	SVO_VERIACC		= 0,
	SVO_VERIGLD		= 1,
	SVO_FILESTART	= 2,
	SVO_FILEEND		= 3,
	SVO_FILEDATA	= 4,
	SVO_NULL2		= 5,
	SVO_NULL3		= 6,
	SVO_PROFILE		= 7,
	SVO_ERRMSG		= 8,
	SVO_NULL4		= 9,
	SVO_NULL5		= 10,
	SVO_VERIACC2	= 11,
	SVO_PING		= 99,
};

enum
{
	TYPE_CLASSIC	= 0,
	TYPE_GOLD		= 1,
	TYPE_HOSTED		= 2,
	TYPE_HIDDEN		= 3,
	TYPE_3D			= 4,
};

class TServer;
void createSVFunctions();
typedef bool (TServer::*TSVSock)(CString&);

struct player
{
	CString account, nick, level;
	float x, y;
	int ap, type;
};

class TServer
{
	public:
		// constructor-destructor
		TServer(CSocket *pSocket);
		~TServer();

		// main loop
		bool doMain();

		// kill client
		void kill();

		// get-value functions
		const CString& getDescription();
		const CString getIp(const CString& pIp = "");
		const CString& getLanguage();
		const CString& getName();
		const CString& getPCount();
		const CString& getPort();
		const CString getType();
		const CString& getUrl();
		const CString& getVersion();
		const CString getServerPacket(const CString& pIp = "");
		double getLastData() { return difftime( time(0), lastData ); }

		// send-packet functions
		void sendCompress();
		void sendPacket(CString pPacket);

		// packet-functions;
		bool parsePacket(CString& pPacket);
		bool msgSVI_NULL(CString& pPacket);
		bool msgSVI_SETNAME(CString& pPacket);
		bool msgSVI_SETDESC(CString& pPacket);
		bool msgSVI_SETLANG(CString& pPacket);
		bool msgSVI_SETVERS(CString& pPacket);
		bool msgSVI_SETURL(CString& pPacket);
		bool msgSVI_SETIP(CString& pPacket);
		bool msgSVI_SETPORT(CString& pPacket);
		bool msgSVI_SETPLYR(CString& pPacket);
		bool msgSVI_VERIACC(CString& pPacket);
		bool msgSVI_VERIGLD(CString& pPacket);
		bool msgSVI_GETFILE(CString& pPacket);
		bool msgSVI_NICKNAME(CString& pPacket);
		bool msgSVI_GETPROF(CString& pPacket);
		bool msgSVI_SETPROF(CString& pPacket);
		bool msgSVI_PLYRADD(CString& pPacket);
		bool msgSVI_PLYRREM(CString& pPacket);
		bool msgSVI_SVRPING(CString& pPacket);
		bool msgSVI_VERIACC2(CString& pPacket);

	private:
		CSocket *sock;
		CString sendBuffer, sockBuffer, outBuffer;

		CString description, ip, language, name, pcount, port, url, version;
		int type;
		std::vector<player *> playerList;
		time_t lastPing, lastData, lastPlayerCount;
};

#endif // TSERVER_H
