#include "main.h"
#include "TLevel.h"
#include "TPlayer.h"
#include "TServerList.h"

const char *__itemList[] = {"greenrupee", "bluerupee", "redrupee", "bombs", "darts", "heart", "glove1", "bow", "bomb", "shield", "sword", "fullheart", "superbomb", "battleaxe", "goldensword", "mirrorshield", "glove2", "lizardshield", "lizardsword", "goldrupee", "fireball", "fireblast", "nukeshot", "joltbomb", "spinattack"};
CSettings *settings = NULL;
std::vector<TPlayer *> playerIds, playerList;

int main(int argc, char *argv[])
{
	// Definitions
	CSocket playerSock;
	TServerList sock;
	playerIds.resize(2);

	// Load Settings
	settings = new CSettings("serveroptions.txt");
	if (!settings->isOpened())
		return ERR_SETTINGS;

	// Initiate Sockets
	if (CSocket::sockStart() != 0)
		return ERR_SOCKETS;

	// Create Packet-Functions
	createPLFunctions();
	createSLFunctions();

	// Listen Sockets
	if (!playerSock.listenSock(settings->getInt("serverport"), 20))
		return ERR_LISTEN;

	// Set Non-Blocking
	playerSock.setSync(false);
	sock.connectServer(settings->getStr("listip"), settings->getInt("listport"));

	// Main Loop
	while (true)
	{
		// Serverlist-Main -- Reconnect if Disconnected
		if (!sock.main())
			sock.reconnectServer();

		// Serverlist Connection -> Connected
		if (sock.getConnected())
			acceptSock(playerSock);

		// Iterate Players
		for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
		{
			TPlayer *player = *i;
			if (player == NULL)
				continue;

			if (!player->doMain())
			{
				delete player;
				i = playerList.erase(i)-1;
			}
		}

		// Wait
		wait(100);
	}

	return ERR_SUCCESS;
}

void acceptSock(CSocket& pSocket)
{
	// Create Sock
	CSocket *newSock = pSocket.accept();
	if (newSock == NULL)
		return;

	newSock->setSync(false);
	newSock->setNagle(false);

	// New Player
	TPlayer *newPlayer = new TPlayer(newSock);
	playerList.push_back(newPlayer);

	// Assign Player Id
	for (unsigned int i = 2; i < playerIds.size(); ++i)
	{
		if (playerIds[i] == NULL)
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
		if ((*i) == pPlayer)
			continue;

		(*i)->sendPacket(pPacket);
	}
}

void sendPacketToLevel(CString pPacket, TLevel *pLevel)
{
	for (std::vector<TPlayer *>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		if ((*i)->getLevel() == pLevel)
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
