/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include "CBaddy.h"
#include "CDatabase.h"
#include "CIni.h"
#include "CListServer.h"
#include "CPlayer.h"
#include "CWeapon.h"
#include "main.h"
#include "CSocket.h"
#include <sys/stat.h>
#include "zlib.h"
#include <math.h>

pt2func CPlayer::msgFuncs[] = {
			&CPlayer::msgLEVELWARP, &CPlayer::msgBOARDMODIFY,
			&CPlayer::msgPLAYERPROPS, &CPlayer::msgNPCPROPS,
			&CPlayer::msgADDBOMB, &CPlayer::msgDELBOMB,
			&CPlayer::msgTOALLMSG, &CPlayer::msgADDHORSE,
			&CPlayer::msgDELHORSE, &CPlayer::msgADDARROW,
			&CPlayer::msgFIRESPY, &CPlayer::msgCARRYTHROWN,
			&CPlayer::msgADDEXTRA, &CPlayer::msgDELEXTRA,
			&CPlayer::msgCLAIMPKER, &CPlayer::msgBADDYPROPS,
			&CPlayer::msgHURTBADDY, &CPlayer::msgADDBADDY,
			&CPlayer::msgSETFLAG, &CPlayer::msgUNSETFLAG,
			&CPlayer::msgOPENCHEST, &CPlayer::msgADDNPC,
			&CPlayer::msgDELNPC, &CPlayer::msgWANTFILE,
			&CPlayer::msgNPCWEAPONIMG, &CPlayer::msgEMPTY25,
			&CPlayer::msgHURTPLAYER,&CPlayer::msgEXPLOSION,
			&CPlayer::msgPRIVMESSAGE,&CPlayer::msgDELNPCWEAPON,
			&CPlayer::msgMODLEVELWARP,&CPlayer::msgPACKCOUNT,
			&CPlayer::msgTAKEEXTRA,&CPlayer::msgADDWEAPON2,
			&CPlayer::msgUPDATEFILE,&CPlayer::msgOUTTERMAP,
			&CPlayer::msgEMPTY36,&CPlayer::msgLANGUAGE,
			&CPlayer::msgTRIGGERACTION,&CPlayer::msgMAPINFO,
			&CPlayer::msgCSHOOT,&CPlayer::msgEMPTY41,
			&CPlayer::msgEMPTY42,&CPlayer::msgEMPTY43,
			&CPlayer::msgSLISTPROCESSES,&CPlayer::msgEMPTY45,
			&CPlayer::msgEMPTY46,&CPlayer::msgEMPTY47,
			&CPlayer::msgEMPTY48,&CPlayer::msgEMPTY49,
			&CPlayer::msgEMPTY50,&CPlayer::msgSWANTSOPTIONS,
			&CPlayer::msgSSETOPTIONS,&CPlayer::msgWANTRCFOLDERS,
			&CPlayer::msgSETRCFOLDERS,&CPlayer::msgSETRESPAWN,
			&CPlayer::msgSETHORSELIFE,&CPlayer::msgSETAPINCRTIME,
			&CPlayer::msgSETBADDYRESP,&CPlayer::msgWANTPLPROPS,
			&CPlayer::msgSETPLPROPS,&CPlayer::msgDISPLAYER,
			&CPlayer::msgUPDLEVELS,&CPlayer::msgADMINMSG,
			&CPlayer::msgPRIVADMINMSG,&CPlayer::msgLISTRCS,
			&CPlayer::msgDISCONNECTRC,&CPlayer::msgAPPLYREASON,
			&CPlayer::msgLISTSFLAGS,&CPlayer::msgSETSFLAGS,
			&CPlayer::msgDADDACCOUNT,&CPlayer::msgDDELACCOUNT,
			&CPlayer::msgDWANTACCLIST,&CPlayer::msgDWANTPLPROPS,
			&CPlayer::msgDWANTACCPLPROPS,&CPlayer::msgDRESETPLPROPS,
			&CPlayer::msgDSETACCPLPROPS,&CPlayer::msgDWANTACCOUNT,
			&CPlayer::msgDSETACCOUNT,&CPlayer::msgDRCCHAT,
			&CPlayer::msgDGETPROFILE,&CPlayer::msgDSETPROFILE,
			&CPlayer::msgWARPPLAYER,&CPlayer::msgDWANTRIGHTS,
			&CPlayer::msgDSETRIGHTS,&CPlayer::msgDWANTCOM,
			&CPlayer::msgDSETCOM,&CPlayer::msgDEDITBAN,
			&CPlayer::msgDSETBAN,&CPlayer::msgDWANTFTP,
			&CPlayer::msgDCHANGEFTP, &CPlayer::msgDENDFTP,
			&CPlayer::msgDFILEFTPDOWN, &CPlayer::msgDFILEFTPUP,
			NULL, NULL, &CPlayer::msgDFILEFTPMOV,
			&CPlayer::msgDFILEFTPDEL, &CPlayer::msgDFILEFTPREN,
		};

//Props that will be sent when the player logs in
bool sendInit[] =
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
};

/*
//Props that will NOT be forwarded because theyre sent p2p through UDP
//(ONLY IF THE PLAYER HAS ENABLED UDP)

bool udpDontForward[] = {
	false, false, false, false, false, false,
	false, false, false, false, true, false,
	false, false, false, true, true, true,
	true, false, false, false, false, false,
	false, false, false, false, false, false,
	false, false, false, false, false, false,
	false, false, false, false, false, false,
	false,false,false,false,false,false,
	false,false,false,false,false,false
};*/
//UDP DONT WORK YET
bool udpDontForward[propscount + 3] =
{
	false, false, false, false, false, false, // 0-5
	false, false, false, false, false, false, // 6-11
	false, false, false, false, false, false, // 12-17
	false, false, false, false, false, false, // 18-23
	false, false, false, false, false, false, // 24-29
	false, false, false, false, false, false, // 30-35
	false, false, false, false, false, false, // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	false, false, false, false, false, false, // 54-59
	false, false, false, false, false, false, // 60-65
	false, false, false, false, false, false, // 66-71
	false, false, false, false, false, false, // 72-77
};
//Props that will be forwarded to people in the same level as you
bool forwardLocal[propscount + 3] =
{
	false, false, true,  false, false, false, // 0-5
	false, false, true,  true,  true,  true,  // 6-11
	true,  true,  false, true,  true,  true,  // 12-17
	true,  true,  true,  true,  false, false, // 18-23
	false, true,  false, false, false, false, // 24-29
	true,  false, true,  false, true,  true,  // 30-35
	true,  true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	true,  true,  true,  true,  true,  true,  // 54-59
	true,  true,  true,  true,  true,  true,  // 60-65
	true,  true,  true,  true,  true,  true,  // 66-71
	true,  true,  true,  false, false, false, // 72-77
};

//Other players props that will be forwarded to you when you log in
bool sendOthersInit[propscount + 3] =
{
	true,  false, false, false, false, false, // 0-5
	false, false, true,  true,  true,  true,  // 6-11
	true,  true,  false, true,  true,  true,  // 12-17
	true,  true,  true,  true,  false, false, // 18-23
	true,  false, false, false, false, false, // 24-29
	true,  true,  true,  false, true,  true,  // 30-35
	true,  true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, true, // 48-53
	false, false, false, false, false, false, // 54-59
	false, false, false, false, false, false, // 60-65
	false, false, false, false, false, false, // 66-71
	false, false, false, false, false, false, // 72-77
};

bool sendRcProps[propscount] =
{
	false, true,  true,  true,  true,  true,  // 0-5
	true,  true,  true,  true,  true,  true,  // 6-11
	false, true,  false, true,  true,  false, // 12-17
	true,  false, true,  true,  true,  true,  // 18-23 (19 Status)
	false, false, true,  true,  true,  true,  // 24-29
	true,  false, true,  false, false, true,  // 30-35
	true,  false, false, false, false, false, // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false  // 48-53
};

const char* itemNames[] = {
	"greenrupee","bluerupee","redrupee","bombs","darts","heart","glove1",
	"bow","bomb","shield","sword","fullheart","superbomb","battleaxe","goldensword",
	"mirrorshield", "glove2","lizardshield","lizardsword","goldrupee","fireball",
	"fireblast","nukeshot","joltbomb", "spinattack"
};

#define itemcount 25

CPlayer::CPlayer(CSocket* pSocket)
: additionalFlags(0), carrySprite(-1), failAttempts(0), key(0), iterator(0),
id(-1), packCount(0), udpPort(0), lastNick(0), statusMsg(0),
firstPacket(true), firstLevel(true), deleteMe(false), allowBomb(false),
level(NOLEVEL), playerSock(pSocket)
{
	lastData = lastChat = lastSave = loginTime = getSysTime();
	lastMessage = lastMovement = getTime();

	iterator = 0x04A80B38;
}

CPlayer::~CPlayer()
{
	if (id >= 0)
	{
		errorOut( "serverlog.txt", CString() << "Leaving " << ((type == CLIENTPLAYER) ? "player" : "RC") << ": " << accountName.text(), true );

		//remove me from level
		leaveLevel();

		//remove from playerlist
		playerList.remove(this);
		//make player id available
		playerIds.replace(id, NULL);

		//tell others players that i have left the game
		CPacket packet;
		packet << (char)OTHERPLPROPS << (short)id << (char)PCONNECTED;
		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer* other = (CPlayer*)playerList[i];
			if(other->type == CLIENTPLAYER)
				other->sendPacket(packet);
			else if(other->type == CLIENTRC)
				other->sendPacket(CPacket() << (char)SDELPLAYER << (short)id);
		}

		// Save any pending weapons.
		for ( int i = 0; i < weaponSend.count(); ++i )
			if ( myWeapons.find( weaponSend[i] ) == -1 )
				myWeapons.add( weaponSend[i] );

		if (type == CLIENTPLAYER)
		{
			saveAccount();
		}
		else if (type == CLIENTRC)
		{
			sendRCPacket(CPacket() << (char)DRCLOG << "RC Disconnected: " << accountName);
		}

		ListServer_Send(CPacket() << (char)SLSPLAYERREM << (char)type << accountName << "\n");
	}
	else newPlayers.remove(this);
	delete playerSock;

	//free entered levels
	for (int i = 0; i < enteredLevels.count(); i++)
	{
		CEnteredLevel* lvl = (CEnteredLevel*)enteredLevels[i];
		delete lvl;
	}
	for(int i = 0; i < fileList.count(); i++)
	{
		COutFile* file = (COutFile*)fileList[i];
		delete file;
	}
}

void CPlayer::main()
{
	char packets[65536];

	// Get data.
	if ( playerSock->getData() == -1 )
	{
		//errorOut("errorlog.txt", CString() << "Client " << accountName << " disconnected." );
		deleteMe = true;
		return;
	}

	// Add obtained data to buffer.
	packetBuffer.writeBytes( playerSock->getBuffer().text(), playerSock->getBuffer().length() );
	playerSock->getBuffer().clear();

	if (packetBuffer.length() >= 128*1024)
	{
		errorOut("errorlog.txt", CString() << "Client " << accountName << " has sent to much data (input buffer >=128k)");
		sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent to much data (>=128k in the input buffer)");
		deleteMe = true;
		compressAndSend();
		return;
	}

	while (packetBuffer.length() >= 2)
	{
		lastData = time(NULL);
		int error, cLen = sizeof(packets);
		unsigned int len = (((unsigned int)(unsigned char)packetBuffer[0]) << 8) + (unsigned int)(unsigned char)packetBuffer[1];

		// Packet might not be fully in yet.
		if ( len > (unsigned int)packetBuffer.length() - 2 )
		{
			//errorOut( "debuglog.txt", CString() << accountName << ": Packet not fully in yet?", true );
			break;
		}

		if ((error = uncompress((Bytef*)packets,(uLongf*)&cLen,(const Bytef*)packetBuffer.text()+2, len)) == Z_OK)
		{
			CPacket lines;
			if(cLen <= 0)
			{
				errorOut("errorlog.txt", CString() << "Client " << accountName << " sent an empty package.");
				sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent an empty data package.");
				deleteMe = true;
				return;
			}

			lines.writeBytes(packets, cLen);
			while(lines.bytesLeft())
			{
				packCount++;

				if(firstPacket)
				{
					processLogin(CPacket() << lines.readString("\n"));
				}
				else
				{
					//parsePacket(CPacket() << lines.readString("\n"));
					int id = lines.readByte1();
					lines.setRead(lines.getRead() - 1);

					if (type == CLIENTRC && id == 93)
					{
						parsePacket(CPacket() << lines.copy(lines.getRead()));
						lines.setRead(lines.length());
					}
					else
					{
						parsePacket(CPacket() << lines.readString("\n"));
					}
				}

				if(deleteMe)
					return;
			}
			packetBuffer.remove(0, len+2);
		}
		else
		{
			errorOut("errorlog.txt", CString() << "Decompression error for player " << accountName);
			deleteMe = true;
			sendPacket(CPacket() << (char)DISMESSAGE << "Decompression error\n");
			compressAndSend();
			return;
		}
	}

	sendWeapons();
	sendFiles();
	compressAndSend();
}

void CPlayer::processLogin(CPacket& pPacket)
{
	if(pPacket.length() <= 0)
		return;

	CPacket packet;
	firstPacket = false;
	key = 0;
	type = pPacket.readByte1();
	if (type == CLIENTPLAYER)
		key = pPacket.readByte1();
	version = pPacket.readChars(7);
	pPacket.readByte1();	//skip byte...?

	#ifdef GSERV22
	mcodec.reset(key);
	#endif

	accountName = pPacket.readChars(pPacket.readByte1());
	CString password = pPacket.readChars(pPacket.readByte1());

	if ( detailedconsole )
	{
		errorOut( "serverlog.txt", CString() <<
			"Account login details:\n" <<
			"Key: " << toString(key) << "\n" <<
			"Version: " << version << "\n" <<
			"Type: " << ((type == CLIENTRC) ? "RC" : "Player") << "\n" <<
			"Account: " << accountName,
			true );
	}
	else
		errorOut( "serverlog.txt", CString() << "New " << ((type == CLIENTRC) ? "RC" : "player") << ": " << accountName, true );

	if (playerList.count() >= maxPlayers)
	{
		errorOut("serverlog.txt", "Player limit reached");
		sendPacket(CPacket() << (char)DISMESSAGE << "This server has reached its player limit.");
		deleteMe = true;
		return;
	}

	CString ip = playerSock->tcpIp();
	if (isIpBanned(ip))
	{
		errorOut("serverlog.txt", CString() << accountName << " is ip banned");
		sendPacket(CPacket() << (char)DISMESSAGE << "Your ip has been banned from this server.");
		deleteMe = true;
		return;
	}

	//Send a verification request to the server for account name and password
	if ( listServerFields[5] == "localhost" )
	{
		for (int i = 0; i < newPlayers.count(); i++)
		{
			CPlayer *player = (CPlayer *)newPlayers[i];

			if (player->accountName == accountName)
			{
				player->sendAccount();
			}
		}
		errorOut("serverlog.txt", "[DEBUG_LOCALHOSTMODE] Password Check Bypassed.", true);
	}
	else
	{
		if (lsConnected)
			ListServer_Send(CPacket() << (char)SLSACCOUNT << (char)accountName.length() << accountName << (char)password.length() << password << "\n");
		else
		{
			errorOut("errorlog.txt", "List server is unavailable");
			sendPacket(CPacket() << (char)DISMESSAGE << "Unable to contact account server.");
			deleteMe = true;
		}
	}
}

void CPlayer::sendAccount()
{
	//CPacket packet;
	if (!loadAccount())  //Do login check
	{
		errorOut("serverlog.txt", CString() << accountName << " is not allowed on server");
		deleteMe = true;
		lastIp = inet_addr(playerSock->tcpIp());
		return;
	}
	loginTime = time(NULL);

	//Server signature. Allows more than 8 players
	sendPacket(CPacket() << (char)UNLIMITEDSIG << (char)73);

	if (!loadOnly)
	{
		CPlayer* player = findPlayerId(accountName);
		if(player != NULL)
		{
			if(type == player->type)
			{
				if (time(NULL) - player->lastData > 30)
				{
					errorOut("serverlog.txt", CString() << "No data for 30 secs from " << player->accountName);
					player->sendPacket(CPacket() << (char)DISMESSAGE << "Someone else has logged into your account.");
					player->deleteMe = true;
				}
				else
				{
					errorOut("serverlog.txt", accountName << " account is already in use.");
					sendPacket(CPacket() << (char) DISMESSAGE << "This account is already in use.");
					deleteMe = true;
					return;
				}
			}
		}
	}

	// Give the player an id.
	id = createPlayerId(this);

	// Recalculate spar deviation.
	if ( type == CLIENTPLAYER )
	{
		// c = sqrt( (350*350 - 50*50) / t )
		// where t = 60 for number of rating periods for deviation to go from 50 to 350
		const float c = 44.721f;
		float t = (float)(getSysTime() - lastSparTime)/86400.0f; // Convert seconds to days: 60/60/24

		// Find the new deviation.
		float deviate = MIN( sqrt((oldDeviation*oldDeviation) + (c*c) * t), 350.0f );

		// Save the old rating and set the new one.
		deviation = deviate;
	}

	// Send out the player's login props.
	if(type == CLIENTPLAYER)
	{
		saveAccount();
		CPacket staff;
		staff << (char)SSTAFFGUILDS;
		for(int i = 0; i < staffGuilds.count(); i++)
			staff << "\"" << staffGuilds[i] << "\",";
		sendPacket(staff);

		CPacket statusl;
		statusl << (char)SSTATUSLIST;
		for(int i = 0; i < statusList.count(); i++)
			statusl << "\"" << statusList[i] << "\",";
		sendPacket(statusl);

		CPacket loginProps;
		loginProps << (char)SPLAYERPROPS;
		for (int i = 0; i < propscount; i++)
		{
			if (sendInit[i])
				loginProps << (char)i << getProp(i);
		}
		sendPacket(loginProps);

		sendPacket( CPacket() << (char)SRPGWINDOW << "\"Welcome to " << listServerFields[0] << "\",\"Graal Reborn - Coded by 39ster, Agret, Joey and Nalin\"" );
	}

	if (type == CLIENTPLAYER)
	{
		sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
		sendPacket(CPacket() << (char)LEVELNAME << levelName);

		if (!sendLevel(levelName, x, y, 0))
		{
			sendPacket(CPacket() << (char)LEVELFAILED);

			// Construct and send special warp packet.
			CPacket warpPacket;
			warpPacket << (char)PLAYERWARPED;
			warpPacket.writeByte1( (char)(unstickmeX * 2) );
			warpPacket.writeByte1( (char)(unstickmeY * 2) );
			warpPacket << unstickmeLevel;
			sendPacket(warpPacket);

			sendPacket(CPacket() << (char)LEVELNAME << unstickmeLevel);
			if (!sendLevel(unstickmeLevel, unstickmeX, unstickmeY, 0))
			{
				errorOut("serverlog.txt", CString() << "Could not find a level for " << accountName);
				deleteMe = true;
			}
		}

		sendPacket(CPacket() << (char)STARTMESSAGE << serverMessage);
	}
	else
	{
		// If the nickname wasn't specified, set it to the account name.
		if ( nickName.length() <= 0 )
			nickName = accountName;
		headImage = staffHead;
		for(int i = 0; i < RCMessage.count(); i++)
			sendPacket(CPacket() << (char)DRCLOG << RCMessage[i]);
	}

	//Create property list for clients
	CPacket playerProps;
	playerProps << (char)OTHERPLPROPS << (short)id << (char)50 << (char)1;
	for (int i = 0; i < propscount; i ++)
	{
		if (sendOthersInit[i])
			playerProps << (char)i << getProp(i);
	}

	//create property list for rcs
	CPacket rcProps;
	rcProps << (char)SADDPLAYER << (short)id;
	rcProps << (char)accountName.length() << accountName;
	rcProps << (char)CURLEVEL << getProp(CURLEVEL);
	rcProps << (char)NICKNAME << getProp(NICKNAME);
	rcProps << (char)HEADGIF << getProp(HEADGIF);
	rcProps << (char)BODYIMG << getProp(BODYIMG);

	// Add the player now.  Do this here so the player is able to see
	// themself on RC.
	newPlayers.remove(this);
	playerList.add(this);
	lastIp = inet_addr(playerSock->tcpIp());

	// Send out the connection props to everybody.
	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];

		// Only continue if the player is a client, not an RC.
		// This let's players see themselves on RC.
		if (other == this && type == CLIENTPLAYER)
			continue;

		// If the player is a client, send the init props to other players and
		// get other player's props.
		if (type == CLIENTPLAYER)
		{
			CPacket otherProps;
			otherProps << (char)OTHERPLPROPS << (short)other->id << (char)50 << (char)1;
			for (int ii = 0; ii < propscount; ii ++)
			{
				if (sendOthersInit[ii])
					otherProps << (char) ii << other->getProp(ii);
			}
			sendPacket(otherProps);
		}

		// If the player is an RC, send the RC specific props.
		else if (type == CLIENTRC)
		{
			CPacket otherProps;
			otherProps << (char)SADDPLAYER << (short)other->id;
			otherProps << (char)other->accountName.length() << other->accountName;
			otherProps << (char)CURLEVEL << other->getProp(CURLEVEL);
			otherProps << (char)NICKNAME << other->getProp(NICKNAME);
			otherProps << (char)HEADGIF << other->getProp(HEADGIF);
			otherProps << (char)BODYIMG << other->getProp(BODYIMG);
			sendPacket(otherProps);
		}

		// If the player we are sending to is an RC, send the New RC message.
		if (other->type == CLIENTRC)
		{
			if ( other != this )
				other->sendPacket(rcProps);
			if(type == CLIENTRC)
			{
				if ( other != this )
					other->sendPacket(CPacket() << (char)DRCLOG << "New RC: " << nickName << " (" << accountName << ")");
				sendPacket(CPacket() << (char)DRCLOG << "New RC: " << other->nickName << " (" << other->accountName << ")");
			}
		}
		else if (other->type == CLIENTPLAYER)
			other->sendPacket(playerProps);
	}

	if (type == CLIENTPLAYER)
	{
		for (int i = 0; i < myWeapons.count(); i++)
			weaponSend.add(myWeapons[i]);
		myWeapons.clear();

		for (int i = 0; i < myFlags.count(); i++)
			sendPacket(CPacket() << (char)SSETFLAG << myFlags[i]);

		for (int i = 0; i < serverFlags.count(); i++)
			sendPacket(CPacket() << (char)SSETFLAG << serverFlags[i]);

		// Delete that pesky bomb.
		sendPacket(CPacket() << (char)SDELNPCWEAPON << "Bomb");
	}

	// Tell the list server that the player connected.
	ListServer_Send(CPacket() << (char)SLSPLAYERADD << (char)accountName.length() << accountName << getProp(NICKNAME) << getProp(CURLEVEL) << getProp(PLAYERX) << getProp(PLAYERY) << getProp(PALIGNMENT) << (char)type << "\n");
}

void CPlayer::parsePacket(CPacket& pPacket)
{
	if ( pPacket.length() <= 0 )
	{
		errorOut( "debuglog.txt", CString() << accountName << ": [CPlayer::parsePacket()] pPacket.length() <= 0", true );
		return;
	}
	CPacket packet;

#ifdef GSERV22
	mcodec.apply(reinterpret_cast<uint8_t*>(pPacket.text()), pPacket.length());
#endif

	//Decrypt packet
	if (type == CLIENTPLAYER)
	{
		iterator *= 0x8088405;
		iterator += key;
		int pos = ((iterator & 0x0FFFF) % pPacket.length())+1;
		packet.writeBytes(pPacket.text(), pos-1);
		packet.writeBytes(pPacket.text()+pos, pPacket.length()-pos);
	}
	else
		packet = pPacket;

	int messageId = packet.readByte1();

	// If the player is not yet fully created, only allow LANGUAGE, MAPINFO,
	// and PLAYERPROP packets through.
	if(id < 0 && messageId != 37 && messageId != 39 && messageId != 2)
		return;

	if (id == -1 && type == CLIENTPLAYER)
		return;

//	if(messageId != BADDYPROPS && messageId != NPCPROPS && showConsolePackets)
//		printf("NEW PACKET: %i: %s\n", messageId, packet.text()+1);

	if ( messageId >= 0 && messageId < clientpackages )
	{
		if ( msgFuncs[messageId] != 0 )
			(*this.*msgFuncs[messageId])(packet);
	}
	else
	{
		deleteMe = true;
		errorOut( "errorlog.txt", CString() << accountName << " sent an incorrect message id [" << toString(messageId) << "]\r\nPacket: " << pPacket.text() << "\r\nMPacket: " << packet.text(), true );
		sendPacket( CPacket() << (char)DISMESSAGE << "You sent an incorrect packet" );
		return;
	}
}


void CPlayer::sendPacket(CPacket& pPacket)
{
	// If the player doesn't have a socket, they have been loaded
	// by an RC for offline editing.
	if ( playerSock == 0 )
		return;

	// Store the packet in outBuff adding a newline if one doesn't exist.
	outBuff << pPacket;
	if ( pPacket[ pPacket.length() - 1 ] != '\n' )
		outBuff << "\n";

	if (outBuff.length() > 4*1024)
		compressAndSend();
}

void CPlayer::compressAndSend()
{
	char buffer[65536];
	unsigned long clen = sizeof(buffer);
	if(outBuff.length() <= 0)
		return;

	int error = compress((unsigned char*)buffer,&clen,(unsigned char*)outBuff.text(), outBuff.length());
	outBuff.clear();

	if (error != Z_OK)
	{
		errorOut("errorlog.txt", CString() << "Compression error for " << accountName);
		return;
	}

	sendBuff.writeByte((char)((clen >> 8) & 0xFF));
	sendBuff.writeByte((char)(clen & 0xFF));
	sendBuff.writeBytes(buffer, clen);

	sendOutGoing();
}

void CPlayer::sendOutGoing()
{
	int sendLength = 0;

	if(sendBuff.length() > 2048000)
	{
		sendBuff.clear();
		errorOut("errorlog.txt", CString() << "Sending is blocked for " << accountName);
		sendPacket(CPacket() << (char)DISMESSAGE << "Sending is blocked");
		deleteMe = true;
		return;
	}

	// Send data.
	while ( sendBuff.length() > 0 )
	{
		int len = 0;
		if ( (len = playerSock->sendData( sendBuff )) > 0 )
			sendBuff.remove( 0, len );
		else if ( len != 0 )
		{
			errorOut( "errorlog.txt", CString() << "Send error to " << accountName );
			deleteMe = true;
			return;
		}
		else
			break;
	}
}

void CPlayer::warp(CString& pLevel, float pX, float pY, time_t pModTime)
{
	// Don't let them warp to a level not in the folder configuration path.
	if ( noFoldersConfig == false )
	{
		if ( isValidFile( CBuffer() << getDataFile(pLevel.text()), 1 ) == false )
		{
			if ( levelName.length() > 0 )
				return;
			else
			{
				sendPacket(CPacket() << (char)DISMESSAGE << "No level available");
				errorOut("serverlog.txt", CString() << "Cannot find a level for " << accountName);
				deleteMe = true;
			}
		}
	}

	x = pX;
	y = pY;

	if (pLevel != levelName)
	{
		if (pModTime == 0) sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << pLevel);
		sendPacket(CPacket() << (char)LEVELNAME << pLevel);

		if (!sendLevel(pLevel, pX, pY, pModTime))
		{
			sendPacket(CPacket() << (char)LEVELFAILED);
			sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
			sendPacket(CPacket() << (char)LEVELNAME << levelName);

			if (!sendLevel(levelName, x, y, 0))
			{
				sendPacket(CPacket() << (char)LEVELFAILED);
				sendPacket(CPacket() << (char)PLAYERWARPED << toString((int)(unstickmeX*2)+32) << toString((int)(unstickmeX*2)+32) << unstickmeLevel);
				sendPacket(CPacket() << (char)LEVELNAME << unstickmeLevel);

				if (!sendLevel(unstickmeLevel, unstickmeX, unstickmeY, 0))
				{
					sendPacket(CPacket() << (char)DISMESSAGE << "No level available");
					errorOut("serverlog.txt", CString() << "Cannot find a level for " << accountName);
					deleteMe = true;
				}
			}
		}
	}
	else
	{
		updateProp(PLAYERX);
		updateProp(PLAYERY);
	}
}

void CPlayer::processChat(CString& pMessage)
{
	if (WordFilter.apply(this, pMessage, FILTERC_CHAT))
		updateProp(CURCHAT);

	CStringList words;
	if ((pMessage == "unstuck me" || pMessage == "unstick me") && jailLevels.find(levelName) < 0)
	{
		if (getTime() - lastMovement >= 30)
		{
			warp(unstickmeLevel, unstickmeX, unstickmeY);
			chatMsg = CString() << "Warped!";
		}
		else
			chatMsg = CString() << "Dont move for 30 seconds before doing '" << pMessage + "'!";

		updateProp(CURCHAT);
	}
	else if (pMessage.find("setnick") == 0)
	{
		if (getTime() - lastNick <= 10)
		{
			chatMsg = "Wait 10 seconds before changing your nick again!";
			updateProp(CURCHAT);
			return;
		}

		lastNick = getTime();
		CString temp = pMessage.text() + 7;
		setNick(temp, true);
	}
	else if (pMessage == "showadmins")
	{
		chatMsg = CString() << "admins: ";

		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer *player = (CPlayer *)playerList[i];
			if (player->type == CLIENTRC)
				chatMsg << (chatMsg.length() == 8 ? "" : ", ") << player->accountName;
		}

		if (chatMsg.length() == 8)
			chatMsg << "(no one)";
		updateProp(CURCHAT);
	}
	else if (pMessage.find("showguild") == 0)
	{
		CString guild = (pMessage == "showguild" ? guildName : pMessage.text() + 9);
		if (guild.length() > 0)
		{
			chatMsg = "";
			for (int i = 0; i < playerList.count(); i++)
			{
				CPlayer *player = (CPlayer *)playerList[i];
				if (player->guildName == guild)
					chatMsg << (chatMsg.length() > 0 ? ", " : "") << player->nickName.copy(0, player->nickName.find("("));
			}
			chatMsg = CString() << "members of '" << guild << "': " << (chatMsg.length() > 0 ? chatMsg : "(no one)");
		}
		else chatMsg = "(you are not in a guild)";
		updateProp(CURCHAT);
	}
	else if (pMessage == "showdeaths")
	{
		chatMsg = CString() << "deaths: " << toString(deaths);
		updateProp(CURCHAT);
	}
	else if (pMessage == "showkills")
	{
		chatMsg = CString() << "kills: " << toString(kills);
		updateProp(CURCHAT);
	}
	else if (pMessage == "showonlinetime")
	{
		chatMsg = CString() << toString(onlineSecs % 60) << "s";
		if (onlineSecs / 60 > 0) chatMsg = CString() << toString((onlineSecs / 60) % 60) << "m " << chatMsg;
		if (onlineSecs / 60 > 0) chatMsg = CString() << toString((onlineSecs / 3600) % 60) << "h " << chatMsg;
		chatMsg = CString() << "onlinetime: " << chatMsg;
		updateProp(CURCHAT);
	}
	else if (pMessage.find("toguild:") == 0)
	{
		CString pm = pMessage.text() + 8;
		pm.trimLeft();

		if (guildName.length() <= 0 || pm.length() < 0)
			return;

		int c = 0;
		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer *player = (CPlayer *)playerList[i];
			if (player->guildName == guildName && player->accountName != accountName)
			{
				player->sendPacket(CPacket() << (char)SPRIVMESSAGE << (short)id << "\"\",\"Guild message:\",\"" << pm << "\"");
				c++;
			}
		}

		chatMsg = CString() << "(" << toString(c) << " guild member" << (c <= 1 ? "" : "s") << " received your message)";
		updateProp(CURCHAT);
	}
	else if (pMessage == "update level" && hasRight(CANUPDATELEVEL))
		CLevel::updateLevel(levelName);

	words.load(pMessage.text(), " ");
	if (words.count() <= 0)
		return;
	/*
	if (words[0] == "packet")
	{
		sendPacket(CPacket() << (char)atoi(words[1].text()) << (char)73);
		chatMsg = CString() << "updated: " << words[1].text();
		updateProp(CURCHAT);
		return;
	}
	*/

	if ( words[0] == "warpto" )
	{
		if(words.count() < 2)
			return;

		CString name;
		name = levelName;

		if (words.count() == 2)
		{
			if ( !hasRight(CANWARPTOPLAYER) && !warptoforall )
			{
				chatMsg = CString() << "(not authorized to warp)";
				updateProp( CURCHAT );
				return;
			}

			CPlayer* other = findPlayerId( words[1], true );
			if ( other == NULL )
				return;
			name = other->levelName;
			x = other->x;
			y = other->y;
		}
		else if ( words.count() > 2 )
		{
			if ( !hasRight(CANWARPXY) && !warptoforall )
			{
				chatMsg = CString() << "(not authorized to warp)";
				updateProp( CURCHAT );
				return;
			}

			x = (float)atof( words[1].text() );
			y = (float)atof( words[2].text() );
			if ( words.count() > 3 )
				name = words[3];
		}
		warp( name, x, y );
	}
	else if (words[0] == "summon")
	{
		if(words.count() <= 1)
			return;

		if (!hasRight(CANWARPPLAYER))
		{
			chatMsg = CString() << "(not authorized to summon)";
			updateProp(CURCHAT);
			return;
		}

		CPlayer* other = findPlayerId(words[1], true);
		if(other != NULL)
			other->warp(levelName, x, y);
	}
	else if (words[0] == "sethead")
	{
		if (words.count() <= 1 || !setheadallowed)
			return;

		// Check if we can find the file.
		char* file = getDataFile(words[1].text());
		if ( strlen(file) == 0 )
		{
			// File not found.  Try appending extensions.
			int j = 0;
			char* ext[] = {".png", ".mng", ".gif"};
			while ( j < 3 )
			{
				file = getDataFile( (CString() << words[1] << ext[j]).text() );
				if ( strlen(file) != 0 )
				{
					words[1] << ext[j];
					break;
				}
				j++;
			}
		}

		// Try and load now.
		if ( strlen( file ) != 0 )
		{
			if ( noFoldersConfig || isValidFile( CBuffer() << file, HEADGIF ) )
			{
				headImage = words[1];
				updateProp(HEADGIF);
			}
		}
		else
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)0 << (char)words[1].length() << words[1] << "\n");
	}
	else if (words[0] == "setbody")
	{
		if (words.count() <= 1 || !setbodyallowed)
			return;

		// Check if we can find the file.
		char* file = getDataFile(words[1].text());
		if ( strlen(file) == 0 )
		{
			// File not found.  Try appending extensions.
			int j = 0;
			char* ext[] = {".png", ".mng", ".gif"};
			while ( j < 3 )
			{
				file = getDataFile( (CString() << words[1] << ext[j]).text() );
				if ( strlen(file) != 0 )
				{
					words[1] << ext[j];
					break;
				}
				j++;
			}
		}

		// Try and load now.
		if ( strlen( file ) != 0 )
		{
			if ( noFoldersConfig || isValidFile( CBuffer() << file, BODYIMG ) )
			{
				bodyImage = words[1];
				updateProp(BODYIMG);
			}
		}
		else
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)1 << (char)words[1].length() << words[1] << "\n");
	}
	else if(words[0] == "setsword")
	{
		if(words.count() <= 1 || !setswordallowed)
			return;

		// Check if we can find the file.
		char* file = getDataFile(words[1].text());
		if ( strlen(file) == 0 )
		{
			// File not found.  Try appending extensions.
			int j = 0;
			char* ext[] = {".png", ".mng", ".gif"};
			while ( j < 3 )
			{
				file = getDataFile( (CString() << words[1] << ext[j]).text() );
				if ( strlen(file) != 0 )
				{
					words[1] << ext[j];
					break;
				}
				j++;
			}
		}

		// Try and load now.
		if ( strlen( file ) != 0 )
		{
			if ( noFoldersConfig || isValidFile( CBuffer() << file, SWORDPOWER ) )
			{
				swordImage = words[1];
				updateProp(SWORDPOWER);
			}
		}
		else
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)2 << (char)words[1].length() << words[1] << "\n");
	}
	else if(words[0] == "setshield")
	{
		if(words.count() <= 1 || !setshieldallowed)
			return;

		// Check if we can find the file.
		char* file = getDataFile(words[1].text());
		if ( strlen(file) == 0 )
		{
			// File not found.  Try appending extensions.
			int j = 0;
			char* ext[] = {".png", ".mng", ".gif"};
			while ( j < 3 )
			{
				file = getDataFile( (CString() << words[1] << ext[j]).text() );
				if ( strlen(file) != 0 )
				{
					words[1] << ext[j];
					break;
				}
				j++;
			}
		}

		// Try and load now.
		if ( strlen( file ) != 0 )
		{
			if ( noFoldersConfig || isValidFile( CBuffer() << file, SHIELDPOWER ) )
			{
				shieldImage = words[1];
				updateProp(SHIELDPOWER);
			}
		}
		else
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)3 << (char)words[1].length() << words[1] << "\n");
	}

	//colour commands
	int cloth  = clothCommands.find(words[0].text());
	if (cloth >= 5) cloth = cloth - 3;
	if (cloth >= 0)
	{
		int colour = colourNames.find(words[1].text());
		if (colour >= 0)
		{
			colors[cloth] = colour;
			updateProp(PLAYERCOLORS);
		}

	}

	if(words.count() <= 2)
		return;
	if(words[0] == "update" && words[1] == "level" && hasRight(CANUPDATELEVEL))
		CLevel::updateLevel(words[2]);
}

void CPlayer::setNick(CString& pNewNick, bool pVerifyGuild)
{
	WordFilter.apply(this, pNewNick, FILTERC_NICK);

	CString tmpGuild, tmpNick;
	tmpNick = (pNewNick.length() > 200 ? pNewNick.copy(0, 200) : pNewNick);
	while (tmpNick[0] == '*' || tmpNick[0] == ' ')
		tmpNick = tmpNick.copy(1, tmpNick.length()-1);

	// Guild Authentication
	int guildStart = tmpNick.find('(') + 1;
	int guildLen = tmpNick.find(')', guildStart) - guildStart;

	if (guildStart > 0)
	{
		tmpGuild = (guildLen > 0 ? tmpNick.copy(guildStart, guildLen) : "");
		tmpNick = tmpNick.copy(0, guildStart - 1);
		tmpNick.trim();

		if (pVerifyGuild)
		{
			if (!guildMemberExists(tmpGuild, accountName, tmpNick))
			{
				ListServer_Send(CPacket() << (char)SLSGUILD << (short)id << (char)accountName.length() << accountName << (char)tmpNick.length() << tmpNick << (char)tmpGuild.length() << tmpGuild << "\n");
				tmpGuild = "";
			}
		}
	}

	if (tmpNick == accountName)
		tmpNick.insert(0, "*");
	if (tmpNick.length() < 1)
		tmpNick = "unknown";
	if (tmpGuild.length() > 0)
		tmpNick << " (" << tmpGuild << ")";

	nickName  = tmpNick;
	guildName = tmpGuild;

	ListServer_Send(CPacket() << (char)SLSNICKNAME << (char)accountName.length() << accountName << getProp(NICKNAME) << "\n");
	sendPacket(CPacket() << (char)SPLAYERPROPS << (char)NICKNAME << getProp(NICKNAME));

	// When RC's log in, they will send their nickname.  When that happens, they still have
	// an ID of -1.  Don't send information to any clients in that case.
	if ( id != -1 )
	{
		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer*other = (CPlayer*)playerList[i];
			if (other != this || other->type == CLIENTRC)
				other->sendPacket(CPacket() << (char)OTHERPLPROPS << (short)id << (char)NICKNAME << getProp(NICKNAME));
		}
	}
}


void CPlayer::setAccPropsRc(CPacket& pPacket, CPlayer* rc)
{
	CPacket props, temp;
	bool hadBomb = false;

	//Skip playerworld
	pPacket.readChars(pPacket.readByte1());
	int len = (unsigned char)pPacket.readByte1();
	props << pPacket.readChars(len);

	CPacket pProps = setProps( props, true, rc );
	sendPacket(CPacket() <<(char)SPLAYERPROPS << pProps);

	// Clear Flags + Weapons
	for (int i = 0; i < myFlags.count(); i++)
		temp << (char)SUNSETFLAG << myFlags[i] << "\n";
	for (int i = 0; i < myWeapons.count(); i++)
	{
		temp << (char)SDELNPCWEAPON << myWeapons[i] << "\n";

		// Attempt to fix the funky client bomb capitalization issue.
		// Also fix the bomb coming back when you set the player props through RC.
		if ( myWeapons[i] == "bomb" )
		{
			temp << (char)SDELNPCWEAPON << "Bomb" << "\n";
			hadBomb = true;
		}
		if ( myWeapons[i] == "Bomb" )
			hadBomb = true;
	}

	// If we never had the bomb, don't let it come back.
	if ( hadBomb == false ) allowBomb = false;
	sendPacket(temp);

	myFlags.clear();
	for(int i = pPacket.readByte2(); i >0; i--)
	{
		int len = (unsigned char)pPacket.readByte1();
		if (len > 0)
			myFlags.add(pPacket.readChars(len));
	}
	for ( int i = 0; i < myFlags.count(); ++i )
		sendPacket( CPacket() << (char)SSETFLAG << myFlags[i] );

	myChests.clear();
	for(int i = pPacket.readByte2(); i > 0; i--)
	{
		int len = pPacket.readByte1();
		int cX = pPacket.readByte1();
		int cY = pPacket.readByte1();
		myChests.add(CString() << toString(cX) << ":" << toString(cY) << ":" << pPacket.readChars(len-2));
	}

	myWeapons.clear();
	for(int i = pPacket.readByte1(); i > 0; i--)
	{
		int len = pPacket.readByte1();
		if(len <= 0)
			continue;
		CString wpn = pPacket.readChars(len);
		weaponSend.add(wpn);
		if ( wpn == "bomb" || wpn == "Bomb" )
		{
			hadBomb = true;
			allowBomb = true;
		}
	}
	sendWeapons();

	// KILL THE BOMB DEAD
	if ( hadBomb == false )
		sendPacket(CPacket() << (char)SDELNPCWEAPON << "Bomb");

	// Re-send the level to the player to update chests and whatnot.
	if ( id != -1 )
	{
		//sendLevel( levelName, x, y, level->modTime );
		compressAndSend();
	}
}

CPacket CPlayer::getAccPropsRC()
{
	CPacket retVal;
	CPacket props;
	retVal << (char)accountName.length() << accountName;
	retVal << (char)worldName.length() << worldName;

	for(int i = 0; i < propscount; i++)
	{
		if(sendRcProps[i])
			props << (char)i << getProp(i);
	}
	retVal.writeByte(props.length()+32);
	retVal << props;
	retVal << (short)myFlags.count();
	for(int i = 0; i < myFlags.count(); i++)
		retVal << (char)myFlags[i].length() << myFlags[i];

	retVal << (short)myChests.count();
	for(int i = 0; i < myChests.count(); i++)
	{
		CStringList chest;
		chest.load(myChests[i].text(), ":");
		if(chest.count() == 3)
		{
			CPacket chestData;
			chestData << (char)atoi(chest[0].text()) << (char)atoi(chest[1].text())
				<< chest[2];
			retVal << (char)chestData.length() << chestData;
		}
	}

	retVal << (char)myWeapons.count();
	for(int i = 0; i < myWeapons.count(); i++)
		retVal << (char)myWeapons[i].length() << myWeapons[i];

	return retVal;
}
//Loads account details from database or newaccount.ini
bool CPlayer::loadAccount()
{
	//Load account
	CStringList accountData;

	//If account doesnt exist yet than use ini file
	bool createNew = loadDBAccount(accountName);

	if ( banned && !hasRight(CANCHANGESTAFFACC) )
	{
		sendPacket(CPacket() << (char)DISMESSAGE << "You have been banned from the server");
		return false;
	}

	CString ip = playerSock->tcpIp();
	if (staffList.findI(accountName) < 0 || !ip.match(adminIp.text()))
	{
		if (staffOnly)
		{
			sendPacket(CPacket() << (char)DISMESSAGE << "This server is currently restricted to staff only.");
			return false;
		}

		if (type == CLIENTRC)
		{
			errorOut("rclog.txt", CString() << "Attempted RC login by " << accountName);
			sendPacket(CPacket() << (char)DISMESSAGE << "You do not have RC rights");
			return false;
		}
	}

	// Allow normal clients to be ip locked too if the ip isn't 0.0.0.0
	if (adminIp.trim() != "0.0.0.0")
	{
		if (!ip.match(adminIp.text()))
		{
			sendPacket(CPacket() << (char)DISMESSAGE << "Your IP doesn't match the allowed IP for the account.");
			return false;
		}
	}

	if (loadOnly || !createNew)
		return loadWorldPropsIni();

	return true;
}

bool CPlayer::sendLevel(CString& pLevel, float pX, float pY, time_t pModTime)
{
	if ( type == CLIENTRC ) return false;

	CPacket packet;
	CString newLevel = pLevel;
	if(!pLevel.length())
		return false;
	int pos = newLevel.find((char)0xa7);
	if(pos >= 0)
		newLevel = newLevel.copy(pos+1, newLevel.length()-pos-1);

	CLevel* nextLevel = CLevel::openMap(newLevel);
	if (nextLevel == NOLEVEL)
		return false;

	leaveLevel();
	level = nextLevel;
	level->players.add(this);
	x = pX;
	y = pY;
	levelName = nextLevel->fileName;
	firstLevel = false;

	time_t l_time = getLeavingTime(level);
	if ( l_time <= 0 )
	{
		if ( pModTime != level->modTime )
		{
			sendPacket(CPacket() << (char)100 << (int)(1+(64*64*2)+1));
			packet.clear();
			packet.writeByte1(101);
			packet.writeBytes((char*)level->tiles, 64*64*2);
			sendPacket(packet);
		}
		compressAndSend();

		//send links
		for (int i = 0; i < level->links.count(); i ++)
		{
			CLink* link = (CLink*)level->links[i];
			sendPacket(CPacket() << (char)LEVELLINK << link->getLinkString());
		}

		for (int i = 0; i < level->signs.count(); i ++)
			sendPacket(CPacket() << (char)LEVELSIGN << level->signs[i]);

		sendPacket(CPacket() << (char)LEVELMODTIME << (long long)level->modTime);
	}

	//Send board changes
	packet.clear();
	packet << (char)LEVELBOARD;
	for (int i = 0; i < level->boardChanges.count(); i++)
	{
		CBoardChange* change = (CBoardChange*)level->boardChanges[i];
		if ( change->modifyTime >= l_time )
			packet << change->getSendData();
	}
	sendPacket(packet);

	// If you are the only person in the level, send the ISLEADER packet.
	if ( level->players.count() == 1 )
		sendPacket(CPacket() << (char)ISLEADER);

	//Send chests
	for (int i = 0; i < level->chests.count(); i ++)
	{
		CChest*chest = (CChest*)level->chests[i];
		CString chestString;
		chestString << toString(chest->x) << ":" << toString(chest->y) <<
			":" << levelName;
		if (myChests.findI(chestString) >= 0)
			sendPacket(CPacket() << (char)LEVELCHEST << (char)1 << (char)chest->x << (char)chest->y);
		else
			sendPacket(CPacket() << (char)LEVELCHEST << (char)0 << (char)chest->x << (char)chest->y << (char)chest->item << (char)chest->signIndex);
	}

	//send horses
	for ( int i = 0; i < level->horses.count(); i++ )
	{
		CHorse* horse = (CHorse*)level->horses[i];
		char dir_bush = (horse->bushes << 2) | (horse->dir & 0x03);
		sendPacket(CPacket() << (char)SADDHORSE << (char)horse->x << (char)horse->y << (char)dir_bush << horse->imageName);
	}

	//send baddies
	for ( int i = 0; i < level->baddies.count(); i++ )
	{
		CBaddy* baddy = (CBaddy*)level->baddies[i];
		if ( baddy->mode != DIE )
			sendPacket(CPacket() << (char)SBADDYPROPS << (char)baddy->id << baddy->getPropList());
	}

	//send npcs
	for ( int i = 0; i < level->npcs.count(); i++ )
	{
		CPacket npcProps;
		CNpc* npc = (CNpc*)level->npcs[i];
		npcProps << (char)SNPCPROPS << (int)npc->id << npc->getPropertyList(l_time);
		if (npcProps.length() > 3)
			sendPacket(npcProps);
	}

	// Remove the player from the enteredLevels thingy.
	for ( int i = enteredLevels.count() - 1; i >= 0; i-- )
	{
		CEnteredLevel* lvl = (CEnteredLevel*)enteredLevels[i];
		if ( lvl == 0 || lvl->level == level )
		{
			delete lvl;
			enteredLevels.remove(i);
		}
	}

	// If the level is a sparring zone and you have 100 AP, change AP to 99 and
	// the apcounter to 1.
	if ( level->sparZone == true && ap == 100 )
	{
		ap = 99;
		apCounter = 1;
		updateProp( PALIGNMENT );
	}

	//tell others i changed maps
	packet.clear();
	packet << (char)OTHERPLPROPS << (short)id << (char)CURLEVEL << getProp(CURLEVEL) << (char)PLAYERX << getProp(PLAYERX) << (char)PLAYERY << getProp(PLAYERY);
	for (int i = 0; i < playerList.count(); i ++)
	{
		CPlayer *other = (CPlayer*)playerList[i];
		if (other != this)
			other->sendPacket(packet);
	}

	//get props
	packet.clear();
	packet << (char)OTHERPLPROPS << (short)id << (char)50 << (char)1;
	for (int i = 0; i < propscount; i++)
	{
		if (forwardLocal[i])
			packet << (char)i << getProp(i);
	}

	//send props
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer*other = (CPlayer*)level->players[i];
		if (other == this)
			continue;

		CPacket otherProps;
		otherProps << (char)OTHERPLPROPS << (short)other->id << (char)50 << (char)1;

		for (int ii = 0; ii < propscount; ii++)
		{
			if (forwardLocal[ii])
				otherProps << (char)ii << other->getProp(ii);
		}
		sendPacket(otherProps);
		other->sendPacket(packet);
	}

	compressAndSend();
	return true;
}

void CPlayer::leaveLevel()
{
	if (level == NOLEVEL)
		return;

	// Remember when the player last visited the level.
	bool found = false;
	for ( int i = 0; i < enteredLevels.count(); ++i )
	{
		CEnteredLevel* entered = (CEnteredLevel*)enteredLevels[i];
		if ( entered->level == level )
		{
			found = true;
			entered->time = getSysTime();
		}
	}
	if ( found == false )
		enteredLevels.add(new CEnteredLevel(level, getSysTime()));

	// Leave the level.
	level->players.remove(this);
	if ( level->players.count() > 0 )
		((CPlayer*)level->players[0])->sendPacket(CPacket() << (char)ISLEADER);

	// Tell everyone I left.
	CPacket otherProps;
	otherProps << (char)OTHERPLPROPS << (short)id << (char)50 << (char)0;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		other->sendPacket(otherProps);
		other->compressAndSend();
		sendPacket(CPacket() << (char)OTHERPLPROPS << (short)other->id << (char)50 << (char)0);
	}
	compressAndSend();

	// Sad hack to try to fix npcs.
	//if ( level->players.count() == 0 ) CLevel::updateLevel(level->fileName);

	level = NOLEVEL;
}

void CPlayer::sendWeapons()
{
	for (int i = 0; i < weaponSend.count(); i++)
	{
		int fIndex = myWeapons.find( weaponSend[i].text() );

		bool defaultWeapon = false;
		for (int ii = 0; ii < itemcount; ii++)
		{
			if (weaponSend[i] == itemNames[ii])
			{
				defaultWeapon = true;

				// Only send if we allow default weapons.
				if ( defaultweapons == true )
				{
					myWeapons.add(weaponSend[i]);
					sendPacket(CPacket() << (char)SADDDEFWEAPON << (char)ii);
				}
				break;
			}
		}

		if ( fIndex < 0 && !defaultWeapon )
			myWeapons.add(weaponSend[i]);

		if (defaultWeapon)
			continue;

		for (int ii = 0; ii < weaponList.count(); ii++)
		{
			CWeapon* weapon = (CWeapon*)weaponList[ii];
			if (weapon->name == weaponSend[i])
			{
				// Delete the weapon if it has changed and the player already has it.
				// This forces the client to refresh that weapon.
				if ( fIndex >= 0 )
				{
//					if ( myWeapons[fIndex].modTime != weapon->modTime )
//					{
//						myWeapons.remove(fIndex);
//						myWeapons.add(weaponSend[i]);
						sendPacket( CPacket() << (char)SDELNPCWEAPON << weapon->name );
//					}
				}
				sendPacket(CPacket() << (char)SADDNPCWEAPON << weapon->getSendData());
				break;
			}
		}
	}
	weaponSend.clear();
}

void CPlayer::sendFiles()
{
	for (int i = 0; i < fileList.count(); i++)
	{
		long long modTime;
		bool failed = true;
		COutFile* file = (COutFile*)fileList[i];
		CBuffer fileData;
		CString shortName = file->fileName;
		CString longName = (file->longName.length() > 0 ? file->longName : getDataFile(shortName.text()));

		if (longName.length() != 0)
		{
			modTime = getFileModTime(longName.text());
			if (modTime > file->modTime)
			{
				if (fileData.load(longName.text()))
				{
					bool isLargeFile = false;
					failed = false;

					// If it is a large file, send the large file packet.
					if (fileData.length() > 32000)
					{
						isLargeFile = true;
						sendPacket(CPacket() << (char)SLARGEFILESTART << shortName);
						sendPacket(CPacket() << (char)SLARGEFILESIZE << (long long)fileData.length());
					}

					// Send the entire file.
					while (fileData.length() > 0)
					{
						int fileLen = CLIP(fileData.length(), 0, 32000);
						int len = 1 + 5 + 1 + shortName.length() + fileLen + 1;
						sendPacket(CPacket() << (char)100 << (int)len);
						sendPacket(CPacket() << (char)102 << (long long)modTime << (char)shortName.length() << shortName << fileData.copy(0, fileLen) << "\n");
						fileData.remove(0, fileLen);
					}

					// If we had sent a large file, tell the client we are now finished.
					if (isLargeFile)
						sendPacket(CPacket() << (char)SLARGEFILEEND << shortName);
				}
			}
			else sendPacket(CPacket() << (char)SFILEUPTODATE << shortName);
		}

		if(failed)
			sendPacket(CPacket() << (char)GIFFAILED << shortName);
		delete file;
	}
	fileList.clear();
}

time_t CPlayer::getLeavingTime(CLevel* pLevel)
{
	for (int i = 0; i < enteredLevels.count(); i++)
	{
		CEnteredLevel* entered = (CEnteredLevel*)enteredLevels[i];
		if ( entered != 0 && entered->level == pLevel )
			return entered->time;
	}
	return 0;
}

void CPlayer::sendLocally(CPacket& pPacket)
{
	if (level->map != NULL)
	{
		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer* other = (CPlayer*)playerList[i];
			if (other == this)
				continue;

			if ((other->level->map == level->map &&
					level->map->nextTo(level->levelIndex, other->level->levelIndex)) ||
					level == other->level)
				other->sendPacket(pPacket);
		}
	}
	else
	{
		for (int i = 0; i < level->players.count(); i++)
		{
			CPlayer* other = (CPlayer*)level->players[i];
			if (other != this)
				other->sendPacket(pPacket);
		}
	}
}

void CPlayer::sendGlobally(CPacket& pPacket)
{
	for ( int i = 0; i < playerList.count(); ++i )
	{
		CPlayer* other = (CPlayer*)playerList[i];
		other->sendPacket( pPacket );
	}
}

void CPlayer::updateProp(int pPropIndex)
{
	CPacket props;
	props << (char)pPropIndex << getProp(pPropIndex);
	setProps(props, true);
	sendPacket(CPacket() << (char)SPLAYERPROPS << props);
}

//Drop random items (can be used when we die)
void CPlayer::dropItems()
{
	if(!dropItemsDead)
		return;

	// Remove gralats from the account.
	int gralats = CLIP( rand() % maxdeathgralats, mindeathgralats, maxdeathgralats );
	rubins -= gralats;

	// Don't allow a negative number of gralats and don't drop more than the player has.
	if ( rubins < 0 )
	{
		gralats -= -rubins;
		rubins = 0;
	}
	updateProp( RUPEESCOUNT );

	// Drop the gralats.
	while ( gralats > 0 )
	{
		int itemId = 0;
		if ( gralats % 100 != gralats )
		{
			gralats -= 100;
			itemId = GOLDRUPEE;
		}
		if ( gralats % 30 != gralats )
		{
			gralats -= 30;
			itemId = REDRUPEE;
		}
		else if ( gralats % 5 != gralats )
		{
			gralats -= 5;
			itemId = BLUERUPEE;
		}
		else if ( gralats > 0 )
		{
			gralats--;
			itemId = GREENRUPEE;
		}

		// Place the item on the level.
		float pX = x + (rand()%7)-2;
		float pY = y + (rand()%7)-2;
		level->items.add(new CItem(pX, pY, itemId));

		for (int ii = 0; ii < level->players.count(); ii++)
			((CPlayer*)level->players[ii])->sendPacket(CPacket() << (char)SADDEXTRA << (char)(pX*2) << (char)(pY*2) << (char)itemId);
	}

	// Drop bombs/darts.
	for ( int i = 0; i < 6; ++i )
	{
		int itemId = 0;
		if ( rand() % 3 == 0 )	// 33% chance.
		{
			if ( i % 2 == 0 )
			{
				bombs -= 5;
				itemId = BOMBS;
			}
			else
			{
				darts -= 5;
				itemId = DARTS;
			}

			// Place the item on the level.
			float pX = x + (rand()%7)-2;
			float pY = y + (rand()%7)-2;
			level->items.add(new CItem(pX, pY, itemId));

			for (int ii = 0; ii < level->players.count(); ii++)
				((CPlayer*)level->players[ii])->sendPacket(CPacket() << (char)SADDEXTRA << (char)(pX*2) << (char)(pY*2) << (char)itemId);
		}
	}
	updateProp(BOMBSCOUNT);
	updateProp(ARROWSCOUNT);
}

CPacket CPlayer::getProp(int pProp)
{
	CPacket retVal;
	switch (pProp)
	{
		case NICKNAME:
			retVal << (char)nickName.length() << nickName;
			break;

		case MAXPOWER:
			retVal.writeByte1((char)maxPower);
			break;

		case CURPOWER:
			retVal.writeByte1((char)(power*2));
			break;

		case RUPEESCOUNT:
			retVal.writeByte3(rubins);
			break;

		case ARROWSCOUNT:
			retVal.writeByte1(darts);
			break;

		case BOMBSCOUNT:
			retVal.writeByte1(bombs);
			break;

		case GLOVEPOWER:
			retVal.writeByte1(glovePower);
			break;

		case BOMBPOWER:
			retVal.writeByte1(bombPower);
			break;

		case SWORDPOWER:
			retVal.writeByte1(swordPower+30);
			retVal.writeByte1(swordImage.length());
			retVal << swordImage;
			break;

		case SHIELDPOWER:
			retVal.writeByte1(shieldPower+10);
			retVal << (char)shieldImage.length() << shieldImage;
			break;

		case PLAYERANI:
			retVal << (char)gAni.length() << gAni;
			break;

		case HEADGIF:
			retVal.writeByte1(100 + headImage.length());
			retVal << headImage;
			break;

		case CURCHAT:
			retVal << (char)chatMsg.length() << chatMsg;
			break;

		case PLAYERCOLORS:
			retVal.writeByte1(colors[0]);
			retVal.writeByte1(colors[1]);
			retVal.writeByte1(colors[2]);
			retVal.writeByte1(colors[3]);
			retVal.writeByte1(colors[4]);
			break;

		case PLAYERID:
			retVal.writeByte2(id);
			break;

		case PLAYERX:
			retVal.writeByte1((char)(x * 2));
			break;

		case PLAYERY:
			retVal.writeByte1((char)(y * 2));
			break;

		case PLAYERSPRITE:
			retVal.writeByte1(sprite);
			break;

		case STATUS:
			retVal.writeByte1(status);
			break;

		case CARRYSPRITE:
			retVal.writeByte1(carrySprite);
			break;

		case CURLEVEL:
			if(type == CLIENTPLAYER)
				retVal << (char)levelName.length() << levelName;
			else retVal << (char)1 << " ";
			break;

		case HORSEGIF:
			retVal << (char)horseImage.length() << horseImage;
			break;

		case HORSEBUSHES:
			retVal.writeByte1(horseBushes);
			break;

		case EFFECTCOLORS:
			retVal.writeByte1(0);
			break;

		case CARRYNPC:
			retVal.writeByte3(0);
			break;

		case APCOUNTER:
			retVal.writeByte2(apCounter+1);
			break;

		case MAGICPOINTS:
			retVal.writeByte1(magicPoints);
			break;

		case KILLSCOUNT:
			retVal.writeByte3(kills);
			break;

		case DEATHSCOUNT:
			retVal.writeByte3(deaths);
			break;

		case ONLINESECS:
			retVal.writeByte3(onlineSecs);
			break;

		case LASTIP:
			retVal.writeByte5(lastIp);
			break;

		case UDPPORT:
			retVal.writeByte3(udpPort);
			break;

		case PALIGNMENT:
			retVal.writeByte1(ap);
			break;

		case PADDITFLAGS:
			retVal.writeByte1(additionalFlags);
			break;

		case PACCOUNTNAME:
			retVal << (char)accountName.length() << accountName;
			break;

		case BODYIMG:
			retVal << (char)bodyImage.length() <<  bodyImage;
			break;

		case RATING:
		{
			int temp = (((int)rating & 0xFFF) << 9) | ((int)deviation & 0x1FF);
			retVal.writeByte3(temp);
		}
		break;

		case GATTRIB1: retVal << (char)myAttr[0].length() << myAttr[0]; break;
		case GATTRIB2: retVal << (char)myAttr[1].length() << myAttr[1]; break;
		case GATTRIB3: retVal << (char)myAttr[2].length() << myAttr[2]; break;
		case GATTRIB4: retVal << (char)myAttr[3].length() << myAttr[3]; break;
		case GATTRIB5: retVal << (char)myAttr[4].length() << myAttr[4]; break;

		case PLAYERZ:
			retVal.writeByte1((char)(z * 2));
			break;

		case GATTRIB6: retVal << (char)myAttr[5].length() << myAttr[5]; break;
		case GATTRIB7: retVal << (char)myAttr[6].length() << myAttr[6]; break;
		case GATTRIB8: retVal << (char)myAttr[7].length() << myAttr[7]; break;
		case GATTRIB9: retVal << (char)myAttr[8].length() << myAttr[8]; break;

		case PCONNECTED:
			// Isn't a stored property.
			break;

		case PLANGUAGE:
			retVal << (char)language.length() << language;
			break;

		case PSTATUSMSG:
			if ( statusMsg > statusList.count() - 1 )  retVal.writeByte1( 0 );
			else retVal.writeByte1( (char)statusMsg );
			break;

		case GATTRIB10: retVal << (char)myAttr[9].length() << myAttr[9]; break;
		case GATTRIB11: retVal << (char)myAttr[10].length() << myAttr[10]; break;
		case GATTRIB12: retVal << (char)myAttr[11].length() << myAttr[11]; break;
		case GATTRIB13: retVal << (char)myAttr[12].length() << myAttr[12]; break;
		case GATTRIB14: retVal << (char)myAttr[13].length() << myAttr[13]; break;
		case GATTRIB15: retVal << (char)myAttr[14].length() << myAttr[14]; break;
		case GATTRIB16: retVal << (char)myAttr[15].length() << myAttr[15]; break;
		case GATTRIB17: retVal << (char)myAttr[16].length() << myAttr[16]; break;
		case GATTRIB18: retVal << (char)myAttr[17].length() << myAttr[17]; break;
		case GATTRIB19: retVal << (char)myAttr[18].length() << myAttr[18]; break;
		case GATTRIB20: retVal << (char)myAttr[19].length() << myAttr[19]; break;
		case GATTRIB21: retVal << (char)myAttr[20].length() << myAttr[20]; break;
		case GATTRIB22: retVal << (char)myAttr[21].length() << myAttr[21]; break;
		case GATTRIB23: retVal << (char)myAttr[22].length() << myAttr[22]; break;
		case GATTRIB24: retVal << (char)myAttr[23].length() << myAttr[23]; break;
		case GATTRIB25: retVal << (char)myAttr[24].length() << myAttr[24]; break;
		case GATTRIB26: retVal << (char)myAttr[25].length() << myAttr[25]; break;
		case GATTRIB27: retVal << (char)myAttr[26].length() << myAttr[26]; break;
		case GATTRIB28: retVal << (char)myAttr[27].length() << myAttr[27]; break;
		case GATTRIB29: retVal << (char)myAttr[28].length() << myAttr[28]; break;
		case GATTRIB30: retVal << (char)myAttr[29].length() << myAttr[29]; break;

		default:
			errorOut("errorlog.txt", CString() << "Unknown getProp( " << toString(pProp) << " ) by " << accountName);
			break;
	}
	return retVal;
}

CPacket CPlayer::setProps(CPacket& pProps, bool pForward, CPlayer* rc)
{
	int len;
	CPacket forwardBuff;
	CPacket forwardBuff2;
	CPacket ret;
	forwardBuff << (char)OTHERPLPROPS << (short)id;
	forwardBuff2 << (char)OTHERPLPROPS << (short)id;

	// This allows for correct RC login.
	if (id == -1 && pForward)
		pForward = false;

	while (pProps.bytesLeft())
	{
//		int startpos = pProps.getRead();
		int index = pProps.readByte1();
		if ( index < 0 || index >= propscount )
		{
			errorOut("errorlog.txt", CString() << "setProps(" << toString(index) << ") index out of bounds by " << accountName);
			deleteMe = true;
			break;
		}

		switch (index)
		{
			case NICKNAME:
			{
				len = (unsigned char)pProps.readByte1();
				if (len > 224)
					break;

				setNick(CString() << pProps.readChars(len), true);
				forwardBuff2 << (char)index << getProp( index );
			}
			break;

			case MAXPOWER:
				maxPower = CLIP((float)pProps.readByte1(), 0.0f, (float)heartLimit);
				break;

			case CURPOWER:
			{
				float oldPower = power;
				power = CLIP((float)pProps.readByte1()/2, 0.0f, maxPower);
				if ( power == 0 && oldPower > 0 && level->sparZone == false )
					dropItems();
				break;
			}
			case RUPEESCOUNT:
				if ( rc != 0 )
				{
					if ( adminCanChangeGralat == true || (rc->type == CLIENTRC && rc->hasStaff() && rc->hasRight(CANCHANGESTAFFACC)) )
						rubins = CLIP((unsigned int)pProps.readByte3(), 0, 9999999);
					else
						pProps.readByte3();
				}
				else rubins = CLIP((unsigned int)pProps.readByte3(), 0, 9999999);
				break;

			case ARROWSCOUNT:
				darts = CLIP((unsigned char)pProps.readByte1(), 0, 99);
				break;

			case BOMBSCOUNT:
				bombs = CLIP((unsigned char)pProps.readByte1(), 0, 99);
				break;

			case GLOVEPOWER:
				glovePower = CLIP((unsigned char)pProps.readByte1(), 0, 3);
				//is 3 maximum??
				break;

			case BOMBPOWER:
				bombPower = CLIP((unsigned char)pProps.readByte1(), 0, 3);
				break;

			case SWORDPOWER:
			{
				int sp = (unsigned char)pProps.readByte1();
				if ( sp <= 4 )
					swordImage = CString() << "sword" << toString(sp) << ".png";
				else
				{
					sp -= 30;
					len = pProps.readByte1();
					if ( len >= 0 )
					{
						CString temp( pProps.readChars( len ) );
						CString temp2( getDataFile( temp.text() ) );
						if ( temp2.length() > 0 )
							if ( noFoldersConfig || isValidFile( temp2, SWORDPOWER ) )
								swordImage = temp;		// Not temp2
					}
				}
				swordPower = CLIP(sp, ((healswords == true) ? -swordLimit : 0), swordLimit);
				break;
			}

			case SHIELDPOWER:
			{
				int sp = (unsigned char)pProps.readByte1();
				if ( sp >= 10 )
				{
					sp -= 10;
					len = (unsigned char)pProps.readByte1();
					if ( len >= 0 )
					{
						CString temp( pProps.readChars(len) );
						CString temp2( getDataFile( temp.text() ) );
						if ( temp2.length() > 0 )
							if ( noFoldersConfig || isValidFile( temp2, SHIELDPOWER ) )
								shieldImage = temp;		// Not temp2
					}
				}
				else if ( sp <= 3)
					shieldImage = CString() << "shield" << toString(sp) << ".png";
				shieldPower = CLIP(sp, 0, shieldLimit);
				break;
			}

			case PLAYERANI:
				len = (unsigned char)pProps.readByte1();
				gAni = pProps.readChars(len);
				break;

			case HEADGIF:
				len = (unsigned char)pProps.readByte1();
				if (len < 100)
				{
					headImage = CString() << "head" << toString(len) << ".png";
					forwardBuff2 << (char)index << getProp( index );
				} else
				{
					CString temp( pProps.readChars(len-100) );
					if ( noFoldersConfig || isValidFile( temp, HEADGIF ) )
					{
						headImage = temp;
						forwardBuff2 << (char)index << getProp( index );
					}
				}
				break;

			case CURCHAT:
				len = (unsigned char)pProps.readByte1();
				len = (len < 0 ? 0 : len);

				chatMsg = pProps.readChars(len);
				chatMsg = (chatMsg.length() > 220 ? chatMsg.copy(0, 220) : chatMsg);

				processChat(chatMsg);
				lastChat = time(NULL);
	   			break;

			case PLAYERCOLORS:
				for (int i = 0; i < 5; i ++)
					colors[i] = (unsigned char)pProps.readByte1();
				break;

			case PLAYERID:
				pProps.readByte2();
				break;

			case PLAYERX:
				x = (float)pProps.readByte1() / 2.0f;
				status &= (-1-1);
				lastMovement = getTime();
				//TO DO: add hack check;
				break;

			case PLAYERY:
				y = (float)pProps.readByte1() / 2.0f;
				status &= (-1-1);
				lastMovement = getTime();
				//TO DO: add hack check;
				break;

			case PLAYERSPRITE:
				sprite = (unsigned char)pProps.readByte1();
				//TO DO: add hack check;
				break;

			case STATUS:
			{
				int oldStatus = status;
				status = (unsigned char)pProps.readByte1();

				if ( id == -1 ) break;

				// When they come back to life, give them hearts.
				if ( (oldStatus & 8) > 0 && (status & 8) == 0 )
				{
					power = CLIP((ap < 20 ? 3 : (ap < 40 ? 5 : maxPower)), 0.0f, maxPower);
					if ( level->players.count() == 1 )
						sendLevel( level->fileName, this->x, this->y, getSysTime() );
//						warp( level->fileName, getProp(PLAYERX), getProp(PLAYERY), getSysTime() );
				}

				// When they die, increase deaths and make somebody else level leader.
				if ( (oldStatus & 8) == 0 && (status & 8) > 0 )
				{
					if(!level->sparZone)
						deaths++;
					if (level->players.count() > 1 && level->players[0] == this)
					{
						level->players.remove(0);
						level->players.add(this);
						((CPlayer*)level->players[0])->sendPacket(CPacket() << (char)ISLEADER);
					}
				}
			}
			break;

			case CARRYSPRITE:
				carrySprite = (unsigned char)pProps.readByte1();
				//TO DO: packet format may be outdated
				break;

			case CURLEVEL:
				len = (unsigned char)pProps.readByte1();
				levelName = pProps.readChars(len);
				//TO DO: add hack check
				break;

			case HORSEGIF:
			{
				len = (unsigned char)pProps.readByte1();
				CString temp( pProps.readChars(len) );
				if ( noFoldersConfig || isValidFile( temp, -1 ) )
					horseImage = temp;
				//TO DO: add hack check
			}
			break;

			case HORSEBUSHES:
				horseBushes = (unsigned char)pProps.readByte1();
				//TO DO: add hack check
				break;

			case EFFECTCOLORS:
				len = (unsigned char)pProps.readByte1();
				if ( len > 0 )
					pProps.readByte4();
	//			pProps.readByte5();
				break;

			case CARRYNPC:
				pProps.readByte3();
				//this doesnt work even when i add the correct code...:/
				//TO DO: add hack check
				break;

			case APCOUNTER:
				apCounter = (unsigned int)pProps.readByte2();
				//TO DO: add hack check
				break;

			case MAGICPOINTS:
				magicPoints = CLIP((unsigned char)pProps.readByte1(), 0, 100);
				//TO DO: add hack check
				break;

			case KILLSCOUNT:
			   //kills = pProps.readByte3();
			   pProps.readByte3();
				//Why should any1 be able to change kills?
				break;

			case DEATHSCOUNT:
				//deaths = pProps.readByte3();
				pProps.readByte3();
				//Why should any1 be able to change deaths?
				break;

			case ONLINESECS:
				//onlineSecs = pProps.readByte3();
				pProps.readByte3();
				//Why should any1 be able to change online seconds?
				break;

			case LASTIP:
				//lastIp = (int)pProps.readByte5();
				pProps.readByte5();
				//Why should any1 be able to change last ip?
				break;

			case UDPPORT:
				udpPort = (unsigned int)pProps.readByte3();
				if(udpPort > 0)
				{
					sendPacket(CPacket() << (char)DISMESSAGE <<
						"This server doesnt support UDP. Please disable UDP in general settings.");
					deleteMe = true;
				}

				if ( id == -1 ) break;
				for(int i = 0; i < playerList.count(); i++)
				{
					CPlayer* other = (CPlayer*)playerList[i];
					if(other != this)
					{
						other->sendPacket(CPacket() << (char)OTHERPLPROPS <<
							(short)id << (char)UDPPORT << (int)udpPort);
					}
				}
				//TO DO: add hack check
				break;

			case PALIGNMENT:
				ap = CLIP(pProps.readByte1(), 0, 100);
				//TO DO: add hack check
				break;

			case PADDITFLAGS:
				additionalFlags = (unsigned char)pProps.readByte1();
				//TO DO: add hack check
				break;

			case PACCOUNTNAME:
				len = (unsigned char)pProps.readByte1();
				pProps.readChars(len);
				//Do not allow account name change
				break;

			case BODYIMG:
			{
				len = (unsigned char)pProps.readByte1();
				CString temp( pProps.readChars(len) );
				if ( noFoldersConfig || isValidFile( temp, BODYIMG ) )
					bodyImage = temp;
				//TO DO: add hack check
			}
			break;

			case RATING:
			{
				int temp = pProps.readByte3();
				//rating = (float)((temp >> 9) & 0xFFF);
				//deviation = (float)(temp & 0x1FF);
			}
			break;

			case GATTRIB1:  myAttr[0]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB2:  myAttr[1]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB3:  myAttr[2]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB4:  myAttr[3]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB5:  myAttr[4]  = pProps.readChars((unsigned char)pProps.readByte1()); break;

			case PEMPTY42:	//Caused when losing a spar? - Beholder
				errorOut( "debuglog.txt", CString() << accountName << " sent PEMPTY42.  Sparring zone? " << ((level->sparZone == true) ? "Yes." : "No.") );
				break;

			case PEMPTY43:	//Caused when tieing a spar? - Beholder
				errorOut( "debuglog.txt", CString() << accountName << " sent PEMPTY43.  Sparring zone? " << ((level->sparZone == true) ? "Yes." : "No.") );
				break;

			case PEMPTY44:	//Caused when winning a spar? - Beholder
				errorOut( "debuglog.txt", CString() << accountName << " sent PEMPTY44.  Sparring zone? " << ((level->sparZone == true) ? "Yes." : "No.") );
				break;

			case PLAYERZ:
				z = (float)pProps.readByte1() / 2;
				status &= (-1-1);
				lastMovement = getTime();
				//TO DO: add hack check;
				break;

			case GATTRIB6:  myAttr[5]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB7:  myAttr[6]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB8:  myAttr[7]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB9:  myAttr[8]  = pProps.readChars((unsigned char)pProps.readByte1()); break;

			case PCONNECTED:
				break;

			case PLANGUAGE:
				len = (unsigned char)pProps.readByte1();
				language = pProps.readChars(len);
				break;

			case PSTATUSMSG:
				statusMsg = (unsigned char)pProps.readByte1();

				// Allows proper RC login.
				if ( id == -1 ) break;

				for ( int i = 0; i < playerList.count(); i++ )
				{
					CPlayer* other = (CPlayer*)playerList[i];
					if ( other != this )
						other->sendPacket(CPacket() << (char)OTHERPLPROPS << (short)id <<
							(char)PSTATUSMSG << (char)statusMsg);
				}
				//TO DO: add hack check
				break;

			case GATTRIB10: myAttr[9]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB11: myAttr[10] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB12: myAttr[11] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB13: myAttr[12] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB14: myAttr[13] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB15: myAttr[14] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB16: myAttr[15] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB17: myAttr[16] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB18: myAttr[17] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB19: myAttr[18] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB20: myAttr[19] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB21: myAttr[20] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB22: myAttr[21] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB23: myAttr[22] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB24: myAttr[23] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB25: myAttr[24] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB26: myAttr[25] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB27: myAttr[26] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB28: myAttr[27] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB29: myAttr[28] = pProps.readChars((unsigned char)pProps.readByte1()); break;
			case GATTRIB30: myAttr[29] = pProps.readChars((unsigned char)pProps.readByte1()); break;

			default:
				errorOut("errorlog.txt", CString() << "Setprops error: " << toString(index) << " By " << accountName);

				// If they send bad props, kick them.
				deleteMe = true;
				return CPacket();
		}

		if ( pForward && forwardLocal[index] == true )
		{
			/*
			int len2 = pProps.getRead() - startpos;
			if (!(udpPort && udpDontForward[index]))
			{
				if (forwardLocal[index])
				forwardBuff.writeBytes(pProps.text()+startpos, len2);
			}
			*/
			forwardBuff << (char)index << getProp(index);
		}
		ret << (char)index << getProp(index);
	}

	// Send head and nick changes to players not in the level.
	// This makes the mini-map work.
	if ( forwardBuff2.length() > 3 )
	{
		for ( int i = 0; i < playerList.count(); ++i )
		{
			CPlayer* other = (CPlayer*)playerList[i];
			if ( other == this ) continue;
			if ( other->level != this->level && other->type == CLIENTPLAYER )
			{
				other->sendPacket( forwardBuff2 );
				other->compressAndSend();
			}
		}
	}

	if (pForward && forwardBuff.length() > 3)
	{
		sendLocally(forwardBuff);
		compressAndSend();
	}

	return ret;
}


void CPlayer::getItem(int pItem)
{
	if (pItem < 0 || pItem >= itemcount)
		return;

	switch (pItem)
	{
		case GREENRUPEE:
			rubins = CLIP(rubins + 1, 0, 9999999);
			updateProp(RUPEESCOUNT);
		break;

		case BLUERUPEE:
			rubins = CLIP(rubins + 5, 0, 9999999);
			updateProp(RUPEESCOUNT);
		break;

		case REDRUPEE:
			rubins = CLIP(rubins + 30, 0, 9999999);
			updateProp(RUPEESCOUNT);
		break;

		case BOMBS:
			bombs = CLIP(bombs + 5, 0, 99);
			updateProp(BOMBSCOUNT);
		break;

		case DARTS:
			darts = CLIP(darts + 5, 0, 99);
			updateProp(ARROWSCOUNT);
		break;

		case HEART:
			power = CLIP(power + 1, 0.0f, maxPower);
			updateProp(CURPOWER);
		break;

		case GLOVE1:
			glovePower = (glovePower < 2 ? 2 : glovePower);
			updateProp(GLOVEPOWER);
		break;

		case BOW:
			weaponSend.add("bow");
		break;

		case BOMB:
			weaponSend.add("bomb");
		break;

		case SHIELD:
			shieldPower = CLIP((shieldPower < 1 ? 1 : shieldPower), 0, shieldLimit);
			updateProp(SHIELDPOWER);
		break;

		case SWORD:
			swordPower = CLIP((swordPower < 1 ? 1 : swordPower), 0, swordLimit);
			updateProp(SWORDPOWER);
		break;

		case FULLHEART:
			maxPower = CLIP(maxPower + 1, 0.0f, (float)heartLimit);
			power = maxPower;
			updateProp(MAXPOWER);
			updateProp(CURPOWER);
		break;

		case SUPERBOMB:
			weaponSend.add("superbomb");
		break;

		case BATTLEAXE:
			swordPower = CLIP((swordPower < 2 ? 2 : swordPower), 0, swordLimit);
			updateProp(SWORDPOWER);
		break;

		case GOLDENSWORD:
			swordPower = CLIP((swordPower < 4 ? 4 : swordPower), 0, swordLimit);
			updateProp(SWORDPOWER);
		break;

		case GLOVE2:
			glovePower = (glovePower < 3 ? 3 : glovePower);
			updateProp(GLOVEPOWER);
		break;

		case LIZARDSHIELD:
			shieldPower = CLIP((shieldPower < 3 ? 3 : shieldPower), 0, shieldLimit);
			updateProp(SHIELDPOWER);
		break;

		case LIZARDSWORD:
			swordPower = CLIP((swordPower < 3 ? 3 : swordPower), 0, swordLimit);
			updateProp(SWORDPOWER);
		break;

		case GOLDRUPEE:
			rubins = CLIP(rubins + 100, 0, 9999999);
			updateProp(RUPEESCOUNT);
		break;

		case FIREBALL:
			weaponSend.add("fireball");
		break;

		case FIREBLAST:
			weaponSend.add("fireblast");
		break;

		case NUKESHOT:
			weaponSend.add("nukeshot");
		break;

		case JOLTBOMB:
			weaponSend.add("joltbomb");
		break;

		case MIRRORSHIELD:
			shieldPower = CLIP((shieldPower < 2 ? 2 : shieldPower), 0, shieldLimit);
			updateProp(SHIELDPOWER);
		break;

		case SPINATTACK:
			if (status & 64)
				break;
			status |= 64;
			updateProp(STATUS);
		break;
	}
}
void CPlayer::sendServerFlag(CString& pFlag)
{
	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];
		if (other != this)
			other->sendPacket(CPacket() << (char)SSETFLAG << pFlag);
	}
}

void CPlayer::setBan(CBuffer &pReason, bool pBan, bool pAppend)
{
	banned = pBan;
	pReason.tokenize();

	if (pAppend)
	{
		banReason.writeChar(',');
		banReason.writeBuffer(pReason);
	}
	else banReason = pReason;
}

//Messages
void CPlayer::msgLEVELWARP(CPacket& pPacket)
{
	int modTime = 0;

	if (pPacket[0]-32 == MODLEVELWARP)
		modTime = (int)pPacket.readByte5();

	float x2 = 0, y2 = 0;
	x2 = (float)(pPacket.readByte1()/2);
	y2 = (float)(pPacket.readByte1()/2);
	CString newLevel = pPacket.readString("");
	warp(newLevel, x2, y2, modTime);
}

void CPlayer::msgMODLEVELWARP(CPacket& pPacket)
{
	msgLEVELWARP(pPacket);
}

void CPlayer::msgOUTTERMAP(CPacket& pPacket)
{
	int modTime = (int)pPacket.readByte5();
	CString mapName = pPacket.readString("");
	CPacket packet;
	CLevel* outterLevel = CLevel::openMap(mapName);

	if (outterLevel == NOLEVEL)
		return;

	bool alreadyVisited = false;
	for (int i = 0; i < enteredLevels.count(); i++)
	{
		CEnteredLevel* entered = (CEnteredLevel*)enteredLevels[i];
		if ( entered->level == outterLevel )
			alreadyVisited = true;
	}

	sendPacket(CPacket() << (char)LEVELNAME << mapName);
	if ((modTime != outterLevel->modTime) || alreadyVisited == false)
	{
		sendPacket(CPacket() << (char)100 << (int)(1+(64*64*2)+1));
		packet.clear();
		packet.writeByte1(101);
		packet.writeBytes((char*)outterLevel->tiles, 64*64*2);
		sendPacket(packet);
		sendPacket(CPacket() << (char)LEVELMODTIME << (long long)outterLevel->modTime);
	} else sendPacket(CPacket() << (char)LEVELBOARD);
	sendPacket(CPacket() << (char)LEVELNAME << levelName);
	if (level->players.count() == 1)
		sendPacket(CPacket() << (char)ISLEADER);
	return;
}
void CPlayer::msgBOARDMODIFY(CPacket& pPacket)
{
	int bX = pPacket.readByte1();
	int bY = pPacket.readByte1();
	int bW = pPacket.readByte1();
	int bH = pPacket.readByte1();
	CPacket tileData;
	tileData << pPacket.copy(5, pPacket.length()-5);
	if (level->changeBoard(tileData, bX, bY, bW, bH, this) == false)
		return;

	if (bX < 0 || bX > 63 || bY < 0 || bY > 63)
		return;

	// Lay items when you destroy objects.
	short oldTile = level->tiles[bX+(bY*64)];
	int dropItem = -1;

	// Bushes, grass, swamp
	if ((oldTile == 2 || oldTile == 0x1a4 || oldTile == 0x1ff ||
			oldTile == 0x3ff)&& bushesDrop)
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
	else if (oldTile == 0x2ac && vasesDrop)
		dropItem = 5;

	// Send the item now.
	if (dropItem >= 0)
	{
		CPacket packet;
		packet << (char)SADDEXTRA << (char)(bX*2) << (char)(bY*2) << (char)dropItem;
		for (int i = 0; i < level->players.count(); i++)
		{
			CPlayer* other = (CPlayer*)level->players[i];
			other->sendPacket(packet);
		}
		level->items.add(new CItem((float)bX, (float)bY, dropItem));
	}
}

void CPlayer::msgPLAYERPROPS(CPacket& pPacket)
{
	setProps(pPacket, true);
}

void CPlayer::msgNPCPROPS(CPacket& pPacket)
{
	CPacket packet;
	CNpc* npc = (CNpc*)npcIds[pPacket.readByte3()];
	if (npc == NULL)
		return;
	if(npc->level != level)
		return;

	packet << (char)SNPCPROPS << pPacket.text() + 1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(packet);
	}
	npc->setProps(pPacket);
}

void CPlayer::msgADDBOMB(CPacket& pPacket)
{
	if ( id == -1 ) return;
	CPacket bombData;
	bombData << (char)SADDBOMB << (short)id << pPacket.text() + 1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(bombData);
	}
}

void CPlayer::msgDELBOMB(CPacket& pPacket)
{
	CPacket bombData;
	bombData << (char)SDELBOMB << pPacket.text()+1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(bombData);
	}
}

void CPlayer::msgTOALLMSG(CPacket& pPacket)
{
	if (jailLevels.find(levelName) >= 0)
		return;
	if ( id == -1 ) return;

	CPacket message;
	message << (char)STOALLMSG << (short)id << pPacket.text()+1;
	WordFilter.apply(this, message, FILTERC_TOALL);

	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];
		if (other != this && !(other->additionalFlags & 4))
			other->sendPacket(message);
	}
}

void CPlayer::msgADDHORSE(CPacket& pPacket)
{
	CPacket horseData;
	horseData << (char)SADDHORSE << pPacket.text() + 1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(horseData);
	}

	float hX = (float)(pPacket.readByte1())/2.0f;
	float hY = (float)(pPacket.readByte1())/2.0f;
	unsigned char dir_bush = (unsigned char)pPacket.readByte1();
	char hdir = dir_bush & 0x03;
	char hbushes = dir_bush >> 2;
	CString image = pPacket.readString("");
	level->horses.add(new CHorse(image, hX, hY, hdir, hbushes));
}

void CPlayer::msgDELHORSE(CPacket& pPacket)
{
	CPacket horseData;
	horseData << (char)SDELHORSE << pPacket.text() + 1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(horseData);
	}

	float hX = (float)pPacket.readByte1()/2.0f;
	float hY = (float)pPacket.readByte1()/2.0f;
	level->removeHorse(hX, hY);
}
void CPlayer::msgADDARROW(CPacket& pPacket)
{
	if ( id == -1 ) return;

	CPacket arrowData;
	arrowData << (char)SADDARROW<< (short)id << pPacket.text() + 1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(arrowData);
	}
}

void CPlayer::msgFIRESPY(CPacket& pPacket)
{
	if ( id == -1 ) return;

	CPacket fireData;
	fireData << (char)SFIRESPY << (short)id << pPacket.text() + 1;
	sendLocally( fireData );
}
void CPlayer::msgCARRYTHROWN(CPacket& pPacket)
{
	if ( id == -1 ) return;
	sendLocally(CPacket() << (char)SCARRYTHROWN << (short)id << pPacket.text()+1);
}
void CPlayer::msgADDEXTRA(CPacket& pPacket)
{
	CPacket packet;
	float iX = ((float)pPacket.readByte1())/2;
	float iY = ((float)pPacket.readByte1())/2;
	int item = pPacket.readByte1();
	level->items.add(new CItem(iX, iY, item));
	packet << (char)SADDEXTRA << pPacket.text() +1;

	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(packet);
	}
}
void CPlayer::msgTAKEEXTRA(CPacket& pPacket)
{
	CPacket packet;

	packet << (char)SDELEXTRA << pPacket.text() +1;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(packet);
	}

	float iX = ((float)pPacket.readByte1())/2;
	float iY = ((float)pPacket.readByte1())/2;
	CItem* item = NULL;
	for (int i = 0; i < level->items.count(); i++)
	{
		item = (CItem*)level->items[i];
		if (item->x == iX && item->y == iY)
			break;
		item = NULL;
	}
	if (item == NULL)
		return;

	if (pPacket[0]-32 == TAKEEXTRA)
		getItem(item->itemId);

	level->items.remove(item);
	delete item;
}

void CPlayer::msgDELEXTRA(CPacket& pPacket)
{
	msgTAKEEXTRA(pPacket);
}

void CPlayer::msgCLAIMPKER(CPacket& pPacket)
{
	CPlayer* other = (CPlayer*)playerIds[pPacket.readByte2()];
	if ( other == NULL ) return;
	if ( other == this ) return;

	// Sparring zone rating code.
	// Uses the glicko rating system.
	if ( level->sparZone )
	{
		float oldStats[4] = { rating, deviation, other->rating, other->deviation };

		// If the IPs are the same, don't update the rating to prevent cheating.
		if ( (CString() << other->playerSock->tcpIp()) == (CString() << this->playerSock->tcpIp()) ) return;

		float gSpar[2] = {1.0f / pow((1.0f+3.0f*pow(0.0057565f,2)*(pow(other->deviation,2))/pow(3.14159265f,2)),0.5f),	//Winner
					  	  1.0f / pow((1.0f+3.0f*pow(0.0057565f,2)*(pow(deviation,2))/pow(3.14159265f,2)),0.5f)};		//Loser
		float ESpar[2] = {1.0f / (1.0f + pow(10.0f,(-gSpar[1]*(other->rating-rating)/400.0f))),							//Winner
						  1.0f / (1.0f + pow(10.0f,(-gSpar[0]*(rating-other->rating)/400.0f)))};						//Loser
		float dSpar[2] = {1.0f / (pow(0.0057565f,2)*pow(gSpar[0],2)*ESpar[0]*(1.0f-ESpar[0])),							//Winner
						  1.0f / (pow(0.0057565f,2)*pow(gSpar[1],2)*ESpar[1]*(1.0f-ESpar[1]))};							//Loser

		float tWinRating = other->rating + (0.0057565f / ( 1.0f / pow(other->deviation,2) + 1.0f/dSpar[0])) * (gSpar[0] * (1.0f - ESpar[0]));
		float tLoseRating = rating + (0.0057565f / ( 1.0f / pow(deviation,2) + 1.0f/dSpar[1])) * (gSpar[1] * (0.0f - ESpar[1]));
  		float tWinDeviation = pow((1.0f/(1.0f/pow(other->deviation,2)+1/dSpar[0])),0.5f);
  		float tLoseDeviation = pow((1.0f/(1.0f/pow(deviation,2)+1/dSpar[1])),0.5f);

		// Cap the rating.
		tWinRating = CLIP( tWinRating, 0.0f, 4000.0f );
		tLoseRating = CLIP( tLoseRating, 0.0f, 4000.0f );
		tWinDeviation = CLIP( tWinDeviation, 50.0f, 350.0f );
		tLoseDeviation = CLIP( tLoseDeviation, 50.0f, 350.0f );

		// Update the Ratings.
		other->rating = tWinRating;
		other->deviation = other->oldDeviation = tWinDeviation;
		this->rating = tLoseRating;
		this->deviation = this->oldDeviation = tLoseDeviation;
		other->lastSparTime = this->lastSparTime = getSysTime();

		if ( oldStats[2] != other->rating || oldStats[3] != other->deviation )
			other->updateProp( RATING );
		if ( oldStats[0] != rating || oldStats[1] != deviation )
			this->updateProp( RATING );
	}
	else
	{
		if (!dontchangekills)
			other->kills++;

		if (apSystem)
		{
			if (other->ap > 0 && ap > 19)
			{
				other->ap -= ((other->ap/20+1)*(ap/20));
				if (other->ap < 0) other->ap = 0;
				other->apCounter = (other->ap < 20 ? aptime[0] : (other->ap < 40 ? aptime[1] : (other->ap < 60 ? aptime[2] : (other->ap < 80 ? aptime[3] : aptime[4]))));
				other->updateProp(PALIGNMENT);
			}
		}
	}
}

void CPlayer::msgBADDYPROPS(CPacket& pPacket)
{
	if ( id == -1 ) return;
	CBaddy* baddy = (CBaddy*)level->baddyIds[pPacket.readByte1()];
	if (baddy == NULL)
		return;
	CPacket baddyProps;
	baddyProps << pPacket.text()+2;
	for (int i = 1; i < level->players.count(); i++)
		((CPlayer*)level->players[i])->sendPacket(CPacket() << (char) SBADDYPROPS << (char)baddy->id << baddyProps);
	baddy->setProps(baddyProps);

}

void CPlayer::msgHURTBADDY(CPacket& pPacket)
{
	CPacket packet;
	packet << (char)SHURTBADDY << pPacket.text() + 1;
	((CPlayer*)level->players[0])->sendPacket(packet);
}

void CPlayer::msgADDBADDY(CPacket& pPacket)
{
	if (level->baddies.count() >= 50)
		return;

	int bX = pPacket.readByte1()/2;
	int bY = pPacket.readByte1()/2;
	char type = pPacket.readByte1();
	CString img = pPacket.readChars(pPacket.bytesLeft()-1);
	CBaddy* baddy = new CBaddy(bX, bY, type);
	baddy->id = level->createBaddyId(baddy);
	if (strlen(getDataFile(img.text())))
		baddy->image = img;
	if(pPacket.bytesLeft())
		baddy->power = MIN(pPacket.readByte1(), 12);

	baddy->respawn = false;
	level->baddies.add(baddy);

	CPacket baddyProps;
	baddyProps << (char)SBADDYPROPS << (char)baddy->id << baddy->getPropList();
	for (int i = 0; i < level->players.count(); i++)
		((CPlayer*)level->players[i])->sendPacket(baddyProps);

}

void CPlayer::msgSETFLAG(CPacket& pPacket)
{
	CString flagNameValue = pPacket.text() + 1;
	CString flagName;

	int pos = flagNameValue.find('=');
	if (pos >= 0)
		flagName = flagNameValue.copy(0, pos);
	else flagName = flagNameValue;

	/* Depreciated?
	if(flagName.length() > 999 || myFlags.count() > 999)
		return;
		*/

	if(flagName.find("this.") == 0)
		return;
	CStringList* flagList = &myFlags;
	bool found = false;
	if (flagName.find("server.") == 0)
	{
		if (dontaddserverflags)
			return;
		flagList = &serverFlags;
		sendServerFlag(flagNameValue);
	}

	for (int i = 0; i < flagList->count(); i++)
	{
		CString name;
		int pos = flagList->item(i).find('=');
		if (pos >= 0)
			name = flagList->item(i).copy(0, pos);
		else name = flagList->item(i);

		if (name == flagName)
		{
			found = true;
			if (flagNameValue == flagName << "=") flagList->remove(i); else
			flagList->replace(i, flagNameValue.text());
			break;
		}
	}
	if (!found)
		flagList->add(flagNameValue);

}

void CPlayer::msgUNSETFLAG(CPacket& pPacket)
{
	CString flagNameValue = pPacket.text() + 1;
	CString flagName;
	int pos = flagNameValue.find('=');
	if (pos >= 0)
		flagName = flagNameValue.copy(0, pos);
	else flagName = flagNameValue;

	CStringList* flagList = &myFlags;
	if (flagName.find("server.") >= 0)
	{
		if (dontaddserverflags)
			return;
		flagList = &serverFlags;
	}

	for (int i = 0; i < flagList->count(); i++)
	{
		CString name;
		int pos = flagList->item(i).find('=');
		if (pos >= 0)
			name = flagList->item(i).copy(0, pos);
		else name = flagList->item(i);

		if (name == flagName)
		{
			flagList->remove(i);
			break;
		}
	}

	if (flagList == &serverFlags)
	{
		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer* other = (CPlayer*)playerList[i];
			if (other != this)
				other->sendPacket(CPacket() << (char)SUNSETFLAG << flagName);
		}
	}

}

void CPlayer::msgOPENCHEST(CPacket& pPacket)
{
	int cX = pPacket.readByte1();
	int cY = pPacket.readByte1();
	CString chest = CString() << toString(cX) << ":" << toString(cY)
		<< ":" << levelName;
	if(myChests.findI(chest) >= 0)
		return;


	for (int i = 0; i < level->chests.count(); i++)
	{
		CChest* chest = (CChest*)level->chests[i];
		if (chest->x == cX && chest->y == cY)
		{
			getItem(chest->item);
			myChests.add(CString() << toString(cX) << ":" << toString(cY) << ":" << levelName);
			break;
		}

	}
}

void CPlayer::msgADDNPC(CPacket& pPacket)
{
	CString image = pPacket.readChars(pPacket.readByte1());
	CString codeFile = pPacket.readChars(pPacket.readByte1());
	float nX = (float)(pPacket.readByte1())/2;
	float nY = (float)(pPacket.readByte1())/2;

	if ( putnpcenabled == true )
		level->addNewNpc(image, codeFile, nX, nY);
}

void CPlayer::msgDELNPC(CPacket& pPacket)
{
	int id = pPacket.readByte3();
	CNpc* npc = (CNpc*)npcIds[id];
	if(npc == NULL)
		return;
	if(npc->level != level)
		return;

	for (int i = 0; i < playerList.count(); i++)
	{
		// SDELNPC doesn't work, so this terrible hack will do.
		/*
		((CPlayer*)playerList[i])->sendPacket(
			CPacket() << (char)SNPCPROPS << (int)npc->id <<
			(char)ACTIONSCRIPT << (short)0 << (char)VISFLAGS << (char)0 <<
			(char)BLOCKFLAGS << (char)0 << (char)NPCMESSAGE << (char)0 );
		((CPlayer*)playerList[i])->sendPacket(CPacket() << (char)SDELNPC << (int)npc->id);
		*/
		((CPlayer*)playerList[i])->sendPacket(CPacket() << (char)SDELNPC2 << (char)levelName.length() << levelName << (int)npc->id);
	}
	level->npcs.remove(npc);
	delete npc;
	compressAndSend();
}

void CPlayer::msgWANTFILE(CPacket& pPacket)
{
	CString fileName = pPacket.readString("");
	if(fileList.count() < 50)
		fileList.add(new COutFile(fileName, 0));
	else
		sendPacket(CPacket() << (char)GIFFAILED << fileName);
}

void CPlayer::msgNPCWEAPONIMG(CPacket& pPacket)
{
	if ( id == -1 ) return;
	sendLocally(CPacket() << (char)SNPCWEAPONIMG << (short)id << pPacket.text()+1);
}

void CPlayer::msgEMPTY25(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY25 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY25:\r\n" << pPacket.text(), false );
}

void CPlayer::msgHURTPLAYER(CPacket& pPacket)
{
	if ( id == -1 ) return;
	int pId = pPacket.readByte2();
	CPlayer* other = (CPlayer*)playerIds[pId];
	if (other == NULL)
		return;
	if(!(other->status & 1))
		other->sendPacket(CPacket() << (char)SPLAYERHURT <<
					  (short)id << pPacket.text() + 3);
}

void CPlayer::msgEXPLOSION(CPacket& pPacket)
{
	if (noExplosions)
		return;
	if ( id == -1 ) return;

	CPacket packet;
	packet << (char)SEXPLOSION  << (short)id << pPacket.text()+1;
	sendLocally(packet);
}

void CPlayer::msgPRIVMESSAGE(CPacket& pPacket)
{
	if ( id == -1 ) return;
	if (getTime() - lastMessage <= 4)
	{
		sendPacket(CPacket() << (char)SADMINMSG <<
				   "Server message:\xa7You can only send messages once every four seconds.");
		return;
	}

	lastMessage = getTime();
	bool jailed = false;
	if (jailLevels.find(levelName.text()) >= 0)
		jailed = true;

	int playerCount = pPacket.readByte2();
	if (pPacket.bytesLeft() < playerCount*2)
		return;

	CString message = pPacket.text() + 3 + (playerCount*2);
	if (message.length() > 1024)
	{
		sendPacket(CPacket() << (char)SADMINMSG << "Server message:\xa7Your message was too long.");
		return;
	}

	WordFilter.apply(this, message, FILTERC_PM);
	CString type;
	type << "\"\",\"" << (playerCount == 1 ? "Private message" : "Mass message") << ":\",";

	for (int i = 0; i < playerCount; i++)
	{
		int pId = pPacket.readByte2();
		CPlayer* other = (CPlayer*)playerIds[pId];
		if (other == NULL)
			continue;
		if (playerCount > 1 && (other->additionalFlags & 1) > 0)
			continue;

		if (jailed && !adminRights)
		{
			if (!other->adminRights)
			{
				sendPacket(CPacket() << (char)SPRIVMESSAGE << (short)other->id << "\"Server Message:\"," << "\"From jail you can only send PMs to admins (RCs).\"");
				continue;
			}
		}
		other->sendPacket(CPacket() << (char)SPRIVMESSAGE << (short)id << type << message);
	}
}

void CPlayer::msgDELNPCWEAPON(CPacket& pPacket)
{
	CString weapon = pPacket.text()+1;
	int index = myWeapons.findI(weapon.text());
	if (index >= 0)
		myWeapons.remove(index);
}

void CPlayer::msgPACKCOUNT(CPacket& pPacket)
{
	int pCount = pPacket.readByte2();
	if (pCount != packCount || packCount > 10000)
	{
		errorOut("errorlog.txt", CString() << accountName << " has sent an incorrect number of packets");
		if (packCount != pCount)
		{
			sendPacket(CPacket() << (char)DISMESSAGE << "The server received uncertified packets from your connection");
			deleteMe = true;
		}
	}
	packCount = 0;
}

void CPlayer::msgADDWEAPON2(CPacket& pPacket)
{
	CString name;
	int type = pPacket.readByte1();
	if (type == 0) //basic weapon (bomb, bow, etc)
	{
		int item = pPacket.readByte1();

		// The client tries to add the bomb when it connects.  Prevent this.
		if ( item == 8 && !allowBomb )
		{
			allowBomb = true;
			return;
		}

		if (item < 0 || item >= itemcount-1)
			return;
		if(myWeapons.findI(itemNames[item]) < 0)
			weaponSend.add(itemNames[item]);
	} else		//Npc weapon
	{
		int npcId = pPacket.readByte3();
		CNpc* npc = (CNpc*)npcIds[npcId];
		if (npc == NULL)
			return;
		name = npc->weaponName;
		if (!name.length())
			return;

		weaponSend.add(name);
		CWeapon* weapon = NULL;
		for (int i = 0; i < weaponList.count(); i++)
		{
			weapon = (CWeapon*)weaponList[i];
			if (weapon->name == name)
			{
				if (weapon->modTime == level->modTime)
					return;
				else break;
			}
			weapon = NULL;
		}
		if (weapon == NULL)
		{
			weapon = new CWeapon;
			weaponList.add(weapon);
		}
		weapon->name = name;
		weapon->image = npc->image;
		weapon->modTime = level->modTime;
		weapon->code = npc->clientCode;
		saveWeapons("weapons.txt");
	}
}
void CPlayer::msgUPDATEFILE(CPacket& pPacket)
{
	long long modTime = pPacket.readByte5();
	CString fileName = pPacket.readString("");

	for (int i = 0; i < defaultFiles.count(); ++i)
	{
		if (fileName == defaultFiles.item(i))
		{
			sendPacket(CPacket() << (char)SFILEUPTODATE << fileName);
			return;
		}
	}

	if(fileList.count() <= 50)
		fileList.add(new COutFile(fileName, modTime));
	else
		sendPacket(CPacket() << (char)GIFFAILED << fileName);
}

void CPlayer::msgEMPTY36(CPacket& pPacket)
{
	// Possibly hitobjects
	//CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY36 was just sent.\"" );
	//errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY36:\r\n" << pPacket.text(), false );
}

void CPlayer::msgLANGUAGE(CPacket& pPacket)
{
	language = pPacket.text()+1;
}

void CPlayer::msgTRIGGERACTION(CPacket& pPacket)
{
	if ( id == -1 ) return;
	//Uncomment below if you need these values for
	//NPC server
	//int npcId = pPacket.readByte3();
	//float x1 = pPacket.readByte1();
	//float x2 = pPacket.readByte1();
	//CString action = pPacket.readString("");
	CPacket packet;
	packet << (char)STRIGGERACTION << (short)id << pPacket.text()+1;
	for(int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if(other != this)
			other->sendPacket(packet);
	}
}

void CPlayer::msgMAPINFO(CPacket& pPacket)
{
//	errorOut( "debuglog.txt", CString() << accountName << " sent packet MAPINFO:\r\n" << pPacket.text(), false );
}

void CPlayer::msgCSHOOT(CPacket& pPacket)
{
	if ( id == -1 ) return;
	CPacket packet;
	packet << (char)SSHOOT << (short)id << pPacket.text() + 1;
	for(int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if(other != this)
			other->sendPacket(packet);
	}
}

void CPlayer::msgEMPTY41(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY41 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY41:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY42(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY42 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY42:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY43(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY43 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY43:\r\n" << pPacket.text(), false );
}

void CPlayer::msgSLISTPROCESSES(CPacket& pPacket)
{/*
	if (cheatwindows.count() <= 0)
		return;

	for (int i = 0; i < cheatwindows.count(); i++)
	{
		CBuffer c = cheatwindows[i]; c.toLower();
		pPacket.toLower();
		if (pPacket.find(c.text()) >= 0)
		{
			CBuffer pLog = CBuffer() << "Player " << accountName << " (ip " << playerSock->tcpIp() << ") is using cheating tools:\n" << cheatwindows[i];
			errorOut("cheatwindows.txt", pLog);
			sendRCPacket(CPacket() << (char)DRCLOG << pLog);

			if (cheatwindowsban)
			{
				pLog = CBuffer() << getTimeStr(0) << "\n" << pLog;
				pLog.tokenize();
				banned = true;
				banReason = CBuffer() << (banReason.length() > 0 ? banReason << ",," << pLog : pLog);
			}

			sendPacket(CPacket() << (char)DISMESSAGE << "Hacking is not allowed!");
			deleteMe = true;
			break;
		}
	}*/
}

void CPlayer::msgEMPTY45(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY45 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY45:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY46(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY46 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY46:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY47(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY47 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY47:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY48(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY48 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY48:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY49(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY49 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY49:\r\n" << pPacket.text(), false );
}

void CPlayer::msgEMPTY50(CPacket& pPacket)
{
	CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"EMPTY50 was just sent.\"" );
	errorOut( "debuglog.txt", CString() << accountName << " sent packet EMPTY50:\r\n" << pPacket.text(), false );
}

//RC
inline bool CPlayer::hasRight(int pRight)
{
	return ((adminRights & pRight) > 0 && (CBuffer() << playerSock->tcpIp()).match( adminIp.text() ) == true);
}

inline bool CPlayer::hasStaff()
{
	return (staffList.findI(accountName) >= 0);
}

/* Sick of fucking graals retarded-ways.. and my attempts to find ways to patch it.. fucking graal.. */
void CPlayer::msgSWANTSOPTIONS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view the server options." );
		return;
	}

	CStringList serverOptions;
	serverOptions.load("serveroptions.txt");

	sendPacket(CPacket() << (char)DSENDSOPTIONS << serverOptions.join("\n").tokenize());
}

void CPlayer::msgSSETOPTIONS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANEDITSERVEROPTION))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to set the server options." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to change the server options.");
		return;
	}

	CString data = pPacket.text() + 1;
	data.untokenize();

	CStringList newOps;
	newOps.load(data.text(), "\n");

	// Workaround a limitation/feature of strtok().
	for ( int i = 0; i < newOps.count(); ++i )
		newOps[i].removeAll( "\r" );

	if (!hasRight(CANCHANGESTAFFACC))
	{
		CStringList serverOptions;
		serverOptions.load("serveroptions.txt");
		for ( int i = 0; i < newOps.count(); i++ )
		{
			CString name = newOps[i].copy(0, newOps[i].find("="));
			name.trim();
			if ( adminNames.findI(name) >= 0 )
			{
				CBuffer opt( (CBuffer)newOps[i] );
				int sOptLoc = serverOptions.findIn( name );
				if ( sOptLoc >= 0 && ((CBuffer() << (CBuffer)serverOptions[sOptLoc]).trimLeft()).copy(0, name.length()) == name )
					newOps.replace( opt, serverOptions[sOptLoc] );
			}
		}
	}

	data = newOps.join("\n");
	data.save("serveroptions.txt");
	loadSettings("serveroptions.txt");
	errorOut( "rclog.txt", CString() << accountName << " has updated the server options." );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has updated the server options.");
}

void CPlayer::msgWANTRCFOLDERS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view the folder configuration." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to view the folder configuration.");
		return;
	}

	CStringList temp;
	temp.load("foldersconfig.txt");

	// Don't allow .. in the folder path.
	for ( int i = 0; i < temp.count(); ++i )
	{
		if ( ((CBuffer)temp[i]).find( ".." ) != -1 )
		{
			temp.remove(i);
			--i;
		}
	}
	sendPacket(CPacket() << (char)SSENDFOLDER << temp.join("\n").tokenize());
}

void CPlayer::msgSETRCFOLDERS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANEDITFOLDEROPTION))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to change the folder configuration." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to change the folder configuration.");
		return;
	}

	// Save the foldersconfig.txt and load it.
	CString data = pPacket.text() + 1;
	data.untokenize();
	data.save("foldersconfig.txt");
	updateFile( "foldersconfig.txt" );

	errorOut( "rclog.txt", CString() << accountName << " has updated the folder configuration." );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has updated the folder configuration.");
}

void CPlayer::msgSETRESPAWN(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgSETHORSELIFE(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgSETAPINCRTIME(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgSETBADDYRESP(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgWANTPLPROPS(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgSETPLPROPS(CPacket& pPacket)
{
	CPlayer* player = (CPlayer*)playerIds[pPacket.readByte2()];
	if(player == NULL)
		return;

	if (type != CLIENTRC || (player->accountName != accountName && !hasRight(CANSETATTRIBS)) || (player->accountName == accountName && !hasRight(CANSETOWNATTRIBS)))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to set a player's attributes." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to set attributes.");
		return;
	}

	player->setAccPropsRc(pPacket, this);
	player->saveAccount();
	errorOut( "rclog.txt", CString() << accountName << " set the attributes of player " << player->accountName );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " set the attributes of player " << player->accountName);
}

void CPlayer::msgDISPLAYER(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANDISCONNECT))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to disconnect a player." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to disconnect players.");
		return;
	}
	CPlayer* other = (CPlayer*)playerIds[pPacket.readByte2()];
	if(other == NULL)
		return;
	other->sendPacket(CPacket() << (char)DISMESSAGE << "The server administrator has disconnected you.");
	other->deleteMe = true;
	errorOut( "rclog.txt", CString() << accountName << " disconnected " << other->accountName );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " disconnected " << other->accountName );
}

void CPlayer::msgUPDLEVELS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANUPDATELEVEL))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to update a level." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to update levels.");
		return;
	}
	int levelCount = pPacket.readByte2();
	for(int i = 0; i < levelCount; i++)
	{
		int len = pPacket.readByte1();
		if(len > 0)
		{
			CString fileName = pPacket.readChars(len);
			CLevel::updateLevel(fileName);
		}
	}
}

void CPlayer::msgADMINMSG(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANADMINMSG))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to send an admin message." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to send admin messages.");
		return;
	}

	CString message = pPacket.readString("");
	for(int i = 0; i < playerList.count(); i++)
	{
		CPlayer* player = (CPlayer*)playerList[i];
		if(player != this)
			player->sendPacket(CPacket() << (char)SADMINMSG << "Admin " << accountName << ":\xa7" << message);
	}
}

void CPlayer::msgPRIVADMINMSG(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANADMINMSG))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to send an admin message." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to send admin messages.");
		return;
	}
	CPlayer* other = (CPlayer*)playerIds[pPacket.readByte2()];
	if(other == NULL)
		return;
	other->sendPacket(CPacket() << (char)SADMINMSG << "Admin " << accountName << ":\xa7" << pPacket.readString(""));
}

void CPlayer::msgLISTRCS(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgDISCONNECTRC(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgAPPLYREASON(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
}

void CPlayer::msgLISTSFLAGS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view the server flags." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to view the server flags.");
		return;
	}
	CPacket packet;
	packet << (char)SSERVERFLAGS << (short)serverFlags.count();
	for(int i = 0; i < serverFlags.count(); i++)
		packet << (char)serverFlags[i].length() << serverFlags[i];
	sendPacket(packet);
}

void CPlayer::msgSETSFLAGS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANSETSERVERFLAG))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to set the server flags." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to change the server flags.");
		return;
	}
	int count = pPacket.readByte2();
	serverFlags.clear();
	for(int i = 0; i < count; i++)
	{
		CString flag = pPacket.readChars(pPacket.readByte1());
		serverFlags.add(flag);

		// Send the new flags to all the players.
		for ( int j = 0; j < playerList.count(); ++j )
		{
			CPlayer* other = (CPlayer*)playerList[j];
			other->sendPacket(CPacket() << (char)SSETFLAG << flag);
		}
	}
	errorOut( "rclog.txt", CString() << accountName << " has updated the server flags." );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has updated the server flags.");
}

void CPlayer::msgDADDACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGESTAFFACC))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to add a new account." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to add new accounts.");
		return;
	}

	CString accname = pPacket.readChars(pPacket.readByte1());
	CString pass = pPacket.readChars(pPacket.readByte1());
	CString email = pPacket.readChars(pPacket.readByte1());
	bool banned = (pPacket.readByte1()>0);
	bool onlyLoad = (pPacket.readByte1()>0);
	pPacket.readByte1(); // adminlvl
	CAccount newAccount;
	newAccount.accountName = accname;
	newAccount.banned = banned;
	newAccount.loadOnly = onlyLoad;
	newAccount.saveAccount(true);
	errorOut( "rclog.txt", CString() << accountName << " has created a new account: " << newAccount.accountName );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has created the new account: " << newAccount.accountName);
}

void CPlayer::msgDDELACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGESTAFFACC))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to delete an account." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to delete accounts.");
		return;
	}

	// Get the account name.
	CString accname = pPacket.readString("");

	// Don't let anybody delete the defaultaccount.
	if ( accname == "defaultaccount" )
	{
		sendPacket( CPacket() << (char)DRCLOG << "Server: Not allowed to delete the default account.  If you wish to do this, manually delete the account from the server." );
		return;
	}

	// Remove the account.
	if (remove(CString(CString() << "accounts" << fSep << accname << ".txt").text()) == 0)
	{
		errorOut( "rclog.txt", CString() << accountName << " has deleted the account:" << accname );
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has deleted the account: " << accname);
	}
	else
		sendPacket(CPacket() << (char)DRCLOG << "Server: Invalid account.");
}

void CPlayer::msgDWANTACCLIST(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view the accounts list." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized for viewing the accounts list.");
		return;
	}

	CString name = pPacket.readChars(pPacket.readByte1());
	CString condition = pPacket.readChars(pPacket.readByte1());
	sendPacket(CPacket() << (char)DACCLIST << getAccountList(name, condition));
}

void CPlayer::msgDWANTPLPROPS(CPacket& pPacket)
{
	CPlayer *player = (CPlayer *)playerIds[pPacket.readByte2()];
	if (player != NULL)
		msgDWANTACCPLPROPS(CPacket() << (char)player->accountName.length() << player->accountName);
}

void CPlayer::msgDWANTACCPLPROPS(CPacket& pPacket)
{
	if ( type != CLIENTRC || !hasStaff() )
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view a player's attributes." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to load player attributes.");
		return;
	}

	CString accname = pPacket.readChars(pPacket.readByte1());
	CPlayer *player = findPlayerId(accname, true);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	sendPacket(CPacket() << (char)DACCPLPROPS  << (short)player->id << player->getAccPropsRC());
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " loaded the attributes of player " << accname);
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDRESETPLPROPS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANRESETATTRIBS))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to reset an account." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to reset accounts.");
		return;
	}

	CString accname = pPacket.readString("");
	CPlayer *player = findPlayerId(accname, true);
	if ( player == 0 )
	{
		// Player isn't playing, so we will temporarily create the player.
		player = new CPlayer(0);
		player->accountName = accname;
	}

	player->loadWorldPropsIni();
	player->saveAccount();
	if (player->id >= 0)
	{
		player->sendPacket(CPacket() << (char)DISMESSAGE << "Your account was reset");
		player->deleteMe = true;
	}
	errorOut( "rclog.txt", CString() << accountName << " has reset the attributes of player " << accname );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has reset the attributes of player " << accname);

	if (player->id == -1)
		delete player;
}

void CPlayer::msgDSETACCPLPROPS(CPacket& pPacket)
{
	CString accname = pPacket.readChars(pPacket.readByte1());
	if (type != CLIENTRC || (accname != accountName && !hasRight(CANSETATTRIBS)) || (accname == accountName && !hasRight(CANSETOWNATTRIBS)))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to change a player's attributes." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to change player attributes.");
		return;
	}

	// Only people with CANCHANGESTAFFACC can alter the default account.
	if ( !hasRight(CANCHANGESTAFFACC) && accname == "defaultaccount" )
	{
		sendRCPacket( CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to alter the default account." );
		return;
	}

	CPlayer *player = findPlayerId(accname, true);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	errorOut( "rclog.txt", CString() << accountName << " set the attributes of player " << player->accountName );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " set the attributes of player " << player->accountName);
	player->setAccPropsRc(pPacket, this);
	player->saveAccount();
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDWANTACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view a player's account." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to view the player's account.");
		return;
	}

	CString accname = pPacket.readString("");
	CPlayer *player = findPlayerId(accname, true);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	sendPacket(CPacket() << (char)DACCOUNT << (char)accname.length() << accname << (char)0 << (char)0 << (char)0 << (char)player->loadOnly << (char)0 << (char)worldName.length() << worldName << (char)0 << (char)0);
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDSETACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGESTAFFACC))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to set a player's account." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to set the player's account.");
		return;
	}

	CString accname = pPacket.readChars(pPacket.readByte1());
	CString pass = pPacket.readChars(pPacket.readByte1());
	CString email = pPacket.readChars(pPacket.readByte1());
	CPlayer *player = findPlayerId(accname, true);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	pPacket.readByte1(); // skip ban
	player->loadOnly = (pPacket.readByte1()>0);
	pPacket.readByte1(); // skip adminlevel
	pPacket.readChars(pPacket.readByte1());	//skip playerworld
	pPacket.readChars(pPacket.readByte1()); // skip ban reason
	player->saveAccount(true);

	errorOut( "rclog.txt", CString() << accountName << " has modified the account of player " << player->accountName );
	if (player->id == -1)
		delete player;
}

/*
Available commands for GServer:
/stats: displays server info
/playerinfo accountname: displays info about an online player
Information about Fraszle:
Upgrade status: gold
Nick name: *Fraszle
Community name: Fraszle
Platform: win
/open accountname: opens the player attributes window
/openrights accountname: opens the rights
/opencomments accountname: opens the comments
/openaccess accountname: opens the (new) ban info
/openacc accountname: opens the account
/openprofile accountname: opens the profile
/reset accountname: resets the account
/localbans: lists all local bans (requires RC2)
/staffactivity accountname: lists staff actions (requires RC2)
/find filepattern: searches for a game files in levels/
/finddef filepattern: searches for default (classic) game files
/updatelevel level[,level]: reloads levels form hard disk
/updatelevelall: updates all loaded levels
/refreshfilelist: scans the folders again (only use when /find is not working correctly)
/shutdown: closes the server
*/

void CPlayer::msgDRCCHAT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to send an RC chat message." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to send RC chat.");
		return;
	}

	CPacket forward;
	CStringList words;
	CString message = pPacket.text()+1;
	if (message.length() < 1)
		return;

	words.load(message.text(), " ");

	if(message[0] != '/')
	{
		sendRCPacket(CPacket() << (char)DRCLOG << nickName << ": " << message);
		return;
	}

	if (words[0] == "/help")
	{
		for(int i = 0; i < RCHelpMessage.count(); i++)
		{
			sendPacket(CPacket() << (char)DRCLOG << RCHelpMessage[i]);
		}
	}
	else if (words[0] == "/find" && words.count() >= 2)
	{
		words.remove(0);
		CString search = words.join(" ");
		CStringList found;

		// Search for files
		for (int i = 0; i < myFolders.count(); i++)
		{
			myFolders[i].setRead(0);
			char *rights = myFolders[i].readString(" ");
			CString folder = myFolders[i].readString("");
			if (strchr(rights, 'r') == NULL)
				continue;

			CStringList files;
			getSubFiles(folder.text(), files);
			for (int i = 0; i < files.count(); i++)
			{
				if (files[i].match(search.text()))
				{
					found.add(CBuffer() << folder.copy(0, folder.length()-1) << files[i]);
					if (found.count() >= 50)
						break;
				}
			}
		}
		//Game files found (relative to levels/, max 50 files):
		//era_sewersa.gmap: downloadable, level, 113994 byte, 2007-08-29 01:02:11
		// Scan through files
		if (found.count() > 0)
		{
			sendPacket(CPacket() << (char)DRCLOG << "Server: Game files found (relative to world/, max 50 files):");
			for (int i = 0; i < found.count(); i++)
			{
				char buffer[80];
				time_t t = getFileModTime(found[i].text());
				struct tm *timeinfo = localtime(&t);
				strftime (buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",timeinfo);
				sendPacket(CPacket() << (char)DRCLOG << found[i] << ": downloadable, NULL, " << toString(getFileSize(found[i].text())) << " byte, " << buffer);
			}
		}
	}
	else if (words[0] == "/open" && hasRight(CANVIEWATTRIBS))
	{
		words.remove(0);
		CString account = words.join(" ");
		msgDWANTACCPLPROPS(CPacket() << (char)account.length() << account);
	}
	else if (words[0] == "/openacc" && hasRight(CANVIEWATTRIBS))
	{
		words.remove(0);
		msgDWANTACCOUNT(CPacket() << words.join(" "));
	}
	else if (words[0] == "/openban" && hasRight(CANBAN))
	{
		words.remove(0);
		msgDEDITBAN(CPacket() << words.join(" "));
	}
	else if (words[0] == "/opencomments" && hasRight(CANCHANGECOMMENTS))
	{
		words.remove(0);
		msgDWANTCOM(CPacket() << words.join(" "));
	}
	else if (words[0] == "/openrights" && hasRight(CANCHANGERIGHTS))
	{
		words.remove(0);
		msgDWANTRIGHTS(CPacket() << words.join(" "));
	}
	else if (words[0] == "/reset" && hasRight(CANRESETATTRIBS))
	{
		words.remove(0);
		msgDRESETPLPROPS(CPacket() << words.join(" "));
	}
	else if (words[0] == "/refreshservermessage" && hasRight(CANEDITSERVEROPTION))
	{
		loadServerMessage();
		errorOut( "rclog.txt", CString() << accountName << " reloaded the server message." );
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " reloaded the server message.");
	}
	else if(words[0] == "/shutdown" && hasRight(CANCHANGESTAFFACC))
	{
		errorOut( "rclog.txt", CString() << accountName << " shutdown the server." );
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " shutdown the server.");
		shutdownServer(0);
	}
	else if (words[0] == "/updatelevel" && hasRight(CANUPDATELEVEL))
	{
		CStringList levels;
		for (int i = 1; i < words.count(); i++)
			CLevel::updateLevel(words[i]);
	}
	else if (words[0] == "/updatelevelall" && hasRight(CANUPDATELEVEL))
	{
		for ( int i = 0; i < levelList.count(); ++i )
		{
			CLevel* level = (CLevel*)levelList[i];
			CLevel::updateLevel( level->fileName );
		}
		errorOut( "rclog.txt", CString() << accountName << " updated all the levels." );
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " updated all the levels.");
	}
}

//reminder
void CPlayer::msgDGETPROFILE(CPacket& pPacket)
{
	if ( id == -1 ) return;
	ListServer_Send(CPacket() << (char)SLSPROFREQ << (short)id << pPacket << "\n");
}

void CPlayer::msgDSETPROFILE(CPacket& pPacket)
{
	CString account = pPacket.readChars(pPacket.readByte1());
	if (findPlayerId(account) != this)
		return;

	ListServer_Send(CPacket() << (char)SLSPROFSET << pPacket << "\n");
}

void CPlayer::msgWARPPLAYER(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANWARPPLAYER))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to warp a player." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to warp players.");
		return;
	}

	CPlayer* player = (CPlayer*)playerIds[pPacket.readByte2()];
	if(player == NULL)
		return;

	float nX = (float)(pPacket.readByte1())/2;
	float nY = (float)(pPacket.readByte1())/2;
	CString name = pPacket.readString("");
	player->warp(name, nX, nY);
	errorOut( "rclog.txt", CString() << accountName << " has warped " << player->accountName << " to " << name << " (" << toString(nX) << ", " << toString(nY) < ")." );
}
void CPlayer::msgDWANTRIGHTS(CPacket& pPacket)
{
	CString accname = pPacket.readString("");
	if (type != CLIENTRC || (accname != accountName && !hasRight(CANCHANGERIGHTS)))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view a player's admin rights." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to view admin rights.");
		return;
	}

	int rights = 0;
	CString folders, ip;
	CPlayer *player = findPlayerId(accname);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	folders = player->myFolders.join("\n").tokenize();
	ip = player->adminIp;
	rights = player->adminRights;
	sendPacket(CPacket() << (char)SSENDRIGHTS << (char)accname.length() << accname << (long long)rights << (char)ip.length() << ip << (short)folders.length() << folders);
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " loaded the rights of " << accname);
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDSETRIGHTS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGERIGHTS))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to set a player's admin rights." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to change admin rights.");
		return;
	}

	CBuffer accname = pPacket.readChars(pPacket.readByte1());
	CPlayer *player = findPlayerId(accname);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	// Don't allow RCs to give rights they don't have.
	// Only affect people who don't have CANCHANGESTAFFACC.
	int n_adminRights = (int)pPacket.readByte5();
	if ( !hasRight(CANCHANGESTAFFACC) )
	{
		for ( int i = 0; i < 20; ++i )
		{
			if ( (this->adminRights & (1<<i)) == 0 )
				n_adminRights &= ~(1<<i);
		}
	}

	// Don't allow you to remove your own CANCHANGESTAFFACC or CANCHANGERIGHTS.
	if ( player == this )
	{
		if ( (n_adminRights & CANCHANGESTAFFACC) == 0 )
			n_adminRights |= CANCHANGESTAFFACC;
		if ( (n_adminRights & CANCHANGERIGHTS) == 0 )
			n_adminRights |= CANCHANGERIGHTS;
	}

	player->adminRights = n_adminRights;
	player->adminIp = pPacket.readChars((unsigned char)pPacket.readByte1());

	// Untokenize and load the directories.
	CString temp(pPacket.readChars(pPacket.readByte2()));
	temp.untokenize();
	player->myFolders.load(temp.text(), "\n");

	// Remove any invalid directories.
	for (int i = 0; i < player->myFolders.count(); i++)
	{
		if (player->myFolders[i].find(':') >= 0 || player->myFolders[i].find("..") >= 0 || player->myFolders[i].find(" /*") >= 0)
		{
			player->myFolders.remove(i);
			i--;
		}
	}

	if ( player->type == CLIENTRC && player->ftpOn == true ) player->msgDWANTFTP(CPacket() << "");
	player->saveAccount(true);
	if (player->id == -1)
	{
		CPlayer *pl = findPlayerId(accname);
		if (pl != NULL) pl->loadDBAccount(accname);
		delete player;
	}
	errorOut( "rclog.txt", CString() << accountName << " has set the rights of " << accname );
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has set the rights of " << accname);
}

void CPlayer::msgDWANTCOM(CPacket&pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGECOMMENTS))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view a player's comments." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to view comments.");
		return;
	}

	CString accname = pPacket.readString("");
	CPlayer *player = findPlayerId(accname, true);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	sendPacket(CPacket() << (char)SSENDCOM << (char)accname.length() << accname << player->comments);
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " loaded the comments of " << accname);
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDSETCOM(CPacket&pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGECOMMENTS))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to set a player's comments." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to change admin comments.");
		return;
	}

	int len = pPacket.readByte1();
	if (len > 0)
	{
		CString accname = pPacket.readChars(len);
		CString comment = pPacket.readString("");
		CPlayer *player = findPlayerId(accname, true);
		if (player == NULL)
		{
			player = new CPlayer(NULL);
			if (!player->loadDBAccount(accname))
			{
				delete player;
				return;
			}
		}

		player->comments = comment;
		player->saveAccount(true);
		errorOut( "rclog.txt", CString() << accountName << " has set the comments of " << player->accountName );
		if (player->id == -1)
			delete player;
	}
}

void CPlayer::msgDEDITBAN(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to view a player's ban." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to view a player's ban.");
		return;
	}

	CString accname = pPacket.readString("");
	CPlayer *player = findPlayerId(accname, true);
	if (player == NULL)
	{
		player = new CPlayer(NULL);
		if (!player->loadDBAccount(accname))
		{
			delete player;
			return;
		}
	}

	sendPacket(CPacket() << (char)SSENDBAN << (char)accname.length() << accname << (char)player->banned << player->banReason);
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has loaded the ban of " << accname);
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDSETBAN(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANBAN))
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to ban a player." );
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: " << accountName << " is not authorized to ban players.");
		return;
	}

	int len = pPacket.readByte1();
	if (len > 0)
	{
		CString accname = pPacket.readChars(len);
		bool ban = (pPacket.readByte1() > 0);
		CString reason = pPacket.readString("");
		CPlayer *player = findPlayerId(accname, true);
		if (player == NULL)
		{
			player = new CPlayer(NULL);
			if (!player->loadDBAccount(accname))
			{
				delete player;
				return;
			}
		}

		player->banned = ban;
		player->banReason = reason;
		player->saveAccount();
		errorOut( "rclog.txt", CString() << accountName << " has set the ban of " << accname );
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has set the ban of " << accname);
		if (player->id == -1)
			delete player;
	}
}

void CPlayer::msgDWANTFTP(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to open a FileBrowser session." );
		return;
	}

	sendPacket(CPacket() << (char)SFOLDERFTP << myFolders.join("\n").tokenize());
	sendPacket(CPacket() << (char)STEXTFTP << "Welcome to the FileBrowser");

	if (myFolders.count() > 0)
	{
		// Set the default folder in case we can't find the last folder.
		myFolders[0].setRead(0);
		CBuffer rights = myFolders[0].readString(" ");
		CBuffer folder = myFolders[0].readString("");
		folder.remove(folder.length() - 1, 1);

		// Now check and see if we can find the last folder
		bool fLastFolder = false;
		if ( lastFolder.length() > 0 )
		{
			// Loop through all the folders.
			for ( int i = 0; i < myFolders.count() && fLastFolder == false; ++i )
			{
				myFolders[i].setRead(0);
				CBuffer mrights = myFolders[i].readString(" ");
				CBuffer mfolder = myFolders[i].readString("");
				mfolder.remove(mfolder.length() - 1, 1);
				//myFolders[i].setRead(0);

				// Check if lastFolder is equal to our folder.
				if ( lastFolder == mfolder )
				{
					rights = mrights;
					folder = lastFolder;
					fLastFolder = true;
				}
			}
		}

		// Send the folder.
		sendPacket(CPacket() << (char)SSENDFTP << (char)folder.length() << folder << listFiles(folder.text(), rights.text()));

		ftpOn = true;
	}
	//myFolders[0].setRead(0);
}

void CPlayer::msgDCHANGEFTP(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
		return;

	CBuffer rights, folder, temp;
	temp = pPacket.text()+1;

	for (int i = 0; i < myFolders.count(); i++)
	{
		myFolders[i].setRead(0);
		rights = myFolders[i].readString(" ");
		folder = myFolders[i].readString("");
		folder.remove(folder.length() - 1, 1);
		if (folder == temp) break; else folder.clear();
	}

	if (folder.length() <= 0)
		return;

	lastFolder = temp;
	sendPacket(CPacket() << (char)STEXTFTP << "Folder changed to " << lastFolder);
	sendPacket(CPacket() << (char)SSENDFTP << (char)lastFolder.length() << lastFolder << listFiles(lastFolder.text(), rights.text()));
}

void CPlayer::msgDENDFTP(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
		return;

	ftpOn = false;
}

void CPlayer::msgDFILEFTPDOWN(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to download a file through a FileBrowser session." );
		return;
	}

	CString shortName = pPacket.text() + 1;
	fileList.add(new COutFile(CString() << lastFolder << shortName));
	errorOut( "rclog.txt", CString() << accountName << " downloaded file " << shortName );
	sendPacket(CPacket() << (char)STEXTFTP << "Downloaded file " << shortName);
}

void CPlayer::msgDFILEFTPUP(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to upload a file through a FileBrowser session." );
		return;
	}

	CBuffer shortName, fileData, fileName;
	shortName = pPacket.readChars((unsigned char)pPacket.readByte1());
	fileName = CString() << lastFolder << shortName;
	fileData = pPacket.copy(pPacket.getRead());
	fileData.save(fileName.text());
	errorOut( "rclog.txt", CString() << accountName << " uploaded file " << fileName );
	sendPacket(CPacket() << (char)STEXTFTP << "Uploaded file " << fileName);
	updateFile(shortName.text());
}

void CPlayer::msgDFILEFTPMOV(CPacket& pPacket)
{
	// learn 2 fucking order...
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to move a file through a FileBrowser session." );
		return;
	}

	CBuffer f1, f2, f3, f4;
	f1 = pPacket.readChars((unsigned char)pPacket.readByte1());
	f2 = pPacket.readString("");
	f2.removeAll("\"");
	f1 << f2;
	f4 << lastFolder << f2;

	errorOut( "rclog.txt", CString() << accountName << " moved file " << f4 << " to " << f1 );

	f3.load(f4.text());
	f3.save(f1.text());
	remove(f4.text());
}

void CPlayer::msgDFILEFTPDEL(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to delete a file through a FileBrowser session." );
		return;
	}

	// Get the file name.
	CString fileName = CString() << lastFolder << pPacket.text() + 1;

	// Don't let us delete defaultaccount.
	if ( fileName == "defaultaccount" )
	{
		sendPacket( CPacket() << (char)STEXTFTP << "Server: Not allowed to delete the default account.  If you wish to do this, manually delete the account from the server." );
		return;
	}

	// Remove the file.
	remove(fileName.text());
	errorOut( "rclog.txt", CString() << accountName << " deleted file " << fileName );
	sendPacket(CPacket() << (char)STEXTFTP << "Deleted file " << fileName);
}

void CPlayer::msgDFILEFTPREN(CPacket& pPacket)
{
	if (type != CLIENTRC)
	{
		if ( type != CLIENTRC ) errorOut("rclog.txt", CString() << "[Hack] " << accountName << " attempted to rename a file through a FileBrowser session." );
		return;
	}

	// Get the file names.
	CString f1 = CString() << lastFolder << pPacket.readChars((unsigned char)pPacket.readByte1());
	CString f2 = CString() << lastFolder << pPacket.readChars((unsigned char)pPacket.readByte1());

	// Don't let us rename defaultaccount.
	if ( f1 == "defaultaccount" || f2 == "defaultaccount" )
	{
		sendPacket( CPacket() << (char)STEXTFTP << "Server: Not allowed to rename the default account." );
		return;
	}

	// Do the renaming.
	rename(f1.text(), f2.text());
	errorOut( "rclog.txt", CString() << accountName << " renamed file " << f1 << " to " << f2 );
	sendPacket(CPacket() << (char)STEXTFTP << "Renamed file " << f1 << " to " << f2);
}
