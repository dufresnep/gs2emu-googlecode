#include "main.h"
#include "TServerList.h"

extern CSettings *settings;

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
	SVO_VERIACC,
	SVO_VERIGLD,
	SVO_GETFILE,
	SVO_NICKNAME,
	SVO_GETPROF,
	SVO_SETPROF,
	SVO_PLYRADD,
	SVO_PLYRREM,
	SVO_SVRPING,
};

enum
{
	SVI_VERIACC,
	SVI_VERIGLD,
	SVI_FILESTART,
	SVI_FILEDATA,
	SVI_FILEEND,
	SVI_NULL2,
	SVI_NULL3,
	SVI_PROFILE,
	SVI_ERRMSG,
	SVI_NULL4,
	SVI_NULL5,
	SVI_PING = 99
};

/*
	Pointer-Functions for Packets
*/
std::vector<TSLSock> TSLFunc;

void createSLFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 20; i++)
		TSLFunc.push_back(&TServerList::msgSVI_NULL);

	// now set non-nulls
//	TSLFunc[SVI_SETNAME] = &TServer::msgSVI_SETNAME;
}

/*
	Constructor - Deconstructor
*/
TServerList::TServerList() { }

TServerList::~TServerList()
{
	exit(-1);
}

/*
	Socket-Control Functions
*/
bool TServerList::getConnected()
{
	return isConnected;
}

bool TServerList::main()
{
	if (!getConnected())
		return false;

	// definitions
	CString line, recvBuffer;
	int lineEnd, size;

	// receive
	if ((size = receiveBytes(recvBuffer, 65535)) < 0)
	{
		closeSock();
		isConnected = false;
		return getConnected();
	}

	// data recieved? parse it
	if (recvBuffer.length() > 0)
	{
		// append new data
		rBuffer.write(recvBuffer);

		// parse data
		if ((lineEnd = rBuffer.findl('\n')) == -1)
			return getConnected();

		line = rBuffer.subString(0, lineEnd + 1);
		rBuffer.removeI(0, line.length());

		// parse each packet seperately
		std::vector<CString> lines = line.tokenize("\n");
		for (unsigned int i = 0; i < lines.size(); i++)
			parsePacket(lines[i]);
	}

	// send out buffer
	sendCompress();
	return getConnected();
}

bool TServerList::connectServer(const CString& pServerIp, int pServerPort)
{
	// Save Server-Details
	serverIp = pServerIp;
	serverPort = pServerPort;

	// Connect to Server
	if (!(isConnected = connectSock(pServerIp.text(), pServerPort)))
		return getConnected();

	// Set Non-Blocking
	setSync(false);

	// Set Some Stuff
	setName(settings->getStr("name"));
	setDesc(settings->getStr("description"));
	setUrl(settings->getStr("url"));
	setVersion(settings->getStr("version"));
	setIp(settings->getStr("serverip", "AUTO"));
	setPort(settings->getInt("serverport", 14899));

	// Return Connection-Status
	return getConnected();
}

void TServerList::sendCompress()
{
	// empty buffer?
	if (sBuffer.isEmpty())
		return;

	// send buffer
	sendBuffer(sBuffer);

	// clear buffer
	sBuffer.clear();
}

void TServerList::sendPacket(CString& pPacket)
{
	// empty buffer?
	if (pPacket.isEmpty())
		return;

	// append '\n'
	if (pPacket[pPacket.length()-1] != '\n')
		pPacket.writeChar('\n');

	// append buffer
	sBuffer.write(pPacket);
}

/*
	Altering Server-Information
*/
void TServerList::setDesc(const CString& pServerDesc)
{
	sendPacket(CString() >> (char)SVO_SETDESC << pServerDesc);
}

void TServerList::setIp(const CString& pServerIp)
{
	sendPacket(CString() >> (char)SVO_SETIP << pServerIp);
}

void TServerList::setName(const CString& pServerName)
{
	sendPacket(CString() >> (char)SVO_SETNAME << pServerName);
}

void TServerList::setPort(int pServerPort)
{
	sendPacket(CString() >> (char)SVO_SETPORT << (CString)pServerPort);
}

void TServerList::setUrl(const CString& pServerUrl)
{
	sendPacket(CString() >> (char)SVO_SETURL << pServerUrl);
}

void TServerList::setVersion(const CString& pServerVersion)
{
	sendPacket(CString() >> (char)SVO_SETVERS << pServerVersion);
}

/*
	Packet-Functions
*/
void TServerList::parsePacket(CString& pPacket)
{
	// read id & packet
	unsigned char id = pPacket.readGUChar();

	// id exists?
	if (id >= (unsigned char)TSLFunc.size())
		return;

	// valid packet, call function
	(*this.*TSLFunc[id])(pPacket);
}

void TServerList::msgSVI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	printf("Unknown Serverlist Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
}
