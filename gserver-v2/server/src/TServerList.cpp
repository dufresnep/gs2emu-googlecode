#include "main.h"
#include "TServerList.h"
#include "CSocket.h"
#include "CSettings.h"
#include "CLog.h"

extern CSettings *settings;
extern CLog serverlog;

/*
	Pointer-Functions for Packets
*/
std::vector<TSLSock> TSLFunc;

void createSLFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 100; i++)
		TSLFunc.push_back(&TServerList::msgSVI_NULL);

	// now set non-nulls
	TSLFunc[SVI_VERIACC] = &TServerList::msgSVI_VERIACC;
	TSLFunc[SVI_VERIGUILD] = &TServerList::msgSVI_VERIGUILD;
	TSLFunc[SVI_FILESTART] = &TServerList::msgSVI_FILESTART;
	TSLFunc[SVI_FILEDATA] = &TServerList::msgSVI_FILEDATA;
	TSLFunc[SVI_FILEEND] = &TServerList::msgSVI_FILEEND;
	TSLFunc[SVI_VERSIONOLD] = &TServerList::msgSVI_VERSIONOLD;
	TSLFunc[SVI_VERSIONCURRENT] = &TServerList::msgSVI_VERSIONCURRENT;
	TSLFunc[SVI_PROFILE] = &TServerList::msgSVI_PROFILE;
	TSLFunc[SVI_ERRMSG] = &TServerList::msgSVI_ERRMSG;
	TSLFunc[SVI_PING] = &TServerList::msgSVI_PING;
}

/*
	Constructor - Deconstructor
*/
TServerList::TServerList()
: isConnected(false)
{
	sock.setProtocol(SOCKET_PROTOCOL_TCP);
	sock.setType(SOCKET_TYPE_CLIENT);
	sock.setOptions(SOCKET_OPTION_NONBLOCKING);
	sock.setDescription("listserver");
}

TServerList::~TServerList()
{
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
	CString line;
	int lineEnd;

	// receive
	if (sock.getData() == -1)
	{
		isConnected = false;
		return getConnected();
	}

	// grab the data now
	rBuffer.write(sock.getBuffer());
	sock.getBuffer().clear();

	// do we have enough data to parse?
	if (rBuffer.length() > 0)
	{
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

	// TODO: ping keepalive packets.

	// send out buffer
	sendCompress();
	return getConnected();
}

bool TServerList::init(const CString& pServerIp, const CString& pServerPort)
{
	// Initialize the socket.
	if (sock.init(pServerIp, pServerPort))
		return false;

	return true;
}

bool TServerList::connectServer()
{
	if (isConnected == true) return true;

	// Connect to Server
	if (sock.connect() == 0)
		isConnected = true;
	else
		return false;

	printf("%s - Connected.\n", sock.getDescription());

	// Set Some Stuff
	setName(settings->getStr("name"));
	setDesc(settings->getStr("description"));
	setUrl(settings->getStr("url"));
	setVersion(settings->getStr("version"));
	setIp(settings->getStr("serverip", "AUTO"));
	setPort(settings->getStr("serverport", "14900"));
	sendCompress();

	// Return Connection-Status
	return getConnected();
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
	bool uc = settings->getBool("underconstruction");
	sendPacket(CString() >> (char)SVO_SETNAME << ((uc == true) ? "U " : "") << pServerName);
}

void TServerList::setPort(const CString& pServerPort)
{
	sendPacket(CString() >> (char)SVO_SETPORT << pServerPort);
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

void TServerList::sendCompress()
{
	// empty buffer?
	if (sBuffer.isEmpty())
		return;

	// send buffer
	sock.sendData(sBuffer);

	// clear buffer
	sBuffer.clear();
}

void TServerList::msgSVI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	printf("Unknown Serverlist Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
}

void TServerList::msgSVI_VERIACC(CString& pPacket)
{
	printf("TODO: TServerList::msgSVI_VERIACC\n");
}

void TServerList::msgSVI_VERIGUILD(CString& pPacket)
{
	printf("TODO: TServerList::msgSVI_VERIGUILD\n");
}

void TServerList::msgSVI_FILESTART(CString& pPacket)
{
	printf("TODO: TServerList::msgSVI_FILESTART\n");
}

void TServerList::msgSVI_FILEDATA(CString& pPacket)
{
	printf("TODO: TServerList::msgSVI_FILEDATA\n");
}

void TServerList::msgSVI_FILEEND(CString& pPacket)
{
	printf("TODO: TServerList::msgSVI_FILEEND\n");
}

void TServerList::msgSVI_VERSIONOLD(CString& pPacket)
{
	serverlog.out(":: You are running an old version of the Graal Reborn gserver.\n"
		":: An updated version is available online.\n");
}

void TServerList::msgSVI_VERSIONCURRENT(CString& pPacket)
{
	// Don't bother telling them they are running the latest version.
}

void TServerList::msgSVI_PROFILE(CString& pPacket)
{
	printf("TODO: TServerList::msgSVI_PROFILE\n");
}

void TServerList::msgSVI_ERRMSG(CString& pPacket)
{
	serverlog.out("%s\n", pPacket.readString("").text());
}

void TServerList::msgSVI_PING(CString& pPacket)
{
	// Sent every 60 seconds.  Do nothing.
}
