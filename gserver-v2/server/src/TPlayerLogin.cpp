#include <vector>
#include "ICommon.h"
#include "TPlayer.h"
#include "TAccount.h"
#include "CSocket.h"
#include "TServerList.h"

#define serverlog	server->getServerLog()
#define rclog		server->getRCLog()
extern bool __sendLogin[propscount];
extern bool __getLogin[propscount];
extern bool __getLoginRC[propscount];

/*
	TPlayer: Manage Account
*/
bool TPlayer::sendLogin()
{
	// Load Player-Account
	// TODO: We actually need to check if it fails because accounts can be banned.
	loadAccount(accountName); // We don't need to check if this fails.. because the defaults have already been loaded :)

	// Server Signature
	// 0x49 (73) is used to tell the client that more than eight
	// players will be playing.
	sendPacket(CString() >> (char)PLO_SIGNATURE >> (char)73);

	// Check if the account is already in use.
	printf("TODO: TPlayer::sendLogin, Check if account is in use.\n");

	// Player's load different than RCs.
	bool failed = false;
	if (type == CLIENTTYPE_CLIENT) failed = sendLoginClient();
	else if (type == CLIENTTYPE_RC) failed = sendLoginRC();
	if (failed) return false;

	// Exchange props with everybody on the server.
	std::vector<TPlayer*> playerList = server->getPlayerList();
	for (std::vector<TPlayer*>::iterator i = playerList.begin(); i != playerList.end(); ++i)
	{
		TPlayer* player = (TPlayer*)*i;
		if (player == this) continue;

		// Get the other player's props.
		if (player->getType() == CLIENTTYPE_CLIENT)
		{
			// Send my props to the player.
			player->sendPacket(this->getProps(__getLogin, sizeof(__getLogin)/sizeof(bool)));

			// Get their props.
			this->sendPacket(player->getProps(__getLogin, sizeof(__getLogin)/sizeof(bool)));
		}
		else if (player->getType() == CLIENTTYPE_RC)
		{
			// Send my props to the player.
			// Send the RC login props since they don't need anything else.
			player->sendPacket(this->getProps(__getLoginRC, sizeof(__getLoginRC)/sizeof(bool)));

			// Get their props.
			CString packet = player->getProps(__getLoginRC, sizeof(__getLoginRC)/sizeof(bool));
			if (packet.length() == 0) continue;

			// RCs send PLO_ADDPLAYER instead of PLO_OTHERPLPROPS.
			// getProps() writes in PLO_OTHERPLPROPS so we alter it here.
			packet[0] = PLO_ADDPLAYER + 32;
			this->sendPacket(packet);
		}
	}

	// Tell the serverlist that the player connected.
	server->getServerList().addPlayer(this);

	sendCompress();
	return true;
}

bool TPlayer::sendLoginClient()
{
	CSettings* settings = &(server->getSettings());

	// Send the player his login props.
	sendProps(__sendLogin, sizeof(__sendLogin) / sizeof(bool));

	// Send the level to the player.
	// setLevel will call sendCompress() for us.
	printf("TODO: TPlayer::sendLoginClient, Send correct level to player.\n");
	if (!setLevel(this->levelName, this->x, this->y, 0, true))
	{
		sendPacket(CString() >> (char)PLO_WARPFAILED);
		if (!setLevel(settings->getStr("unstickmelevel"), settings->getFloat("unstickmex"), settings->getFloat("unstickmey"), 0, true))
		{
			sendPacket(CString() >> (char)PLO_DISCMESSAGE << "No level available.");
			serverlog.out(CString() << "Cannot find level for " << accountName << "\n");
			return false;
		}
	}

	// Recalculate player spar deviation.
	printf("TODO: TPlayer::sendLoginClient, Recalculate sparring deviation.\n");

	// Send out what guilds should be placed in the Staff section of the playerlist.
	printf("TODO: TPlayer::sendLoginClient, Send staff guilds.\n");

	// Send out the server's available status list options.
	printf("TODO: TPlayer::sendLoginClient, Send status list values.\n");

	// TODO: Send out RPG Window greeting?

	// Send the start message to the player.
	printf("TODO: TPlayer::sendLoginClient, Send the start message to the player.\n");

	// Send the player's weapons.
	printf("TODO: TPlayer::sendLoginClient, Send weapons to player.\n");

	// Send the player's flags.
	printf("TODO: TPlayer::sendLoginClient, Send flags to the player.\n");

	// Send the server's flags to the player.
	printf("TODO: TPlayer::sendLoginClient, Send the server's flags to the player.\n");

	// Delete the bomb.  It gets automagically added by the client for
	// God knows which reason.  Bomb must be capitalized.
	sendPacket(CString() >> (char)PLO_DELNPCWEAPON << "Bomb");

	return true;
}

bool TPlayer::sendLoginRC()
{
	// If no nickname was specified, set the nickname to the account name.
	if (nickName.length() == 0)
		nickName = accountName;

	// Set the head to the server's set staff head.
	printf("TODO: TPlayer::sendLoginRC, Set the RC head to the staff head.\n");

	// Send the RC join message to the RC.
	server->sendPacketTo(CLIENTTYPE_RC, CString() >> (char)PLO_RCMESSAGE << "New RC: " << this->nickName << " (" << this->accountName << ")");
	this->sendPacket(CString() >> (char)PLO_RCMESSAGE << "Welcome to RC.");

	return true;
}
