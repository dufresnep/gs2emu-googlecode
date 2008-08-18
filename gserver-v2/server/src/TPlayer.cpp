#include <time.h>
#include <math.h>
#include "ICommon.h"
#include "CSocket.h"
#include "TServer.h"
#include "TPlayer.h"
#include "TAccount.h"
#include "codec.h"

/*
	Logs
*/
#define serverlog	server->getServerLog()
#define rclog		server->getRCLog()

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
	true,  true,  false, false, true, // 78-82
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
	true,  true,  false, false, true, // 78-82
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
	true,  true,  false, false, true, // 78-82
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
	true,  true,  false, false, true, // 78-82
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
	TPLFunc[PLI_NPCPROPS] = &TPlayer::msgPLI_NPCPROPS;
	TPLFunc[PLI_BOMBADD] = &TPlayer::msgPLI_BOMBADD;
	TPLFunc[PLI_BOMBDEL] = &TPlayer::msgPLI_BOMBDEL;
	TPLFunc[PLI_TOALL] = &TPlayer::msgPLI_TOALL;
	TPLFunc[PLI_HORSEADD] = &TPlayer::msgPLI_HORSEADD;
	TPLFunc[PLI_HORSEDEL] = &TPlayer::msgPLI_HORSEDEL;
	TPLFunc[PLI_ARROWADD] = &TPlayer::msgPLI_ARROWADD;
	TPLFunc[PLI_FIRESPY] = &TPlayer::msgPLI_FIRESPY;
	TPLFunc[PLI_THROWCARRIED] = &TPlayer::msgPLI_THROWCARRIED;
	TPLFunc[PLI_ITEMADD] = &TPlayer::msgPLI_ITEMADD;
	TPLFunc[PLI_ITEMDEL] = &TPlayer::msgPLI_ITEMDEL;
	TPLFunc[PLI_CLAIMPKER] = &TPlayer::msgPLI_CLAIMPKER;
	TPLFunc[PLI_BADDYPROPS] = &TPlayer::msgPLI_BADDYPROPS;
	TPLFunc[PLI_BADDYHURT] = &TPlayer::msgPLI_BADDYHURT;
	TPLFunc[PLI_BADDYADD] = &TPlayer::msgPLI_BADDYADD;

	TPLFunc[PLI_OPENCHEST] = &TPlayer::msgPLI_OPENCHEST;

	TPLFunc[PLI_WANTFILE] = &TPlayer::msgPLI_WANTFILE;
	TPLFunc[PLI_SHOWIMG] = &TPlayer::msgPLI_SHOWIMG;
	TPLFunc[PLI_NPCWEAPONDEL] = &TPlayer::msgPLI_NPCWEAPONDEL;
	TPLFunc[PLI_FORCELEVELWARP] = &TPlayer::msgPLI_LEVELWARP;	// Shared with PLI_LEVELWARP
	TPLFunc[PLI_WEAPONADD] = &TPlayer::msgPLI_WEAPONADD;
	TPLFunc[PLI_ITEMTAKE] = &TPlayer::msgPLI_ITEMDEL;			// Shared with PLI_ITEMDEL
	TPLFunc[PLI_UPDATEFILE] = &TPlayer::msgPLI_UPDATEFILE;
	TPLFunc[PLI_ADJACENTLEVEL] = &TPlayer::msgPLI_ADJACENTLEVEL;
	TPLFunc[PLI_LANGUAGE] = &TPlayer::msgPLI_LANGUAGE;
	TPLFunc[PLI_TRIGGERACTION] = &TPlayer::msgPLI_TRIGGERACTION;
	TPLFunc[PLI_MAPINFO] = &TPlayer::msgPLI_MAPINFO;
	TPLFunc[PLI_UNKNOWN46] = &TPlayer::msgPLI_UNKNOWN46;
}


/*
	Constructor - Deconstructor
*/
TPlayer::TPlayer(TServer* pServer, CSocket *pSocket)
: TAccount(pServer),
playerSock(pSocket), iterator(0x04A80B38), key(0),
PLE_POST22(false), os("wind"), codepage(1252), level(0),
id(0), type(CLIENTTYPE_AWAIT), server(pServer), allowBomb(false), hadBomb(false)
{
	// TODO: lastChat and lastMessage
	lastData = lastMovement = lastChat = lastMessage = lastSave = time(0);
	printf("Created for: %s\n", playerSock->tcpIp());
}

TPlayer::~TPlayer()
{
	if (id >= 0)
	{
		// TODO: save pending weapons.

		// Save account.
		if (type == CLIENTTYPE_CLIENT)
			saveAccount();

		// Remove from the level.
		if (level) level->removePlayer(this);

		// Announce our departure to other clients.
		server->sendPacketTo(CLIENTTYPE_CLIENT, CString() >> (char)PLO_OTHERPLPROPS >> (short)id >> (char)PLPROP_PCONNECTED, this);
		server->sendPacketTo(CLIENTTYPE_RC, CString() >> (char)PLO_DELPLAYER >> (short)id, this);
	}

	// Clean up.
	for (std::vector<SCachedLevel*>::iterator i = cachedLevels.begin(); i != cachedLevels.end(); )
	{
		SCachedLevel* cl = *i;
		delete cl;
		i = cachedLevels.erase(i);
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
	rBuffer.setRead(0);
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

	// send out buffer
	sendCompress();
	return true;
}

bool TPlayer::doTimedEvents()
{
	time_t currTime = time(0);

	// Only run for clients.
	if (type != CLIENTTYPE_CLIENT) return true;

	// Increase online time.
	onlineTime++;

	// Disconnect if players are inactive.
	CSettings* settings = server->getSettings();
	if (settings->getBool("disconnectifnotmoved"))
	{
		if ((int)difftime(currTime, lastMovement) > settings->getInt("maxnomovement", 1200))
		{
			serverlog.out("Client %s has been disconnected due to inactivity.\n", accountName.text());
			sendPacket(CString() >> (char)PLO_DISCMESSAGE << "You have been disconnected due to inactivity.");
			return false;
		}
	}

	// Disconnect if no data has been received in 5 minutes.
	if ((int)difftime(currTime, lastData) > 300)
	{
		serverlog.out("Client %s has timed out.\n", accountName.text());
		return false;
	}

	// Increase player AP.
	if (settings->getBool("apsystem") && level != 0)
	{
		if (!(status & PLSTATUS_PAUSED) && level->getSparringZone() == false)
			apCounter--;

		if (apCounter <= 0)
		{
			if (ap < 100)
			{
				ap++;
				setProps(CString() >> (char)PLPROP_ALIGNMENT >> (char)ap, true, true);
			}
			if (ap < 20) apCounter = settings->getInt("aptime0", 30);
			else if (ap < 40) apCounter = settings->getInt("aptime1", 90);
			else if (ap < 60) apCounter = settings->getInt("aptime2", 300);
			else if (ap < 80) apCounter = settings->getInt("aptime3", 600);
			else apCounter = settings->getInt("aptime4", 1200);
		}
	}

	// Save player account every 5 minutes.
	if ((int)difftime(currTime, lastSave) > 300)
	{
		lastSave = currTime;
		saveAccount();
	}

	return true;
}

bool TPlayer::parsePacket(CString& pPacket)
{
	// first packet.. maybe
	if (type == CLIENTTYPE_AWAIT)
		msgPLI_LOGIN(CString() << pPacket.readString("\n"));

	while (pPacket.bytesLeft() > 0)
	{
		// grab packet
		CString curPacket = pPacket.readString("\n");
		//if (curPacket[curPacket.length()-1] == '\n')
		//	curPacket.removeI(curPacket.length() - 1, 1);

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
	if (PLE_POST22)
	{
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
	TPlayer: Set Properties
*/
bool TPlayer::setLevel(const CString& pLevelName, float x, float y, time_t modTime)
{
	std::vector<TPlayer*>* playerList = server->getPlayerList();

	if (level != 0)
	{
		// Save the time we left the level for the client-side caching.
		bool found = false;
		for (std::vector<SCachedLevel*>::iterator i = cachedLevels.begin(); i != cachedLevels.end(); ++i)
		{
			SCachedLevel* cl = *i;
			if (cl->level == level)
			{
				cl->modTime = time(0);
				found = true;
			}
		}
		if (found == false) cachedLevels.push_back(new SCachedLevel(level, time(0)));

		// Remove self from list of players in level.
		level->removePlayer(this);

		// Send PLO_ISLEADER to new level leader.
		TPlayer* leader = level->getPlayer(0);
		if (leader != 0) leader->sendPacket(CString() >> (char)PLO_ISLEADER);

		// Tell everyone I left.
		server->sendPacketToLevel(this->getProps(0, 0) >> (char)PLPROP_JOINLEAVELVL >> (char)0, level, this);
		for (std::vector<TPlayer*>::iterator i = playerList->begin(); i != playerList->end(); ++i)
		{
			TPlayer* player = (TPlayer*)*i;
			if (player == this) continue;
			if (player->getLevel() != level) continue;
			this->sendPacket(player->getProps(0, 0) >> (char)PLPROP_JOINLEAVELVL >> (char)0);
		}
	}

	// Open Level
	level = TLevel::findLevel(pLevelName, server);
	if (level == 0)
		return false;

	// Add myself to the level playerlist.
	level->addPlayer(this);
	levelName = level->getLevelName();

	// Set x/y location.
	this->x = x;
	this->y = y;

	// Tell the client their new level.
	TGMap* gmap = server->getLevelGMap(level);
	if (modTime == 0)
	{
		// GMaps use a special warp packet.
		if (gmap != 0)
			sendPacket(CString() >> (char)PLO_PLAYERWARPGMAP >> (char)(x * 2) >> (char)(y * 2) >> (char)(z * 2) >> (char)gmap->getLevelX(levelName) >> (char)gmap->getLevelY(levelName) << gmap->getMapName());
		else
			sendPacket(CString() >> (char)PLO_PLAYERWARP >> (char)(x * 2) >> (char)(y * 2) << levelName);
	}
	sendPacket(CString() >> (char)PLO_LEVELNAME << levelName);

	// Send Level
	time_t l_time = getCachedLevelModTime(level);
	if (l_time == 0)
	{
		if (modTime != level->getModTime())
		{
			sendPacket(CString() >> (char)PLO_RAWDATA >> (int)(1+(64*64*2)+1));
			sendPacket(CString() << level->getBoardPacket());
		}
		sendCompress();

		// Send links, signs, and mod time.
		sendPacket(CString() << level->getLinksPacket());
		sendPacket(CString() << level->getSignsPacket());
		sendPacket(CString() >> (char)PLO_LEVELMODTIME >> (long long)level->getModTime());
	}
	//if (gmap) sendPacket(CString() >> (char)PLO_LEVELNAME << gmap->getMapName());

	// Send board changes, chests, horses, and baddies.
	sendPacket(CString() << level->getBoardChangesPacket(l_time));
	sendPacket(CString() << level->getChestPacket(this));
	sendPacket(CString() << level->getHorsePacket());
	sendPacket(CString() << level->getBaddyPacket());

	// Tell the client if there are any ghost players in the level.
	// Graal Reborn doesn't support trial accounts so pass 0 (no ghosts) instead of 1 (ghosts present).
	//sendPacket(CString() >> (char)PLO_GHOSTICON >> (char)0);

	// If we are the leader, send it now.
	if (level->getPlayer(0) == this)
		sendPacket(CString() >> (char)PLO_ISLEADER);

	// TODO: Send new world time.

	// Send NPCs.
	sendPacket(CString() >> (char)PLO_SETNPCLEVEL << ((gmap != 0) ? gmap->getMapName() : levelName));
	//sendPacket(CString() >> (char)PLO_SETNPCLEVEL << levelName);
	sendPacket(CString() << level->getNpcsPacket(l_time));

	// If the level is a sparring zone and you have 100 AP, change AP to 99 and
	// the apcounter to 1.
	if (level->getSparringZone() && ap == 100)
	{
		ap = 99;
		apCounter = 1;
		setProps(CString() >> (char)PLPROP_ALIGNMENT >> (char)ap, true, true);
	}

	// Inform everybody as to the client's new location.  This will update the minimap.
	server->sendPacketToAll(this->getProps(0,0) >> (char)PLPROP_CURLEVEL << this->getProp(PLPROP_CURLEVEL) >> (char)PLPROP_X << this->getProp(PLPROP_X) >> (char)PLPROP_Y << this->getProp(PLPROP_Y), this);

	// Do props stuff.
	server->sendPacketToLevel(this->getProps(__getLogin, sizeof(__getLogin)/sizeof(bool)), this->level, this);
	for (std::vector<TPlayer*>::iterator i = playerList->begin(); i != playerList->end(); ++i)
	{
		TPlayer* player = (TPlayer*)*i;
		if (player == this) continue;
		this->sendPacket(player->getProps(__getLogin, sizeof(__getLogin)/sizeof(bool)));
	}

	sendCompress();

	return true;
}

time_t TPlayer::getCachedLevelModTime(const TLevel* level) const
{
	for (std::vector<SCachedLevel*>::const_iterator i = cachedLevels.begin(); i != cachedLevels.end(); ++i)
	{
		SCachedLevel* cl = *i;
		if (cl->level == level)
			return cl->modTime;
	}
	return 0;
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
	return sendLogin();
}

bool TPlayer::msgPLI_LEVELWARP(CString& pPacket)
{
	printf( "msgPLI_LEVELWARP\n" );
	time_t modTime = 0;

	if (pPacket[0] - 32 == PLI_FORCELEVELWARP)
		modTime = (time_t)pPacket.readGUInt5();

	float loc[2] = {(float)(pPacket.readGChar() / 2), (float)(pPacket.readGChar() / 2)};
	CString newLevel = pPacket.readString("");
	setLevel(newLevel, loc[0], loc[1], modTime);
	// TODO: Correct warp function.

	return true;
}

bool TPlayer::msgPLI_BOARDMODIFY(CString& pPacket)
{
	CSettings* settings = server->getSettings();
	signed char loc[2] = {pPacket.readGChar(), pPacket.readGChar()};
	signed char dim[2] = {pPacket.readGChar(), pPacket.readGChar()};
	CString tiles = pPacket.readString("");

	// Alter level data.
	if (level->alterBoard(tiles, loc[0], loc[1], dim[0], dim[1], this))
		server->sendPacketToLevel(CString() >> (char)PLO_BOARDMODIFY << pPacket.text() + 1, level);

	if (loc[0] < 0 || loc[0] > 63 || loc[1] < 0 || loc[1] > 63) return true;

	// Lay items when you destroy objects.
	short oldTile = (getLevel()->getTiles())[loc[0] + (loc[1] * 64)];
	int dropItem = -1;
	bool bushitems = settings->getBool("bushitems", true);
	bool vasesdrop = settings->getBool("vasesdrop", true);
	int tiledroprate = settings->getInt("tiledroprate", 50);

	// Bushes, grass, swamp.
	// Bushes, grass, swamp
	if ((oldTile == 2 || oldTile == 0x1a4 || oldTile == 0x1ff ||
		oldTile == 0x3ff) && bushitems)
	{
		if ( tiledroprate > 0 )
		{
			if ( (rand() % 100) < tiledroprate )
			{
				int index = rand() % 6;
				dropItem = index;
			}
		}
	}
	// Vase.
	else if (oldTile == 0x2ac && vasesdrop)
		dropItem = 5;

	// Send the item now.
	// TODO: Make this a more generic function.
	if (dropItem >= 0)
	{
		CString packet;
		packet >> (char)(loc[0] * 2) >> (char)(loc[1] * 2) >> (char)dropItem;

		msgPLI_ITEMADD(CString() << packet);
		sendPacket(CString() >> (char)PLO_ITEMADD << packet);
	}

	return true;
}

bool TPlayer::msgPLI_PLAYERPROPS(CString& pPacket)
{
	this->setProps(pPacket, true);
	return true;
}

bool TPlayer::msgPLI_NPCPROPS(CString& pPacket)
{
	int npcId = pPacket.readGUInt();
	CString npcProps = pPacket.readString("");

	//printf( "npcId: %d\n", npcId );
	//printf( "pPacket: %s\n", npcProps.text());
	//for (int i = 0; i < pPacket.length(); ++i) printf( "%02x ", (unsigned char)pPacket[i] );
	//printf( "\n" );

	TNPC* npc = server->getNPC(npcId);
	if (npc == 0)
		return true;

	if (npc->getLevel() != level)
		return true;

	CString packet = CString() >> (char)PLO_NPCPROPS << pPacket.text() + 1;
	server->sendPacketToLevel(packet, level, this);
	npc->setProps(npcProps);

	return true;
}

bool TPlayer::msgPLI_BOMBADD(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_BOMBADD >> (short)id << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_BOMBDEL(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_BOMBDEL << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_TOALL(CString& pPacket)
{
	// TODO: jail levels.
	CString message = pPacket.readString("");
	// TODO: word filter.

	std::vector<TPlayer*>* playerList = server->getPlayerList();
	for (std::vector<TPlayer*>::iterator i = playerList->begin(); i != playerList->end(); ++i)
	{
		TPlayer* player = *i;
		if (player == this) continue;

		// See if the player is allowing toalls.
		unsigned char flags = strtoint(player->getProp(PLPROP_ADDITFLAGS));
		if (flags & PLFLAG_NOTOALL) continue;

		player->sendPacket(CString() >> (char)PLO_TOALL << message);
	}
	return true;
}

bool TPlayer::msgPLI_HORSEADD(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_HORSEADD << pPacket.text() + 1, level, this);

	float hx = (float)pPacket.readGUChar() / 2.0f;
	float hy = (float)pPacket.readGUChar() / 2.0f;
	CString image = pPacket.readChars(pPacket.readGUChar());

	level->addHorse(hx, hy, image);
	return true;
}

bool TPlayer::msgPLI_HORSEDEL(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_HORSEDEL << pPacket.text() + 1, level, this);

	float hx = (float)pPacket.readGUChar() / 2.0f;
	float hy = (float)pPacket.readGUChar() / 2.0f;

	level->removeHorse(hx, hy);
	return true;
}

bool TPlayer::msgPLI_ARROWADD(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_ARROWADD >> (short)id << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_FIRESPY(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_FIRESPY >> (short)id << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_THROWCARRIED(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_THROWCARRIED >> (short)id << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_ITEMADD(CString& pPacket)
{
	float iX = (float)pPacket.readGUChar() / 2.0f;
	float iY = (float)pPacket.readGUChar() / 2.0f;
	unsigned char item = pPacket.readGUChar();

	level->addItem(iX, iY, item);
	server->sendPacketToLevel(CString() >> (char)PLO_ITEMADD << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_ITEMDEL(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_ITEMDEL << pPacket.text() + 1, level, this);

	float iX = (float)pPacket.readGUChar() / 2.0f;
	float iY = (float)pPacket.readGUChar() / 2.0f;

	// Remove the item from the level, getting the type of the item in the process.
	char item = level->removeItem(iX, iY);
	if (item == -1) return true;

	// If this is a PLI_ITEMTAKE packet, give the item to the player.
	if (pPacket[0] - 32 == PLI_ITEMTAKE)
		this->setProps(TLevelItem::getItemPlayerProp(item, this), true, true);

	return true;
}

bool TPlayer::msgPLI_CLAIMPKER(CString& pPacket)
{
	// Get the player who killed us.
	int pId = pPacket.readGShort();
	TPlayer* player = server->getPlayer(pId);
	if (player == 0 || player == this) return true;

	// Sparring zone rating code.
	// Uses the glicko rating system.
	if (level == 0) return true;
	if ( level->getSparringZone() )
	{
		// Get some stats we are going to use.
		// Need to parse the other player's PLPROP_RATING.
		int otherRating = player->getProp(PLPROP_RATING).readGInt();
		float oldStats[4] = { rating, deviation, (float)((otherRating >> 9) & 0xFFF), (float)(otherRating & 0x1FF) };

		// If the IPs are the same, don't update the rating to prevent cheating.
		if (this->getProp(PLPROP_LASTIP).readGInt5() == player->getProp(PLPROP_LASTIP).readGInt5()) return true;

		float gSpar[2] = {1.0f / pow((1.0f+3.0f*pow(0.0057565f,2)*(pow(oldStats[3],2))/pow(3.14159265f,2)),0.5f),	//Winner
					  	  1.0f / pow((1.0f+3.0f*pow(0.0057565f,2)*(pow(oldStats[1],2))/pow(3.14159265f,2)),0.5f)};	//Loser
		float ESpar[2] = {1.0f / (1.0f + pow(10.0f,(-gSpar[1]*(oldStats[2]-oldStats[0])/400.0f))),					//Winner
						  1.0f / (1.0f + pow(10.0f,(-gSpar[0]*(oldStats[0]-oldStats[2])/400.0f)))};					//Loser
		float dSpar[2] = {1.0f / (pow(0.0057565f,2)*pow(gSpar[0],2)*ESpar[0]*(1.0f-ESpar[0])),						//Winner
						  1.0f / (pow(0.0057565f,2)*pow(gSpar[1],2)*ESpar[1]*(1.0f-ESpar[1]))};						//Loser

		float tWinRating = oldStats[2] + (0.0057565f / ( 1.0f / pow(oldStats[3],2) + 1.0f/dSpar[0])) * (gSpar[0] * (1.0f - ESpar[0]));
		float tLoseRating = oldStats[0] + (0.0057565f / ( 1.0f / pow(oldStats[1],2) + 1.0f/dSpar[1])) * (gSpar[1] * (0.0f - ESpar[1]));
  		float tWinDeviation = pow((1.0f/(1.0f/pow(oldStats[3],2)+1/dSpar[0])),0.5f);
  		float tLoseDeviation = pow((1.0f/(1.0f/pow(oldStats[1],2)+1/dSpar[1])),0.5f);

		// Cap the rating.
		tWinRating = clip( tWinRating, 0.0f, 4000.0f );
		tLoseRating = clip( tLoseRating, 0.0f, 4000.0f );
		tWinDeviation = clip( tWinDeviation, 50.0f, 350.0f );
		tLoseDeviation = clip( tLoseDeviation, 50.0f, 350.0f );

		// Update the Ratings.
		// setProps will cause it to grab the new rating and send it to everybody in the level.
		// Therefore, just pass a dummy value.  setProps doesn't alter your rating for packet hacking reasons.
		if ( oldStats[0] != tLoseRating || oldStats[1] != tLoseDeviation )
		{
			setRating((int)tLoseRating, (int)tLoseDeviation);
			this->setProps(CString() >> (char)PLPROP_RATING >> (int)0, true);
		}
		if ( oldStats[2] != tWinRating || oldStats[3] != tWinDeviation )
		{
			player->setRating((int)tWinRating, (int)tWinDeviation);
			player->setProps(CString() >> (char)PLPROP_RATING >> (int)0, true);
		}
		this->setLastSparTime(time(0));
		player->setLastSparTime(time(0));
	}
	else
	{
		CSettings* settings = server->getSettings();

		// Give a kill to the player who killed me.
		if (settings->getBool("dontchangekills", false) == false)
			player->setKills(player->getProp(PLPROP_KILLSCOUNT).readGInt() + 1);

		// Now, adjust their AP if allowed.
		if (settings->getBool("apsystem", true))
		{
			char oAp = player->getProp(PLPROP_ALIGNMENT).readGChar();

			// If I have 20 or more AP, they lose AP.
			if (oAp > 0 && ap > 19)
			{
				int aptime[] = {settings->getInt("aptime0", 30), settings->getInt("aptime1", 90),
					settings->getInt("aptime2", 300), settings->getInt("aptime3", 600),
					settings->getInt("aptime4", 1200)};
				oAp -= (((oAp / 20) + 1) * (ap / 20));
				if (oAp < 0) oAp = 0;
				player->setApCounter((oAp < 20 ? aptime[0] : (oAp < 40 ? aptime[1] : (oAp < 60 ? aptime[2] : (oAp < 80 ? aptime[3] : aptime[4])))));
				player->setProps(CString() >> (char)PLPROP_ALIGNMENT >> (char)ap, true, true);
			}
		}
	}

	return true;
}

bool TPlayer::msgPLI_BADDYPROPS(CString& pPacket)
{
	char id = pPacket.readGChar();
	CString props = pPacket.readString("");

	// Get the baddy.
	TLevelBaddy* baddy = level->getBaddy(id);
	if (baddy == 0) return true;

	// Set the props and send to everybody in the level.
	baddy->setProps(props);
	server->sendPacketToLevel(CString() >> (char)PLO_BADDYPROPS >> (char)baddy->getId() << props, level);
	return true;
}

bool TPlayer::msgPLI_BADDYHURT(CString& pPacket)
{
	TPlayer* leader = level->getPlayer(0);
	if (leader == 0) return true;
	leader->sendPacket(CString() >> (char)PLO_BADDYHURT << pPacket.text() + 1);
	return true;
}

bool TPlayer::msgPLI_BADDYADD(CString& pPacket)
{
	float bX = (float)pPacket.readGChar() / 2.0f;
	float bY = (float)pPacket.readGChar() / 2.0f;
	char bType = pPacket.readGChar();
	CString bImage = pPacket.readChars(pPacket.bytesLeft() - 1);
	char bPower = pPacket.readGChar();
	bPower = min(bPower, 12);		// Hard-limit to 6 hearts.

	// Add the baddy.
	TLevelBaddy* baddy = level->addBaddy(bX, bY, bType);
	if (baddy == 0) return true;

	// Set the baddy props.
	baddy->setRespawn(false);
	baddy->setProps(CString() >> (char)BDPROP_POWERIMAGE >> (char)bPower >> (char)bImage.length() << bImage);

	// Send the props to everybody in the level.
	server->sendPacketToLevel(CString() >> (char)PLO_BADDYPROPS >> (char)baddy->getId() << baddy->getProps(), level);
	return true;
}


bool TPlayer::msgPLI_OPENCHEST(CString& pPacket)
{
	unsigned char cX = pPacket.readGUChar();
	unsigned char cY = pPacket.readGUChar();
	std::vector<TLevelChest *>* levelChests = level->getLevelChests();

	for (std::vector<TLevelChest*>::iterator i = levelChests->begin(); i != levelChests->end(); ++i)
	{
		TLevelChest* chest = *i;
		if (chest->getX() == cX && chest->getY() == cY)
		{
			int chestItem = chest->getItemIndex();
			this->setProps(TLevelItem::getItemPlayerProp((char)chestItem, this), true, true);
			// TODO: save chest to player.
		}
	}
	return true;
}

bool TPlayer::msgPLI_WANTFILE(CString& pPacket)
{
	CFileSystem* fileSystem = server->getFileSystem();

	// Load file.
	CString file = pPacket.readString("");
	CString fileData = fileSystem->load(file);
	time_t modTime = fileSystem->getModTime(file);

	if (fileData.length() == 0) return true;

	printf( "msgPLI_WANTFILE: %s\n", file.text() );

	// See if we have enough room in the packet for the file.
	// If not, we need to send it as a big file.
	// 1 (PLO_FILE) + 5 (modTime) + 1 (file.length()) + file.length() + 1 (\n)
	bool isBigFile = false;
	int packetLength = 1 + 5 + 1 + file.length() + 1;
	if (fileData.length() > (0xFFFF - 0x20 - packetLength))
		isBigFile = true;

	// If we are sending a big file, let the client know now.
	if (isBigFile) sendPacket(CString() >> (char)PLO_LARGEFILESTART << file);

	// Send the file now.
	while (fileData.length() != 0)
	{
		int sendSize = 0xFFFF - 0x20 - packetLength;
		sendSize = clip(sendSize, 0, fileData.length());
		sendPacket(CString() >> (char)PLO_RAWDATA >> (int)(packetLength + sendSize));
		sendPacket(CString() >> (char)PLO_FILE >> (long long)modTime >> (char)file.length() << file << fileData.subString(0, sendSize));
		fileData.removeI(0, sendSize);
	}

	// If we had sent a large file, let the client know we finished sending it.
	if (isBigFile) sendPacket(CString() >> (char)PLO_LARGEFILEEND << file);

	sendCompress();

	return true;
}

bool TPlayer::msgPLI_SHOWIMG(CString& pPacket)
{
	server->sendPacketToLevel(CString() >> (char)PLO_SHOWIMG >> (short)id << pPacket.text() + 1, level, this);
	return true;
}

bool TPlayer::msgPLI_NPCWEAPONDEL(CString& pPacket)
{
	CString weapon = pPacket.readString("");
	for (std::vector<CString>::iterator i = weaponList.begin(); i != weaponList.end(); )
	{
		if (*i == weapon)
			i = weaponList.erase(i);
		else ++i;
	}
	return true;
}

bool TPlayer::msgPLI_WEAPONADD(CString& pPacket)
{
	CSettings* settings = server->getSettings();
	char type = pPacket.readGChar();

	// Type 0 means it is a default weapon.
	if (type == 0)
	{
		std::vector<TWeapon*>* weaponList = server->getWeaponList();

		if (settings->getBool("defaultweapons", true) == false)
			return true;

		char item = pPacket.readGChar();
		if (item == 8 && allowBomb == false)
		{
			allowBomb = true;
			return true;
		}

		TWeapon* weapon = server->getWeapon(TLevelItem::getItemName(item));
		if (weapon == 0)
		{
			weapon = new TWeapon(item);
			weaponList->push_back(weapon);
		}
		sendPacket(CString() << weapon->getWeaponPacket());
	}
	// NPC weapons.
	else
	{
		std::vector<TNPC*>* npcIds = server->getNPCIdList();
		std::vector<TWeapon*>* weaponList = server->getWeaponList();

		// Get the NPC id.
		int npcId = pPacket.readGInt();
		TNPC* npc = (*npcIds)[npcId];
		if (npc == 0)
			return true;

		// Get the name of the weapon.
		CString name = npc->getWeaponName();
		if (name.length() == 0)
			return true;

		// See if we can find the weapon in the server weapon list.
		TWeapon* weapon = server->getWeapon(name);

		// If weapon is 0, that means the NPC was not found.  Add the NPC to the list.
		if (weapon == 0)
		{
			weapon = new TWeapon(npc);
			weaponList->push_back(weapon);
		}

		// Check and see if the weapon has changed recently.  If it has, we should
		// send the new NPC to everybody on the server.
		bool foundThis = false;
		if (weapon->getModTime() != npc->getLevel()->getModTime())
		{
			std::vector<TPlayer*>* playerList = server->getPlayerList();
			for (std::vector<TPlayer*>::iterator i = playerList->begin(); i != playerList->end(); ++i)
			{
				TPlayer* player = *i;
				if (player->hasWeapon(weapon->getName()))
				{
					if (player == this) foundThis = true;
					player->sendPacket(CString() >> (char)PLO_NPCWEAPONDEL << weapon->getName());
					player->sendPacket(CString() << weapon->getWeaponPacket());
				}
			}
		}

		// Send the weapon to the player now.
		if (foundThis == false)
			sendPacket(CString() << weapon->getWeaponPacket());

		// TODO: saving weapons
	}
	return true;
}


bool TPlayer::msgPLI_UPDATEFILE(CString& pPacket)
{
	CFileSystem* fileSystem = server->getFileSystem();

	// Get the packet data and file mod time.
	time_t modTime = pPacket.readGUInt5();
	CString file = pPacket.readString("");
	time_t fModTime = fileSystem->getModTime(file);

	// If the file on disk is different, send it to the player.
	if (fModTime != modTime)
		return msgPLI_WANTFILE(file);

	sendPacket(CString() >> (char)PLO_FILESENDFAILED << file);
	return true;
}

bool TPlayer::msgPLI_ADJACENTLEVEL(CString& pPacket)
{
	time_t modTime = pPacket.readGUInt5();
	CString levelName = pPacket.readString("");
	CString packet;
	TLevel* adjacentLevel = TLevel::findLevel(levelName, server);

	if (adjacentLevel == 0)
		return true;

	bool alreadyVisited = false;
	for (std::vector<SCachedLevel*>::const_iterator i = cachedLevels.begin(); i != cachedLevels.end(); ++i)
	{
		SCachedLevel* cl = *i;
		if (cl->level == adjacentLevel)
		{
			alreadyVisited = true;
			break;
		}
	}

	// Send the new level.
	TGMap* gmap = server->getLevelGMap(adjacentLevel);
	sendPacket(CString() >> (char)PLO_LEVELNAME << /*((gmap != 0) ? gmap->getMapName() : */adjacentLevel->getLevelName()/*)*/);
	//sendPacket(CString() >> (char)PLO_LEVELNAME << ((gmap != 0) ? gmap->getMapName() : adjacentLevel->getLevelName()));
	if ((modTime != adjacentLevel->getModTime()) || alreadyVisited == false)
	{
		sendPacket(CString() >> (char)PLO_RAWDATA >> (int)(1+(64*64*2)+1));
		sendPacket(CString() << adjacentLevel->getBoardPacket());
		sendCompress();

		// Send links, board changes, chests, and modification time.
		sendPacket(CString() >> (char)PLO_LEVELMODTIME >> (long long)adjacentLevel->getModTime());
		sendPacket(CString() << adjacentLevel->getLinksPacket());
		sendPacket(CString() << adjacentLevel->getChestPacket(this));
	}
	sendPacket(CString() << adjacentLevel->getBoardChangesPacket(modTime));

	// Set our old level back to normal.
	//sendPacket(CString() >> (char)PLO_LEVELNAME << /*((gmap != 0) ? gmap->getMapName() : */level->getLevelName()/*)*/);
	sendPacket(CString() >> (char)PLO_LEVELNAME << ((gmap != 0) ? gmap->getMapName() : level->getLevelName()));
	if (level->getPlayer(0) == this)
		sendPacket(CString() >> (char)PLO_ISLEADER);

	return true;
}

bool TPlayer::msgPLI_LANGUAGE(CString& pPacket)
{
	language = pPacket.readString("");
	return true;
}

bool TPlayer::msgPLI_TRIGGERACTION(CString& pPacket)
{
	int npcId = pPacket.readGInt();
	float x = (float)pPacket.readGChar() / 2.0f;
	float y = (float)pPacket.readGChar() / 2.0f;
	CString action = pPacket.readString("");

	// We don't have an NPCserver, so, for now, just pass it along.
	CString packet;
	packet >> (char)PLO_TRIGGERACTION >> (short)this->id << pPacket.text()+1;
	server->sendPacketToLevel(packet, this->level, this);
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
	CString packet = pPacket.readString("");
	printf( "%s\n", packet.text() );
	for (int i = 0; i < packet.length(); ++i) printf( "%02x ", (unsigned char)packet[i] );
	printf( "\n" );
	return true;
}
