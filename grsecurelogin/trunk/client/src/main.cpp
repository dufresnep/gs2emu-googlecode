#include <signal.h>
#include <stdlib.h>
#include <map>
#include "main.h"
#include "ICommon.h"
#include "IUtil.h"
#include "CLog.h"
#include "CSocket.h"
#include "CSettings.h"
#include "md5.h"

// Function pointer for signal handling.
typedef void (*sighandler_t)(int);

// Pointer-Functions for Packets
typedef bool (*TSVSock)(CString&);
std::vector<TSVSock> TSVFunc;

CLog clientlog("grsecurelogin.log.txt");
CSocket sock;
CString rBuffer, sBuffer, oBuffer;
CString account, password;

int main(int argc, char* argv[])
{
	char buff[1024];

	// Shut down the server if we get a kill signal.
	signal(SIGINT, (sighandler_t) shutdownClient);
	signal(SIGTERM, (sighandler_t) shutdownClient);

	clientlog.clear();
	clientlog.out("Simple Graal Reborn secure login client.\n");

	// Load Server Settings
	CSettings settings(CString() << "grsecurelogin.ini");
	if (!settings.isOpened())
	{
		clientlog.out("[Error] Could not open grsecurelogin.ini.\n");
		return ERR_SETTINGS;
	}

	// Create Packet-Functions
	createSVFunctions();

	// Get account.
	account = settings.getStr("account");
	if (account.length() == 0)
	{
		memset((void*)buff, 0, sizeof(buff));
		printf("Account: ");
		account = gets(buff);
		account.removeAllI("\n");
	}

	// Get password.
	password = settings.getStr("password");
	if (password.length() == 0)
	{
		memset((void*)buff, 0, sizeof(buff));
		printf("Password: ");
		password = gets(buff);
		password.removeAllI("\n");
	}

	// Initialize socket.
	sock.setType(SOCKET_TYPE_CLIENT);
	sock.setProtocol(SOCKET_PROTOCOL_TCP);
	sock.setOptions(SOCKET_OPTION_NONBLOCKING);
	sock.setDescription("sock");
	if (sock.init(settings.getStr("listhost", "listserver.graal.in"), settings.getStr("listport", "14922")))
	{
		clientlog.out("[Error] Could not initialize sockets.\n");
		return ERR_SOCKETS;
	}
	if (sock.connect())
	{
		clientlog.out("[Error] Could not connect socket.\n");
		return ERR_SOCKETS;
	}

	// Send the connection packets.
	doConnect();

	// Run the program.
	while (doMain())
		Sleep(100);

	// Close the socket.
	sock.disconnect();

	// Give the user the ability to copy it out of te terminal window if they so choose.
	printf("Press ENTER to end the program...\n");
	int c = getc(stdin);

	// Destroy the sockets.
	//CSocket::socketSystemDestroy();

	return ERR_SUCCESS;
}

void doConnect()
{
	sendPacket(CString() >> (char)SVO_SECURELOGIN
		>> (char)account.length() << account
		>> (char)password.length() << password );
	sendCompress();
}

bool doMain()
{
	if (sock.getState() == SOCKET_STATE_DISCONNECTED)
		return false;

	// definitions
	CString unBuffer;

	// receive
	if (sock.getData() == -1)
		return false;

	// grab the data now
	rBuffer.write(sock.getBuffer().text(), sock.getBuffer().length());
	sock.getBuffer().clear();

	// parse data
	rBuffer.setRead(0);
	while (rBuffer.length() >= 2)
	{
		// packet length
		unsigned int len = (unsigned int)rBuffer.readShort();
		if (len > (unsigned int)rBuffer.length()-2)
			break;

		// get packet
		unBuffer = rBuffer.readChars(len);
		rBuffer.removeI(0, len+2);

		// decrypt packet
		unBuffer.zuncompressI();

		// well theres your buffer
		if (!parsePacket(unBuffer))
			return false;
	}

	// send out buffer
	sendCompress();
	return true;
}


bool parsePacket(CString& pPacket)
{
	while (pPacket.bytesLeft() > 0)
	{
		// grab packet
		CString curPacket = pPacket.readString("\n");

		// read id & packet
		int id = curPacket.readGUChar();

		// check lengths
		if (id >= (unsigned char)TSVFunc.size())
			return false;

		// valid packet, call function
		if (!(*TSVFunc[id])(curPacket))
			return false;
	}

	return true;
}

void sendPacket(CString pPacket)
{
	// empty buffer?
	if (pPacket.isEmpty())
		return;

	// append '\n'
	if (pPacket[pPacket.length()-1] != '\n')
		pPacket.writeChar('\n');

	// Send the stored buffer if the new packet will put it over 64KB.
	// We don't want to keep compressing packets over and over if we can avoid it.
	if (sBuffer.length() + pPacket.length() > 0x10000)
		sendCompress();

	// append buffer
	sBuffer.write(pPacket.text(), pPacket.length());
}

void sendCompress()
{
	// empty buffer?
	if (sBuffer.isEmpty())
	{
		// If we still have some data in the out buffer, try sending it again.
		if (oBuffer.isEmpty() == false)
			sock.sendData(oBuffer);
		return;
	}

	// Compress the packet and add it to the out buffer.
	sBuffer.zcompressI();
	oBuffer << (short)sBuffer.length() << sBuffer;

	// Send oBuffer.
	sock.sendData(oBuffer);

	// Clear the send buffer.
	sBuffer.clear();
}

/*
	Extra-Cool Functions :D
*/
void shutdownClient(int signal)
{
	clientlog.out("\n\nGot an abort signal, shutting down...\n");
	sock.disconnect();
	exit(1);
}

void createSVFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 224; i++)
		TSVFunc.push_back(&msgSVI_NULL);

	// now set non-nulls
	TSVFunc[SVI_SECURELOGIN] = &msgSVI_SECURELOGIN;
}

/*
	Packet functions.
*/
bool msgSVI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	clientlog.out("Unknown Server Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	return true;
}

bool msgSVI_SECURELOGIN(CString& pPacket)
{
	if (pPacket.bytesLeft() == 0)
	{
		clientlog.out("Password invalid.\n");
		sock.disconnect();
		return false;
	}

	int transaction = pPacket.readGInt5();
	CString salt = pPacket.readString("");

	// MD5 the password.
	char md5_password[33];
	CalculateMD5(password.text(), password.length(), (char*)&md5_password);

	// Append the salt to the password MD5.
	CString passwordsalt;
	passwordsalt.write(md5_password, 32);
	passwordsalt << salt;

	// MD5 the password + salt.
	char md5_passwordsalt[33];
	CalculateMD5(passwordsalt.text(), passwordsalt.length(), (char*)&md5_passwordsalt);

	// Encode our resulting secure password.
	CString result((int)transaction);
	result << "\xa7";
	result.write(md5_passwordsalt, 32);
	clientlog.out("Encrypted password: %s\n", result.text());
	printf("\n------- NOTE: Windows users: -------\n");
	printf("Check grsecurelogin.log.txt for a copy/paste-able password.\n");
	printf("The Windows console doesn't display the \xa7 character correctly.\n");
	printf("------------------------------------\n");

	return false;
}
