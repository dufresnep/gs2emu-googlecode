#include <vector>
#include <math.h>
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

	// If we have an NPC Server, send this to prevent clients from sending
	// npc props it modifies.
	//sendPacket(CString() >> (char)PLO_HASNPCSERVER);

	// Check if the account is already in use.
	{
		std::vector<TPlayer*>* playerList = server->getPlayerList();
		for (std::vector<TPlayer*>::iterator i = playerList->begin(); i != playerList->end(); ++i)
		{
			TPlayer* player = *i;
			CString oacc = player->getProp(PLPROP_ACCOUNTNAME).subString(1);
			unsigned short oid = player->getProp(PLPROP_ID).readGUShort();
			if (oacc == accountName && player->getType() == type && oid != id)
			{
				if ((int)difftime(time(0), player->getLastData()) > 30)
				{
					player->sendPacket(CString() >> (char)PLO_DISCMESSAGE << "Someone else has logged into your account.");
					player->disconnect();
				}
				else
				{
					sendPacket(CString() >> (char)PLO_DISCMESSAGE << "Account is already in use.");
					return false;
				}
			}
		}
	}

	// Player's load different than RCs.
	bool succeeded = false;
	if (isClient()) succeeded = sendLoginClient();
	else if (isRC()) succeeded = sendLoginRC();
	if (succeeded == false) return false;

	// Exchange props with everybody on the server.
	{
		// RC props are sent in a "special" way.  As in retarded.
		CString myRCProps;
		myRCProps >> (char)PLO_ADDPLAYER >> (short)id
			>> (char)accountName.length() << accountName
			>> (char)PLPROP_CURLEVEL << getProp(PLPROP_CURLEVEL)
			>> (char)PLPROP_NICKNAME << getProp(PLPROP_NICKNAME)
			>> (char)PLPROP_HEADGIF << getProp(PLPROP_HEADGIF)
			>> (char)PLPROP_BODYIMG << getProp(PLPROP_BODYIMG);

		std::vector<TPlayer*>* playerList = server->getPlayerList();
		for (std::vector<TPlayer*>::iterator i = playerList->begin(); i != playerList->end(); ++i)
		{
			TPlayer* player = (TPlayer*)*i;
			if (player == this) continue;

			// Send the other player my props.
			if (player->isClient())
				player->sendPacket(this->getProps(__getLogin, sizeof(__getLogin)/sizeof(bool)));
			else
				player->sendPacket(myRCProps);

			// Get my props now.
			if (isClient())
				this->sendPacket(player->getProps(__getLogin, sizeof(__getLogin)/sizeof(bool)));
			else
			{
				// Get the other player's RC props.
				CString otherRCProps;
				otherRCProps >> (char)PLO_ADDPLAYER >> (short)player->getId()
					>> (char)player->getAccountName().length() << player->getAccountName()
					>> (char)PLPROP_CURLEVEL << player->getProp(PLPROP_CURLEVEL)
					>> (char)PLPROP_NICKNAME << player->getProp(PLPROP_NICKNAME)
					>> (char)PLPROP_HEADGIF << player->getProp(PLPROP_HEADGIF)
					>> (char)PLPROP_BODYIMG << player->getProp(PLPROP_BODYIMG);
				this->sendPacket(otherRCProps);
			}
		}
	}

	// Tell the serverlist that the player connected.
	server->getServerList()->addPlayer(this);

	// Set loaded to true so our account is saved when we leave.
	loaded = true;

	return true;
}

bool TPlayer::sendLoginClient()
{
	CSettings* settings = server->getSettings();

	// Send the player his login props.
	sendProps(__sendLogin, sizeof(__sendLogin) / sizeof(bool));

	// Workaround for 2.19+ clients.  They don't request the map file when used with setmap.
	// So, just send them all the maps loaded into the server.
	// TODO: Client version specific instead of encryption era specific?
	if (in_codec.getGen() == ENCRYPT_GEN_4)
	{
		for (std::vector<TMap*>::iterator i = server->getMapList()->begin(); i != server->getMapList()->end(); ++i)
		{
			TMap* map = *i;
			if (map->getType() == MAPTYPE_BIGMAP)
				msgPLI_WANTFILE(CString() << map->getMapName());
		}
	}

	// Send the level to the player.
	// warp will call sendCompress() for us.
	if (warp(levelName, x, y) == false)
	{
		sendPacket(CString() >> (char)PLO_DISCMESSAGE << "No level available.");
		serverlog.out(CString() << "Cannot find level for " << accountName << "\n");
		return false;
	}

	// Recalculate player spar deviation.
	{
		// c = sqrt( (350*350 - 50*50) / t )
		// where t = 60 for number of rating periods for deviation to go from 50 to 350
		const float c = 44.721f;
		float t = (float)(time(0) - lastSparTime)/86400.0f; // Convert seconds to days: 60/60/24

		// Find the new deviation.
		float deviate = MIN( sqrt((oldDeviation*oldDeviation) + (c*c) * t), 350.0f );

		// Save the old rating and set the new one.
		deviation = deviate;
	}

	// Send out what guilds should be placed in the Staff section of the playerlist.
	std::vector<CString> guilds = settings->getStr("staffguilds").tokenize(",");
	CString guildPacket = CString() >> (char)PLO_STAFFGUILDS;
	for (std::vector<CString>::iterator i = guilds.begin(); i != guilds.end(); ++i)
		guildPacket << "\"" << ((CString)(*i)).trim() << "\",";
	sendPacket(guildPacket);

	// Send out the server's available status list options.
	std::vector<CString>* plicons = server->getStatusList();
	CString pliconPacket = CString() >> (char)PLO_STATUSLIST;
	for (std::vector<CString>::iterator i = plicons->begin(); i != plicons->end(); ++i)
		pliconPacket << "\"" << ((CString)(*i)).trim() << "\",";
	sendPacket(pliconPacket);

	// Send out RPG Window greeting.
	sendPacket(CString() >> (char)PLO_RPGWINDOW << "\"Welcome to " << settings->getStr("name") << ".\",\"Graal Reborn GServer programmed by Joey and Nalin.\"" );

	// Send the start message to the player.
	sendPacket(CString() >> (char)PLO_STARTMESSAGE << *(server->getServerMessage()));

	// Delete the bomb.  It gets automagically added by the client for
	// God knows which reason.  Bomb must be capitalized.
	sendPacket(CString() >> (char)PLO_NPCWEAPONDEL << "Bomb");

	// Send the player's weapons.
	for (std::vector<CString>::iterator i = weaponList.begin(); i != weaponList.end(); ++i)
	{
		TWeapon* weapon = server->getWeapon(*i);
		if (weapon == 0)
		{
			// Let's check to see if it is a default weapon.  If so, we can add it to the server now.
			int wId = TLevelItem::getItemId(*i);
			if (wId != -1)
			{
				CString defWeapPacket = CString() >> (char)PLI_WEAPONADD >> (char)0 >> (char)wId;
				defWeapPacket.readGChar();
				msgPLI_WEAPONADD(defWeapPacket);
				continue;
			}
		}
		sendPacket(weapon->getWeaponPacket());
	}

	// Send the player's flags.
	for (std::vector<CString>::iterator i = flagList.begin(); i != flagList.end(); ++i)
		sendPacket(CString() >> (char)PLO_FLAGSET << *i);

	// Send the server's flags to the player.
	std::vector<CString>* serverFlags = server->getServerFlags();
	for (std::vector<CString>::iterator i = serverFlags->begin(); i != serverFlags->end(); ++i)
		sendPacket(CString() >> (char)PLO_FLAGSET << *i);

	//sendPacket(CString() >> (char)195 >> (char)4 << "idle" << "\"SETBACKTO \"");
	//sendPacket(CString() >> (char)195 >> (char)4 << "walk" << "\"SETBACKTO \"");
	//sendPacket(CString() >> (char)195 >> (char)5 << "sword" << "\"SETBACKTO idle\"");

	return true;
}

bool TPlayer::sendLoginRC()
{
	// If no nickname was specified, set the nickname to the account name.
	if (nickName.length() == 0)
		nickName = accountName;

	// Set the head to the server's set staff head.
	headImg = server->getSettings()->getStr("staffhead", "head25.png");

	// Send the RC join message to the RC.
	// TODO: Custom RC join message (rcmessage.txt)
	sendPacket(CString() >> (char)PLO_RC_CHAT << "Welcome to RC.");
	server->sendPacketTo(CLIENTTYPE_RC, CString() >> (char)PLO_RC_CHAT << "New RC: " << nickName << " (" << accountName << ")");

	return true;
}