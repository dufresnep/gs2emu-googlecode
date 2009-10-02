#include "TGserverConn.h"
#include "TNPC.h"
#include "TPlayerNC.h"

class TPlayerNC;
class TNPCServer;



TGserverConn::TGserverConn():fileQueue(&gserverSock)
{
	fileQueue.setCodec(ENCRYPT_GEN_3,0);
}

bool TGserverConn::connect(CString host, int port)
{
	// Initialize socket.
	gserverSock.setType(SOCKET_TYPE_CLIENT);
    gserverSock.setProtocol(SOCKET_PROTOCOL_TCP);
    gserverSock.setDescription("gserverSocket");

	NPCLOGIN = 34;
	type = PLTYPE_AWAIT;

	int result;
	result = gserverSock.init(host.text(),CString(port).text(),1);

	if (result==0) 
	{	
		cout << "Connecting to GServer. . ." << endl;

		if (gserverSock.connect() == SOCKET_OK)
		{
			cout << "Connected to GServer!" << endl;
		}
		else 
		{
			cout << "Unable to connect to gserver!" << endl;
			return false;
		}
	}

	createFunctions();

	return true;
}


void TGserverConn::login(CString nickname,CString account,CString password, int port)
{
	cout << "Sending Login Data. . " << endl;
	
	CString packet;

	fileQueue.addPacket(CString() >> (char)2 << "LNX00001" >> (char)account.length() << account
		>> (char)password.length() << password >> (short)port);

	fileQueue.addPacket(CString() >> (char)GO_PLAYERPROPS >> (char)PLPROP_NICKNAME >>  (char)nickname.length() << nickname);
	fileQueue.sendCompress();
}

bool TGserverConn::created = false;
typedef bool (TGserverConn::*TPLSock)(CString&);
std::vector<TPLSock> TPLFunc(255, &TGserverConn::msgPLI_NULL);

void TGserverConn::createFunctions()
{
	if (TGserverConn::created)
		return;

	TPLFunc[GI_WEAPONADD]		= &TGserverConn::msgGI_NPCWEAPONADD;
	TPLFunc[GI_OTHERPLPROPS]	= &TGserverConn::msgGI_OTHERPLPROPS;
	TPLFunc[GI_FLAGSET]			= &TGserverConn::msgGI_FLAGSET;
	TPLFunc[GI_PRIVATEMESSAGE]	= &TGserverConn::msgGI_PRIVATEMESSAGE;
	TPLFunc[GI_NEWWORLDTIME]	= &TGserverConn::msgGI_NEWWORLDTIME;
	TPLFunc[GI_NC_REQUEST]		= &TGserverConn::msgGI_NC_REQUEST;
	TPLFunc[GI_TRIGGERACTION]	= &TGserverConn::msgGI_TRIGGERACTION;
	TPLFunc[GI_LEVELNAME]		= &TGserverConn::msgGI_LEVELNAME;
	TPLFunc[GI_LEVELLINK]		= &TGserverConn::msgGI_LEVELLINK;
	TPLFunc[GI_LEVELSIGN]		= &TGserverConn::msgGI_LEVELSIGN;	
	TPLFunc[GI_NPCPROPS]		= &TGserverConn::msgGI_NPCPROPS;
	TPLFunc[GI_LEVELMODTIME]	= &TGserverConn::msgGI_LEVELMODTIME;
	TPLFunc[GI_NPCDEL]			= &TGserverConn::msgGI_NPCDEL; 
	TPLFunc[GI_NPCDEL2]			= &TGserverConn::msgGI_NPCDEL; 
	TPLFunc[GI_PLAYER_RIGHTS]   = &TGserverConn::msgGI_PLAYER_RIGHTS;
	TPLFunc[GI_PLAYERPROPS]		= &TGserverConn::msgGI_NPCDEL;

	created = true;
	// Finished
}

bool TGserverConn::msgPLI_NULL(CString& pPacket)
{
	CString packet = pPacket.readGUChar();
	pPacket.setRead(0);
	printf("Unknown Player Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	for (int i = 0; i < pPacket.length(); ++i) printf("%02x ", (unsigned char)((pPacket.text())[i])); printf("\n");

	return true;
}
bool TGserverConn::msgGI_NPCADDCLASS(CString& pPacket)
{
	CString className = pPacket.readChars(pPacket.readGUChar());
	CString classScript = pPacket.readString("");

	server->NC_AddClass(0,className,classScript);

	return true;
}

bool TGserverConn::msgGI_TRIGGERACTION(CString& pPacket)
{
	int playerId = pPacket.readGShort();
	unsigned int npcId = pPacket.readGUInt();
	float loc[2] = {(float)pPacket.readGUChar() / 2.0f, (float)pPacket.readGUChar() / 2.0f};
	
	CString action = pPacket.readString("").trim();
	
	if (loc[0] == 0.0f && loc[1] == 0.0f)
	{
		server->triggerAction(playerId,action.readString(","),action);
	}
	else
	{
		server->triggerAction(playerId,npcId,loc[0],loc[1],action.readString(","),action);
	}

	return true;
}

bool TGserverConn::msgGI_LEVELNAME(CString& pPacket)
{
	CString levelName = pPacket.readString();

	TLevel * level = server->getLevel(levelName);

	if (level == 0)
	{
		server->AddLevel(levelName);
		return true;
	}

	server->getLevel(server->getCurrentLevel())->clearLevel();

	//sets the level to monitor for changes
	server->UpdateCurrentLevel(levelName);
	
	return true;
}

bool TGserverConn::msgGI_LEVELMODTIME(CString& pPacket)
{
	time_t modTime = pPacket.readGInt5();
	server->getLevel(server->getCurrentLevel())->setModTime(modTime);

	return true;
}
bool TGserverConn::msgGI_LEVELLINK(CString & pPacket)
{
	//TODO

	return true;
}
bool TGserverConn::msgGI_LEVELSIGN(CString & pPacket)
{
	//TODO

	return true;
}

bool TGserverConn::msgGI_NPCPROPS(CString& pPacket)
{
	unsigned int npcId = pPacket.readGUInt();
	CString npcProps = pPacket.readString("");

	TNPC * npc = server->getNPC(npcId);

	if (npc == 0 )
	{
		server->addNPC(npcId,npcProps);
		return true;
	}

	npc->setProps(npcProps);

	return true;
}

bool TGserverConn::msgGI_PLAYER_RIGHTS(CString& pPacket)
{
	CString account = pPacket.readChars(pPacket.readGUChar());
	long rights = pPacket.readGInt5();
	CString ip = pPacket.readChars(pPacket.readGUChar());

	// Untokenize and load the directories.
	CString folders = pPacket.readChars(pPacket.readGUShort());
	folders.guntokenizeI();
	std::vector<CString> fList;
	
	fList = folders.tokenize("\n");

	// Create a folder map.
	std::map<CString, CString> folderMap;
	for (std::vector<CString>::iterator i = fList.begin(); i != fList.end(); ++i)
	{
		CString rights("r");
		CString wild("*");
		CString folder(*i);
		rights = folder.readString(" ").trim().toLower();
		folder.removeI(0, folder.readPos());
		folder.replaceAllI("\\", "/");
		folder.trimI();
		if (folder.find("WEAPONS") >= 0 || folder.find("CLASSES") >= 0 || folder.find("NPCS") >= 0) 
		{
			if (folder[folder.length() - 1] != '/')
			{
				
				int pos = folder.findl('/');
				if (pos != -1)
				{
					wild = folder.subString(pos + 1);
					folder.removeI(pos + 1);
				}
			}
			folderMap[folder] << rights << ":" << wild << "\n";
		}
	}
	server->setNCRights(account,folderMap);

	return true;
}

bool TGserverConn::msgGI_NC_REQUEST(CString& pPacket)
{
	int packetId = pPacket.readGUChar();
	int playerId = pPacket.readGShort();

	switch (packetId)
	{
	case NCI_PLAYERWEAPONS:
		{
			CString weapons = pPacket.readString("");
			weapons.removeAllI("\"");
			
			TPlayer *pPlayer = server->getGraalPlayer(playerId);
		
			std::vector<CString> weaponList = weapons.tokenize(",");
			
			pPlayer->UpdateWeaponList(weaponList);
		}
		break;

		case NCI_PLAYERWEAPONADD:
		{
			unsigned short pid = pPacket.readGUShort();
			TPlayer* pl = server->getGraalPlayer(playerId);
			if (pl != 0)
				pl->AddWeapon(pPacket.readString(""));
		}
		break;
		case NCI_PLAYERWEAPONDEL:
		{
			unsigned short pid = pPacket.readGUShort();
			TPlayer* pl = server->getGraalPlayer(playerId);
			if (pl != 0)
				pl->DeleteWeapon(pPacket.readString(""));
		}
		break;
	}

	return true;
}

//BUG player triggeraction call under (playerchats) sent before gserver sends npc server new player chat. o.O
//Not that its needed. . .
bool TGserverConn::msgGI_OTHERPLPROPS(CString& pPacket)
{
	int playerId = pPacket.readGShort();
	CString playerProps = pPacket.readString("");

	TPlayer * pPlayer = server->getGraalPlayer(playerId);

	if (pPlayer == 0) server->addGraalPlayer(playerId,playerProps);
	else pPlayer->SetProps(playerProps,false);

	return true;
}

bool TGserverConn::msgGI_NEWWORLDTIME(CString& pPacket)
{
	int newWorldTime = pPacket.readGInt4(); 
	server->updateTime(newWorldTime);

	return true;
}
//TODO: Implement some functions to interact with npc server.
bool TGserverConn::msgGI_PRIVATEMESSAGE(CString& pPacket)
{
	short playerId = pPacket.readGUShort();
	CString message = pPacket.readString("");

	CString pmMessageType("\"\",");
	pmMessageType << "\"Private message:\",";
	CString pmMessage = "\"I am the NPC Server! I don't do much, but my owner is hard at work programming me to do cool things!\"";

	fileQueue.addPacket(CString() >> (char)GO_NC_QUERY >> (char)NCO_SENDPM  >> (short)playerId << pmMessageType << pmMessage); 
	fileQueue.sendCompress();

	return true;
}


void TGserverConn::sendPM(int playerId, CString message)
{
	fileQueue.addPacket(CString() >> (char)GO_NC_QUERY >> (char)NCO_SENDPM  >> (short)playerId << "\"\",\"Private message:\",\"" << message << "\""); 
	fileQueue.sendCompress();
}


void TGserverConn::sendToGserver(CString pPacket)
{
	fileQueue.addPacket(pPacket);
	fileQueue.sendCompress();
}


bool TGserverConn::msgGI_NPCWEAPONADD(CString& pPacket)
{
	CString weapon = pPacket.readChars(pPacket.readGUChar());
	CString image = pPacket.readChars(pPacket.readGUChar());
	CString code = pPacket.readString("");

	server->NC_AddWeapon(0,weapon,image,code);
	cout << "Loaded weapon [" << weapon.text() << "]" << endl;

	return true;
}

bool TGserverConn::msgGI_FLAGSET(CString& pPacket)
{
	//send server flag info
	//TODO

	/*CString flagPacket = pPacket.readString("");
	CString flagName, flagValue;
	if (flagPacket.find("=") != -1)
	{
		flagName  = flagPacket.readString("=");
		flagValue = flagPacket.readString("");
	}
		else
	{
		flagName = flagPacket;
		flagValue = "1";
	}

	// set flag
	mServerFlags[flagName] = flagValue;
	*/
	return true;
}

bool TGserverConn::msgGI_NPCDEL(CString& pPacket)
{
	/*
	(2:56:32 AM) Nalin: When the level is reset, it deletes all the level npcs.  It sends PLO_NPCDEL or PLO_NPCDEL2 to the client to let it know the npc was deleted.
	(2:57:25 AM) Lnxmad: but its not needed by npcserver
	(2:57:42 AM) Nalin: Meh.
	(2:58:00 AM) Lnxmad: im already clearing the level
	(2:58:13 AM) Lnxmad: =p
	(2:58:25 AM) Nalin: I'm not going to program in a special case to NOT send it to the npc-server.
	(2:58:44 AM) Lnxmad: I know, I just wanted to make sure I didnt need it for anything.
	(2:59:16 AM) Lnxmad: I'll quote you in the empty function.
	*/
	return true;
}

bool TGserverConn::parsePacket(CString& pPacket)
{
        while (pPacket.bytesLeft() > 0)
        {
                // grab packet
                CString curPacket = pPacket.readString("\n");

                // read id & packet
               int id = curPacket.readGUChar();

                
               //valid packet, call function
               if (!(*this.*TPLFunc[id])(curPacket))
			return false;
        }
        return true;
}

void TGserverConn::sendCompress()
{
        // empty buffer?
        if (sBuffer.isEmpty())
        {
                // If we still have some data in the out buffer, try sending it again.
                if (oBuffer.isEmpty() == false)
					dataSize = oBuffer.length();
                        gserverSock.sendData(oBuffer.text(),&dataSize);
                return;
        }

        // Compress the packet and add it to the out buffer.
        sBuffer.zcompressI();
        oBuffer << CString() << (short)sBuffer.length() << sBuffer;

        // Send oBuffer.
		dataSize = oBuffer.length();
		gserverSock.sendData(oBuffer.text(),&dataSize);

        // Clear the send buffer.
		oBuffer.clear();
        sBuffer.clear();
}

bool TGserverConn::onRecv()
{
		// Update our socket manager.
        // definitions
        CString unBuffer;
		dataSize = 0;

        // receive
		char* data = gserverSock.getData(&dataSize);
        if (dataSize <= 0) return false;
        // grab the data now
		rBuffer.write(data, dataSize);
      

        // parse data
        rBuffer.setRead(0);
        while (rBuffer.length() >= 2)
        {
                // packet length
                unsigned int len = (unsigned int)rBuffer.readShort();
                if (len > (unsigned int)rBuffer.length()-2)
                        break;

                // get packet
                unBuffer = rBuffer.readChars(len);
                rBuffer.removeI(0, len+2);

                // decrypt packet
                unBuffer.zuncompressI();

                // well theres your buffer
                if (!parsePacket(unBuffer))
                        return false;
        }
	return true;
}