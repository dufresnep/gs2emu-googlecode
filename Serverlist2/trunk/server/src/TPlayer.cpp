#include "main.h"
#include "TPlayer.h"
#include "CLog.h"

extern CSettings *settings;
extern std::vector<TPlayer *> playerList;
extern CLog clientlog;

/*
	Pointer-Functions for Packets
*/
std::vector<TPLSock> plfunc;

void createPLFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 256; i++)
		plfunc.push_back(&TPlayer::msgPLI_NULL);

	// now set non-nulls
	plfunc[PLI_V1VER] = &TPlayer::msgPLI_V1VER;
	plfunc[PLI_SERVERLIST] = &TPlayer::msgPLI_SERVERLIST;
	plfunc[PLI_V2SERVERLISTRC] = &TPlayer::msgPLI_V2SERVERLISTRC;
	plfunc[PLI_V2ENCRYPTKEYCL] = &TPlayer::msgPLI_V2ENCRYPTKEYCL;
}

/*
	Constructor - Deconstructor
*/
TPlayer::TPlayer(CSocket *pSocket)
: sock(pSocket), key(0), version(PLV_PRE22)
{
}

TPlayer::~TPlayer()
{
	delete sock;
}

/*
	Loops
*/
bool TPlayer::doMain()
{
	// sock exist?
	if (sock == NULL)
		return false;

	// definitions
	CString unBuffer;

	// receive
	if ( sock->getData() == -1 )
		return false;

	// append new data
	sockBuffer.write( sock->getBuffer().text(), sock->getBuffer().length() );
	sock->getBuffer().clear();

	// parse data
	sockBuffer.setRead(0);
	while (sockBuffer.length() >= 2)
	{
		// packet length
		unsigned int len = sockBuffer.readShort();
		if (len > (unsigned int)sockBuffer.length()-2)
			break;

		// version 2.2+
		if (version == PLV_POST22)
		{
			// Read the compression type.
			unsigned char compressType = (unsigned char)sockBuffer.readChar();

			// Pull out the entire packet and discard the first three bytes.
			unBuffer = sockBuffer.subString( 0, len + 2 );
			unBuffer.removeI( 0, 3 );

			// Decrypt.
			in_codec.limitfromtype( compressType );
			in_codec.apply(reinterpret_cast<uint8_t*>(unBuffer.text()), unBuffer.length());

			// Uncompress?
			if ( compressType == ENCRYPT22_ZLIB )
				unBuffer.zuncompressI();
			else if ( compressType == ENCRYPT22_BZ2 )
				unBuffer.bzuncompressI();

			// Make sure we actually have something.
			if (unBuffer.length() < 1)
				return false;
		}
		// anything below version 2.2
		else if (version == PLV_PRE22)
		{
			// uncompress
			unBuffer = sockBuffer.remove(0, 2).zuncompress();
			if (unBuffer.length() < 1)
				return false;
		}

		// remove read-data
		sockBuffer.removeI(0, len+2);

		// well theres your buffer
		if (!parsePacket(unBuffer))
			return false;
	}

	// send out buffer
	sendCompress();
	return true;
}

/*
	Send-Packet Functions
*/
void TPlayer::sendCompress()
{
	// empty buffer?
	if (sendBuffer.isEmpty())
	{
		// If we still have some data in the out buffer, try sending it again.
		if (outBuffer.isEmpty() == false)
			sock->sendData(outBuffer);
		return;
	}

	// compress buffer
	if (PLV_POST22)
	{
		// Choose which compression to use and apply it.
		int compressionType = ENCRYPT22_UNCOMPRESSED;
		if (sendBuffer.length() > 0x2000)	// 8KB
		{
			compressionType = ENCRYPT22_BZ2;
			sendBuffer.bzcompressI();
		}
		else if (sendBuffer.length() > 40)
		{
			compressionType = ENCRYPT22_ZLIB;
			sendBuffer.zcompressI();
		}

		// Encrypt the packet and add it to the out buffer.
		out_codec.limitfromtype(compressionType);
		out_codec.apply(reinterpret_cast<uint8_t*>(sendBuffer.text()), sendBuffer.length());
		outBuffer << (short)(sendBuffer.length() + 1) << (char)compressionType << sendBuffer;

		// Send oBuffer.
		sock->sendData(outBuffer);
	}
	else
	{
		// Compress the packet and add it to the out buffer.
		sendBuffer.zcompressI();
		outBuffer << (short)sendBuffer.length() << sendBuffer;

		// Send oBuffer.
		sock->sendData(outBuffer);
	}

	// Clear the send buffer.
	sendBuffer.clear();
}

void TPlayer::sendPacket(CString pPacket, bool pSendNow)
{
	// empty buffer?
	if (pPacket.isEmpty())
		return;

	// remove trailing \n so we can encrypt.
	if (pPacket[pPacket.length()-1] == '\n')
		pPacket.removeI( pPacket.length() - 1, 1 );

	// append '\n'
	pPacket.writeChar('\n');

	// append buffer
	sendBuffer.write(pPacket);

	// send buffer now?
	if (pSendNow)
		sendCompress();
}

/*
	Packet-Functions
*/
bool TPlayer::parsePacket(CString& pPacket)
{
	while (pPacket.bytesLeft() > 0)
	{
		// read id & packet
		int id = pPacket.readGUChar();
		CString str = pPacket.readString("\n");

		// check lengths
		if (str.length() < 0 || id > (int)plfunc.size())
			continue;

		// valid packet, call function
		if (!(*this.*plfunc[id])(str))
			return false;
	}

	return true;
}

bool TPlayer::msgPLI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	clientlog.out( "Unknown Player Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1 );
	return true;
}

bool TPlayer::msgPLI_V1VER(CString& pPacket)
{
	/*
	// definitions
	std::vector<CString>::iterator result;
	std::vector<CString> versions = CString("newmain").tokenize(",");

	// search for version
	if ((result = std::find(versions.begin(), versions.end(), pPacket.text())) == versions.end())
		return;
	*/

	pPacket.readString("");
	return true;
}

bool TPlayer::msgPLI_SERVERLIST(CString& pPacket)
{
	// definitions
	CString account, password;
	int res;

	// read data
	account  = pPacket.readChars(pPacket.readGUChar());
	password = pPacket.readChars(pPacket.readGUChar());
	printf("Account: %s\n", account.text());
	printf("Password: %s\n", password.text());

	// verify account
	res = verifyAccount(account, password);
	printf( "Verify: %d\n", res );

	switch (res)
	{
		case ACCSTAT_NORMAL:
			if (getServerCount() > 0)
				sendPacket(CString() >> (char)PLO_SVRLIST << getServerList(), true);
			sendPacket(CString() >> (char)PLO_STATUS << "Welcome to " << settings->getStr("name") << ", " << account << "." << "\r" << "There are " << CString(getServerCount()) << " server(s) online.");
			sendPacket(CString() >> (char)PLO_SITEURL << settings->getStr("url"));
			sendPacket(CString() >> (char)PLO_UPGURL << settings->getStr("donateUrl"));
		return true;

		default:
			sendPacket(CString() >> (char)PLO_ERROR << getAccountError(res));
		return false;
	}
}

bool TPlayer::msgPLI_V2SERVERLISTRC(CString& pPacket)
{
	version = PLV_POST22;
	key = pPacket.readGChar();
	in_codec.reset(key);
	out_codec.reset(key);
	return true;
}

bool TPlayer::msgPLI_V2ENCRYPTKEYCL(CString& pPacket)
{
	version = PLV_POST22;
	key = pPacket.readGChar();
	in_codec.reset(key);
	out_codec.reset(key);
	return true;
}
