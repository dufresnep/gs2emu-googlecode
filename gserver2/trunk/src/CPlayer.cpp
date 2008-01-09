// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CPlayer.h"
#include "CIni.h"
#include "CWeapon.h"
#include "CBaddy.h"
#include "CDatabase.h"
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
            &CPlayer::msgEMPTY44,&CPlayer::msgEMPTY45,
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
            &CPlayer::msgDFILEFTPDOWN, &CPlayer::msgDFILEFTPUP
        };

//Props that will be sent when the player logs in
bool sendInit[] = {
    false,true ,true ,true ,true ,true,
    true ,true ,true ,true,true ,true,
    false ,true ,false,true ,true ,true,
    true ,true ,true,true ,true,false,
    false,true, true ,true ,false,false,
    false,false ,true,false,false,true,
    false,false,false ,false,false,false, //36
    false,false,false ,false,false,false,
    false,false,false ,false,false,false
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
bool udpDontForward[propscount] = {
    false, false, false, false, false, false,
    false, false, false, false, false, false,
    false, false, false, false, false, false,
    false, false, false, false, false, false,
    false, false, false, false, false, false,
    false, false, false, false, false, false,
    false, false, false, false, false, false,
    false,false,false,false,false,false,
    false,false,false,false,false,false
};
//Props that will be forwarded to people in the same level as you
bool forwardLocal[propscount] = {
    false,false,true ,false,false,false,
    false,false,true,true,true ,true,
    true ,true,false,true ,true,true,
    true,true, true,true,false,false,
    false,true ,false,false,true ,true,
    true, false,true,false,true,true,
    true,true ,true,true ,true ,true,
    false,false,false,false,false,false,
    false,false,false,false,false,false
};

//Other players props that will be forwarded to you when you log in
bool sendOthersInit[propscount] = {
    true ,false,false,false,false,false,
    false,false,true,true,true ,true ,
    true,true ,false,true ,true,true, //2
    true ,true ,true,true ,false,false,
    true ,false,false,false,false,false,
    true,true,true,false,true,true,
    false ,true ,true,true ,true ,true ,//36
    false,false,false,false,false,false,
    false,false,false,false,false,false
};

bool sendRcProps[propscount] =
{
    false,true ,true ,true ,true ,true ,
    true ,true ,true ,true ,true ,true ,
    false ,true ,false,true ,true,false,
    true ,false ,true, true ,true,true,           //19 Status
    false ,false ,true ,true,true,true,
    true ,false ,true ,false ,false ,true ,
    false ,false,false ,false,false,false,
    false,false,false,false,false,false,
    false,false,false,false,false,false
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
	loadOnly = deleteMe = false;
    firstPacket = firstLevel = true;
    key = adminRights =  0;
    lastData = lastMovement = lastSave = time(NULL);
    lastNick = 0;

    iterator = 0x04A80B38;
    x = y = 32;
    z = 0;
    rubins = darts = bombCount = 99;
    glovePower = bombPower = swordPower = shieldPower = 1;
    power = maxPower = 3;
    kills = deaths = udpPort = statusMsg= rating =0;
    sprite = 2;
    status = 16;
    onlineSecs = ap = horseBushes = magicPoints = 0;
    apCounter = 100;
    type = lastIp = additionalFlags = adminLevel = 0;
    memset(colors, 0, sizeof(colors));
    levelName = "";
    gAni = "idle";
    headImage = "head0.png";
    swordImage = "sword1.png";
    bodyImage = "body.png";
    nickName = "Unknown";
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
        if(type == CLIENTPLAYER)
        {
            saveAccount();
            saveWorldProps();
        }
			else if (type == CLIENTRC)
		{
			sendRCPacket(CPacket() << (char)DRCLOG << "RC Disconnected: " << nickName << "(" << accountName << ")");
		}
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
        errorOut(CString() << "Client " << accountName << " disconnected with sock error: "
            <<toString(size));
        deleteMe = true;
        compressAndSend();
        return;
    }

	if(receiveBuff.length() >=128*1024)
	{
		errorOut(CString() << "Too much data from " << accountName);
		sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent to much data (>=128k in the input buffer)");
		deleteMe = true;
		return;
	}

    while(receiveBuff.length()>=2)
    {
        lastData = time(NULL);
        int error, cLen = sizeof(packets);
        int len = (((unsigned int)(unsigned char)receiveBuff[0]) << 8) + (unsigned int)(unsigned char)receiveBuff[1];
		if(len > receiveBuff.length()-2 || len < 0)
		{
			errorOut(CString() << "Client " << accountName << " sent a wrong data package length: " << toString(len));
			sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent a wrong data package length.");
			deleteMe = true;
			return;
		}

        if ((error = uncompress((Bytef*)packets,(uLongf*)&cLen,(const Bytef*)receiveBuff.text()+2, len)) == Z_OK)
        {
            CPacket lines;
			if(cLen <= 0)
			{
				errorOut(CString() << "Client " << accountName << " sent an empty package.");
				sendPacket(CPacket() << (char)DISMESSAGE << "Your Graal.exe has sent an empty data package.");
				deleteMe = true;
				return;
			}

            lines.writeBytes(packets, cLen);
            while(lines.bytesLeft())
            {
                packCount++;

                if(firstPacket)
                    processLogin(CPacket() << lines.readString("\n"));
                else parsePacket(CPacket() << lines.readString("\n"));
				if(deleteMe)
					return;
            }
            receiveBuff.remove(0, len+2);
        }
        else
        {
            errorOut(CString() << "Decompression error for player " <<
				accountName);
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
    pPacket.readByte1();    //skip byte...?

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
        errorOut("Player limit reached");
        sendPacket(CPacket() << (char)DISMESSAGE << "This server has reached its player limit.");
        deleteMe = true;
        return;
    }

    CString ip = playerSock->tcpIp();
    if (isIpBanned(ip))
    {
        errorOut(CString() << accountName << " is ip banned");
        sendPacket(CPacket() << (char)DISMESSAGE << "Your ip has been banned from this server.");
        deleteMe = true;
        return;
    }

    //Send a verification request to the server for account name and password
    if(lsConnected)
        listServer.sendBuffer(CPacket() << (char)SLVACCOUNT << (char)accountName.length() << accountName << (char)password.length() << password << "\n");
    else
    {
        errorOut("List server is unavailable");
        sendPacket(CPacket() << (char)DISMESSAGE << "Unable to contact account server.");
        deleteMe = true;
    }
}

void CPlayer::sendAccount()
{
    CPacket packet;

    if(loadAccount())  //Do login check
    {
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
                        errorOut(CString() << "No data for 30 secs from " << player->accountName);
                        player->sendPacket(CPacket() << (char)DISMESSAGE << "Someone else has logged into your account.");
                        player->deleteMe = true;
                    }
                    else
                    {
                        errorOut(accountName << " account is already in use.");
                        sendPacket(CPacket() << (char) DISMESSAGE << "This account is already in use.");
                        deleteMe = true;
                        return;
                    }
                }
            }
        }
        errorOut(CString() << "New player: " << accountName);
        saveAccount();
        saveWorldProps();
        id = createPlayerId(this);
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

        if(type == CLIENTPLAYER)
		{
			sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
			sendPacket(CPacket() << (char)LEVELNAME << levelName);

			if (!sendLevel(levelName, x, y, 0))
				sendLevel(unstickmeLevel, x, y, 0);

			sendPacket(CPacket() << (char)STARTMESSAGE << serverMessage);
		} else
		{
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
					other->sendPacket(CPacket() << (char)DRCLOG << "New RC: " << nickName << " (" << accountName << ")");
			}
			else if(other->type == CLIENTPLAYER)
				other->sendPacket(playerProps);
        }
        if(type == CLIENTPLAYER)
        {
            for (int i = 0; i < myWeapons.count(); i++)
                weaponSend.add(myWeapons[i]);

            for (int i = 0; i < myFlags.count(); i++)
                sendPacket(CPacket() << (char)SSETFLAG << myFlags[i]);

            for (int i = 0; i < serverFlags.count(); i++)
                sendPacket(CPacket() << (char)SSETFLAG << serverFlags[i]);
        }
        newPlayers.remove(this);
        playerList.add(this);
    } else
    {
        errorOut(CString() << accountName << " is not allowed on server");
        deleteMe = true;
    }
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
    }else
        packet = pPacket;

    int messageId = packet.readByte1();
    if(id < 0 && messageId != 37 && messageId != 39)
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
    outBuff << pPacket << "\n";
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
		errorOut(CString() << "Compression error for " << accountName);
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
		errorOut(CString() << "Sending is blocked for " << accountName);
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
            errorOut(CString() << "Send error to " << accountName);
            deleteMe = true;
            return;
        }
        else break;
    }

}
void CPlayer::warp(CString& pLevel, float pX, float pY)
{
    x = pX;
    y = pY;
    if(pLevel != levelName)
    {
        sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << pLevel);
        sendPacket(CPacket() << (char)LEVELNAME << pLevel);
        if(!sendLevel(pLevel, x, y, 0))
        {
            sendPacket(CPacket() << (char)LEVELFAILED);
            sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
            sendPacket(CPacket() << (char)LEVELNAME << levelName);
            if(!sendLevel(levelName, x, y, 0))
            {
                sendPacket(CPacket() << (char)DISMESSAGE << "No level available");
                errorOut(CString() << "Cannot find a level for " << accountName);
                deleteMe = true;
            }
        }
    } else
	{
		updateProp(PLAYERX);
		updateProp(PLAYERY);
	}
}
void CPlayer::processChat(CString& pMessage)
{
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

    if (words[0] == "warpto" &&(hasRight(CANWARPPLAYER) || hasRight(CANWARPXY)))
    {
        if(words.count() <= 1)
            return;
		if (!hasRight(CANWARPPLAYER))
		{
			chatMsg = CString() << "(not authorized to warp)";
			updateProp(CURCHAT);
			return;
		}

        CString name;
        name = levelName;

        if (words.count() == 2)
        {
            CPlayer* other = findPlayerId(words[1]);
            if(other == NULL)
                return;
            name = other->levelName;
            x = other->x;
            y = other->y;
        } else if(words.count() >= 3 && hasRight(CANWARPXY))
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
		if (!hasRight(CANSUMMON))
		{
			chatMsg = CString() << "(not authorized to summon)";
			updateProp(CURCHAT);
			return;
		}

        CPlayer* other = findPlayerId(words[1]);
        if(other != NULL)
			other->warp(levelName, x, y);
    }
    else if (words[0] == "sethead")
    {
        if (words.count() <= 1)
            return;

        if(strlen(getDataFile(words[1].text())))
        {
            headImage = words[1];
            updateProp(HEADGIF);
        }
            else
        {
            listServer.sendBuffer(CPacket() << (char)SLVFILE << (short)id << (char)0 << (char)words[1].length() << words[1]);
        }
    }
    else if (words[0] == "setbody")
    {
        if (words.count() <= 1)
            return;

        if(strlen(getDataFile(words[1].text())))
        {
            bodyImage = words[1];
            updateProp(BODYIMG);
        }
            else
        {
            listServer.sendBuffer(CPacket() << (char)SLVFILE << (short)id << (char)1 << (char)words[1].length() << words[1]);
        }
    } else if(words[0] == "setsword")
    {
        if(words.count() <= 1)
            return;

        if(strlen(getDataFile(words[1].text())))
        {
            swordImage = words[1];
            updateProp(SWORDPOWER);
        }
            else
        {
            listServer.sendBuffer(CPacket() << (char)SLVFILE << (short)id << (char)2 << (char)words[1].length() << words[1]);
        }
    } else if(words[0] == "setshield")
    {
        if(words.count() <= 1)
            return;

        if (strlen(getDataFile(words[1].text())))
        {
            shieldImage = words[1];
            updateProp(SHIELDPOWER);
        }
            else
        {
            listServer.sendBuffer(CPacket() << (char)SLVFILE << (short)id << (char)3 << (char)words[1].length() << words[1]);
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
                listServer.sendBuffer(CPacket() << (char)SLVGUILD << (short)id << (char)accountName.length() << accountName << (char)tmpNick.length() << tmpNick << (char)tmpGuild.length() << tmpGuild);
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

	listServer.sendBuffer(CPacket() << (char)SLVNICKNAME << (char)accountName.length() << accountName << getProp(NICKNAME));
    sendPacket(CPacket() << (char)SPLAYERPROPS << (char)NICKNAME << getProp(NICKNAME));
    for (int i = 0; i < playerList.count(); i++)
    {
        CPlayer*other = (CPlayer*)playerList[i];
        if (other != this)
            other->sendPacket(CPacket() << (char)OTHERPLPROPS << (short)id << (char)NICKNAME << getProp(NICKNAME));
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
	    temp << (char)SUNSETFLAG << myFlags[i];
	for (int i = 0; i < myWeapons.count(); i++)
	    temp << (char)SDELNPCWEAPON << myWeapons[i];
	sendPacket(temp);

	myFlags.clear();
	for(int i = pPacket.readByte2(); i >0; i--)
	{
		int len = pPacket.readByte1();
        if(len > 0)
            myFlags.add(pPacket.readChars(len));
	}

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
	if (staffList.find(accountName) < 0 || !ip.match(adminIp.text()))
	{
		if (staffOnly)
		{
			sendPacket(CPacket() << (char)DISMESSAGE << "This server is currently restricted to staff only.");
			return false;
		}

		if (type == CLIENTRC)
		{
			errorOut(CString() << "Attempted RC login by " << accountName);
			sendPacket(CPacket() << (char)DISMESSAGE << "You do not have RC rights");
			return false;
		}
	}

    if(loadOnly || !loadWorldProps(accountName) || !createNew)
        return loadWorldPropsIni();

    return true;
}

bool CPlayer::sendLevel(CString& pLevel, float pX, float pY, int pModTime)
{
    CPacket packet;
    CString newLevel = pLevel;
	if(!pLevel.length())
		return false;
    int pos = newLevel.find('§');
    if(pos >= 0)
    {
        //fuck playerworlds...just find the real level name
        //playerWorld = newLevel.copy(0, pos);
        newLevel = newLevel.copy(pos+1, newLevel.length()-pos-1);
    }

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
        else sendPacket(CPacket() << (char)LEVELCHEST << (char)0 << (char)chest->x << (char)chest->y << (char)chest->item << (char)chest->signIndex);
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
        if (myWeapons.find(weaponSend[i].text()) < 0)
            myWeapons.add(weaponSend[i]);

        bool defaultWeapon = false;
        for (int ii = 0; ii < itemcount; ii++)
        {
            if (weaponSend[i] == itemNames[ii])
            {
                defaultWeapon = true;
                sendPacket(CPacket() << (char)SADDDEFWEAPON << (char)ii);
                break;
            }
        }
        if (defaultWeapon)
            continue;
        for (int ii = 0; ii < weaponList.count(); ii++)
        {
            CWeapon* weapon = (CWeapon*)weaponList[ii];
            if (weapon->name == weaponSend[i])
            {
                sendPacket(CPacket() << (char)SADDNPCWEAPON <<
                           weapon->getSendData());
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
		CString longName = getDataFile(shortName.text());

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

                    // old method
                    sendPacket(CPacket() << (char)102 << (long long)modTime << (char)shortName.length() << shortName << fileData);

                    // my method of sending big-files -- doesn't work =[
                    /*while (fileData.length() > 0)
                    {
                        CString temp = fileData.copy(0, (fileData.length() > 64500 ? 64500 : fileData.length()));
                        fileData.remove(0, temp.length());
                        sendPacket(CPacket() << (char)102 << (long long)modTime << (char)shortName.length() << shortName << temp);
                    }*/
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
				if (bombCount >= 5)
				{
					valid = true;
					bombCount -= 5;
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
        retVal.writeByte1(bombCount);
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

    case GATTRIB1:
    case GATTRIB2:
    case GATTRIB3:
    case GATTRIB4:
    case GATTRIB5:
    {
        int index = pProp-GATTRIB1;
        retVal << (char)ganiAttribs[index].length() << ganiAttribs[index];
    }
    break;
	case RATING:
		retVal.writeByte2(rating);
	break;
    default:
		errorOut(CString() << "Unknown getProp() by " << accountName);
        break;
    }
    return retVal;
}

void CPlayer::setProps(CPacket& pProps, bool pForward)
{
    int len;
    CPacket forwardBuff;
    forwardBuff << (char) OTHERPLPROPS << (short)id;
    while (pProps.bytesLeft())
    {
//        int startpos = pProps.getRead();
        int index = pProps.readByte1();
        if(index < 0 || index >= propscount-1)
        {
            errorOut(CString() << "setProps(" << toString(index) << ") index out of bounds by " << accountName);
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
            bombCount = CLIP(pProps.readByte1(), 0, 99);
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
                        ((CPlayer*)level->players[0])->sendPacket(CPacket() <<
                                (char)ISLEADER);
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

        case EFFECTCOLORS:
			pProps.readByte5();
            break;

        case CARRYNPC:
            pProps.readByte3();
            //this doesnt work even when i add the correct code...:/
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
            udpPort = pProps.readByte3();
            if(udpPort > 0)
            {
                sendPacket(CPacket() << (char)DISMESSAGE <<
                    "This server doesnt support UDP. Please disable UDP in general settings.");
                deleteMe = true;
            }
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

        case GATTRIB1:
        case GATTRIB2:
        case GATTRIB3:
        case GATTRIB4:
        case GATTRIB5:
        {
            int i = index-GATTRIB1;
            len = pProps.readByte1();
            if(len >= 0)
                ganiAttribs[i] = pProps.readChars(len);
        }

		case RATING:
			rating = pProps.readByte2();
			break;
        default:
			errorOut(CString() << "Setprops error: " << toString(index) << " By " << accountName);
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
        }
    }

    if (pForward && forwardBuff.length() > 0)
        sendLocally(forwardBuff);
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
            bombCount = CLIP(bombCount + 5, 0, 99);
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
    sendPacket(CPacket() << (char)LEVELNAME << newLevel);

    if(!sendLevel(newLevel, x2, y2, modTime))
	{
		sendPacket(CPacket() << (char)LEVELFAILED);
		sendPacket(CPacket() << (char)PLAYERWARPED << getProp(PLAYERX) << getProp(PLAYERY) << levelName);
		sendPacket(CPacket() << (char)LEVELNAME << levelName);

		if(!sendLevel(levelName, x, y, 0))
		{
			sendPacket(CPacket() << (char)DISMESSAGE << "Cannot open previous level");
			errorOut(CString() << "Cannot find a level for " << accountName);
			deleteMe = true;
		}
	}
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
    CPacket message;
    message << (char)STOALLMSG << (short)id << pPacket.text()+1;

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
    if (other == NULL)
        return;
    if(level->sparZone)
        return;

    other->kills ++;
	if (apSystem)
	{
		if (other->ap > 0)
		{
			other->ap -= (other->ap/20+1)*(ap/20);
			if(ap < 0)
				ap = 0;
			other->updateProp(PALIGNMENT);
		}
	}

}

void CPlayer::msgBADDYPROPS(CPacket& pPacket)
{
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
        flagList = &serverFlags;

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
    sendLocally(CPacket() << (char)SNPCWEAPONIMG << (short)id << pPacket.text()+1);
}

void CPlayer::msgEMPTY25(CPacket& pPacket)
{}
void CPlayer::msgHURTPLAYER(CPacket& pPacket)
{
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

    CPacket packet;
    packet << (char)SEXPLOSION  << (short)id << pPacket.text()+1;
    sendLocally(packet);
}

void CPlayer::msgPRIVMESSAGE(CPacket& pPacket)
{
	if(getTime() - lastMessage <= 4)
	{
		sendPacket(CPacket() << (char)SADMINMSG <<
                   "Server message:§You can only send messages once every four seconds.");
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
        sendPacket(CPacket() << (char)SADMINMSG << "Server message:§Your message was too long.");
        return;
    }

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
        errorOut(CString() << accountName << " has sent an incorrect number of packets");
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
void CPlayer::msgEMPTY44(CPacket& pPacket)
{}
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
void CPlayer::msgSWANTSOPTIONS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	//want server options
	CString serverOptions;

	serverOptions << "\"" << "maxPlayers=" << toString(maxPlayers) << "\",";
	serverOptions << "\"" << "maps=" << mapNames.join(",") << "\",";
	serverOptions << "\"" << "maxNoMovement=" << toString(maxNoMovement) << "\",";
	serverOptions << "\"" << "bushesDrop=" << toString((int)bushesDrop) << "\",";
	serverOptions << "\"" << "vasesDrop=" << toString((int)vasesDrop) << "\",";
	serverOptions << "\"" << "tileRespawn=" << toString(tileRespawn) << "\",";
	serverOptions << "\"" << "horseLife=" << toString(horseLife) << "\",";
	serverOptions << "\"" << "apInterval=" << toString(apInterval) << "\",";
	serverOptions << "\"" << "unstickmex=" << toString(unstickmeX) << "\",";
	serverOptions << "\"" << "unstickmey=" << toString(unstickmeY) << "\",";
	serverOptions << "\"" << "unstickmelevel=" << unstickmeLevel << "\",";
	serverOptions << "\"" << "dropItemsDead=" << toString((int)dropItemsDead) << "\",";
	sendPacket(CPacket() << (char)DSENDSOPTIONS << serverOptions);
}
void CPlayer::msgSSETOPTIONS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	CString options = pPacket.text() + 1;
}

void CPlayer::msgWANTRCFOLDERS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	sendPacket(CPacket() << (char)SSENDFOLDER << rcFolders.join(","));
}

void CPlayer::msgSETRCFOLDERS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANEDITFOLDEROPTION))
	{
		sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for changing the folder configuration.");
		return;
	}
	CString folders = pPacket.text()+1;
	rcFolders.load(folders.text(), ",");
	rcFolders.save("rcfolders.txt");
	sendPacket(CPacket() << (char)DRCLOG << accountName << " has updated the folder configuration.");
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
    if(type != CLIENTRC)
        return;
    if(!hasRight(CANSETATTRIBS))
    {
        sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for settings attributes.");
        return;
    }
	CPlayer* player = (CPlayer*)playerIds[pPacket.readByte2()];
	if(player == NULL)
		return;
	player->setAccPropsRc(pPacket);
	player->saveWorldProps();
	sendPacket(CPacket() << (char)DRCLOG << accountName << " set attributes of player " << player->accountName);
}
void CPlayer::msgDISPLAYER(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANDISCONNECT))
	{
	    sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for disconnecting players.");
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
    if(type != CLIENTRC)
        return;
    if(!hasRight(CANUPDATELEVEL))
    {
        sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to update levels.");
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
    if(type != CLIENTRC)
        return;
    if (!hasRight(CANADMINMSG))
    {
        sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to send admin messages.");
        return;
    }

    CString message = pPacket.readString("");
	for(int i = 0; i < playerList.count(); i++)
	{
		CPlayer* player = (CPlayer*)playerList[i];
		if(player != this)
            player->sendPacket(CPacket() << (char)SADMINMSG << "Administrator " <<
                      accountName << ":§" << message);
	}

}
void CPlayer::msgPRIVADMINMSG(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
    if(!hasRight(CANADMINMSG))
    {
        sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to send admin messages.");
        return;
    }
	CPlayer* other = (CPlayer*)playerIds[pPacket.readByte2()];
	if(other == NULL)
		return;
	other->sendPacket(CPacket() << (char)SADMINMSG << "Admin " << accountName <<
		": " << pPacket.readString(""));
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
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
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
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANSETSERVERFLAG))
	{
	    sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized to change the server flags.");
		return;
	}
	int count = pPacket.readByte2();
	serverFlags.clear();
	for(int i = 0; i < count; i++)
	{
		CString flag = pPacket.readChars(pPacket.readByte1());
		serverFlags.add(flag);
	}
	sendPacket(CPacket() << (char)DRCLOG << accountName << " updated the server flags.");
}
void CPlayer::msgDADDACCOUNT(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(adminLevel < 5)
	{
	    sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for adding accounts.");
		return;
	}
    CString accname = pPacket.readChars(pPacket.readByte1());
    CString pass = pPacket.readChars(pPacket.readByte1());
    CString email = pPacket.readChars(pPacket.readByte1());
    bool banned = (pPacket.readByte1()>0);
    bool onlyLoad = (pPacket.readByte1()>0);
    int adminLvl = pPacket.readByte1();
    CAccount newAccount;
    newAccount.accountName = accname;
    newAccount.banned = banned;
    newAccount.loadOnly = onlyLoad;
    newAccount.adminLevel = adminLvl;
    newAccount.saveAccount();


}
void CPlayer::msgDDELACCOUNT(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(adminLevel < 5)
	{
	    sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for deleting accounts.");
		return;
	}
	CString accname = pPacket.readString("");
	doQuery(CString() << "DELETE FROM accounts" <<
		" WHERE accname LIKE '" << accname << "'");
    doQuery(CString() << "DELETE FROM " << worldName <<
        " WHERE accname LIKE '" << accname << "'");

}
void CPlayer::msgDWANTACCLIST(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for viewing the accounts list.");
		return;
	}
	CString name = pPacket.readChars(pPacket.readByte1());
	CString condition = pPacket.readChars(pPacket.readByte1());
	sendPacket(CPacket() << (char)DACCLIST << getAccountList(name, condition));
}
void CPlayer::msgDWANTPLPROPS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	CPlayer *player = (CPlayer*)playerIds[pPacket.readByte2()];
	if(player != NULL)
        sendPacket(CPacket() << (char)DACCPLPROPS  << (short) player->id <<
            player->getAccPropsRC());
}
void CPlayer::msgDWANTACCPLPROPS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	CString accname = pPacket.readChars(pPacket.readByte1());
	CPlayer* player = findPlayerId(accname);
	if(player)
	{
		sendPacket(CPacket() << (char)DACCPLPROPS  << (short) player->id <<
			player->getAccPropsRC());
	} else
	{
		CPlayer player(NULL);
		if(!player.loadDBAccount(accname))
			return;
		player.loadDBAccount(accname);
		player.loadWorldProps(accname);
		sendPacket(CPacket() << (char)DACCPLPROPS  << (short) 0 <<
		player.getAccPropsRC());
	}
}
void CPlayer::msgDRESETPLPROPS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANRESETATTRIBS))
	{
	    sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: not authorized for resetting accounts");
		return;
	}
	CString account = pPacket.readString("");
	doQuery(CString() << "DELETE FROM " << worldName <<
			" WHERE accname LIKE '" << account << "'");
    //If account is online now than reset stats, load ini and disconnect
	CPlayer* player = findPlayerId(account);
	if(player)
	{
		player->myWeapons.clear();
		player->myChests.clear();
		player->myFlags.clear();
		player->loadWorldPropsIni();
		player->sendPacket(CPacket() << (char)DISMESSAGE << "Your account was reset");
		player->deleteMe =true;
	}
	sendPacket(CPacket() << (char)DRCLOG << accountName << " has reset the attributes of " << account);
}
void CPlayer::msgDSETACCPLPROPS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANSETATTRIBS))
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have rights to change attributes");
		return;
	}
	CString accname = pPacket.readChars(pPacket.readByte1());
	CPlayer* player = findPlayerId(accname);
	//If account is online
	if(player)
	{
		player->setAccPropsRc(pPacket);
		player->saveWorldProps();
	}
	else
	{
		CPlayer player(NULL);
		player.loadWorldProps(accname);
		player.setAccPropsRc(pPacket);
		player.saveWorldProps();
	}
}
void CPlayer::msgDWANTACCOUNT(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	CString accname = pPacket.readString("");
	CPlayer* player = findPlayerId(accname);
	if(player)
	{
		sendPacket(CPacket() << (char)DACCOUNT <<
			(char)accname.length() << accname <<
			(char)0 << (char)0 <<
			(char)player->banned <<
			(char)player->loadOnly <<
			(char)player->adminLevel <<
			(char)worldName.length() << worldName <<
			(char)0 <<
			(char)0);
	} else
	{
        CAccount account;
        if(!account.loadDBAccount(accname))
            return;

        sendPacket(CPacket() << (char)DACCOUNT <<
            (char)accname.length() << accname <<
            (char)0 << (char)0 <<
            (char)account.banned <<
            (char)account.loadOnly <<
            (char)account.adminLevel <<
            (char)worldName.length() << worldName <<
            (char)0 <<
            (char)0);
	}

}
void CPlayer::msgDSETACCOUNT(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(adminLevel < 5)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have enough rights");
		return;
	}
	CString accname = pPacket.readChars(pPacket.readByte1());
	CString pass = pPacket.readChars(pPacket.readByte1());
	CString email = pPacket.readChars(pPacket.readByte1());
	CPlayer* player = findPlayerId(accname);
	if(player)
	{
		player->banned = (pPacket.readByte1()>0);
		player->loadOnly = (pPacket.readByte1()>0);
		player->adminLevel = pPacket.readByte1();
		pPacket.readChars(pPacket.readByte1());	//skip playerworld
		player->comments = pPacket.readChars(pPacket.readByte1());
		player->saveAccount();
	} else
	{
		CAccount account;
		if(!account.loadDBAccount(accname))
			return;
		account.banned = (pPacket.readByte1()>0);
		account.loadOnly = (pPacket.readByte1()>0);
		account.adminLevel = pPacket.readByte1();
		pPacket.readChars(pPacket.readByte1());	//skip playerworld
		account.comments = pPacket.readChars(pPacket.readByte1());
		account.saveAccount();
	}

}
void CPlayer::msgDRCCHAT(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
    if(!adminLevel)
    {
        sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
        return;
    }
    CStringList words;
    CString message = pPacket.text()+1;
    words.load(message.text(), " ");
    if(words.count() < 1)
        return;


    if(message[0] != '/')
    {
        sendRCPacket(CPacket() << (char)DRCLOG << nickName << ": " << message);
        return;
    }

    CPacket forward;
    //Commands with 1 word
    if(words[0] == "/help")
    {
        for(int i = 0; i < RCHelpMessage.count(); i++)
            sendPacket(CPacket() << (char)DRCLOG << RCHelpMessage[i]);
    }
    if(words[0] == "/reloadweapons" && adminLevel >= 4)
    {
        if(loadWeapons("weapons.txt"))
			sendPacket(CPacket() << (char)DRCLOG << accountName << " reloaded the weapons list.");
    }

	else if(words[0] == "/reloadstaffguilds" && adminLevel >= 4)
	{
		staffGuilds.load("staffguilds.txt");
		sendPacket(CPacket() << (char)DRCLOG << accountName << " reloaded the staff guilds.");
	}

    else if(words[0] == "/shutdown" && adminLevel >= 5)
    {
        sendPacket(CPacket() << (char)DRCLOG << accountName << " shutdown the server.");
        shutdownServer();
    }

    else if(words[0] == "/cachesubdirs" && adminLevel >= 4)
    {
        subDirs.clear();
        getSubDirs(dataDir);
        sendPacket(CPacket() << (char)DRCLOG << accountName << " reloaded the directory cache.");

    }

    else if(words[0] == "/reloadservermessage" && adminLevel >= 4)
    {
        loadServerMessage();
        sendPacket(CPacket() << (char)DRCLOG << accountName << " reloaded the server message.");
    }

    //Commands with 2 words
    if(words.count() < 2)
        return;

    if(words[0] == "/banip" && hasRight(CANBAN))
    {
        CStringList ipBans;
        ipBans.load("ipbans.txt");
        if(ipBans.find(words[1]) < 0)
        {
            ipBans.add(words[1]);
            ipBans.save("ipbans.txt");
            sendPacket(CPacket() << (char)DRCLOG << accountName << " banned IP " << words[1] << ".");
        } else sendPacket(CPacket() << (char)DRCLOG << accountName << " prob: IP " << words[1] << " is already banned.");
    }
		else if(words[0] == "/removeguild" && hasRight(CANSETSERVEROPTION))
    {
        if(removeGuild(words[1]))
            sendPacket(CPacket() << (char)DRCLOG << accountName << " removed guild " << words[1] << ".");

    }else if(words[0] == "/addguild" && adminLevel >= 3)
    {
        if(addGuild(words[1]))
            sendPacket(CPacket() << (char)DRCLOG << accountName << " added guild " << words[1] << ".");
    }

    //Commands with 3 words
    if(words.count() < 3)
        return;

    if(words[0] == "/addguildmember" && adminLevel >= 3)
    {
        if(addGuildMember(words[1], words[2]))
            sendPacket(CPacket() << (char)DRCLOG << accountName << " added " << words[2] << " to guild " << words[1] << ".");
    } else if(words[0] == "/removeguildmemberremoveguildmember" && adminLevel >= 3)
    {
        if(removeGuildMember(words[1], words[2]))
            sendPacket(CPacket() << (char)DRCLOG << accountName << " removed " << words[2] << " from guild " << words[1] << ".");
//            message << nickName << " removed guild member " << words[2] << " from guild " words[1] << ".";
    }

/*    if(message[0] != '/')
    {
        forward << (char)DRCLOG << message;
        for(int i = 0; i < playerList.count(); i++)
        {
            CPlayer* other = (CPlayer*)playerList[i];
            if(other->type == CLIENTRC)
                other->sendPacket(forward);
        }
    }*/


}

void CPlayer::msgDGETPROFILE(CPacket& pPacket)
{
    CPacket profile;
    CString account = pPacket.text()+1;

    CPlayer* player = findPlayerId(account);
    if (player == NULL)
        return;

    profile << getProfile(account);
    int time = player->onlineSecs;

    CString line;
    //Online time
    line << toString((int)time/3600) << " hrs ";
    line << toString((int)(time/60)%60) << " mins ";
    line << toString((int)time%60) << " secs";
	profile << (char)line.length() << line;

   // Kills:=playerkills,Deaths:=playerdeaths,Maxpower:=playerfullhearts,Rating:=playerrating,Alignment:=playerap,Gralat:=playerrupees,Swordpower:=playerswordpower,Spin Attack:=canspin,Power:=playerhearts,Darts:=playerdarts,Bombs:=playerbombs,MP:=playermp,Shieldpower:=playershieldpower,Glovepower:=playerglovepower

	for (int i = 0; i < profileList.count(); i++)
	{
		CStringList a;
		a.load(profileList[i].text(), ":=");
		if (a[0].length() < 1)
			continue;

		CString n;

		if (a[1] == "playerkills")
			n = toString(player->kills);
		else if (a[1] == "playerdeaths")
			n = toString(player->deaths);
		else if (a[1] == "playerfullhearts")
			n = toString(player->maxPower);
		else if (a[1] == "playerrating")
			n = toString(player->rating);
		else if (a[1] == "playerap")
			n = toString(player->ap);
		else if (a[1] == "playerrupees")
			n = toString(player->rubins);
		else if (a[1] == "playerswordpower")
			n = toString(player->swordPower);
		else if (a[1] == "canspin")
			n = (player->status & 64 ? "true" : "false");
		else if (a[1] == "playerhearts")
			n = toString(player->power);
		else if (a[1] == "playerdarts")
			n = toString(player->darts);
		else if (a[1] == "playerbombs")
			n = toString(player->bombCount);
		else if (a[1] == "playermp")
			n = toString(player->magicPoints);
		else if (a[1] == "playershieldpower")
			n = toString(player->shieldPower);
		else if (a[1] == "playerglovepower")
			n = toString(player->glovePower);
		else
		{
			for (int i = 0; i < player->myFlags.count(); i++)
			{
				CStringList b;
				b.load(player->myFlags[i].text(), "=");
				if (b[0] == a[1])
				{
					n = b[1];
					break;
				}
			}
		}

		profile << (char)(a[0].length() + n.length() + 2) << a[0] << ":=" << n;
	}

    sendPacket(CPacket() << (char)DPROFILE << profile);
}
void CPlayer::msgDSETPROFILE(CPacket& pPacket)
{
    CString account = pPacket.readChars(pPacket.readByte1());
    if(findPlayerId(account) != this)
        return;
    setProfile(account, pPacket);
}

void CPlayer::msgWARPPLAYER(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANWARPPLAYER))
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have rights to warp players");
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
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	CString accname = pPacket.readString("");
	int rights = 0;
	CString ip;
	CPlayer* player = findPlayerId(accname);
	if(player)
	{
		rights = player->adminRights;
		ip = player->adminIp;
	}
	else
	{
		CAccount account;
		if(!account.loadDBAccount(accname))
			return;
		rights = account.adminRights;
		ip = account.adminIp;
	}

	CPacket packet;
	packet << (char)SSENDRIGHTS;
	packet << (char)accname.length() << accname;
	packet << (long long)rights;
	packet << (char)ip.length() << ip;	//ip range
	packet << (char)0;
	packet << (char)0; //folder rights
	sendPacket(packet);

}

void CPlayer::msgDSETRIGHTS(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANCHANGERIGHTS))
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have rights to change rights");
		return;
	}
	CString accname = pPacket.readChars(pPacket.readByte1());
	CPlayer* player = findPlayerId(accname);
	//If account is online now
	if(player)
	{
		player->adminRights = (int)pPacket.readByte5();
		player->adminIp = pPacket.readChars(pPacket.readByte1());
		player->saveAccount();
	}
	else
	{
		CAccount account;
		if(!account.loadDBAccount(accname))
			return;
		account.adminRights = (int)pPacket.readByte5();
		account.adminIp = pPacket.readChars(pPacket.readByte1());
		account.saveAccount();
	}

}

void CPlayer::msgDWANTCOM(CPacket&pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	CString accname = pPacket.readString("");
	CPlayer* player = findPlayerId(accname);
	if(player)
	{
		sendPacket(CPacket() << (char)SSENDCOM << (char)accname.length() << accname
			<< player->comments);
	} else
	{
		CAccount account;
		if(!account.loadDBAccount(accname))
			return;
		sendPacket(CPacket() << (char)SSENDCOM << (char)accname.length() << accname
			<< account.comments);
	}
}
void CPlayer::msgDSETCOM(CPacket&pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANCHANGECOMMENTS))
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have rights to change comments");
		return;
	}
	int len = pPacket.readByte1();
	if(len > 0)
	{
        CString accname = pPacket.readChars(len);
        CString comment = pPacket.readString("");
        CPlayer* player = findPlayerId(accname);
        if(player)
        {
            player->comments = comment;
            player->saveAccount();
        } else
        {
            CAccount account;
            account.loadDBAccount(accname);
            account.comments = comment;
            account.saveAccount();
        }
	}
}
void CPlayer::msgDEDITBAN(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!adminLevel)
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have any rights");
		return;
	}
	CString accname = pPacket.readString("");
	CAccount account;
	if(!account.loadDBAccount(accname))
		return;
	sendPacket(CPacket() << (char)SSENDBAN << (char)accname.length() << accname <<
			(char)account.banned << account.comments);


}
void CPlayer::msgDSETBAN(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	if(!hasRight(CANBAN))
	{
	    sendPacket(CPacket() << (char)DRCLOG << "Server: You do not have rights to ban");
		return;
	}
    int len = pPacket.readByte1();
    if(len > 0)
    {
        CString accname = pPacket.readChars(len);
        CPlayer* player = findPlayerId(accname);
        //If account is online now
        if(player)
        {
            player->banned = (pPacket.readByte1()>0);
            if(player->banned)
            {
                player->sendPacket(CPacket() << (char)DISMESSAGE << "You have been kicked from the server");
                player->deleteMe = true;
            }
            player->saveAccount();
        } else
        {
            CAccount account;
            account.loadDBAccount(accname);
            account.banned = (pPacket.readByte1()>0);
            account.comments = pPacket.readString("");
            account.saveAccount();
        }
    }
}

void CPlayer::msgDWANTFTP(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
	CString path;
	path <<  "world" << fSep;
    sendPacket(CPacket() << (char)SFOLDERFTP << rcFolders.join(","));
    sendPacket(CPacket() << (char)STEXTFTP <<"Welcome to the FileBrowser");
    sendPacket(CPacket() << (char)SSENDFTP << (char)1 << "/" << listFiles(path.text()));
	//sendPacket(CPacket() << (char)68 << "\"rw */*\"");

}

void CPlayer::msgDCHANGEFTP(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
    CString folder = pPacket.text() + 1;
    CString fullPath;
	fullPath << "world" << fSep << folder;
    sendPacket(CPacket() << (char)STEXTFTP << "Folder changed to " << folder);
    sendPacket(CPacket() << (char)SSENDFTP << (char)folder.length() << folder << listFiles(fullPath.text()));
}
void CPlayer::msgDENDFTP(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
}
void CPlayer::msgDFILEFTPDOWN(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
    CString shortName = pPacket.text() + 1;
    fileList.add(new COutFile(shortName, 0));
}
void CPlayer::msgDFILEFTPUP(CPacket& pPacket)
{
    if(type != CLIENTRC)
        return;
}
