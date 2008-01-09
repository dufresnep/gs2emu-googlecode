// GraalReborn Server - Revision 50
// (C) GraalReborn 2007

#include "main.h"
#include "CIni.h"
#include "CPlayer.h"
#include "CSocket.h"
#include "CWeapon.h"
#include "CDatabase.h"
#include "CLevel.h"
#include "CDatabase.h"
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#define colourcount 20

CString rev = "50";
#ifdef WIN32
    char fSep[] = "\\";
    char dataDir[] = "world\\";
#else
    #include <unistd.h>
    #include <dirent.h>
    char fSep[] = "/";
    char dataDir[] = "world/";
#endif

bool apSystem, bushesDrop, dropItemsDead, globalGuilds, hasShutdown = false, lsConnected = false, noExplosions, serverRunning, showConsolePackets, showQuery, staffOnly, vasesDrop;
const char* __colours[] = {"white", "yellow", "orange", "pink", "red", "darkred", "lightgreen", "green", "darkgreen", "lightblue", "blue", "darkblue", "brown", "cynober", "purple", "darkpurple", "lightgray", "gray", "black", "transparent"};
const char* __cloths[]  = {"setskin", "setcoat", "setsleeve", "setshoe", "setbelt", "setsleeves", "setshoes"};
CLevel* NOLEVEL = new CLevel();
CList playerList, newPlayers, playerIds, weaponList, npcList, npcIds, levelList;
CSocket listServer, serverSock;
CString databaseFile, listServerFields[6], listServerIp, serverMessage, shareFolder, staffHead, worldName, unstickmeLevel;
CStringList clothCommands, colourNames, globalGuildList, jailLevels, mapNames, profileList, RCBans, rcFolders, RCMessage, RCHelpMessage, serverFlags, staffGuilds, staffList, statusList, subDirs;
FILE* _errorFile;
float unstickmeX, unstickmeY;
int apInterval, baddyRespawn, gameTime = 1, heartLimit, horseLife, idleDisconnect, listServerPort, maxNoMovement, maxPlayers, nwTime, serverPort, serverTime = 0, shieldLimit, swordLimit, tileRespawn;

void acceptNewPlayers(CSocket &pSocket);
void addListServer();
void doTimer();
bool loadSettings(char* pFile);
bool loadWeapons(char* pFile);
void processListServer();
void sendRCPacket(CPacket& pPacket);
void shutdownServer();

int main()
{
    //CSocket responseSock;
	if((_errorFile = fopen("logs/errorlog.log", "a")) == NULL)
	{
		printf("Unable to open errorlog.log\n");
		return 1;
	}
	colourNames.load(__colours, colourcount);
	clothCommands.load(__cloths, 7);

    if(!loadSettings("settings.ini"))
    {
		errorOut("Unable to load server settings..");
        return 1;
    }

    if(!loadWeapons("weapons.txt"))
    {
        errorOut("Unable to load weapons from weapons.txt..");
        return 1;
    }

	if(!openDatabase("gserver.db"))
	{
		errorOut("Unable to connect to database gserver.db");
		return 1;
	}


	srand((int)time(NULL));
	loadServerMessage();
	RCMessage.load("rcmessage.txt");
	RCHelpMessage.load("rchelp.txt");
	serverFlags.load("serverflags.txt");
    CSocket::sockStart();

    if(!serverSock.listen(serverPort, 20))
    {
        errorOut(CString() << "SOCK ERROR: Unable to listen on port: "
			<< toString(serverPort));
        return 1;
    }
/*
    if(!responseSock.udpBind(serverPort + 1))
    {
        errorOut(CString() << "SOCK ERROR: Unable to bind UDP response socket to port "
			<< toString(serverPort + 1));
        return 1;
    }
*/
    addListServer();
    serverSock.setSync(false);
	//responseSock.setSync(false);
	rcFolders.load("rcfolders.txt");

	printf("Caching sub dirs\n");
    getSubDirs(dataDir);
	if(shareFolder.length()>1)
		getSubDirs(shareFolder.text());

    printf("GServer 2 by 39ster\n"
        "Special thanks to Marlon, Agret, pacMASTA, 39ster and others for porting the \noriginal 1.39 gserver to 2.*\n"
        "Server listening on port: %i\nServer version: Revision %s\n", serverPort, listServerFields[3].text());

    serverRunning = true;

	playerIds.add(0);
    npcIds.add(0);
	printf("Loading maps..");
    for(int i = 0; i < mapNames.count(); i++)
        CMap::openMap(mapNames[i].trim());
    printf("Done\n");

    atexit(shutdownServer);
	errorOut("Server started");
    while(serverRunning)
    {
        long long second = time(NULL);
        while(second == time(NULL))
        {
            acceptNewPlayers(serverSock);
            for(int i = 0; i < newPlayers.count(); i ++)
            {
                CPlayer* player = (CPlayer*)newPlayers[i];
                player->main();
                if(player->deleteMe)
                {
                    delete player;
                    i--;
                }
            }

            for(int i = 0; i < playerList.count(); i++)
            {
                CPlayer* player = (CPlayer*)playerList[i];
                player->main();
                if(player->deleteMe)
                {
                    delete player;
                    i--;
                }
            }

			// Was moved so it can process faster. - Joey
			processListServer();
            wait(100);
        }
        doTimer();
		gameTime ++;
		NOLEVEL->reset();

		//Every 10 seconds
		if(gameTime %10 == 0)
		{
		    CPacket pPacket;
			CString file;

			for (int i = 0; i < playerList.count(); i++)
			{
				CPlayer *player = (CPlayer *)playerList[i];
				file << player->accountName << "," << player->nickName << "," << player->levelName << "," << toString(player->x) << "," << toString(player->y) << "," << toString(player->ap) << "\n";
				pPacket << (char)player->accountName.length() << player->accountName << player->getProp(NICKNAME) << player->getProp(LEVELNAME) << player->getProp(PLAYERX) << player->getProp(PLAYERY) << player->getProp(PALIGNMENT);
			}

		    if(lsConnected)
		    {
                listServer.sendBuffer(CPacket() << (char)SLSCOUNT << (char)playerList.count() << pPacket << "\n");
		    } else addListServer();

			file.save("logs/playerlist.txt");
			serverFlags.save("serverflags.txt");
		}

		//Every 5 seconds?
		int current = getNWTime();
		if (nwTime != current)
		{
			nwTime = current;
			for (int i = 0; i < playerList.count(); i++)
			{
				CPacket out;
				out << (char)NEWWORLDTIME;
				out.writeByte4(current);
				((CPlayer*)playerList[i])->sendPacket(out);
			}
		}
    }
}

void doTimer()
{
    for(int i = 0; i < levelList.count(); i++)
    {
        CLevel* level = (CLevel*)levelList[i];
        level->animate();
    }

    for(int i = 0; i < playerList.count(); i++)
    {
		CPlayer* player = (CPlayer*)playerList[i];

		if (idleDisconnect)
		{
			if (time(NULL) - player->lastMovement > 1200 && player->type == CLIENTPLAYER)
			{
			    errorOut(CString() << "Client " << player->accountName << " didn't move for >20 mins.");
				player->sendPacket(CPacket() << (char)DISMESSAGE << "You have been disconnected because you didn't move.");
			    player->deleteMe = true;
			    continue;
			}
		}

		if (time(NULL) - player->lastData > 300 && player->type == CLIENTPLAYER)
		{
		    errorOut(CString() << "No data from " << player->accountName << " for 300 secs");
		    player->deleteMe = true;
		    continue;
		}

		player->onlineSecs++;
		if(player->type == CLIENTPLAYER)
		{
            if (time(NULL) - player->lastSave <= 0)
            {
                player->saveAccount();
                player->saveWorldProps();
                player->lastSave = time(NULL);
            }
		}

		if (apSystem)
		{
			if(!(player->status & 1))
				player->apCounter--;

			if (player->apCounter <= 0)
			{
				if(player->ap < 100)
				{
					player->ap++;
					player->updateProp(PALIGNMENT);
				}
				player->apCounter = (apInterval * MAX((int)(player->ap/10), 1));
			}
		}
	}
}

void acceptNewPlayers(CSocket& pSocket)
{
    CSocket* newSock = pSocket.accept();
    if(newSock == 0)
        return;

    newSock->setSync(false);
	newSock->setNagle(false);
    newPlayers.add(new CPlayer(newSock));
    printf("Connection accepted...awaiting login details\n");
}

bool loadSettings(char* pFile)
{
    CIni iniSettings;
    if(!iniSettings.load(pFile))
        return false;

	databaseFile = iniSettings.readString("server", "databasefile", "gserver.db");
    maxPlayers = (int)iniSettings.readReal("server", "maxplayers", 64);
    serverPort = (int)iniSettings.readReal("server", "serverport", 14804);
    maxNoMovement = (int)iniSettings.readReal("server", "maxnomovement", 100);
	worldName = iniSettings.readString("server", "worldname", "main");
	shareFolder = iniSettings.readString("server", "sharefolder", "");
	if (shareFolder[0] != fSep[0])
		shareFolder << fSep;

	globalGuildList.load(iniSettings.readString("server", "allowedglobalguilds", ""), ",");
	globalGuilds = (iniSettings.readReal("server", "globalguilds", 1) > 0);
	jailLevels.load(iniSettings.readString("server", "jaillevels", ""), ",");
	mapNames.load(iniSettings.readString("server", "maps", ""), ",");
	profileList.load(iniSettings.readString("server", "profilevars", "playerkills,playerdeaths,playerfullhearts,playerrating,playerap,playerrupees,playerswordpower,canspin"), ",");
    showConsolePackets = (iniSettings.readReal("server", "showconsolepackets", 1) > 0);
	showQuery = (iniSettings.readReal("server", "showquery", 1) > 0);
	staffGuilds.load(iniSettings.readString("server", "staffguilds", "Server"), ",");
	staffHead = iniSettings.readString("server", "staffhead", "head25.png");
	staffList.load(iniSettings.readString("server", "staff", ""), ",");
	staffOnly = (iniSettings.readReal("server", "staffonly", 1) > 0);
	statusList.load(iniSettings.readString("server", "playerlisticons", ""), ",");

	baddyRespawn = (int)iniSettings.readReal("level", "baddyrespawn", 60);
	bushesDrop = (iniSettings.readReal("level", "bushesdrop", 1) > 0);
	horseLife = (int)iniSettings.readReal("level", "horselife", 10);
	noExplosions = (iniSettings.readReal("level", "noexplosions", 1) > 0);
	tileRespawn = (int)iniSettings.readReal("level", "tilerespawn", 15);
	vasesDrop = (iniSettings.readReal("level", "vasesdrop", 1) > 0);

	apInterval = (int)iniSettings.readReal("player", "apinterval", 100);
	apSystem = (iniSettings.readReal("level", "apsystem", 1) > 0);
    dropItemsDead = (iniSettings.readReal("player", "dropitemsdead", 1) > 0);
	idleDisconnect = (iniSettings.readReal("player", "disconnectifnotmoved", 1) > 0);
	heartLimit = (int)iniSettings.readReal("player", "heartlimit", 3);
	shieldLimit = (int)iniSettings.readReal("player", "shieldlimit", 1);
	swordLimit = (int)iniSettings.readReal("player", "swordlimit", 1);
	unstickmeX = (float)iniSettings.readReal("player", "unstickmex", 32);
	unstickmeY = (float)iniSettings.readReal("player", "unstickmey", 32);
	unstickmeLevel = iniSettings.readString("player", "unstickmelevel", "startlevel.graal");

    listServerPort = (int)iniSettings.readReal("listserver", "listport", 14800);
    listServerIp = iniSettings.readString("listserver", "listip", "127.0.0.1");
    listServerFields[0] = iniSettings.readString("listserver", "name", "???");
    listServerFields[1] = iniSettings.readString("listserver", "description", "???");
    listServerFields[2] = iniSettings.readString("listserver", "language", "???");
    listServerFields[3] = rev;
    listServerFields[4] = iniSettings.readString("listserver", "url", "???");
    listServerFields[5] = iniSettings.readString("listserver", "myip", "AUTO");
    return true;
}

#ifdef WIN32
void getSubDirs(char *pDir)
{
    CString searchdir = CString() << pDir << "*";
	WIN32_FIND_DATA filedata;
	HANDLE hFind = FindFirstFile(searchdir.text(), &filedata);
	subDirs.add(pDir);

	if(hFind!=NULL)
	{
		do
		{
			if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(filedata.cFileName[0] != '.')
				{
					CString directory = CString() << pDir << filedata.cFileName << fSep;
					getSubDirs(directory.text());
				}
			}
		}while(FindNextFile(hFind, &filedata));
	}
	FindClose(hFind);
}

void getSubFiles(char* pDir, CStringList& pOut)
{
	CString searchdir = CString() << pDir << "*";
    WIN32_FIND_DATA filedata;
	HANDLE hFind = FindFirstFile(searchdir.text(), &filedata);
    if(hFind!=NULL)
	{
		do
		{
			if(!(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			    pOut.add(filedata.cFileName);
		}while(FindNextFile(hFind, &filedata));
	}
	FindClose(hFind);
}
#else
void getSubDirs(char *pDir)
{
	DIR *dir;
	struct stat statx;
	struct dirent *ent;
	if ((dir = opendir(pDir)) == NULL)
        return;
    subDirs.add(pDir);
	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] != '.')
		{
			CString directory = CString() << pDir << ent->d_name << fSep;
			stat(directory.text(), &statx);
			if (statx.st_mode & S_IFDIR)
				getSubDirs(directory.text());
		}
	}
	closedir(dir);
}

void getSubFiles(char* pDir, CStringList& pOut)
{
	DIR *dir;
	struct stat statx;
	struct dirent *ent;
	if ((dir = opendir(pDir)) == NULL)
        return;
	while ((ent = readdir(dir)) != NULL)
	{
		CString fullName = CString() << pDir << ent->d_name;
		stat(fullName.text(), &statx);
		if (!(statx.st_mode & S_IFDIR))
			pOut.add(ent->d_name);
	}
	closedir(dir);
}
#endif

bool loadWeapons(char* pFile)
{
	CStringList weaponData;
	if(!weaponData.load(pFile))
		return false;
    weaponList.clear();
	for(int i = 0; i < weaponData.count(); i++)
	{
		CString word = weaponData[i].readString(" ");
		if(word == "NEWWEAPON")
		{
			CStringList parameters;
			parameters.load(weaponData[i].text() + word.length() + 1, ",");
			if(parameters.count() <= 2)
				continue;

			CWeapon* weapon = new CWeapon;
			weapon->name = parameters[0].trim();
			weapon->image = parameters[1].trim();
			weapon->modTime = (int)atoi(parameters[2].trim().text());

			for(i++; i < weaponData.count() && weaponData[i] != "ENDWEAPON"; i++)
				weapon->code << weaponData[i] << "§";
			weaponList.add(weapon);
		}
	}
	return true;
}

void saveWeapons(char* pFile)
{
	CStringList weaponData;
	for(int i = 0; i < weaponList.count(); i++)
	{
		CString code;
		char modTime[30];
		int index;

		CWeapon* weapon = (CWeapon*)weaponList[i];
		index = weaponData.add("NEWWEAPON ");
		sprintf(modTime, "%li", (long int)weapon->modTime);
		weaponData[index] << weapon->name << "," << weapon->image << "," << modTime;
		code = weapon->code;
		char* line = strtok(code.text(), "§");
		while(line != NULL)
		{
			weaponData.add(line);
			line = strtok(NULL, "§");
		}
		weaponData.add("ENDWEAPON\r\n");
	}
	weaponData.save(pFile);
}

char* getDataFile(char* pFile)
{
    static char path[260];
	FILE* file;
	for(int i = 0; i < subDirs.count(); i++)
	{
		strncpy(path, subDirs[i].text(), sizeof(path));
		strncat(path, pFile, sizeof(path));
		if((file = fopen(path, "r")) != 0)
		{
			fclose(file);
			return path;
		}
	}
	path[0] = 0;

	return path;
}

long long getFileModTime(char* pFile)
{
	struct stat fileStat;
	if(strlen(pFile) <=0)
        return 0;
	if(stat(pFile, &fileStat) != -1)
        return fileStat.st_mtime;
	return 0;
}

int getFileSize(char* pFile)
{
    struct stat fileStat;
	if(strlen(pFile) <=0)
        return 0;
	if(stat(pFile, &fileStat) != -1)
        return fileStat.st_size;
	return 0;
}
int createPlayerId(CPlayer* pPlayer)
{
    for(int i = 1; i < playerIds.count(); i++)
    {
        if(playerIds[i] == NULL)
        {
            playerIds.replace(i, pPlayer);
            return i;
        }
    }
    return playerIds.add(pPlayer);
}

int createNpcId(CNpc* pNpc)
{
    for(int i = 1; i < npcIds.count(); i++)
    {
        if(npcIds[i] == NULL)
        {
            npcIds.replace(i, pNpc);
            return i;
        }
    }
    return npcIds.add(pNpc);
}

void loadServerMessage()
{
	CStringList fileData;
	if(!fileData.load("servermessage.html"))
		return;

    serverMessage.clear();
	for(int i = 0; i < fileData.count(); i++)
		serverMessage << fileData[i] << " ";
}

CPlayer* findPlayerId(CString& pAccountName)
{
    CPlayer* rcFound = NULL;
    for(int i = 0; i < playerList.count(); i++)
    {
        CPlayer* other = (CPlayer*)playerList[i];
        if(stricmp(other->accountName.text(), pAccountName.text()) == 0)
        {
            if(other->type == CLIENTRC)
            {
                rcFound = other;
            } else return other;
        }
    }
    return rcFound;
}

bool isIpBanned(CString& pIp)
{
	CStringList ipList;
	if(!ipList.load("ipbans.txt"))
		return false;

	for(int i = 0; i < ipList.count(); i++)
	{
		if(!ipList[i].length() || ipList[i][0] == '#')
			continue;
		if(pIp.match(ipList[i].text()))
			return true;
	}
	return false;
}

void processListServer()
{
    if(!lsConnected)
        return;

    CBuffer receiveBuff;
    if(listServer.receiveBytes(receiveBuff, 65536) < 0)
    {
        errorOut("Disconnected from list server");
        lsConnected = false;
        return;
    }

    CStringList lines;
    lines.load(receiveBuff.text(), "\n");
    for (int i = 0; i < lines.count(); i++)
    {
        CPacket line = CPacket() << lines[i];
        int messageId = line.readByte1();

        switch (messageId)
        {
            case GSVOLD:
            {
                CString rev3 = line.readByte1();
                CString rev2 = line.readString("");
                CString newrev;
                #ifdef WIN32
                newrev << "Revision" << rev2.text() << ".exe";
                #else
                newrev << "Revision" << rev2.text();
                #endif
                 listServer.sendBuffer(CPacket() << (char)SLVFILE << (short)-1 << (char)0 << (char)newrev.length() << newrev.text());
                 printf("*** SERVER VERSION CHECK ***\nYou're running an old version of the GServer.\nYou're running GServer Revision %s while GServer Revision %s is the latest.\nGServer will upgrade itself.\n*** SERVER VERSION CHECK ***\n",rev.text(),rev2.text());

                 break;
            }

            case GSVCURRENT:
            {
                 printf("*** SERVER VERSION CHECK ***\nYou're running an up-to-date server. :)\n*** SERVER VERSION CHECK ***\n");
                 break;
            }

            case GSVACCOUNT:
            {
                CString accountName = line.readChars(line.readByte1());
                CString errorMsg = line.readString("");

                for (int i = 0; i < newPlayers.count(); i++)
                {
                    CPlayer *player = (CPlayer *)newPlayers[i];

                    if (player->accountName == accountName)
                    {
                        if (errorMsg == "SUCCESS")
                        {
                            player->sendAccount();
                        }
                            else
                        {
                            player->sendPacket(CPacket() << (char)DISMESSAGE << errorMsg);
                            player->deleteMe = true;
                        }

                        break;
                    }
                }

                break;
            }

            case GSVGUILD:
            {
                int playerId = line.readByte2();
                CPlayer *player = (CPlayer *)playerIds[playerId];

                if (player != NULL)
                {
                    CString nick = line.readChars((unsigned char)line.readByte1());
                    player->setNick(nick, false);
                }

                break;
            }

            case GSVFILEC:
            {
				CString fileData, fileName = CString() << dataDir << "global" << fSep << line.readChars(line.readByte1());
				fileData.save(fileName.text());

				break;
            }

            case GSVFILED:
            {
				CString fileName = line.readChars(line.readByte1());
				CPlayer *player = (CPlayer *)playerIds[line.readByte2()];

                switch (line.readByte1())
                {
                    case 0: // head
                        player->headImage = fileName;
                        player->updateProp(HEADGIF);
                    break;

                    case 1: // body
                        player->bodyImage = fileName;
                        player->updateProp(BODYIMG);
                    break;

                    case 2: // sword
                        player->swordImage = fileName;
                        player->updateProp(SWORDPOWER);
                    break;

                    case 3: // shield
                        player->shieldImage = fileName;
                        player->updateProp(SHIELDPOWER);
                    break;
                }

				break;
            }

			case GSVFILES:
			{
                CString fileData, fileName, newData, shortName;
				shortName = line.readChars(line.readByte1());
    int pos = shortName.find("Revision");

    if(pos >= 0)
    {
                #ifdef WIN32
           		fileName = CString() << "GServer-NEW.exe";
           		#else
           		fileName = CString() << "GServer-NEW";
           		#endif
				newData = line.readString("");

    } else  {

				fileName = CString() << dataDir << "global" << fSep << shortName.text();
				newData = line.readString("");
    }
				fileData.load(fileName.text());
				fileData << newData.B64_Decode();
				fileData.save(fileName.text());

				break;
			}

            default:
                printf("Invalid List Server Message: %i\n", messageId);
            break;
        }
    }

}
void addListServer()
{
    if(!listServer.connect(listServerIp.text(), listServerPort))
    {
		errorOut("Unable to connect to list server");
        lsConnected = false;
        return;
    }

    //send gserver details to list server
    listServer.sendBuffer(CPacket() <<
        (char)SLSNAME << listServerFields[0]  << "\n" <<
        (char)SLSDESC << listServerFields[1]  << "\n" <<
        (char)SLSLANG << listServerFields[2]  << "\n" <<
        (char)SLSVER  << listServerFields[3]  << "\n" <<
        (char)SLSURL  << listServerFields[4]  << "\n" <<
        (char)SLSIP   << listServerFields[5]  << "\n" <<
        (char)SLSPORT << toString(serverPort) << "\n");

    listServer.setSync(false);
    lsConnected = true;

}

char* removeGifExtension(CString& pFileName)
{
    int pos = pFileName.find(".gif", pFileName.length()-5);
    if(pos >= 0)
    {
        CBuffer::retBuffer = pFileName.copy(0, pos);
    } else CBuffer::retBuffer = pFileName;
    return CBuffer::retBuffer.text();
}

void shutdownServer()
{
    if(hasShutdown)
        return;
    hasShutdown = true;
    serverRunning = false;
    if(lsConnected)
        listServer.closeSock();
	errorOut("Server shutdown..");
    serverSock.closeSock();
    saveWeapons("weapons.txt");
    serverFlags.save("serverflags.txt");
    for(int i = playerList.count()-1; i >= 0; i--)
        delete ((CPlayer*)playerList[i]);

    for(int i = 0; i < levelList.count(); i++)
    {
        CLevel* level = (CLevel*)levelList[i];
        level->saveNpcs();
        delete level;
    }

    for(int i = 0; i < CMap::mapList.count(); i++)
        delete((CMap*)CMap::mapList[i]);

    for(int i = 0; i < weaponList.count(); i++)
        delete ((CWeapon*)weaponList[i]);
	fclose(_errorFile);
}


void errorOut(char* pError)
{
	time_t curtime;
	time(&curtime);
	char timestr[30];
	printf("Log: %s\n", pError);
	strftime(timestr, sizeof(timestr), "%x %X", localtime(&curtime));
	fprintf(_errorFile, "[%s]  %s\n", timestr, pError);
	fflush(_errorFile);
}

CPacket listFiles(char* pDir)
{
    CPacket retVal;
    CStringList files;
    struct stat fileStat;
    getSubFiles(pDir, files);
    CString rights = "rw";
    for(int i = 0; i < files.count(); i++)
    {
        CPacket dir;
        CString fullName;
		fullName << pDir << files[i];
        stat(fullName.text(), &fileStat);
		dir << (char)files[i].length() << files[i] <<
			(char)rights.length() << rights << (long long)fileStat.st_size << (long long)fileStat.st_mtime;
		retVal << " " << (char)dir.length() << dir;
    }
    return retVal;
}

CString getFileExtension(CString& pFileName)
{
    CString retVal;
    int pos = pFileName.find('.');
    if(pos >= 0)
    {
        for(int i = pos; i < pFileName.length(); i++)
            retVal.writeChar(pFileName[i]);
    }
    return retVal;
}

void sendAllPacket(CPacket& pPacket)
{
	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];
		other->sendPacket(pPacket);
	}
}

void sendPlayerPacket(CPacket& pPacket)
{
	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];

		if (other->type == CLIENTPLAYER)
			other->sendPacket(pPacket);
	}
}

void sendRCPacket(CPacket& pPacket)
{
	for (int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];

		if (other->type == CLIENTRC)
			other->sendPacket(pPacket);
	}
}
