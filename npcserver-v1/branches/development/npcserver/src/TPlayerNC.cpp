#include "TPlayerNC.h"
#include "TNPCServer.h"


TPlayerNC::TPlayerNC(CSocket *pSocket, TNPCServer* pNPCServer):fileQueue(pSocket)
{
	server = pNPCServer;
	type = PLTYPE_AWAIT;

	playerSock = pSocket;
	
	createFunctions();
}
TPlayerNC::~TPlayerNC()
{
	if (playerSock) 
	{
		fileQueue.sendCompress();
		delete playerSock;
	}
}

typedef bool (TPlayerNC::*TPLSock)(CString&);
std::vector<TPLSock> TPLFunc(255, &TPlayerNC::msgPLI_NULL);

void TPlayerNC::createFunctions()
{

	TPLFunc[PLI_RC_CHAT]          = &TPlayerNC::msgPLI_RC_CHAT;
	TPLFunc[PLI_NC_WEAPONLISTGET] = &TPlayerNC::msgPLI_RC_WEAPONLISTGET; 
	TPLFunc[PLI_NC_WEAPONGET]     = &TPlayerNC::msgPLI_NC_WEAPONGET;
	TPLFunc[PLI_NC_WEAPONADD]     = &TPlayerNC::msgPLI_NC_WEAPONADD;
	TPLFunc[PLI_NC_WEAPONDELETE]  = &TPlayerNC::msgPLI_NC_WEAPONDELETE;
	TPLFunc[PLI_NC_LEVELLISTGET]  = &TPlayerNC::msgPLI_NC_LEVELLISTGET;
	TPLFunc[PLI_NC_NPCADD]        = &TPlayerNC::msgPLI_NC_NPCADD; // {111}{GSTRING info}  - (info) name,id,type,scripter,starting level,x,y
	TPLFunc[PLI_NC_CLASSEDIT]	  = &TPlayerNC::msgPLI_NC_CLASSEDIT;
	TPLFunc[PLI_NC_CLASSADD]      = &TPlayerNC::msgPLI_NC_CLASSADD;
	TPLFunc[PLI_NC_CLASSDELETE]   = &TPlayerNC::msgPLI_NC_CLASSDELETE;
	// Finished

}

bool TPlayerNC::onRecv()
{
	if (playerSock->getState() == SOCKET_STATE_DISCONNECTED)
                return false;
	CString packetBuffer;
	CString rBuffer, sBuffer, oBuffer;

	unsigned int dataSize;
	// Get data.
	
  // definitions
    CString unBuffer;

    // receive
	char* data = playerSock->getData(&dataSize);
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

bool TPlayerNC::parsePacket(CString &pPacket)
{
	if (type == PLTYPE_AWAIT)
	{
		if (msgPLI_LOGIN(CString() << pPacket.readString("\n")) == false)
			return false;
	}

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

void TPlayerNC::sendCompress()
{
        // empty buffer?
        if (sBuffer.isEmpty())
        {
                // If we still have some data in the out buffer, try sending it again.
                if (oBuffer.isEmpty() == false)
					dataSize = oBuffer.length();
                        playerSock->sendData(oBuffer.text(),&dataSize);
                return;
        }

        // Compress the packet and add it to the out buffer.
        sBuffer.zcompressI();
        oBuffer << CString() << (short)sBuffer.length() << sBuffer;

        // Send oBuffer.
		dataSize = oBuffer.length();
		playerSock->sendData(oBuffer.text(),&dataSize);

		oBuffer.clear();
        // Clear the send buffer.
        sBuffer.clear();
}

bool TPlayerNC::sendPacket(CString pPacket)
{

	fileQueue.addPacket(pPacket);
	fileQueue.sendCompress();
	return true;
}


bool TPlayerNC::msgPLI_LOGIN(CString& pPacket)
{
	// Read Client-Type
	
	type = (1 << pPacket.readGChar());
	bool getKey = false;
	switch (type)
	{
		case PLTYPE_NC:
			// cout << "NC Connecting" << endl;
			// in_codec.setGen(ENCRYPT_GEN_3);
			// getKey = false;
			break;
		default:
			cout << "Unknown (" << type << ")" << endl;
			sendPacket(CString() >> (char)PLO_DISCMESSAGE << "Your client type is unknown.  Please inform the Graal Reborn staff.  Type: " << CString((int)type) << ".");
			return false;
			break;
	}

	version = pPacket.readChars(8);

	if (version != "NCL21075" && version != "NCL11012") //TODO: Better denying of old versions, I think best way is through GServer.
	{
		server->rcChat("Your nc version (" + version + ") is not allowed on this server.");
		sendPacket(CString() >> (char)PLO_DISCMESSAGE << "Your nc version is not allowed on this server.");
		playerSock->disconnect();
		return false;
	}

	// Read Account & Password
	account = pPacket.readChars(pPacket.readGUChar());
	password = pPacket.readChars(pPacket.readGUChar());

	//serverlog.out("Key: %d\n", key);
	printf("Account:  %s\n", account.text());
	printf("Version:  %s\n", version.text());

	if(server->CheckMultipleNC(account)) //TODO: Better handling of previous logins, this treats your new one as a duplicate still
	{
	  sendPacket(CString() >> (char)PLO_DISCMESSAGE << "Multiple connections are not allowed on nc");
	  playerSock->disconnect();
	  return false;
	}


	server->sendToGserver(CString() >> (char)GO_REQUEST_RIGHTS << account);
	
	//Message
	sendPacket(CString() >> (char)PLO_RC_CHAT << server->getMOTD());
	sendPacket(CString() >> (char)PLO_RC_CHAT << "Programmed by Lnxmad & Agret");

	server->rcChat("New NC: " + account);

	std::map<CString, TScriptClass *> * classList = server->getClassList();
	
	for (std::map<CString, TScriptClass *>::const_iterator i = classList->begin(); i != classList->end(); ++i)
	{
		sendPacket(CString() >> (char)PLO_NC_CLASSADD << i->second->GetScriptClassName());
	}

	return true;
}

//RC FUNCTIONS

bool TPlayerNC::msgPLI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	printf("Unknown Player Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	for (int i = 0; i < pPacket.length(); ++i) printf("%02x ", (unsigned char)((pPacket.text())[i])); printf("\n");

	// If we are getting a whole bunch of invalid packets, something went wrong.  Disconnect the player.
	
	return true;
}

bool TPlayerNC::msgPLI_NC_NPCADD(CString& pPacket)
{

	//TODO RIGHTS
	// printf("Unknown Player Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	// Unknown Player Packet: 2 ("TEsting THis",1000,OBJECT,Agret,onlinestartlocal.nw,30.5,30)

//	server->rcChat(pPacket.text()+1);

	CString line = pPacket.text()+1;
	std::vector<CString> names = line.tokenize(",");
	
	/*for (std::vector<CString>::iterator j = names.begin(); j != names.end(); ++j)
	{
		server->ncChat("Hey: " + names[0]);
	}*/

	// server->ncChat(line); // Debug print - Name,1000,OBJECT,Agret,onlinestartlocal.nw,30.5,30

	CString npcName = names[0].replaceAll("\"", "");
	int npcID = atoi(names[1].text());
	CString npcType = names[2];
	CString npcScripter = names[3];
	CString npcStartLevel = names[4];
	float npcStartX = (float)atof(names[5].text());
	float npcStartY = (float)atof(names[6].text());

	if (npcName.length() < 1 || npcName.length() > 30)
	{
		sendPacket(CString() >> (char)PLO_RC_CHAT << CString("Error - Blank/Invalid NPC Name."));
		return false;
	}
	if (npcID < 1 || npcID > 1000000){
		sendPacket(CString() >> (char)PLO_RC_CHAT << CString("Error - Invalid NPC ID."));
		return false;
	}
	// server->ncChat("NPC (Server): NPC " + npcName + " Added (ID: " + npcID + "). StartX:" + npcStartX + " StartY: " + npcStartY + " Scripter: " + npcScripter);
	server->ncChat(account + " added NPC \"" + npcName + "\" (ID " + npcID + ")");

	return true;
}

/*bool TPlayerNC::msgPLI_RC_NPCLISTGET(CString& pPacket)
{
	CString packet = CString() >> (char)PLO_NC_WEAPONLISTGET;

	std::map<CString, TScriptWeapon *> * weaponList = server->getWeaponList();
	
	for (std::map<CString, TScriptWeapon *>::const_iterator i = weaponList->begin(); i != weaponList->end(); ++i)
	{
		CString weaponName = i->second->getName();

		for (std::map<CString, CString>::const_iterator t = folderRights.begin(); t != folderRights.end(); ++t)
		{
			if ((*t).first.find("WEAPONS") >= 0)
			{
				CString path(CString() << "WEAPONS/"<<weaponName);
	
				std::vector<CString> rights = (*t).second.tokenize("\n");
				
				for (std::vector<CString>::iterator c = rights.begin(); c != rights.end(); ++c)
				{
					CString right(CString() << (*t).first << (*c).tokenize(":")[1].trim());
					if (path.match(right))
					{
						CString imageName  = i->second->getImage();
						CString scriptData = i->second->getFullScript();
						packet >> (char)weaponName.length() << weaponName;
						break;
					}
				}
			}	
		}
	}

	sendPacket(packet);
	return true;
}*/

bool TPlayerNC::msgPLI_NC_CLASSDELETE(CString& pPacket)
{
	//TODO RIGHTS
	server->NC_DeleteClass(this,pPacket.readString(""));

	return true;
}

bool TPlayerNC::msgPLI_NC_CLASSADD(CString& pPacket)
{

	//TODO RIGHTS
	CString className = pPacket.readChars(pPacket.readGUChar());
	CString scriptData = pPacket.readString("");

	server->NC_AddClass(this,className,scriptData);

	return true;
}

bool TPlayerNC::msgPLI_NC_CLASSEDIT(CString& pPacket)
{

	//TODO RIGHTS
	CString className = pPacket.readString("");

	TScriptClass * pClass = server->getClass(className);

	if (pClass == 0) return true;

	sendPacket(CString() >> (char)PLO_NC_CLASSGET >> (char)pClass->GetScriptClassName().length() << pClass->GetScriptClassName() << pClass->GetClassCode());

	return true;

}

bool TPlayerNC::msgPLI_NC_LEVELLISTGET(CString& pPacket)
{
	//sendPacket(CString() >> (char)PLO_NC_LEVELLIST << server->levelList.text()); 

	return true;
}

bool TPlayerNC::msgPLI_RC_CHAT(CString& pPacket)
{
	CString message = pPacket.readString("");
	if (message.isEmpty()) return true;
	std::vector<CString> words = message.tokenize();

	if (words[0].text()[0] != '/')
	{
		//sendPacket(CString() >> (char)PLO_RC_CHAT  << ": " << message);
		return true;
	}else{ // message is the full message // words[0] is the first word i.e. /test, it tokenizes by a space
		if(message == "/help")
		{
			sendPacket(CString() >> (char)PLO_RC_CHAT  << "Available Commands For The NPC-Server:");
			sendPacket(CString() >> (char)PLO_RC_CHAT  << "/weaponlist: Lists the current weapons loaded by NPC-Server");
			sendPacket(CString() >> (char)PLO_RC_CHAT  << "/reloadweapons: Reloads all weapons (not implemented)");
		}else if(message == "/weaponlist" || message == "/weaponslist")
		{
			sendPacket(CString() >> (char)PLO_RC_CHAT  << "Weapon List:");
			std::map<CString, TScriptWeapon *> * weaponList = server->getWeaponList();
			
			for (std::map<CString, TScriptWeapon *>::const_iterator i = weaponList->begin(); i != weaponList->end(); ++i)
			{
				sendPacket(CString() >> (char)PLO_RC_CHAT  << i->second->getName());
			}

			sendPacket(CString() >> (char)PLO_RC_CHAT  << "End of list:");
		}
	}

	return true;
}

bool TPlayerNC::msgPLI_RC_WEAPONLISTGET(CString& pPacket)
{
	CString packet = CString() >> (char)PLO_NC_WEAPONLISTGET;

	std::map<CString, TScriptWeapon *> * weaponList = server->getWeaponList();
	
	for (std::map<CString, TScriptWeapon *>::const_iterator i = weaponList->begin(); i != weaponList->end(); ++i)
	{
		CString weaponName = i->second->getName();

		for (std::map<CString, CString>::const_iterator t = folderRights.begin(); t != folderRights.end(); ++t)
		{
			if ((*t).first.find("WEAPONS") >= 0)
			{
				CString path(CString() << "WEAPONS/"<<weaponName);
	
				std::vector<CString> rights = (*t).second.tokenize("\n");
				
				for (std::vector<CString>::iterator c = rights.begin(); c != rights.end(); ++c)
				{
					CString right(CString() << (*t).first << (*c).tokenize(":")[1].trim());
					if (path.match(right))
					{
						CString imageName  = i->second->getImage();
						CString scriptData = i->second->getFullScript();
						packet >> (char)weaponName.length() << weaponName;
						break;
					}
				}
			}	
		}
	}

	sendPacket(packet);
	return true;
}

bool TPlayerNC::msgPLI_NC_WEAPONGET(CString& pPacket)
{
	CString weaponName = pPacket.readString("");

	for (std::map<CString, CString>::const_iterator t = folderRights.begin(); t != folderRights.end(); ++t)
	{
		if ((*t).first.find("WEAPONS") >= 0)
		{
			CString path(CString() << "WEAPONS/" << weaponName);

			std::vector<CString> rights = (*t).second.tokenize("\n");
			
			for (std::vector<CString>::iterator c = rights.begin(); c != rights.end(); ++c)
			{
				CString right(CString() << (*t).first << (*c).tokenize(":")[1].trim());
				if (path.match(right))
				{
					TScriptWeapon * pWeapon = server->getWeapon(weaponName);

					//Weapon not found
					if (pWeapon == 0) return true;

					sendPacket(CString() >> (char)PLO_NC_WEAPONGET >> (char)pWeapon->getName().length() << pWeapon->getName()
					>> (char)pWeapon->getImage().length() << pWeapon->getImage()
					<< pWeapon->getFullScript()); 
					return true;
				}
			}
		}	
	}

	sendPacket(CString() >> (char)PLO_RC_CHAT << CString("No write access to weapon ") << weaponName);

	return true;
}

bool TPlayerNC::msgPLI_NC_WEAPONADD(CString& pPacket)
{
	//TODO RIGHTS
	CString weapon = pPacket.readChars(pPacket.readGUChar());
	CString image = pPacket.readChars(pPacket.readGUChar());
	CString code = pPacket.readString("");

	for (std::map<CString, CString>::const_iterator t = folderRights.begin(); t != folderRights.end(); ++t)
	{
		if ((*t).first.find("WEAPONS") >= 0)
		{
			CString path(CString() << "WEAPONS/"<<weapon);
	
			std::vector<CString> rights = (*t).second.tokenize("\n");
				
			for (std::vector<CString>::iterator c = rights.begin(); c != rights.end(); ++c)
			{
				CString right(CString() << (*t).first << (*c).tokenize(":")[1].trim());
				if (path.match(right))
				{
					if ((*c).tokenize(":")[0].find("w") >= 0)
					{
						server->NC_AddWeapon(this,weapon,image,code);
						return true;
					}
				}
			}
		}	
	}

	sendPacket(CString() >> (char)PLO_RC_CHAT << CString("No write access to weapon ") << weapon);
	return true;
}

bool TPlayerNC::msgPLI_NC_WEAPONDELETE(CString& pPacket)
{
	//TODO RIGHTS
	CString weapon = pPacket.readString("");

	server->NC_DeleteWeapon(this,weapon);

	return true;
}

/*
for (std::map<CString, TScriptWeapon *>::const_iterator i = weaponList->begin(); i != weaponList->end(); ++i)
	{
		CString weaponName = i->second->getName();
		weaponName.match
		std::vector<CString> weaponPaths = weaponName.tokenize("/");

		for (std::map<CString, CString>::const_iterator t = folderRights.begin(); t != folderRights.end(); ++t)
		{
			if ((*t).first.find("WEAPONS") >= 0)
			{
				std::vector<CString> rightPaths = (*t).first.tokenize("/");
				std::vector<CString> rights = (*t).second.tokenize(":");
				CString wild = rights[1].readString("*");

				//No sub Directories in right or weapon
				if (weaponPaths.size() == 1 && rightPaths.size() == 1) 
				{
					if (rights[0].find("r") >= 0 || weaponName.find(wild) == 0)
					{
						if (wild.isEmpty())
						{
							CString imageName  = i->second->getImage();
							CString scriptData = i->second->getFullScript();
							packet >> (char)weaponName.length() << weaponName;
						}
					}
				}
				else
				{
					std::vector<CString>::const_iterator weaponPathsI = weaponPaths.begin();
					std::vector<CString>::const_iterator rightsPathsI = rightPaths.begin();

					//If right has no sub dir in it, skip it because weapon is has a sub dir
					if (rightPaths.size() == 1) continue;

					//get rid of WEAPONS path
					rightsPathsI++;

					while((*weaponPathsI) == (*rightsPathsI))
					{
						++weaponPathsI;
						++rightsPathsI;

						if (rightsPathsI == rightPaths.end()) break;
					}
					
					//skip it if the path is not at the end, but the paths do not match
					if (rightsPathsI != rightPaths.end() && (*weaponPathsI) != (*rightsPathsI)) continue;

					//Do we have read rights?
					if (rights[0].find("r") >= 0)
					{
						if (wild.isEmpty() || (*weaponPathsI).find(wild) == 0)
						{
							CString imageName  = i->second->getImage();
							CString scriptData = i->second->getFullScript();
							packet >> (char)weaponName.length() << weaponName;

						}
					}
				}
			}
		}
	}
*/