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

TServer::~TServer()
{
	for (std::vector<TPlayer*>::iterator i = playerList.begin(); i != playerList.end(); )
	{
		delete *i;
		i = playerList.erase(i);
	}

	for (std::vector<TNPC*>::iterator i = npcList.begin(); i != npcList.end(); )
	{
		delete *i;
		i = npcList.erase(i);
	}

	for (std::vector<TLevel*>::iterator i = levelList.begin(); i != levelList.end(); )
	{
		delete *i;
		i = levelList.erase(i);
	}

	for (std::vector<TMap*>::iterator i = mapList.begin(); i != mapList.end(); )
	{
		delete *i;
		i = mapList.erase(i);
	}

	for (std::vector<TWeapon*>::iterator i = weaponList.begin(); i != weaponList.end(); )
	{
		delete *i;
		i = weaponList.erase(i);
	}
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

	// Load gmaps.
	std::vector<CString> gmaps = settings.getStr("gmaps").guntokenize().tokenize("\n");
	for (std::vector<CString>::iterator i = gmaps.begin(); i != gmaps.end(); ++i)
	{
		// Check for blank lines.
		if (*i == "\r") continue;

		// Load the gmap.
		TMap* gmap = new TMap(MAPTYPE_GMAP);
		if (gmap->load(CString() << *i << ".gmap", this) == false)
		{
			serverlog.out(CString() << "[Error] Could not load " << *i << ".gmap" << "\n");
			delete gmap;
			continue;
		}

		mapList.push_back(gmap);
	}

	// Load bigmaps.
	std::vector<CString> bigmaps = settings.getStr("maps").guntokenize().tokenize("\n");
	for (std::vector<CString>::iterator i = bigmaps.begin(); i != bigmaps.end(); ++i)
	{
		// Check for blank lines.
		if (*i == "\r") continue;

		// Load the bigmap.
		TMap* bigmap = new TMap(MAPTYPE_BIGMAP);
		if (bigmap->load(*i, this) == false)
		{
			serverlog.out(CString() << "[Error] Could not load " << *i << "\n");
			delete bigmap;
			continue;
		}

		mapList.push_back(bigmap);
	}

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

			// Get rid of the player now.
			playerIds[player->getId()] = 0;
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

TMap* TServer::getLevelMap(const TLevel* pLevel) const
{
	if (pLevel == 0) return 0;
	for (std::vector<TMap*>::const_iterator i = mapList.begin(); i != mapList.end(); ++i)
	{
		TMap* pMap = *i;
		if (pMap->isLevelOnMap(pLevel->getLevelName()))
			return pMap;
	}
	return 0;
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

void TServer::sendPacketToLevel(CString pPacket, TMap* pMap, TPlayer* pPlayer, bool sendToSelf) const
{
	for (std::vector<TPlayer *>::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getType() != CLIENTTYPE_CLIENT) continue;
		if ((*i) == pPlayer)
		{
			if (sendToSelf) pPlayer->sendPacket(pPacket);
			continue;
		}
		if ((*i)->getMap() == pMap)
		{
			int ogmap[2], sgmap[2];
			switch (pMap->getType())
			{
				case MAPTYPE_GMAP:
					ogmap[0] = (*i)->getProp(PLPROP_GMAPLEVELX).readGUChar();
					ogmap[1] = (*i)->getProp(PLPROP_GMAPLEVELY).readGUChar();
					sgmap[0] = pPlayer->getProp(PLPROP_GMAPLEVELX).readGUChar();
					sgmap[1] = pPlayer->getProp(PLPROP_GMAPLEVELY).readGUChar();
					break;
				
				default:
				case MAPTYPE_BIGMAP:
					ogmap[0] = pMap->getLevelX((*i)->getLevel()->getLevelName());
					ogmap[1] = pMap->getLevelY((*i)->getLevel()->getLevelName());
					sgmap[0] = pMap->getLevelX(pPlayer->getLevel()->getLevelName());
					sgmap[1] = pMap->getLevelY(pPlayer->getLevel()->getLevelName());
					break;
			}

			if (abs(ogmap[0] - sgmap[0]) < 2 && abs(ogmap[1] - sgmap[1]) < 2)
				(*i)->sendPacket(pPacket);
		}
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
