#include "TNPCServer.h"



TNPCServer::TNPCServer():fileQueue(&playerSock)
{
	m_version = "0.5 beta"; 

	SetupPlayerPropAccess();
	SetupNPCPropAccess();
	RegisterFunctions();

	gmBindSystemLib(&npcMachine);
}

TNPCServer::~TNPCServer()
{
	for (std::vector<TPlayerNC*>::iterator i = mNCPlayers.begin(); i != mNCPlayers.end(); )
	{
		delete *i;
		i = mNCPlayers.erase(i);
	}
	mNCPlayers.clear();

	for (std::map<int, TNPC *>::iterator i = npcList.begin(); i != npcList.end(); )
	{
		delete i->second;
		npcList.erase(i++);
	}
	npcList.clear();

	for (std::vector<TLevel*>::iterator i = mLevelList.begin(); i != mLevelList.end(); )
	{
		delete *i;
		i = mLevelList.erase(i);
	}
	mLevelList.clear();

	for (std::map<CString, TScriptWeapon *>::iterator i = mWeapons.begin(); i != mWeapons.end(); )
	{
		delete i->second;
		mWeapons.erase(i++);
	}
	mWeapons.clear();

	this->rcChat("NPC-Server shutting down. . .");

    playerSock.disconnect();
	mGserverConnection.gserverSock.disconnect();

	// Clean up the socket manager.  Pass false so we don't cause a crash.
	sockManager.cleanup(false);

	npcMachine.CollectGarbage(true);
}


bool TNPCServer::startServer()
{
	mGserverConnection.server = this;
	CString serverpath = getHomePath();

	serverSettings.setSeparator("=");
	serverSettings.loadFile(CString() << serverpath << "/settings.txt");
	
	mHost = serverSettings.getStr("npcserver_ip","localhost");
	mPort = (int)strtofloat(serverSettings.getStr("npcserver_port","14901"));

	mGserverHost = serverSettings.getStr("gserver_ip","localhost");
	mGserverPort = (int)strtofloat(serverSettings.getStr("gserver_port","14802"));

	mAccount = serverSettings.getStr("account","(npcserver)");
	mPassword = serverSettings.getStr("password","testpass");
	nickname = serverSettings.getStr("nickname","NPC-Server (Server)");

	motd = serverSettings.getStr("motd",CString() << "Welcome to NPC Server (" << getVersion() << ")");
	
	playerSock.setType(SOCKET_TYPE_SERVER);
	playerSock.setProtocol(SOCKET_PROTOCOL_TCP);
	playerSock.setDescription("playerSock");

	// Start listening on the player socket.
	if (playerSock.init(mHost.text(), CString(mPort).text()))
	{
		cout << "** [Error] Could not initialize listening socket...\n";
		return false;
	}
	if (playerSock.connect())
	{
		cout << "** [Error] Could not connect listening socket...\n";
		return false;
	}

	sockManager.registerSocket((CSocketStub*)this);

	if (mGserverConnection.connect(mGserverHost,mGserverPort))
	{
		mGserverConnection.login(nickname,mAccount,mPassword,mPort);

		sockManager.registerSocket((CSocketStub*)&mGserverConnection);
		
	}

	return true;
}

bool TNPCServer::onRecv()
{
	acceptNewPlayers(playerSock);

	return true;
}

bool TNPCServer::doMain()
{
	sockManager.update(0,5000);

	if (!isGserverConnected()) return false;

	//Timeout for npcs. Minumim 0.1 seconds or 100 milliseconds
	for (std::vector<TLevel *>::iterator i = mLevelList.begin(); i != mLevelList.end(); ++i)
	{
		(*i)->DoTimedEvents();
	}

	// Every second, do some events.
	if (time(0) != lastTimer) doTimedEvents();

	return true;
}

void TNPCServer::doTimedEvents()
{
	lastTimer = time(0);

	std::vector<TPlayerNC *> playersToDelete;

	for (std::vector<TPlayerNC *>::iterator i = mNCPlayers.begin(); i != mNCPlayers.end(); ++i)
	{
		TPlayerNC *playernc = (TPlayerNC *)*i;
		if (playernc == 0)
			continue;

		if (playernc->playerSock == 0 || playernc->playerSock->getState() == SOCKET_STATE_DISCONNECTED)
		{
			playersToDelete.push_back(playernc);
		}
	}

	for (std::vector<TPlayerNC*>::iterator i = playersToDelete.begin(); i != playersToDelete.end(); )
	{
		TPlayerNC* player = *i;

		cout << "NC Disconnected: " << player->account.text() << endl;

		// Remove the player from the socket manager.
		sockManager.unregisterSocket((CSocketStub*)player);

		// Get rid of the player now.
		vecRemove<TPlayerNC*>(mNCPlayers, player);

		// Delete and go to the next player.
		if (player) delete player;
		i = playersToDelete.erase(i);
	}
}



bool TNPCServer::isGserverConnected()
{
	if (mGserverConnection.gserverSock.getState() == SOCKET_STATE_DISCONNECTED)
		return false;
	else return true;
}

void TNPCServer::addGraalPlayer(int id, CString props)
{
	mGraalPlayers[id] = new TPlayer(this,id);
	mGraalPlayers[id]->SetProps(props,false);
	mGserverConnection.sendToGserver(CString()>> (char)GO_NC_QUERY  >> (char)NCO_PLAYERWEAPONSGET >> (short)id);
}

TPlayer	* TNPCServer::getGraalPlayer(int id)
{
	return (mGraalPlayers.find(id) != mGraalPlayers.end() ? mGraalPlayers[id] : 0);
}

TPlayer	* TNPCServer::getGraalPlayer(CString account)
{
	for (std::map<int,TPlayer * >::iterator i = mGraalPlayers.begin(); i != mGraalPlayers.end(); ++i)
	{
		if (i->second->GetAccount() == account && !i->second->GetLevel().isEmpty()) return i->second;
	}

	return 0;
}

TPlayerNC * TNPCServer::getNC(CString account)
{

	for (std::vector<TPlayerNC *>::iterator i = mNCPlayers.begin(); i != mNCPlayers.end(); ++i)
	{
		if ((*i)->account == account) return *i;
	
	}

	return 0;
}

int TNPCServer::getPlayerCount()
{
	return mGraalPlayers.size();
}

void TNPCServer::RequestLevel(CString level)
{
	sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_LEVELGET << level);
}

void TNPCServer::AddLevel(CString level)
{

	TLevel * newLevel =  new TLevel(this,level);
	currentLevel = level;
	
	std::map<int, TPlayer *> * playerList = getGraalPlayers();

	for (std::map<int,TPlayer * >::iterator i = playerList->begin(); i != playerList->end(); ++i)
	{
		if (i->second->GetLevel() == newLevel->getLevelName() && !i->second->GetLevel().isEmpty()) 
		{
			newLevel->addPlayer(i->second);	
		}
	}

	mLevelList.push_back(newLevel);
}

void TNPCServer::UpdateCurrentLevel(CString level)
{
	currentLevel = level;
}

void TNPCServer::addNPC(int npcId, CString props)
{
	TNPC * newNPC = new TNPC(npcId,this,getLevel(currentLevel),&npcMachine,true);

	newNPC->setProps(props);
	npcList[npcId] = newNPC;

	getLevel(currentLevel)->addNPC(newNPC);
}

void TNPCServer::deleteNPC(TNPC * npc)
{
	mapRemove<int,TNPC *>(npcList,npc);
	npcList[npc->getId()] = 0;
	delete npc;
}

TNPC * TNPCServer::getNPC(int id)
{
	return (npcList.find(id) != npcList.end() ? npcList[id] : 0);
}

void TNPCServer::triggerAction(int playerId,CString action,CString params)
{
	TPlayer * pPlayer = getGraalPlayer(playerId);

	if (pPlayer == 0) return;

	std::vector<CString> weaponList = pPlayer->getWeaponList();

	std::vector<CString> paramList = params.tokenize(",");

	if (paramList.size() < 2) return;

	TScriptWeapon * pWeapon = getWeapon(paramList[1]);
	
	if (pWeapon == 0) return;
		pWeapon->callFunction(action,pPlayer,params);

}

void TNPCServer::triggerAction(int playerid, int npcid,float x, float y,CString action,CString params)
	{
	TPlayer * player = getGraalPlayer(playerid);
	
	if (player == 0) return;

	TLevel * playerLevel = getLevel(player->GetLevel());

	playerLevel->callSpecificNPC(x,y,action,player,params);
}

void TNPCServer::sendToGserver(CString pPacket)
{
	mGserverConnection.sendToGserver(pPacket);
}

void TNPCServer::sendPlayerPM(CString account,CString message)
{
	TPlayer * pPlayer = getGraalPlayer(account);

	if (pPlayer == 0) return;

	mGserverConnection.sendPM(pPlayer->GetId(),message);
}

void TNPCServer::setNCRights(CString account, std::map<CString, CString> rights)
{
	TPlayerNC * player = getNC(account);

	if (player) player->folderRights = rights;
}

//Weapon functionality
bool TNPCServer::NC_AddWeapon(TPlayerNC * pPlayerNC,CString name, CString image,CString code)
{
	TScriptWeapon * pWeapon = getWeapon(name);

	if (pWeapon == 0)
	{
		pWeapon = new TScriptWeapon(name,image,code,this,&npcMachine);

		mWeapons[name] = pWeapon;
	}
	else
	{
		pWeapon->update(image,code);
	}

	if (pPlayerNC != 0)//if nc is 0, gserver is sending weapons, dont send to rc
	{
		ncChat(pPlayerNC->account + " has added/updated weapon " + name);

		mGserverConnection.sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_WEAPONADD >> (char)name.length() << name.text()
		>> (char)image.length() << image.text() 
		<< code.text()); 
	}

	return true;
}


bool TNPCServer::NC_DeleteWeapon(TPlayerNC * pPlayerNC, CString weapon)
{
	TScriptWeapon * pWeapon = getWeapon(weapon);

	if (pWeapon == 0) return false;
	
	mapRemove<CString, TScriptWeapon *>(mWeapons, pWeapon);
	delete pWeapon;

	ncChat(pPlayerNC->account + " has deleted weapon " + weapon);

	mGserverConnection.sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_WEAPONDEL << weapon.text());
	
	return true;
}

TLevel* TNPCServer::getLevel(const CString& pLevel)
{
	if (mLevelList.empty()) return 0;

	// Find Appropriate Level by Name
	for (std::vector<TLevel *>::iterator i = mLevelList.begin(); i != mLevelList.end(); )
	{
		if ((*i) == 0)
		{
			i = mLevelList.erase(i);
			continue;
		}

		if ((*i)->getLevelName().toLower() == pLevel.toLower())
			return (*i);

		++i;
	}

	return 0;
}

//Class functionality
bool TNPCServer::NC_AddClass(TPlayerNC * pPlayerNC,CString className,CString scriptData)
{
	TScriptClass * pClass = getClass(className);

	if (pClass == 0)
	{
		pClass = new TScriptClass(className,scriptData);

		mClasses[className] = pClass;
	}
	else
	{
		pClass->SetClassCode(scriptData);
	}

	
	if (pPlayerNC != 0)//if nc is 0 Gserver sending new class.
	{
		CString message = pPlayerNC->account + " has added/updated class " + className;
		
		ncChat(message);
		//mGserverConnection.sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCREQ_CLASSADD >> (char)className.length() << className.text()
		//<< scriptData.text()); 
	}

	return true;
}

bool TNPCServer::NC_DeleteClass(TPlayerNC * pPlayerNC, CString className)
{

	return true;
}


TScriptClass * TNPCServer::getClass(const CString& className)
{
	return (mClasses.find(className) != mClasses.end() ? mClasses[className] : 0);
}

TScriptWeapon * TNPCServer::getWeapon(const CString& weaponName)
{
	return (mWeapons.find(weaponName) != mWeapons.end() ? mWeapons[weaponName] : 0);
}


void TNPCServer::updateTime(int time)
{
	newWorldTime = time;		
}

void TNPCServer::sendToAllNC(CString& pPacket)
{
	for (std::vector<TPlayerNC * >::iterator i = mNCPlayers.begin(); i != mNCPlayers.end(); ++i)
	{
		TPlayerNC * pPlayer = *i;
		pPlayer->sendPacket(pPacket);
	}
}

void TNPCServer::rcChat(CString  message)
{
	mGserverConnection.sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_SENDTORC << message);
}

void TNPCServer::ncChat(CString message)
{
	sendToAllNC(CString() >> (char)PLO_RC_CHAT << message.text());
}


void TNPCServer::acceptNewPlayers(CSocket& pSocket)
{
	CSocket* newSock = pSocket.accept();
    if(newSock == 0)
                return;

	TPlayerNC *pPlayer = new TPlayerNC(newSock,this);
	
	mNCPlayers.push_back(pPlayer);
	sockManager.registerSocket((CSocketStub*)pPlayer);
	cout << "New Connection: " << newSock->getRemoteIp() << endl;
		return;
}

void TNPCServer::RegisterFunctions()
{
	
	TPlayer::s_typeId = npcMachine.CreateUserType("TPlayer");
	TNPC::s_typeId = npcMachine.CreateUserType("TNPC");
	TScriptWeapon::s_typeId  = npcMachine.CreateUserType("TScriptWeapon");
	
	static gmFunctionEntry GlobalFunctions[] = 
	{
		{"sendtorc",func_sendtorc,this},
		{"findplayer",func_findplayer,this},
		{"getPlayerCount",func_getPlayerCount,this},
		{"sendPM",func_sendPM,this},
		{"tokenize",func_tokenize},
		{"getPlayers",func_getPlayers,this},
	};

	static gmFunctionEntry PlayerFunctions[] = 
	{
		{"setlevel2",func_setlevel2,NULL},
		{"addweapon",func_addweapon,NULL},
		{"removeweapon",func_removeweapon,NULL},
	};

	static gmFunctionEntry NPCFunctions[] = 
	{
		{"setTimer",func_setTimer,NULL},
		{"setshape",func_setshape,NULL},
	};

	npcMachine.RegisterTypeOperator(TScriptWeapon::s_typeId, O_GETDOT,NULL,TScriptWeapon::TScriptWeaponGetDot);
	npcMachine.RegisterTypeOperator(TScriptWeapon::s_typeId, O_SETDOT,NULL,TScriptWeapon::TScriptWeaponSetDot);

	npcMachine.RegisterTypeOperator(TPlayer::s_typeId, O_GETDOT, NULL, TPlayerGetDot); 
	npcMachine.RegisterTypeOperator(TPlayer::s_typeId, O_SETDOT, NULL, TPlayerSetDot);
	
	npcMachine.RegisterTypeOperator(TNPC::s_typeId, O_GETDOT, NULL, TNPC::TNPCGetDot); 
	npcMachine.RegisterTypeOperator(TNPC::s_typeId, O_SETDOT, NULL, TNPC::TNPCSetDot);

	npcMachine.RegisterTypeLibrary(TPlayer::s_typeId,PlayerFunctions, sizeof(PlayerFunctions) / sizeof(PlayerFunctions[0]));
	npcMachine.RegisterLibrary(GlobalFunctions, sizeof(GlobalFunctions) / sizeof(GlobalFunctions[0]));
	npcMachine.RegisterTypeLibrary(TNPC::s_typeId,NPCFunctions, sizeof(NPCFunctions) / sizeof(NPCFunctions[0]));
	
	npcMachine.EnableGC();
}

void TNPCServer::SetupNPCPropAccess()
{
	//Set and Get Table				//Readonly,propEnumValue,valueType,packetType
	npcPropAccess["image"].Set(false,NPCPROP_IMAGE,PROP_STRING,PROP_STRING);
	npcPropAccess["fullhearts"].Set(false,PLPROP_MAXPOWER,PROP_CHAR,PROP_INT);
	npcPropAccess["power"].Set(false,NPCPROP_POWER,PROP_FLOAT,PROP_CHAR);
	npcPropAccess["rupees"].Set(false,NPCPROP_RUPEES,PROP_INT,PROP_INT);
	npcPropAccess["arrows"].Set(false,NPCPROP_ARROWS,PROP_INT,PROP_CHAR);
	npcPropAccess["bombs"].Set(false,NPCPROP_BOMBS,PROP_INT,PROP_CHAR);
	npcPropAccess["sword"].Set(false,NPCPROP_SWORDIMAGE,PROP_STRING,PROP_STRING);
	npcPropAccess["bombpower"].Set(false,NPCPROP_BOMBPOWER,PROP_INT,PROP_INT);
	npcPropAccess["chat"].Set(false,NPCPROP_MESSAGE,PROP_STRING,PROP_STRING);
	npcPropAccess["glovepower"].Set(false,NPCPROP_GLOVEPOWER,PROP_INT,PROP_INT);
	npcPropAccess["gani"].Set(false,NPCPROP_GANI,PROP_STRING,PROP_STRING);
	npcPropAccess["head"].Set(false,NPCPROP_HEADIMAGE,PROP_STRING,PROP_STRING);
	npcPropAccess["ap"].Set(false,NPCPROP_ALIGNMENT,PROP_INT,PROP_CHAR);
	npcPropAccess["nick"].Set(false,NPCPROP_NICKNAME,PROP_INT,PROP_INT);
	npcPropAccess["shield"].Set(false,NPCPROP_SHIELDIMAGE,PROP_STRING,PROP_STRING);
	npcPropAccess["body"].Set(false,NPCPROP_BODYIMAGE,PROP_STRING,PROP_STRING);
	npcPropAccess["x"].Set(false,NPCPROP_X,PROP_FLOAT,PROP_CHAR);
	npcPropAccess["y"].Set(false,NPCPROP_Y,PROP_FLOAT,PROP_CHAR);

	//Read Only
	npcPropAccess["id"].Set(false,NPCPROP_ID,PROP_INT,PROP_INT);
	npcPropAccess["level"].Set(false,NPCLEVEL,PROP_STRING,PROP_STRING);
}

void TNPCServer::SetupPlayerPropAccess()
{
	//Set and Get Table				//Readonly,propEnumValue,valueType,packetType
	playerPropAccess["nick"].Set(false,PLPROP_NICKNAME,PROP_STRING,PROP_STRING);
	playerPropAccess["fullhearts"].Set(false,PLPROP_MAXPOWER,PROP_CHAR,PROP_INT);
	playerPropAccess["hearts"].Set(false,PLPROP_CURPOWER,PROP_INT,PROP_CHAR);
	playerPropAccess["rupees"].Set(false,PLPROP_RUPEESCOUNT,PROP_INT,PROP_INT);
	playerPropAccess["arrows"].Set(false,PLPROP_ARROWSCOUNT,PROP_INT,PROP_CHAR);
	playerPropAccess["bombs"].Set(false,PLPROP_BOMBSCOUNT,PROP_INT,PROP_CHAR);
	playerPropAccess["sword"].Set(false,PLPROP_SWORDPOWER,PROP_STRING,PROP_STRING);
	playerPropAccess["bombpower"].Set(false,PLPROP_BOMBPOWER,PROP_INT,PROP_INT);
	playerPropAccess["chat"].Set(false,PLPROP_CURCHAT,PROP_STRING,PROP_STRING);
	playerPropAccess["glovepower"].Set(false,PLPROP_GLOVEPOWER,PROP_INT,PROP_INT);
	playerPropAccess["gani"].Set(false,PLPROP_GANI,PROP_STRING,PROP_STRING);
	playerPropAccess["head"].Set(false,PLPROP_HEADGIF,PROP_STRING,PROP_STRING);
	playerPropAccess["ap"].Set(false,PLPROP_ALIGNMENT,PROP_INT,PROP_CHAR);
	playerPropAccess["shield"].Set(false,PLPROP_SHIELDPOWER,PROP_STRING,PROP_STRING);
	playerPropAccess["mp"].Set(false,PLPROP_MAGICPOINTS,PROP_INT,PROP_CHAR);
	playerPropAccess["body"].Set(false,PLPROP_BODYIMG,PROP_STRING,PROP_STRING);
	playerPropAccess["x"].Set(false,PLPROP_X,PROP_FLOAT,PROP_CHAR);
	playerPropAccess["y"].Set(false,PLPROP_Y,PROP_FLOAT,PROP_CHAR);

	//Read Only
	playerPropAccess["account"].Set(true,PLPROP_ACCOUNTNAME,PROP_STRING,PROP_STRING);
	playerPropAccess["level"].Set(true,PLPROP_CURLEVEL,PROP_STRING,PROP_STRING);
	playerPropAccess["language"].Set(true,PLPROP_PLANGUAGE,PROP_STRING,PROP_STRING);
	playerPropAccess["kills"].Set(true,PLPROP_KILLSCOUNT,PROP_INT,PROP_INT);
	playerPropAccess["deaths"].Set(true,PLPROP_DEATHSCOUNT,PROP_INT,PROP_INT);

}
