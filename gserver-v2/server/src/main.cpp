#include <signal.h>
#include <stdlib.h>
#include "main.h"
#include "TLevel.h"
#include "TPlayer.h"
#include "TServerList.h"
#include "CSocket.h"
#include "CLog.h"

// Function pointer for signal handling.
typedef void (*sighandler_t)(int);

const char *__itemList[] = {"greenrupee", "bluerupee", "redrupee", "bombs", "darts", "heart", "glove1", "bow", "bomb", "shield", "sword", "fullheart", "superbomb", "battleaxe", "goldensword", "mirrorshield", "glove2", "lizardshield", "lizardsword", "goldrupee", "fireball", "fireblast", "nukeshot", "joltbomb", "spinattack"};
CSettings *settings = 0;
std::vector<TPlayer *> playerIds, playerList;
bool running = true;

// Logging files.
CLog serverlog("logs/serverlog.txt");
CLog rclog("logs/rclog.txt");

// Home path of the gserver.
CString homepath;
static void getBasePath();

int main(int argc, char* argv[])
{
	// Shut down the server if we get a kill signal.
	signal(SIGINT, (sighandler_t) shutdownServer);
	signal(SIGTERM, (sighandler_t) shutdownServer);

	// Fill homepath with the home directory of the server
	getBasePath();

	// Player ids 0 and 1 break things.
	// Don't allow a player to have one of those ids.
	playerIds.resize(2);

	serverlog.out("Starting server\n");

	// Load Settings
	settings = new CSettings(CString() << homepath << "config/serveroptions.txt");
	if (!settings->isOpened())
	{
		serverlog.out("[Error] Could not open config/serveroptions.txt\n");
		return ERR_SETTINGS;
	}

	// Create Packet-Functions
	createPLFunctions();
	createSLFunctions();

	// Initialize the player socket.
	CSocket playerSock;
	playerSock.setType(SOCKET_TYPE_SERVER);
	playerSock.setProtocol(SOCKET_PROTOCOL_TCP);
	playerSock.setOptions(SOCKET_OPTION_NONBLOCKING);
	playerSock.setDescription("playerSock");

	// Start listening on the player socket.
	if (playerSock.init("", settings->getStr("serverport")))
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
	TServerList sock;
	if (!sock.init(settings->getStr("listip"), settings->getStr("listport")))
	{
		serverlog.out("[Error] Cound not initialize serverlist socket.\n");
		return ERR_LISTEN;
	}
	sock.connectServer();

	// Main Loop
	serverlog.out("Main loop\n");
	while (running)
	{
		// Serverlist-Main -- Reconnect if Disconnected
		if (!sock.main())
			sock.connectServer();

		// Serverlist Connection -> Connected
		//if (sock.getConnected())
			acceptSock(playerSock);

		// Iterate Players
		for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end();)
		{
			TPlayer *player = (TPlayer*)*i;
			if (player == 0)
				continue;

			if (!player->doMain())
			{
				delete player;
				i = playerList.erase(i);
			}
			else ++i;
		}

		// Wait
		wait(100);
	}

	// Destroy the sockets.
	//CSocket::socketSystemDestroy();

	return ERR_SUCCESS;
}

void acceptSock(CSocket& pSocket)
{
	// Create Sock
	CSocket *newSock = pSocket.accept();
	if (newSock == 0)
		return;

	// New Player
	TPlayer *newPlayer = new TPlayer(newSock);
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

/*
	Extra-Cool Functions :D
*/
CString getDataFile(const CString& pFile)
{
	return pFile;
}

int findItemId(const CString& pItemName)
{
	for (unsigned int i = 0; i < sizeof(__itemList) / sizeof(const char *); ++i)
	{
		if (__itemList[i] == pItemName)
			return i;
	}

	return -1;
}

/*
	Packet-Sending Functions
*/
void sendPacketToAll(CString pPacket)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
		(*i)->sendPacket(pPacket);
}

void sendPacketToAll(CString pPacket, TPlayer *pPlayer)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i) == pPlayer) continue;
		(*i)->sendPacket(pPacket);
	}
}

void sendPacketToLevel(CString pPacket, TLevel *pLevel)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getType() != CLIENTTYPE_CLIENT) continue;
		if ((*i)->getLevel() == pLevel)
			(*i)->sendPacket(pPacket);
	}
}

void sendPacketToLevel(CString pPacket, TLevel *pLevel, TPlayer *pPlayer)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i) == pPlayer || (*i)->getType() != CLIENTTYPE_CLIENT) continue;
		if ((*i)->getLevel() == pLevel)
			(*i)->sendPacket(pPacket);
	}
}

void sendPacketTo(int who, CString pPacket)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getType() == who)
			(*i)->sendPacket(pPacket);
	}
}

void sendPacketTo(int who, CString pPacket, TPlayer* pPlayer)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i) == pPlayer) continue;
		if ((*i)->getType() == who)
			(*i)->sendPacket(pPacket);
	}
}

void sendPacketToRC(CString pPacket)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getType() == CLIENTTYPE_RC)
			(*i)->sendPacket(pPacket);
	}
}

void getBasePath()
{
	#if defined(_WIN32) || defined(_WIN64)
	// Get the path.
	char path[ MAX_PATH ];
	GetModuleFileNameA(0, path, MAX_PATH);

	// Find the program exe and remove it from the path.
	// Assign the path to homepath.
	homepath = path;
	int pos = homepath.findl('\\');
	if (pos == -1) homepath.clear();
	else if (pos != (homepath.length() - 1))
		homepath.removeI(++pos, homepath.length());
#else
	// Get the path to the program.
	char path[260];
	memset((void*)path, 0, 260);
	readlink("/proc/self/exe", path, sizeof(path));

	// Assign the path to homepath.
	char* end = strrchr(path, '/');
	if (end != 0)
	{
		end++;
		if (end != 0) *end = '\0';
		homepath = path;
	}
#endif
}

void shutdownServer(int signal)
{
	serverlog.out("Server is now shutting down...\n");
	running = false;
}
