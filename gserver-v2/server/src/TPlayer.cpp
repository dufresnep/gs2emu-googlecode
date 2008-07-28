#include <time.h>
#include "main.h"
#include "TPlayer.h"
#include "TAccount.h"
#include "CSocket.h"
#include "CLog.h"
#include "codec.h"

/*
	External
*/
extern std::vector<TPlayer *> playerIds, playerList;
extern CString homepath;
extern CLog serverlog;
extern CLog rclog;
extern CSettings* settings;

/*
	Global Definitions
*/
// Enum per Attr
int __attrPackets[30] = { 37, 38, 39, 40, 41, 46, 47, 48, 49, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 };

// Sent on Login
bool __sendLogin[propscount] =
{
	false, true,  true,  true,  true,  true,  // 0-5
	true , true,  true,  true,  true,  true,  // 6-11
	false, true,  false, true,  true,  true,  // 12-17
	true , true,  true,  true,  true,  false, // 18-23
	false, true,  true,  true,  false, false, // 24-29
	false, false, true,  false, false, true,  // 30-35
	true,  true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	true,  true,  true,  true,  true,  true,  // 54-59
	true,  true,  true,  true,  true,  true,  // 60-65
	true,  true,  true,  true,  true,  true,  // 66-71
	true,  true,  true,  false, false, false, // 72-77
	true,  true, // 78-79
};

bool __getLogin[propscount] =
{
	true,  false, false, false, false, false, // 0-5
	false, false, true,  true,  true,  true,  // 6-11
	true,  true,  false, true,  true,  true,  // 12-17
	true,  true,  true,  true,  false, false, // 18-23
	true,  false, false, false, false, false, // 24-29
	true,  true,  true,  false, true,  true,  // 30-35
	true,  true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, true,  false, false, true,  // 48-53
	false, false, false, false, false, false, // 54-59
	false, false, false, false, false, false, // 60-65
	false, false, false, false, false, false, // 66-71
	false, false, false, false, false, false, // 72-77
	true,  true, // 78-79
};

bool __getLoginRC[propscount] =
{
	// Account, level, nickname, headimg, bodyimg
	true,  false, false, false, false, false, // 0-5
	false, false, false, false, false, true,  // 6-11
	false, false, false, false, false, false, // 12-17
	false, false, true,  false, false, false, // 18-23
	false, false, false, false, false, false, // 24-29
	false, false, false, false, true,  true,  // 30-35
	false, false, false, false, false, false, // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	false, false, false, false, false, false, // 54-59
	false, false, false, false, false, false, // 60-65
	false, false, false, false, false, false, // 66-71
	false, false, false, false, false, false, // 72-77
	false, false, // 78-79
};

bool __sendLocal[propscount] =
{
	false, false, true,  false, false, false, // 0-5
	false, false, true,  true,  true,  true,  // 6-11
	true,  true,  false, true,  true,  true,  // 12-17
	true,  true,  true,  true,  false, false, // 18-23
	false, true,  false, false, false, false, // 24-29
	true,  false, true,  false, true,  true,  // 30-35
	true,  true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, true,  false, false, false, // 48-53
	true,  true,  true,  true,  true,  true,  // 54-59
	true,  true,  true,  true,  true,  true,  // 60-65
	true,  true,  true,  true,  true,  true,  // 66-71
	true,  true,  true,  false, false, false, // 72-77
	true,  true, // 78-79
};

/*
	Pointer-Functions for Packets
*/
std::vector<TPLSock> TPLFunc;

void createPLFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 200; i++)
		TPLFunc.push_back(&TPlayer::msgPLI_NULL);

	// now set non-nulls
	TPLFunc[PLI_LEVELWARP] = &TPlayer::msgPLI_LEVELWARP;
	TPLFunc[PLI_BOARDMODIFY] = &TPlayer::msgPLI_BOARDMODIFY;
	TPLFunc[PLI_PLAYERPROPS] = &TPlayer::msgPLI_PLAYERPROPS;
	TPLFunc[PLI_WANTFILE] = &TPlayer::msgPLI_WANTFILE;
	TPLFunc[PLI_NPCWEAPONIMG] = &TPlayer::msgPLI_NPCWEAPONIMG;
	TPLFunc[PLI_UPDATEFILE] = &TPlayer::msgPLI_UPDATEFILE;
	TPLFunc[PLI_LANGUAGE] = &TPlayer::msgPLI_LANGUAGE;
	TPLFunc[PLI_TRIGGERACTION] = &TPlayer::msgPLI_TRIGGERACTION;
	TPLFunc[PLI_MAPINFO] = &TPlayer::msgPLI_MAPINFO;
	TPLFunc[PLI_UNKNOWN46] = &TPlayer::msgPLI_UNKNOWN46;
}


/*
	Constructor - Deconstructor
*/
TPlayer::TPlayer(CSocket *pSocket)
: TAccount(),
playerSock(pSocket), iterator(0x04A80B38), key(0),
PLE_POST22(false), os("wind"), codepage(1252), level(0),
id(0), type(CLIENTTYPE_AWAIT)
{
	// TODO: lastChat and lastMessage
	lastData = lastMovement = lastChat = lastMessage = time(0);
	lastTimer = time(0);
	printf("Created for: %s\n", playerSock->tcpIp());
}

TPlayer::~TPlayer()
{
	if (id >= 0)
	{
		playerIds[id] = 0;
		//vecRemove<TPlayer*>(playerList, this);
		//vecRemove(playerList, this);

		// TODO: save pending weapons.
		if (type == CLIENTTYPE_CLIENT)
			saveAccount();

		// Announce our departure to other clients.
		sendPacketTo(CLIENTTYPE_CLIENT, CString() >> (char)PLO_OTHERPLPROPS >> (short)id >> (char)PLPROP_PCONNECTED, this);
		sendPacketTo(CLIENTTYPE_RC, CString() >> (char)PLO_DELPLAYER >> (short)id, this);
	}

	printf("Destroyed for: %s\n", playerSock->tcpIp());
	delete playerSock;
}


/*
	Socket-Control Functions
*/
bool TPlayer::doMain()
{
	if (playerSock == 0)
		return false;

	// definitions
	CString unBuffer;

	// receive
	if (playerSock->getData() == -1)
		return false;

	// grab the data now
	rBuffer.write(playerSock->getBuffer().text(), playerSock->getBuffer().length());
	playerSock->getBuffer().clear();

	// parse data
	while (rBuffer.length() >= 2)
	{
		// New data.
		lastData = time(0);

		// packet length
		unsigned int len = (unsigned int)rBuffer.readShort();
		if (len > (unsigned int)rBuffer.length()-2)
			break;

		// get packet
		unBuffer = rBuffer.readChars(len);
		rBuffer.removeI(0, len+2);

		// decrypt packet
		if (PLE_POST22)
			decryptPacket(unBuffer);
		else
			unBuffer.zuncompressI();

		// well theres your buffer
		if (!parsePacket(unBuffer))
			return false;
	}

	// Every second, do some events.
	if (time(0) != lastTimer) doTimedEvents();

	// send out buffer
	sendCompress();
	return true;
}

bool TPlayer::doTimedEvents()
{
	lastTimer = time(0);

	// Only run for clients.
	if (type != CLIENTTYPE_CLIENT) return true;

	// Increase online time.
	onlineTime++;

	// Disconnect if players are inactive.
	if (settings->getBool("disconnectifnotmoved"))
	{
		if ((int)difftime(lastTimer, lastMovement) > settings->getInt("maxnomovement", 1200))
		{
			serverlog.out("Client %s has been disconnected due to inactivity.\n", accountName.text());
			sendPacket(CString() >> (char)PLO_DISCMESSAGE << "You have been disconnected due to inactivity.");
			return false;
		}
	}

	// Disconnect if no data has been received in 5 minutes.
	if ((int)difftime(lastTimer, lastData) > 300)
	{
		serverlog.out("Client %s has timed out.\n", accountName.text());
		return false;
	}

	// Increase player AP.
	if (settings->getBool("apsystem"))
	{
		if (!(status & 1) /* TODO: sparzone check */)
			apCounter++;
		if (apCounter <= 0)
		{
			if (ap < 100)
			{
				ap++;
				// TODO: updateProp
			}
			if (ap < 20) apCounter = settings->getInt("aptime0", 30);
			else if (ap < 40) apCounter = settings->getInt("aptime1", 90);
			else if (ap < 60) apCounter = settings->getInt("aptime2", 300);
			else if (ap < 80) apCounter = settings->getInt("aptime3", 600);
			else apCounter = settings->getInt("aptime4", 1200);
		}
	}

	// TODO: save player account every now and then.
	return true;
}

bool TPlayer::parsePacket(CString& pPacket)
{
	// first packet.. maybe
	if (type == CLIENTTYPE_AWAIT)
		msgPLI_LOGIN(CString() << pPacket.readString("\n"));

	while (pPacket.bytesLeft() > 0)
	{
		// grab packet -- are you sure that \n is really still there.. kinda wierd that it is -- i've done some tests and it never stayed... - Joey
		CString curPacket = pPacket.readString("\n");
		if (curPacket[curPacket.length()-1] == '\n')
			curPacket.removeI(curPacket.length() - 1, 1);

		// decrypt packet
		if (!PLE_POST22)
			decryptPacket(curPacket);

		// read id & packet
		int id = curPacket.readGUChar();

		// check lengths
		if (id >= (unsigned char)TPLFunc.size())
			return false;

		// valid packet, call function
		if (!(*this.*TPLFunc[id])(curPacket))
			return false;
	}

	return true;
}

void TPlayer::decryptPacket(CString& pPacket)
{
	// Version 2.01 - 2.18 Encryption
	if (!PLE_POST22)
	{
		if (type != CLIENTTYPE_CLIENT)
			return;

		iterator *= 0x8088405;
		iterator += key;
		int pos  = ((iterator & 0x0FFFF) % pPacket.length());
		pPacket.removeI(pos, 1);
		return;
	}

	// Version 2.19 - 2.31 Encryption
	int pType = pPacket.readChar();
	pPacket.removeI(0, 1);

	// Decrypt Packet
	in_codec.limitfromtype(pType);
	in_codec.apply(reinterpret_cast<uint8_t*>(pPacket.text()), pPacket.length());

	// Uncompress Packet
	if (pType == ENCRYPT22_ZLIB)
		pPacket.zuncompressI();
	else if (pType == ENCRYPT22_BZ2)
		pPacket.bzuncompressI();
}

void TPlayer::sendPacket(CString pPacket)
{
	// empty buffer?
	if (pPacket.isEmpty())
		return;

	// append '\n'
	if (pPacket[pPacket.length()-1] != '\n')
		pPacket.writeChar('\n');

	// append buffer
	sBuffer.write(pPacket.text(), pPacket.length());

	// Send Data
	if (sBuffer.length() > 4096)
		sendCompress();
}

void TPlayer::sendCompress()
{
	// empty buffer?
	if (sBuffer.isEmpty())
		return;

	// compress buffer
	if (PLE_POST22) // anyway we can better this -- less instructions = better :p
	{
		/*
		FILE* f = fopen("test2.out", "wb");
		fwrite(sBuffer.text(), 1, sBuffer.length(), f);
		fclose(f);
		exit(0);
		*/
		// Choose which compression to use and apply it.
		int compressionType = ENCRYPT22_UNCOMPRESSED;
		if (sBuffer.length() > 0x2000)	// 8KB
		{
			compressionType = ENCRYPT22_BZ2;
			sBuffer.bzcompressI();
		}
		else if (sBuffer.length() > 40)
		{
			compressionType = ENCRYPT22_ZLIB;
			sBuffer.zcompressI();
		}

		// Encrypt the packet and send it out.
		out_codec.limitfromtype(compressionType);
		out_codec.apply(reinterpret_cast<uint8_t*>(sBuffer.text()), sBuffer.length());
		playerSock->sendData(CString() << (short)(sBuffer.length()+1) << (char)compressionType << sBuffer);
	}
	else
	{
		sBuffer.zcompressI();
		playerSock->sendData(CString() << (short)sBuffer.length() << sBuffer);
	}

	// clear buffer
	sBuffer.clear();
}


/*
	TPlayer: Get Properties
*/
TLevel* TPlayer::getLevel()
{
	return level;
}


/*
	TPlayer: Set Properties
*/
void TPlayer::setLevel(const CString& pLevelName)
{
	// Open Level
	// TODO: Work out the entire levelname/directories problem.
	level = TLevel::findLevel(pLevelName);
	if (level == 0)
	{
		printf("TODO: TLevel::findLevel returned 0.\n");
		return;
	}

	// Send Level
	sendPacket(CString() >> (char)PLO_PLAYERWARP << getProp(PLPROP_X) << getProp(PLPROP_Y) << level->getLevelName());
	sendPacket(CString() >> (char)PLO_LEVELNAME << level->getLevelName());
	sendPacket(CString() >> (char)PLO_BOARDPACKETSIZE >> (int)(1+(64*64*2)+1));
	sendPacket(CString() << level->getBoardPacket());
	sendCompress();
}

void TPlayer::setNick(const CString& pNickName)
{
	nickName = pNickName;
}


/*
	TPlayer: Packet functions
*/
bool TPlayer::msgPLI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	printf("Unknown Player Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	return true;
}

bool TPlayer::msgPLI_LOGIN(CString& pPacket)
{
	// Read Player-Ip
	accountIp = inet_addr(playerSock->tcpIp());

	// Read Client-Type
	type = pPacket.readGChar();
	switch (type)
	{
		case CLIENTTYPE_CLIENT:
			serverlog.out("2.171 client logging in.\n");
			break;
		case CLIENTTYPE_CLIENT22:
			serverlog.out("Post 2.2 client logging in.\n");
			PLE_POST22 = true;
			type = CLIENTTYPE_CLIENT;
			break;
		case CLIENTTYPE_RC:
			serverlog.out("RC logging in.\n");
			break;
	}

	printf("Test: %i\n", PLE_POST22);

	// Get Iterator-Key
	if (type == CLIENTTYPE_CLIENT)
	{
		key = (unsigned char)pPacket.readGChar();
		in_codec.reset(key);
		out_codec.reset(key);
	}

	// Read Client-Version
	version = pPacket.readChars(8);

	// Read Account & Password
	accountName = pPacket.readChars(pPacket.readGUChar());
	CString password = pPacket.readChars(pPacket.readGUChar());

	serverlog.out("Key: %d\n", key);
	serverlog.out("Version: %s\n", version.text());
	serverlog.out("Account: %s\n", accountName.text());
	serverlog.out("Password: %s\n", password.text());
	//printf("Key: %d\n", key);
	//printf("Version: %s\n", version.text());
	//printf("Account: %s\n", accountName.text());
	//printf("Password: %s\n", password.text());

	// Check for available slots on the server.
	printf("TODO: TPlayer::msgPLI_LOGIN(), Check for available slots.\n");

	// Check if they are ip-banned or not.
	printf("TODO: TPlayer::msgPLI_LOGIN(), Check if player is ip-banned.\n");

	// Verify login details with the serverlist.
	// TODO: Don't forget localhost mode.  Need to global-ify the serverlist
	// class to do this.
	printf("TODO: TPlayer::msgPLI_LOGIN(), Verify login information.\n");

	// Process Login
	// TODO: This should be sent only when the serverlist verifies the login.
	sendLogin();

	return true;
}

bool TPlayer::msgPLI_LEVELWARP(CString& pPacket)
{
	printf("TODO: TPlayer::msgPLI_LEVELWARP\n");
	return true;
}

bool TPlayer::msgPLI_BOARDMODIFY(CString& pPacket)
{
	printf("TODO: TPlayer::msgPLI_BOARDMODIFY\n");
	return true;
}

bool TPlayer::msgPLI_PLAYERPROPS(CString& pPacket)
{
	this->setProps(pPacket, true);
	return true;
}

bool TPlayer::msgPLI_WANTFILE(CString& pPacket)
{
	printf("TODO: TPlayer::msgPLI_WANTFILE\n");
	return true;
}

bool TPlayer::msgPLI_NPCWEAPONIMG(CString& pPacket)
{
	// TODO
	return true;
}

bool TPlayer::msgPLI_UPDATEFILE(CString& pPacket)
{
	printf("TODO: TPlayer::msgPLI_UPDATEFILE\n");
	//pPacket.readString("");
	return true;
}

bool TPlayer::msgPLI_LANGUAGE(CString& pPacket)
{
	//language = pPacket.readChars(pPacket.readGUChar());
	language = pPacket.readString("");
	return true;
}

bool TPlayer::msgPLI_TRIGGERACTION(CString& pPacket)
{
	int npcId = pPacket.readGInt();
	float x = (float)pPacket.readGChar() / 2;
	float y = (float)pPacket.readGChar() / 2;
	CString action = pPacket.readString("");

	// We don't have an NPCserver, so, for now, just pass it along.
	CString packet;
	packet >> (char)PLO_TRIGGERACTION >> (short)this->id << pPacket.text()+1;
	sendPacketToLevel(packet, this->level, this);
	return true;
}

bool TPlayer::msgPLI_MAPINFO(CString& pPacket)
{
	// Don't know what this does exactly.  Might be gmap related.
	pPacket.readString("");
	return true;
}

bool TPlayer::msgPLI_UNKNOWN46(CString& pPacket)
{
	printf("TODO: TPlayer::msgPLI_UNKNOWN46\n");
	pPacket.readString("");
	return true;
}
