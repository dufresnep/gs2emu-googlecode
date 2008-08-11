#include "ICommon.h"
#include "CSocket.h"
#include "CSettings.h"
#include "CFileSystem.h"
#include "TServer.h"

extern CString homepath;

TServer::TServer(CString pName)
: name(pName), lastTimer(time(0))
{
	// Player ids 0 and 1 break things.  NPC id 0 breaks things.
	// Don't allow anything to have one of those ids.
	playerIds.resize(2);
	npcIds.resize(1);

	// This has the full path to the server directory.
	serverpath = CString() << homepath << "servers/" << name << "/";

	// Set up the log files.
	serverlog.setFilename(CString() << serverpath << "logs/serverlog.txt");
	rclog.setFilename(CString() << serverpath << "logs/rclog.txt");

	serverlist.setServer(this);
	filesystem.setServer(this);
}

int TServer::init()
{
	// Load Settings
	settings.setSeparator("=");
	settings.loadFile(CString() << serverpath << "config/serveroptions.txt");
	if (!settings.isOpened())
	{
		serverlog.out("[Error] Could not open config/serveroptions.txt\n");
		return ERR_SETTINGS;
	}

	// Load file system.
	filesystem.init();

	// Initialize the player socket.
	playerSock.setType(SOCKET_TYPE_SERVER);
	playerSock.setProtocol(SOCKET_PROTOCOL_TCP);
	playerSock.setOptions(SOCKET_OPTION_NONBLOCKING);
	playerSock.setDescription("playerSock");

	// Start listening on the player socket.
	if (playerSock.init("", settings.getStr("serverport")))
	{
		serverlog.out("[Error] Could not initialize listening socket.\n");
		return ERR_LISTEN;
	}
	if (playerSock.connect())
	{
		serverlog.out("[Error] Could not connect listening socket.\n");
		return ERR_LISTEN;
	}

	// Connect to the serverlist.
	if (!serverlist.init(settings.getStr("listip"), settings.getStr("listport")))
	{
		serverlog.out("[Error] Cound not initialize serverlist socket.\n");
		return ERR_LISTEN;
	}
	serverlist.connectServer();

	return 0;
}

bool TServer::doMain()
{
	// Serverlist-Main -- Reconnect if Disconnected
	if (!serverlist.main())
		serverlist.connectServer();

	// Serverlist Connection -> Connected
	acceptSock(playerSock);

	// Iterate Players
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end();)
	{
		TPlayer *player = (TPlayer*)*i;
		if (player == 0)
			continue;

		if (!player->doMain())
		{
			// Remove the player from the serverlist.
			serverlist.remPlayer(player->getProp(PLPROP_ACCOUNTNAME).removeI(0,1), player->getType());

			delete player;
			i = playerList.erase(i);
		}
		else ++i;
	}

	// Every second, do some events.
	if (time(0) != lastTimer) doTimedEvents();

	return true;
}

bool TServer::doTimedEvents()
{
	lastTimer = time(0);

	// Do player events.
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		TPlayer *player = (TPlayer*)*i;
		if (player == 0)
			continue;

		player->doTimedEvents();
	}

	// Do level events.
	for (std::vector<TLevel *>::iterator i = levelList.begin(); i != levelList.end(); ++i)
	{
		TLevel* level = *i;
		if (level == 0)
			continue;

		level->doTimedEvents();
	}

	return true;
}

void TServer::acceptSock(CSocket& pSocket)
{
	// Create Sock
	CSocket *newSock = pSocket.accept();
	if (newSock == 0)
		return;

	// New Player
	TPlayer *newPlayer = new TPlayer(this, newSock);
	playerList.push_back(newPlayer);

	// Assign Player Id
	for (unsigned int i = 2; i < playerIds.size(); ++i)
	{
		if (playerIds[i] == 0)
		{
			playerIds[i] = newPlayer;
			newPlayer->setId(i);
			return;
		}
	}

	newPlayer->setId(playerIds.size());
	playerIds.push_back(newPlayer);
}

TWeapon* TServer::getWeapon(const CString& name)
{
	for (std::vector<TWeapon*>::iterator i = weaponList.begin(); i != weaponList.end(); ++i)
	{
		TWeapon* weapon = *i;
		if (weapon->getName() == name)
			return weapon;
	}
	return 0;
}

TPlayer* TServer::getPlayer(const unsigned int id)
{
	if (id >= playerIds.size()) return 0;
	return playerIds[id];
}

TNPC* TServer::getNPC(const unsigned int id)
{
	if (id >= npcIds.size()) return 0;
	return npcIds[id];
}

TNPC* TServer::addNewNPC(const CString& pImage, const CString& pScript, float pX, float pY, TLevel* pLevel, bool pLevelNPC)
{
	// New Npc
	TNPC* newNPC = new TNPC(pImage, pScript, pX, pY, pLevel, pLevelNPC);
	npcList.push_back(newNPC);

	// Assign NPC Id
	for (unsigned int i = 1; i < npcIds.size(); ++i)
	{
		if (npcIds[i] == 0)
		{
			npcIds[i] = newNPC;
			newNPC->setId(i);
			return newNPC;
		}
	}

	newNPC->setId(npcIds.size());
	npcIds.push_back(newNPC);

	return newNPC;
}

/*
	Packet-Sending Functions
*/
void TServer::sendPacketToAll(CString pPacket) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
		(*i)->sendPacket(pPacket);
}

void TServer::sendPacketToAll(CString pPacket, TPlayer *pPlayer) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i) == pPlayer) continue;
		(*i)->sendPacket(pPacket);
	}
}

void TServer::sendPacketToLevel(CString pPacket, TLevel *pLevel) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getType() != CLIENTTYPE_CLIENT) continue;
		if ((*i)->getLevel() == pLevel)
			(*i)->sendPacket(pPacket);
	}
}

void TServer::sendPacketToLevel(CString pPacket, TLevel *pLevel, TPlayer *pPlayer) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i) == pPlayer || (*i)->getType() != CLIENTTYPE_CLIENT) continue;
		if ((*i)->getLevel() == pLevel)
			(*i)->sendPacket(pPacket);
	}
}

void TServer::sendPacketTo(int who, CString pPacket) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getType() == who)
			(*i)->sendPacket(pPacket);
	}
}

void TServer::sendPacketTo(int who, CString pPacket, TPlayer* pPlayer) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i) == pPlayer) continue;
		if ((*i)->getType() == who)
			(*i)->sendPacket(pPacket);
	}
}
