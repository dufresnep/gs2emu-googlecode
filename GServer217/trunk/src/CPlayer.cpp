// GraalReborn Server - Revision 51
// (C) GraalReborn 2007

#include "CBaddy.h"
#include "CDatabase.h"
#include "CIni.h"
#include "CListServer.h"
#include "CPlayer.h"
#include "CWeapon.h"
#include "zlib.h"
#include "main.h"
#include <sys/stat.h>

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
			&CPlayer::msgTRIGGERACTION,&CPlayer::msgEMPTY39,
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
	false, true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	true,  true,  true,  true,  true,  true,  // 54-59
	true,  true,  true,  true,  true,  true,  // 60-65
	true,  true,  true,  true,  true,  true,  // 66-71
	true,  true,  true, false, false, false,  // 72-77
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
	false, true,  false, false, true,  true,  // 24-29
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
	false, true , true,  true , true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	false, false, false, false, false, false, // 54-59
	false, false, false, false, false, false, // 60-65
	false, false, false, false, false, false, // 66-71
	false, false, false, false, false, false, // 72-77
};

bool sendRcProps[propscount] =
{
	false,true ,true ,true ,true ,true ,
	true ,true ,true ,true ,true ,true ,
	false ,true ,false,true ,true,false,
	true ,false ,true, true ,true,true,		   //19 Status
	false ,false ,true ,true,true,true,
	true ,false ,true ,false ,false ,true ,
	false ,false,false ,false,false,false,
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false  // 48-53
};

char* itemNames[] = {
	"greenrupee","bluerupee","redrupee","bombs","darts","heart","glove1",
	"bow","bomb","shield","sword","fullheart","superbomb","battleaxe","goldensword",
	"mirrorshield", "glove2","lizardshield","lizardsword","goldrupee","fireball",
	"fireblast","nukeshot","joltbomb", "spinattack"
};

#define itemcount 25

CPlayer::CPlayer(CSocket* pSocket)
{
	playerSock = pSocket;
	loadOnly = deleteMe = allowBomb = false;
	firstPacket = firstLevel = true;
	key = adminRights =  0;
	lastData = lastMovement = lastSave = loginTime = time(NULL);
	lastNick = 0;

	iterator = 0x04A80B38;
	x = y = 32;
	z = 0;
	darts = 10;
	bombs = 5;
	rubins = 0;
	glovePower = bombPower = swordPower = shieldPower = 1;
	power = maxPower = 3;
	kills = deaths = udpPort = statusMsg= rating =0;
	sprite = 2;
	status = 20;
	ap = 50;
	onlineSecs = horseBushes = magicPoints = 0;
	apCounter = 100;
	type = lastIp = additionalFlags = 0;
	memset(colors, 0, sizeof(colors));
	levelName = "";
	gAni = "idle";
	headImage = "head0.png";
	swordImage = "sword1.png";
	bodyImage = "body.png";
	nickName = "unknown";
	shieldImage = "shield1.png";
	id = -1;
	level = NOLEVEL;
	carrySprite = -1;
	packCount = failAttempts = 0;
	banned = false;
	lastMessage = 0;
}

CPlayer::~CPlayer()
{
	if (id >= 0)
	{
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

		if (type == CLIENTPLAYER)
		{
			saveAccount();
		}
			else if (type == CLIENTRC)
		{
			sendRCPacket(CPacket() << (char)DRCLOG << "RC Disconnected: " << accountName);
		}

		ListServer_Send(CPacket() << (char)SLSPLAYERREM << (char)type << accountName);
	}
	else newPlayers.remove(this);
	delete playerSock;

	//remove me from level
	if (level != NOLEVEL)
	{
		if (level->players.count() > 1 && level->players[0] == this)
			((CPlayer*)level->players[1])->sendPacket(CPacket() << (char)ISLEADER);
		level->players.remove(this);
	}

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
	CBuffer receiveBuff;
	int size=0;
	if ((size = playerSock->receiveBytes(receiveBuff, 65536)) < 0)
	{
		errorOut("rclog.txt", CString() << "Client " << accountName << " disconnected with sock error: " << toString(size));
		deleteMe = true;
		compressAndSend();
		return;
	}

	if (receiveBuff.length() >= 128*1024)
	{
		errorOut("rclog.txt", CString() << "Client " << accountName << "has sent to much data (input buffer >=128k)");
		sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent to much data (>=128k in the input buffer)");
		deleteMe = true;
		return;
	}

	while (receiveBuff.length() >= 2)
	{
		lastData = time(NULL);
		int error, cLen = sizeof(packets);
		unsigned int len = (((unsigned int)(unsigned char)receiveBuff[0]) << 8) + (unsigned int)(unsigned char)receiveBuff[1];

		// Packet might not be fully in yet.
		if ( len > (unsigned int)receiveBuff.length() - 2 )
			break;
/*
		if(len > receiveBuff.length() - 2 || len < 0)
		{
			errorOut("rclog.txt", CString() << "Client " << accountName << " sent a wrong data package length: " << toString(len) << "->" << toString(receiveBuff.length()));
			sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent a wrong data package length.");
			deleteMe = true;
			return;
		}
*/

		if ((error = uncompress((Bytef*)packets,(uLongf*)&cLen,(const Bytef*)receiveBuff.text()+2, len)) == Z_OK)
		{
			CPacket lines;
			if(cLen <= 0)
			{
				errorOut("rclog.txt", CString() << "Client " << accountName << " sent an empty package.");
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
			receiveBuff.remove(0, len+2);
		}
		else
		{
			errorOut("rclog.txt", CString() << "Decompression error for player " << accountName);
			deleteMe = true;
			sendPacket(CPacket() << (char)DISMESSAGE << "Decompression error\n");
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

	printf("Account login details:\n"
		   "Key: %i\n"
		   "Version: %s\n"
		   "Type: %i\n"
		   "Account: %s\n"
		   "Password: ***\n", key, version.text(), type, accountName.text());

	if (playerList.count() >= maxPlayers)
	{
		errorOut("rclog.txt", "Player limit reached");
		sendPacket(CPacket() << (char)DISMESSAGE << "This server has reached its player limit.");
		deleteMe = true;
		return;
	}

	CString ip = playerSock->tcpIp();
	if (isIpBanned(ip))
	{
		errorOut("rclog.txt", CString() << accountName << " is ip banned");
		sendPacket(CPacket() << (char)DISMESSAGE << "Your ip has been banned from this server.");
		deleteMe = true;
		return;
	}

	//Send a verification request to the server for account name and password
	if (lsConnected)
		ListServer_Send(CPacket() << (char)SLSACCOUNT << (char)accountName.length() << accountName << (char)password.length() << password << "\n");
	else
	{
		errorOut("rclog.txt", "List server is unavailable");
		sendPacket(CPacket() << (char)DISMESSAGE << "Unable to contact account server.");
		deleteMe = true;
	}
}

void CPlayer::sendAccount()
{
	CPacket packet;
	if (!loadAccount())  //Do login check
	{
		errorOut("rclog.txt", CString() << accountName << " is not allowed on server");
		deleteMe = true;
		lastIp = inet_addr(playerSock->tcpIp());
		return;
	}

	loginTime = time(NULL);
	ListServer_Send(CPacket() << (char)SLSPLAYERADD << (char)accountName.length() << accountName << getProp(NICKNAME) << getProp(CURLEVEL) << getProp(PLAYERX) << getProp(PLAYERY) << getProp(PALIGNMENT) << (char)type);

	//Server signiture. Allows more than 8 players
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
					errorOut("rclog.txt", CString() << "No data for 30 secs from " << player->accountName);
					player->sendPacket(CPacket() << (char)DISMESSAGE << "Someone else has logged into your account.");
					player->deleteMe = true;
				}
				else
				{
					errorOut("rclog.txt", accountName << " account is already in use.");
					sendPacket(CPacket() << (char) DISMESSAGE << "This account is already in use.");
					deleteMe = true;
					return;
				}
			}
		}
	}

	errorOut("rclog.txt", CString() << "New player: " << accountName);
	id = createPlayerId(this);
	saveAccount();

	if(type == CLIENTPLAYER)
	{
		sendPacket(CPacket() << (char)UNLIMITEDSIG << (char)73);

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

		packet << (char)SPLAYERPROPS;
		for (int i = 0; i < propscount; i++)
		{
			if (sendInit[i])
				packet << (char)i << getProp(i);
		}

		packet << (char)SRPGWINDOW << "\"Welcome to " << listServerFields[0] << "\",\"Graal Reborn - Coded by 39ster, Agret, and Joey\"";
		sendPacket(packet);
	}

	if (type == CLIENTPLAYER)
	{
		sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
		sendPacket(CPacket() << (char)LEVELNAME << levelName);

		if (!sendLevel(levelName, x, y, 0))
		{
			if (!sendLevel(unstickmeLevel, x, y, 0))
				deleteMe = true;
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

	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];
		if(other == this)
			continue;
		if(type == CLIENTPLAYER)
		{
			CPacket otherProps;
			otherProps << (char)OTHERPLPROPS << (short)other->id << (char)50 << (char)1;
			for (int ii = 0; ii < propscount; ii ++)
			{
				if (sendOthersInit[ii])
					otherProps << (char) ii << other->getProp(ii);
			}
			sendPacket(otherProps);
		} else if(type == CLIENTRC)
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

		if(other->type == CLIENTRC)
		{
			other->sendPacket(rcProps);
			if(type == CLIENTRC)
			{
				other->sendPacket(CPacket() << (char)DRCLOG << "New RC: " << nickName << " (" << accountName << ")");
				sendPacket(CPacket() << (char)DRCLOG << "New RC: " << other->nickName << " (" << other->accountName << ")");
			}
		}
		else if(other->type == CLIENTPLAYER)
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

	newPlayers.remove(this);
	playerList.add(this);
	lastIp = inet_addr(playerSock->tcpIp());
}

void CPlayer::parsePacket(CPacket& pPacket)
{
	if(pPacket.length() <= 0)
		return;
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

	if(messageId != BADDYPROPS && messageId != NPCPROPS && showConsolePackets)
		printf("NEW PACKET: %i: %s\n", messageId, packet.text()+1);

	if (messageId >= 0 &&  messageId < clientpackages)
		(*this.*msgFuncs[messageId])(packet);
	else if(messageId < 0)
	{
		//deleteMe = true;
		//errorOut(CString() << accountName << " sent an incorrect message id [" << toString(messageId) << "]\nPacket: " << pPacket.text() + 1 << "\n");
		//sendPacket(CPacket() << (char)DISMESSAGE << "You sent an incorrect packet");
		//return;
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
		errorOut("rclog.txt", CString() << "Compression error for " << accountName);
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
		errorOut("rclog.txt", CString() << "Sending is blocked for " << accountName);
		sendPacket(CPacket() << (char)DISMESSAGE << "Sending is blocked");
		deleteMe = true;
		return;
	}

	while (sendBuff.length()>0)
	{
		sendLength = MIN(sendBuff.length(), 1024);
		int len = 0;
		if ((len = playerSock->sendBytes(sendBuff.text(), sendLength)) > 0)
			sendBuff.remove(0, len);
		else if(len < 0)
		{
			errorOut("rclog.txt", CString() << "Send error to " << accountName);
			deleteMe = true;
			return;
		}
		else break;
	}
}

void CPlayer::warp(CString& pLevel, float pX, float pY, int pModTime)
{
	x = pX;
	y = pY;

	if (pLevel != levelName)
	{
		if (pModTime == 0) sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << pLevel);
		sendPacket(CPacket() << (char)LEVELNAME << pLevel);

		if (!sendLevel(pLevel, x, y, pModTime))
		{
			sendPacket(CPacket() << (char)LEVELFAILED);
			sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
			sendPacket(CPacket() << (char)LEVELNAME << levelName);

			if (!sendLevel(levelName, x, y, 0))
			{
				sendPacket(CPacket() << (char)DISMESSAGE << "No level available");
				errorOut("rclog.txt", CString() << "Cannot find a level for " << accountName);
				deleteMe = true;
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
		if (time(NULL) - lastMovement >= 30)
		{
			warp(unstickmeLevel, unstickmeX, unstickmeY);
			chatMsg = CString() << "Warped!";
		}
			else
		{
			chatMsg = CString() << "Dont move for 30 seconds before doing '" << pMessage + "'!";
		}

		updateProp(CURCHAT);
	}
		else if (pMessage.find("setnick") == 0)
	{
		if (time(NULL) - lastNick <= 10)
		{
			chatMsg = "Wait 10 seconds before changing your nick again!";
			updateProp(CURCHAT);
			return;
		}

		lastNick = time(NULL);
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
	{
		CLevel::updateLevel(levelName);
	}

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

	if (words[0] == "warpto" && (hasRight(CANWARPPLAYER) || hasRight(CANWARPXY) || warptoforall))
	{
		if(words.count() <= 1)
			return;
		if (!hasRight(CANWARPPLAYER) && !warptoforall)
		{
			chatMsg = CString() << "(not authorized to warp)";
			updateProp(CURCHAT);
			return;
		}

		CString name;
		name = levelName;

		if (words.count() == 2)
		{
			CPlayer* other = findPlayerId(words[1], true);
			if(other == NULL)
				return;
			name = other->levelName;
			x = other->x;
			y = other->y;
		} else if(words.count() >= 3 && (hasRight(CANWARPXY) || warptoforall))
		{
			x = (float)atof(words[1].text());
			y = (float)atof(words[2].text());
			if(words.count() >= 4)
				name = words[3];
		}
		warp(name, x, y);
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

		if(strlen(getDataFile(words[1].text())))
		{
			headImage = words[1];
			updateProp(HEADGIF);
		}
			else
		{
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)0 << (char)words[1].length() << words[1]);
		}
	}
	else if (words[0] == "setbody")
	{
		if (words.count() <= 1 || !setbodyallowed)
			return;

		if(strlen(getDataFile(words[1].text())))
		{
			bodyImage = words[1];
			updateProp(BODYIMG);
		}
			else
		{
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)1 << (char)words[1].length() << words[1]);
		}
	} else if(words[0] == "setsword")
	{
		if(words.count() <= 1 || !setswordallowed)
			return;

		if(strlen(getDataFile(words[1].text())))
		{
			swordImage = words[1];
			updateProp(SWORDPOWER);
		}
			else
		{
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)2 << (char)words[1].length() << words[1]);
		}
	} else if(words[0] == "setshield")
	{
		if(words.count() <= 1 || !setshieldallowed)
			return;

		if (strlen(getDataFile(words[1].text())))
		{
			shieldImage = words[1];
			updateProp(SHIELDPOWER);
		}
			else
		{
			ListServer_Send(CPacket() << (char)SLSFILE << (short)id << (char)3 << (char)words[1].length() << words[1]);
		}
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
				ListServer_Send(CPacket() << (char)SLSGUILD << (short)id << (char)accountName.length() << accountName << (char)tmpNick.length() << tmpNick << (char)tmpGuild.length() << tmpGuild);
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

	ListServer_Send(CPacket() << (char)SLSNICKNAME << (char)accountName.length() << accountName << getProp(NICKNAME));
	sendPacket(CPacket() << (char)SPLAYERPROPS << (char)NICKNAME << getProp(NICKNAME));

	// When RC's log in, they will send their nickname.  When that happens, they still have
	// an ID of -1.  Don't send information to any clients in that case.
	if ( id != -1 )
	{
		for (int i = 0; i < playerList.count(); i++)
		{
			CPlayer*other = (CPlayer*)playerList[i];
			if (other != this)
				other->sendPacket(CPacket() << (char)OTHERPLPROPS << (short)id << (char)NICKNAME << getProp(NICKNAME));
		}
	}
}


void CPlayer::setAccPropsRc(CPacket& pPacket)
{
	CPacket props, temp;
	//Skip playerworld
	pPacket.readChars(pPacket.readByte1());
	int len = (unsigned char)pPacket.readByte1();
	props << pPacket.readChars(len);
	setProps(props, true);
	sendPacket(CPacket() <<(char)SPLAYERPROPS << props);

	// Clear Flags + Weapons
	for (int i = 0; i < myFlags.count(); i++)
		temp << (char)SUNSETFLAG << myFlags[i] << "\n";
	for (int i = 0; i < myWeapons.count(); i++)
	{
		temp << (char)SDELNPCWEAPON << myWeapons[i] << "\n";

		// Attempt to fix the funky client bomb capitalization issue.
		if ( myWeapons[i] == "bomb" )
			temp << (char)SDELNPCWEAPON << "Bomb" << "\n";
	}
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
	}
	sendWeapons();

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

	if (banned)
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

	if (loadOnly || !createNew)
		return loadWorldPropsIni();

	return true;
}

bool CPlayer::sendLevel(CString& pLevel, float pX, float pY, int pModTime)
{
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
	int time = getLeavingTime(level);
	if(time <= 0)
	{
		if (pModTime != level->modTime)
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
		enteredLevels.add(new CEnteredLevel(level, getTime()));
	}

	//Send board changes
	packet.clear();
	packet << (char)LEVELBOARD;
	for (int i = 0; i < level->boardChanges.count(); i++)
	{
		CBoardChange* change = (CBoardChange*)level->boardChanges[i];
		if(change->modifyTime >= time)
			packet << change->getSendData();
	}
	sendPacket(packet);

	//Send chests
	for (int i = 0; i < level->chests.count(); i ++)
	{
		CChest*chest = (CChest*)level->chests[i];
		CString chestString;
		chestString << toString(chest->x) << ":" << toString(chest->y) <<
			":" << levelName;
		if (myChests.findI(chestString) >= 0)
		{
			sendPacket(CPacket() << (char)LEVELCHEST << (char)1 << (char)chest->x << (char)chest->y);
		}
		else
		sendPacket(CPacket() << (char)LEVELCHEST << (char)0 << (char)chest->x << (char)chest->y << (char)chest->item << (char)chest->signIndex);
	}

	//send horses
	for(int i = 0; i < level->horses.count(); i++)
	{
		CHorse* horse = (CHorse*)level->horses[i];
		sendPacket(CPacket() << (char)SADDHORSE << (char)horse->x << (char)horse->y << horse->imageName);
	}
	//send baddies
	for (int i = 0; i < level->baddies.count(); i++)
	{
		CBaddy* baddy = (CBaddy*)level->baddies[i];
		sendPacket(CPacket() << (char)SBADDYPROPS << (char)baddy->id << baddy->getPropList());
	}

	//send npcs
	for (int i = 0; i < level->npcs.count(); i++)
	{
		CPacket npcProps;
		CNpc* npc = (CNpc*)level->npcs[i];
		npcProps << (char)SNPCPROPS << (int)npc->id << npc->getPropertyList(time);
		if (npcProps.length() > 3)
			sendPacket(npcProps);
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
//	int pl = 0;
	packet.clear();
	packet << (char)OTHERPLPROPS << (short)id << (char)50 << (char)1;
	for (int i = 0; i < propscount; i++)
	{
		if (forwardLocal[i])
		{
			packet << (char)i << getProp(i);
/*
			// Limit the packet to only 5 props
			if ( (pl+1) % 5 == 0 )
			{
				packet << "\n";
				packet << (char)OTHERPLPROPS << (short)id;// << (char)50 << (char)1;
			}
			pl++;
*/
		}
	}
//packet.save( "props.raw" );

	//send props
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer*other = (CPlayer*)level->players[i];
		if (other == this)
			continue;

		CPacket otherProps;
		otherProps << (char)OTHERPLPROPS << (short)other->id << (char)50 << (char)1;

		//pl = 0;
		for (int ii = 0; ii < propscount; ii++)
		{
			if (forwardLocal[ii])
			{
				otherProps << (char)ii << other->getProp(ii);
/*
				// Limit the packet to only 5 props
				if ( (pl+1) % 5 == 0 )
				{
					otherProps << "\n";
					otherProps << (char)OTHERPLPROPS << (short)other->id;// << (char)50 << (char)1;
					pl++;
				}
*/
			}
		}
		sendPacket(otherProps);
		other->sendPacket(packet);
	}

	compressAndSend();
	if (level->players.count() == 1)
		sendPacket(CPacket() << (char)ISLEADER);
	return true;
}

void CPlayer::leaveLevel()
{
	CPacket otherProps;
	if (level == NOLEVEL)
		return;

	if (level->players[0] == this && level->players.count()>1)
		((CPlayer*)level->players[1])->sendPacket(CPacket() << (char)ISLEADER);

	level->players.remove(this);
	otherProps << (char)OTHERPLPROPS << (short)id << (char)50 << (char)0;
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer*other = (CPlayer*)level->players[i];
		other->sendPacket(otherProps);
		sendPacket(CPacket() << (char)OTHERPLPROPS << (short)other->id << (char)50 << (char)0);
	}

	for (int i = 0; i < enteredLevels.count(); i++)
	{
		CEnteredLevel*l = (CEnteredLevel*)enteredLevels[i];
		if (l->level == level)
		{
			l->time = getTime();
			l->level = NOLEVEL;
			break;
		}
	}
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

		if(longName.length())
		{
			modTime = getFileModTime(longName.text());
			if (modTime != file->modTime)
			{
				if (fileData.load(longName.text()) && fileData.length() <= 64500)
				{
					failed = false;
					int len = 1 + 5 + 1 + shortName.length() + fileData.length() + 1;
					sendPacket(CPacket() << (char)100 << (int)len);
					sendPacket(CPacket() << (char)102 << (long long)modTime << (char)shortName.length() << shortName << fileData << "\n");
				}
			}
		}
		if(failed)
			sendPacket(CPacket() << (char)GIFFAILED << shortName);
		delete file;
	}
	fileList.clear();
}

int CPlayer::getLeavingTime(CLevel* pLevel)
{
	for (int i = 0; i < enteredLevels.count(); i++)
	{
		CEnteredLevel* entered = (CEnteredLevel*)enteredLevels[i];
		if (entered->level == pLevel)
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

	for (int i = 0; i < 10; i ++)
	{
		int itemId = rand()%10;
		bool valid = false;

		switch (itemId)
		{
			case GREENRUPEE:
				if (rubins >= 1)
				{
					valid = true;
					rubins--;
					updateProp(RUPEESCOUNT);
				}
			break;

			case BLUERUPEE:
				if (rubins >= 5)
				{
					valid = true;
					rubins -= 5;
					updateProp(RUPEESCOUNT);
				}
			break;

			case REDRUPEE:
				if (rubins >= 30)
				{
					valid = true;
					rubins -= 30;
					updateProp(RUPEESCOUNT);
				}
			break;

			case BOMBS:
				if (bombs >= 5)
				{
					valid = true;
					bombs -= 5;
					updateProp(BOMBSCOUNT);
				}
			break;

			case DARTS:
				if (darts >= 5)
				{
					valid = true;
					darts -= 5;
					updateProp(ARROWSCOUNT);
				}
			break;

			default:
				continue;
		}

		if (valid)
		{
			float pX = x + (rand()%7)-2;
			float pY = y + (rand()%7)-2;
			level->items.add(new CItem(pX, pY, itemId));

			for (int ii = 0; ii < level->players.count(); ii++)
				((CPlayer*)level->players[ii])->sendPacket(CPacket() << (char)SADDEXTRA << (char)(pX*2) << (char)(pY*2) << (char)itemId);
		}
	}
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

	case PLAYERANI:
		retVal << (char)gAni.length() << gAni;
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

	case HEADGIF:
		retVal.writeByte1(100 + headImage.length());
		retVal << headImage;
		break;

	case BODYIMG:
		retVal << (char)bodyImage.length() <<  bodyImage;
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
	case CARRYNPC:
		retVal.writeByte3(0);
		break;

	case EFFECTCOLORS:
		retVal.writeByte1(0);
		break;
	case ONLINESECS:
		retVal.writeByte3(onlineSecs);
		break;

	case LASTIP:
		retVal.writeByte5(lastIp);
		break;

	case PALIGNMENT:
		retVal.writeByte1(ap);
		break;

	case PACCOUNTNAME:
		retVal << (char)accountName.length() << accountName;
		break;

	case PADDITFLAGS:
		retVal.writeByte1(additionalFlags);
		break;

	case PSTATUSMSG:
		retVal.writeByte1(statusMsg);
		break;
	case UDPPORT:
		retVal.writeByte3(udpPort);
		break;

	case GATTRIB1: retVal << (char)myAttr[0].length() << myAttr[0]; break;
	case GATTRIB2: retVal << (char)myAttr[1].length() << myAttr[1]; break;
	case GATTRIB3: retVal << (char)myAttr[2].length() << myAttr[2]; break;
	case GATTRIB4: retVal << (char)myAttr[3].length() << myAttr[3]; break;
	case GATTRIB5: retVal << (char)myAttr[4].length() << myAttr[4]; break;
	case GATTRIB6: retVal << (char)myAttr[5].length() << myAttr[5]; break;
	case GATTRIB7: retVal << (char)myAttr[6].length() << myAttr[6]; break;
	case GATTRIB8: retVal << (char)myAttr[7].length() << myAttr[7]; break;
	case GATTRIB9: retVal << (char)myAttr[8].length() << myAttr[8]; break;
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

	case RATING:
		retVal.writeByte2(rating);
	break;
	default:
		errorOut("rclog.txt", CString() << "Unknown getProp() by " << accountName);
		break;
	}
	return retVal;
}

void CPlayer::setProps(CPacket& pProps, bool pForward)
{
	int len;
	CPacket forwardBuff;
	forwardBuff << (char) OTHERPLPROPS << (short)id;

	// This allows for correct RC login.
	if (id == -1 && pForward)
		pForward = false;

	while (pProps.bytesLeft())
	{
//		int startpos = pProps.getRead();
		int index = pProps.readByte1();
		if(index < 0 || index >= propscount-1)
		{
			errorOut("rclog.txt", CString() << "setProps(" << toString(index) << ") index out of bounds by " << accountName);
			break;
		}

		switch (index)
		{
		case NICKNAME:
		{
			len = pProps.readByte1();
			if (len <= 0 || len > 224)
				break;

			setNick(CString() << pProps.readChars(len), true);
		}
		break;

		case MAXPOWER:
			maxPower = CLIP((float)pProps.readByte1(), 0.0f, (float)heartLimit);
			break;

		case CURPOWER:
		{
			float oldPower = power;
			power = CLIP((float)pProps.readByte1()/2, 0.0f, maxPower);
			if(power == 0 && oldPower > 0)
				dropItems();
			break;
		}
		case RUPEESCOUNT:
			rubins = CLIP(pProps.readByte3(), 0, 9999999);
			break;

		case ARROWSCOUNT:
			darts = CLIP(pProps.readByte1(), 0, 99);
			break;

		case BOMBSCOUNT:
			bombs = CLIP(pProps.readByte1(), 0, 99);
			break;

		case GLOVEPOWER:
			glovePower = CLIP(pProps.readByte1(), 0, 3);
			//is 3 maximum??
			break;

		case BOMBPOWER:
			bombPower = CLIP(pProps.readByte1(), 0, 3);
			break;

		case PLAYERANI:
			len = pProps.readByte1();
			if (len >= 0)
				gAni = pProps.readChars(len);
			break;

		case SWORDPOWER:
		{
			int sp = pProps.readByte1();
			if(sp >= 30)
			{
				sp -= 30;
				len = pProps.readByte1();
				if(len >= 0)
					swordImage = pProps.readChars(len);
			} else if(sp <= 4)
				swordImage = CString() << "sword" << toString(sp) << ".png";
			swordPower = CLIP(sp, 0, swordLimit);
			//check this
			break;
		}
		case SHIELDPOWER:
		{
			int sp = pProps.readByte1();
			if(sp >= 10)
			{
				sp -= 10;
				len = pProps.readByte1();
				if(len >= 0)
					shieldImage = pProps.readChars(len);
			} else if(sp <= 3)
				shieldImage = CString() << "shield" << toString(sp) << ".png";
			shieldPower = CLIP(sp, 0, shieldLimit);
			//check this
			break;
		}
		case HEADGIF:
			len = pProps.readByte1();
			if (len < 100)
			{
				if (len >= 0)
					headImage = CString() << "head" << toString(len) << ".png";
			} else
			{
				if (len > 100)
					headImage = pProps.readChars(len-100);
			}
		break;

		case CURCHAT:
			len = (unsigned char)pProps.readByte1();
			len = (len < 0 ? 0 : len);

			chatMsg = pProps.readChars(len);
			chatMsg = (chatMsg.length() > 220 ? chatMsg.copy(0, 220) : chatMsg);

			processChat(chatMsg);
			lastMovement = time(NULL);
	   	break;

		case PLAYERCOLORS:
			for (int i = 0; i < 5; i ++)
				colors[i] = pProps.readByte1();
		break;

		case PLAYERID:
			pProps.readByte2();
			break;

		case PLAYERX:
			x = (float)pProps.readByte1() / 2;
			status &= (-1-1);
			lastMovement = time(NULL);
			//TO DO: add hack check;
			break;

		case PLAYERY:
			y = (float)pProps.readByte1() / 2;
			status &= (-1-1);
			lastMovement = time(NULL);
			//TO DO: add hack check;
			break;

		case PLAYERZ:
			z = (float)pProps.readByte1() / 2;
			status &= (-1-1);
			lastMovement = time(NULL);
			//TO DO: add hack check;
			break;

		case PLAYERSPRITE:
			sprite = pProps.readByte1();
			//TO DO: add hack check;
			break;

		case STATUS:
		{
			int oldStatus = status;
			status = pProps.readByte1();

			if ( id == -1 ) break;
			if ((oldStatus & 8) == 0)
			{
				if ((status & 8) > 0) // dead
				{
					power = CLIP((ap < 20 ? 3 : (ap < 40 ? 5 : maxPower)), 0.0f, maxPower);
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
		}
		break;

		case CARRYSPRITE:
			carrySprite = pProps.readByte1();
			//TO DO: packet format may be outdated
			break;

		case CURLEVEL:
			len = pProps.readByte1();
			if(len >= 0)
				levelName = pProps.readChars(len);
			//TO DO: add hack check
			break;

		case HORSEGIF:
			len = pProps.readByte1();
			if(len >= 0)
				horseImage = pProps.readChars(len);
			//TO DO: add hack check
			break;

		case HORSEBUSHES:
			horseBushes = pProps.readByte1();
			//TO DO: add hack check
			break;

		case APCOUNTER:
			apCounter = pProps.readByte2();
			//TO DO: add hack check
			break;

		case MAGICPOINTS:
			magicPoints = CLIP(pProps.readByte1(), 0, 100);
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

		case CARRYNPC:
			pProps.readByte3();
			//this doesnt work even when i add the correct code...:/
			//TO DO: add hack check
			break;

		case EFFECTCOLORS:
			len = pProps.readByte(1);
			if ( len > 0 )
				pProps.readByte4();
//			pProps.readByte5();
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

		case PALIGNMENT:
			ap = CLIP(pProps.readByte1(), 0, 100);
			//TO DO: add hack check
			break;

		case UDPPORT:
			udpPort = pProps.readByte3();
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

		case PADDITFLAGS:
			additionalFlags = pProps.readByte1();
			//TO DO: add hack check
			break;

		case PACCOUNTNAME:
			len = pProps.readByte1();
			if (len >= 0)
				pProps.readChars(len);
			//Do not allow account name change
			break;

		case BODYIMG:
			len = pProps.readByte1();
			if (len >= 0)
				bodyImage = pProps.readChars(len);
			//TO DO: add hack check
			break;

		case PSTATUSMSG:
			statusMsg = pProps.readByte1();

		// Allows proper RC login.
		if ( id == -1 ) break;

			for(int i = 0; i < playerList.count(); i++)
			{
				CPlayer*other = (CPlayer*)playerList[i];
				if(other != this)
					other->sendPacket(CPacket() << (char)OTHERPLPROPS << (short)id <<
						(char)PSTATUSMSG << (char)statusMsg);
			}
			//TO DO: add hack check
			break;

		case PLANGUAGE:
			len = pProps.readByte1();
			if(len >= 0)
				language = pProps.readChars(len);
		break;

		case GATTRIB1:  myAttr[0]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB2:  myAttr[1]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB3:  myAttr[2]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB4:  myAttr[3]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB5:  myAttr[4]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB6:  myAttr[5]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB7:  myAttr[6]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB8:  myAttr[7]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
		case GATTRIB9:  myAttr[8]  = pProps.readChars((unsigned char)pProps.readByte1()); break;
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

		case RATING:
			rating = pProps.readByte2();
			break;
		default:
			errorOut("rclog.txt", CString() << "Setprops error: " << toString(index) << " By " << accountName);
			return;
		}

		if (pForward)
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
			/*
			if ( (pl+1) % 5 == 0 )
			{
				forwardBuff << "\n";
				forwardBuff << (char)OTHERPLPROPS << (short)id;
			}
			pl++;
			*/
		}
	}

	if (pForward && forwardBuff.length() > 0)
	{
		sendLocally(forwardBuff);
		compressAndSend();
	}
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

	sendPacket(CPacket() << (char)LEVELNAME << mapName);
	if (modTime != outterLevel->modTime)
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
	level->changeBoard(tileData, bX, bY, bW, bH);

	if (bX < 0 || bX > 63 || bY < 0 || bY > 63)
		return;

	//Lay items when u destroy objects
	short oldTile = level->tiles[bX+(bY*64)];
	int dropItem = -1;
	//Bushes, grass, swamp
	if ((oldTile == 2 || oldTile == 0x1a4 || oldTile == 0x1ff ||
			oldTile == 0x3ff)&& bushesDrop)
	{
		int index = rand()%10;
		if (index <= 5)
			dropItem = index;

	}//vase
	else if (oldTile == 0x2ac && vasesDrop)
		dropItem = 5;

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

	float hX = (float)(pPacket.readByte1())/2;
	float hY = (float)(pPacket.readByte1())/2;
	CString image = pPacket.readChars(pPacket.readByte1());
	level->horses.add(new CHorse(image, hX, hY));
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

	float hX = (float)(pPacket.readByte1()/2);
	float hY = (float)(pPacket.readByte1()/2);
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
	for (int i = 0; i < level->players.count(); i++)
	{
		CPlayer* other = (CPlayer*)level->players[i];
		if (other != this)
			other->sendPacket(fireData);
	}
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
	if (other == NULL || level->sparZone)
		return;

	if (!dontchangekills)
		other->kills ++;

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
	float nX = (float)(pPacket.readByte1()/2);
	float nY = (float)(pPacket.readByte1()/2);
	level->addNewNpc(image, codeFile, nX, nY);


}

void CPlayer::msgDELNPC(CPacket& pPacket)
{

	CNpc* npc = (CNpc*)npcIds[pPacket.readByte3()];
	if(npc == NULL)
		return;
	if(npc->level != level)
		return;

	for(int i = 0; i < playerList.count(); i++)
	{
		((CPlayer*)playerList[i])->sendPacket(CPacket() <<
			(char)SDELNPC << (int)npc->id);
	}
	level->npcs.remove(npc);
	delete npc;
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
{}
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
	if(getTime() - lastMessage <= 4)
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
		errorOut("rclog.txt", CString() << accountName << " has sent an incorrect number of packets");
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
	if(fileList.count() <= 50)
		fileList.add(new COutFile(fileName, modTime));
	else
		sendPacket(CPacket() << (char)GIFFAILED << fileName);
}

void CPlayer::msgEMPTY36(CPacket& pPacket)
{}
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
void CPlayer::msgEMPTY39(CPacket& pPacket)
{}
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
{}
void CPlayer::msgEMPTY42(CPacket& pPacket)
{}
void CPlayer::msgEMPTY43(CPacket& pPacket)
{}

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
{}
void CPlayer::msgEMPTY46(CPacket& pPacket)
{}
void CPlayer::msgEMPTY47(CPacket& pPacket)
{}
void CPlayer::msgEMPTY48(CPacket& pPacket)
{}
void CPlayer::msgEMPTY49(CPacket& pPacket)
{}
void CPlayer::msgEMPTY50(CPacket& pPacket)
{}

//RC
inline bool CPlayer::hasRight(int pRight)
{
	return ((adminRights & pRight) > 0 && hasStaff());
}

inline bool CPlayer::hasStaff()
{
	return (staffList.findI(accountName) >= 0);
}

/* Sick of fucking graals retarded-ways.. and my attempts to find ways to patch it.. fucking graal.. */
void CPlayer::msgSWANTSOPTIONS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
		return;

	CStringList serverOptions;
	serverOptions.load("serveroptions.txt");

	if (!hasRight(CANCHANGESTAFFACC))
	{
		for (int i = 0; i < serverOptions.count(); i++)
		{
			CString name = serverOptions[i].copy(0, serverOptions[i].find("="));
			name.trim();

			if (adminNames.find(name) >= 0 || serverOptions[i] == "#Serveroptions")
			{
				serverOptions.remove(i);
				i--;
			}
		}
	}

	sendPacket(CPacket() << (char)DSENDSOPTIONS << serverOptions.join("\n").tokenize());
}

void CPlayer::msgSSETOPTIONS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANEDITSERVEROPTION))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for changing the server options");
		return;
	}

	CString data = pPacket.text() + 1;
	data.untokenize();
	//data.replaceAll("\n", "\n");

	CStringList newOps;
	newOps.load(data.text(), "\n");

	if (!hasRight(CANCHANGESTAFFACC))
	{
		CStringList serverOptions;
		serverOptions.load("serveroptions.txt");
		for (int i = 0; i < newOps.count(); i++)
		{
			CString name = newOps[i].copy(0, newOps[i].find("="));
			name.trim();
			if (adminNames.findI(name) >= 0)
				newOps.remove(i);
		}

		newOps.add(" ");
		newOps.add("#Serveroptions");

		for (int i = 0; i < serverOptions.count(); i++)
		{
			CString name = serverOptions[i].copy(0, serverOptions[i].find("="));
			name.trim();
			if (adminNames.findI(name) >= 0)
				newOps.add(serverOptions[i]);
		}
	}

	data = newOps.join("\n");
	data.save("serveroptions.txt");
	loadSettings("serveroptions.txt");
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has updated the server options");
}

void CPlayer::msgWANTRCFOLDERS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		sendRCPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}

	CStringList temp;
	temp.load("foldersconfig.txt");
	sendPacket(CPacket() << (char)SSENDFOLDER << temp.join("\n").tokenize());
}

void CPlayer::msgSETRCFOLDERS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANEDITFOLDEROPTION))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for changing the folder configuration");
		return;
	}

	CString data = pPacket.text() + 1;
	data.untokenize();//.replaceAll("\n", "\n");
	data.save("foldersconfig.txt");
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has updated the folder configuration");
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
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for settings attributes.");
		return;
	}

	player->setAccPropsRc(pPacket);
	player->saveAccount();
	sendPacket(CPacket() << (char)DRCLOG << accountName << " set attributes of player " << player->accountName);
}

void CPlayer::msgDISPLAYER(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANDISCONNECT))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for disconnecting players.");
		return;
	}
	CPlayer* other = (CPlayer*)playerIds[pPacket.readByte2()];
	if(other == NULL)
		return;
	other->sendPacket(CPacket() << (char)DISMESSAGE << "The server administrator has disconnected you.");
	other->deleteMe = true;
	sendPacket(CPacket() << (char)DRCLOG << accountName << " disconnects " << other->accountName << ".");
}

void CPlayer::msgUPDLEVELS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANUPDATELEVEL))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to update levels.");
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
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to send admin messages.");
		return;
	}

	CString message = pPacket.readString("");
	for(int i = 0; i < playerList.count(); i++)
	{
		CPlayer* player = (CPlayer*)playerList[i];
		if(player != this)
			player->sendPacket(CPacket() << (char)SADMINMSG << "Administrator " << accountName << ":\xa7" << message);
	}
}

void CPlayer::msgPRIVADMINMSG(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANADMINMSG))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to send admin messages.");
		return;
	}
	CPlayer* other = (CPlayer*)playerIds[pPacket.readByte2()];
	if(other == NULL)
		return;
	other->sendPacket(CPacket() << (char)SADMINMSG << "Admin " << accountName << ": " << pPacket.readString(""));
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
		sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
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
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to change the server flags.");
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
	sendPacket(CPacket() << (char)DRCLOG << accountName << " has updated the server flags.");
}

void CPlayer::msgDADDACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGESTAFFACC))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for adding accounts.");
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
}

void CPlayer::msgDDELACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGESTAFFACC))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for deleting accounts.");
		return;
	}

	CString accname = pPacket.readString("");
	if (remove(CString(CString() << "accounts" << fSep << accname << ".txt").text()) == 0)
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has deleted account of " << accname << ".");
	else
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: invalid account");
}

void CPlayer::msgDWANTACCLIST(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for viewing the accounts list.");
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
	if (type != CLIENTRC || !hasStaff())
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for loading attributes");
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
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " loaded player attributes of " << accname);
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDRESETPLPROPS(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANRESETATTRIBS))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for resetting accounts");
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

	player->loadWorldPropsIni();
	player->saveAccount();
	if (player->id >= 0)
	{
		player->sendPacket(CPacket() << (char)DISMESSAGE << "Your account was reset");
		player->deleteMe = true;
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has reset the attributes of " << accname);
	}

	if (player->id == -1)
		delete player;
}

void CPlayer::msgDSETACCPLPROPS(CPacket& pPacket)
{
	CString accname = pPacket.readChars(pPacket.readByte1());
	if (type != CLIENTRC || (accname != accountName && !hasRight(CANSETATTRIBS)) || (accname == accountName && !hasRight(CANSETOWNATTRIBS)))
	{
		sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have rights to change attributes");
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

	player->setAccPropsRc(pPacket);
	player->saveAccount();
	if (player->id == -1)
		delete player;
}

void CPlayer::msgDWANTACCOUNT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
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
		sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have enough rights");
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
/refreshfilelist: scans the folders again (only use when /find is not working correctly)
/shutdown: closes the server
*/

void CPlayer::msgDRCCHAT(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
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
		else if (words[0] == "/refreshfilelist" && hasRight(CANEDITSERVEROPTION))
	{
		subDirs.clear();
		getSubDirs(dataDir);
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " refreshed the file list.");
	}
		else if (words[0] == "/refreshservermessage" && hasRight(CANEDITSERVEROPTION))
	{
		loadServerMessage();
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " reloaded the server message.");
	}
		else if(words[0] == "/shutdown" && hasRight(CANCHANGESTAFFACC))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " shutdown the server.");
		shutdownServer();
	}
		else if (words[0] == "/updatelevel" && hasRight(CANUPDATELEVEL))
	{
		CStringList levels;
		for (int i = 1; i < words.count(); i++)
			CLevel::updateLevel(words[i]);
	}
}

//reminder
void CPlayer::msgDGETPROFILE(CPacket& pPacket)
{
	if ( id == -1 ) return;
	ListServer_Send(CPacket() << (char)SLSPROFREQ << (short)id << pPacket);
}

void CPlayer::msgDSETPROFILE(CPacket& pPacket)
{
	CString account = pPacket.readChars(pPacket.readByte1());
	if (findPlayerId(account) != this)
		return;

	ListServer_Send(CPacket() << (char)SLSPROFSET << pPacket);
}

void CPlayer::msgWARPPLAYER(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasRight(CANWARPPLAYER))
	{
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for warping players");
		return;
	}

	CPlayer* player = (CPlayer*)playerIds[pPacket.readByte2()];
	if(player == NULL)
		return;

	float nX = (float)(pPacket.readByte1())/2;
	float nY = (float)(pPacket.readByte1())/2;
	CString name = pPacket.readString("");
	player->warp(name, nX, nY);
}
void CPlayer::msgDWANTRIGHTS(CPacket& pPacket)
{
	CString accname = pPacket.readString("");
	if (type != CLIENTRC || (accname != accountName && !hasRight(CANCHANGERIGHTS)))
	{
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for viewing admin rights");
		return;
	}

	int rights = 0;
	CString folders, ip;
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
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for changing admin rights");
		return;
	}

	CBuffer accname = pPacket.readChars(pPacket.readByte1());
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

	player->adminRights = (int)pPacket.readByte5();
	player->adminIp = pPacket.readChars((unsigned char)pPacket.readByte1());

	// Untokenize and load the directories.
	CString temp(pPacket.readChars(pPacket.readByte2()));
	temp.untokenize();
	myFolders.clear();
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

	if (player->ftpOn) player->msgDWANTFTP(CPacket() << "");
	player->saveAccount(true);
	if (player->id == -1)
	{
		CPlayer *pl = findPlayerId(accname);
		if (pl != NULL) pl->loadDBAccount(accname);
		delete player;
	}
	sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has set the rights of " << accname);
}

void CPlayer::msgDWANTCOM(CPacket&pPacket)
{
	if (type != CLIENTRC || !hasRight(CANCHANGECOMMENTS))
	{
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for viewing comments");
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
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for changing admin rights");
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
		if (player->id == -1)
			delete player;
	}
}

void CPlayer::msgDEDITBAN(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
	{
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for banning players");
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
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for banning players");
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
		sendRCPacket(CPacket() << (char)DRCLOG << accountName << " has set the ban of " << accname);
		if (player->id == -1)
			delete player;
	}
}

void CPlayer::msgDWANTFTP(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
		return;

	sendPacket(CPacket() << (char)SFOLDERFTP << myFolders.join("\n").tokenize());
	sendPacket(CPacket() << (char)STEXTFTP << "Welcome to the FileBrowser");

	if (myFolders.count() > 0)
	{
		myFolders[0].setRead(0);
		CBuffer rights = myFolders[0].readString(" ");
		CBuffer folder = myFolders[0].readString("");
		folder.remove(folder.length() - 1, 1);
		sendPacket(CPacket() << (char)SSENDFTP << (char)folder.length() << folder << listFiles(folder.text(), rights.text()));
		ftpOn = true;
	}
	myFolders[0].setRead(0);
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
		return;

	CString shortName = pPacket.text() + 1;
	fileList.add(new COutFile(CString() << lastFolder << shortName));
	sendPacket(CPacket() << (char)STEXTFTP << "Downloaded file " << shortName);
}

void CPlayer::msgDFILEFTPUP(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
		return;

	CBuffer shortName, fileData, fileName;
	shortName = pPacket.readChars((unsigned char)pPacket.readByte1());
	fileName = CString() << lastFolder << shortName;
	fileData = pPacket.copy(pPacket.getRead());
	fileData.save(fileName.text());
	sendPacket(CPacket() << (char)STEXTFTP << "Uploaded file " << fileName);
	updateFile(shortName.text());
}

void CPlayer::msgDFILEFTPMOV(CPacket& pPacket)
{
	// learn 2 fucking order...
	if (type != CLIENTRC || !hasStaff())
		return;

	CBuffer f1, f2, f3, f4;
	f1 = pPacket.readChars((unsigned char)pPacket.readByte1());
	f2 = pPacket.readString("");
	f2.removeAll("\"");
	f1 << f2;
	f4 << lastFolder << f2;

	f3.load(f4.text());
	f3.save(f1.text());
	remove(f4.text());
}

void CPlayer::msgDFILEFTPDEL(CPacket& pPacket)
{
	if (type != CLIENTRC || !hasStaff())
		return;
	// no security.. oh well
	CString fileName = CString() << lastFolder << pPacket.text() + 1;
	remove(fileName.text());
	sendPacket(CPacket() << (char)STEXTFTP << "Deleted file " << fileName);
}

void CPlayer::msgDFILEFTPREN(CPacket& pPacket)
{
	if(type != CLIENTRC)
		return;
	// no security.. oh well
	CString f1 = CString() << lastFolder << pPacket.readChars((unsigned char)pPacket.readByte1());
	CString f2 = CString() << lastFolder << pPacket.readChars((unsigned char)pPacket.readByte1());
	rename(f1.text(), f2.text());
	sendPacket(CPacket() << (char)STEXTFTP << "Renamed file " << f1 << " to " << f2);
}
