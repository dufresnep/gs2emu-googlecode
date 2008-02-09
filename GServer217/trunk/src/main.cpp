// GraalReborn Server - Revision 51
// (C) GraalReborn 2007

#include "main.h"
#include "CIni.h"
#include "CPlayer.h"
#include "CSocket.h"
#include "CWeapon.h"
#include "CDatabase.h"
#include "CLevel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>

#ifdef WIN32
//	#ifdef _MSC_VER
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
//	#else
//		#include <dir.h>
//	#endif

	char fSep[] = "\\";
	char dataDir[] = "world\\";
#else
	#include <unistd.h>
	#include <dirent.h>
	char fSep[] = "/";
	char dataDir[] = "world/";
#endif

bool apSystem, bushesDrop, cheatwindowsban, dontaddserverflags, dontchangekills, dropItemsDead, globalGuilds, hasShutdown = false, lsConnected = false, noExplosions, serverRunning, setbodyallowed, setheadallowed, setswordallowed, setshieldallowed, showConsolePackets, staffOnly, vasesDrop, warptoforall, defaultweapons;
bool clientsidePushPull, detailedconsole;
const char* __admin[]   = {"description", "listport", "listip", "language", "maxplayers", "myip", "name", "serverport", "sharefolder", "showconsolepackets", "url", "worldname"};
const char* __colours[] = {"white", "yellow", "orange", "pink", "red", "darkred", "lightgreen", "green", "darkgreen", "lightblue", "blue", "darkblue", "brown", "cynober", "purple", "darkpurple", "lightgray", "gray", "black", "transparent"};
const char* __cloths[]  = {"setskin", "setcoat", "setsleeve", "setshoe", "setbelt", "setsleeves", "setshoes"};
CLevel* NOLEVEL = new CLevel();
CList newPlayers, playerList, playerIds, settingList, weaponList, npcList, npcIds, levelList;
CSocket responseSock, serverSock;
CString listServerFields[6], listServerIp, serverMessage, shareFolder, staffHead, worldName, unstickmeLevel;
CStringList adminNames, cheatwindows, clothCommands, colourNames, globalGuildList, jailLevels, mapNames, profileList, RCBans, RCMessage, RCHelpMessage, serverFlags, staffGuilds, staffList, statusList, subDirs;
CStringList folderConfig;
CWordFilter WordFilter;
float unstickmeX, unstickmeY;
int aptime[5], baddyRespawn, cheatwindowstime, gameTime = 1, heartLimit, horseLife, idleDisconnect, listServerPort, maxNoMovement, maxPlayers, nwTime, serverPort, serverTime = 0, shieldLimit, swordLimit, tileRespawn;

void acceptNewPlayers(CSocket &pSocket);
void doTimer();
void sendRCPacket(CPacket& pPacket);
void shutdownServer();

int main()
{
	/* Main Initiating */
	adminNames.load(__admin, sizeof(__admin) / 4);
	colourNames.load(__colours, sizeof(__colours) / 4);
	clothCommands.load(__cloths, sizeof(__cloths) / 4);
	playerIds.add(0);
	npcIds.add(0);
	srand((int)time(NULL));

	/* Load Settings */
	if (!loadSettings("serveroptions.txt"))
	{
		errorOut("rclog.txt", "Unable to load server settings..");
		return 1;
	}

	/* Load Weapons */
	if (!loadWeapons("weapons.txt"))
	{
		errorOut("rclog.txt", "Unable to load weapons from weapons.txt..");
		return 1;
	}

	/* Initialize Sockets */
	CSocket::sockStart();
	if(!serverSock.listen(serverPort, 20))
	{
		errorOut("rclog.txt", CString() << "SOCK ERROR: Unable to listen on port: " << toString(serverPort));
		return 1;
	}

	serverSock.setSync(false);

	/* Sub-Directory Caching */
	printf("[%s] Caching sub dirs\n",getTimeStr(1).text());
	getSubDirs(dataDir);
	if(shareFolder.length()>1)
		getSubDirs(shareFolder.text());

	/* Load Maps */
	for(int i = 0; i < mapNames.count(); i++)
		CMap::openMap(mapNames[i].trim());

	/* Load Important Files */
	updateFile("rchelp.txt");
	updateFile("rcmessage.txt");
	updateFile("rules.txt");
	updateFile("serverflags.txt");
	updateFile("servermessage.html");
	updateFile("foldersconfig.txt");

	/* Server Finished Loading */
	printf("GServer 2 by 39ster\nSpecial thanks to Marlon, Agret, Pac300, 39ster and others for porting the \noriginal 1.39 gserver to 2.1\nServer listening on port: %i\nServer version: Build %s\n\n", serverPort, listServerFields[3].text());
	atexit(shutdownServer);
	errorOut("rclog.txt", "Server started");
	serverRunning = true;

	if (!lsConnected)
	{
		ListServer_Connect();
	}

	while (serverRunning)
	{
		long long second = time(NULL);

		while (second == time(NULL))
		{
			acceptNewPlayers(serverSock);
			for (int i = 0; i < newPlayers.count(); i ++)
			{
				CPlayer* player = (CPlayer*)newPlayers[i];
				player->main();
				if (player->deleteMe)
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
			ListServer_Main();
			wait(100);
		}
		doTimer();
		gameTime ++;
		NOLEVEL->reset();

		// Every 60 seconds
		if (gameTime % 60 == 0)
		{
			ListServer_Send(CPacket() << (char)SLSPING);
		}

		// Every 10 seconds
		if (gameTime % 10 == 0)
		{
			CPacket pPacket;
			CString file;

			for (int i = 0; i < playerList.count(); i++)
			{
				CPlayer *player = (CPlayer *)playerList[i];
				file << player->accountName << "," << player->nickName << "," << player->levelName << "," << toString(player->x) << "," << toString(player->y) << "," << toString(player->ap) << "\n";
			}

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
	/* Reconnect Listserver if Disconnected */
	if (!lsConnected)
	{
		ListServer_Connect();
	}

	/* Level-Animations */
	for(int i = 0; i < levelList.count(); i++)
	{
		CLevel* level = (CLevel*)levelList[i];
		level->animate();
	}

	/* Do Player-Timed Actions */
	for(int i = 0; i < playerList.count(); i++)
	{
		CPlayer* player = (CPlayer*)playerList[i];

		if (player->type == CLIENTPLAYER)
		{
			if (idleDisconnect)
			{
				if ((time(NULL) - player->lastMovement > maxNoMovement) &&
					(time(NULL) - player->lastChat > maxNoMovement) )
				{
					errorOut("rclog.txt", CString() << "Client " << player->accountName << " had no activity for over " << toString(maxNoMovement) << " seconds.");
					player->sendPacket(CPacket() << (char)DISMESSAGE << "You have been disconnected because of inactivity.");
					player->deleteMe = true;
					continue;
				}
			}

			if (time(NULL) - player->lastData > 300)
			{
				errorOut("rclog.txt", CString() << "No data from " << player->accountName << " for 300 secs");
				player->deleteMe = true;
				continue;
			}

			/*if (time(NULL) - player->lastCheck > cheatwindowstime)
			{
				player->lastCheck = time(NULL);
				player->sendPacket(CPacket() << (char)SPROCCESSES << (char)73);
			}*/

			player->onlineSecs++;

			if (time(NULL) - player->lastSave <= 0)
			{
				player->saveAccount();
				player->lastSave = time(NULL);
			}

			if (apSystem)
			{
				if(!(player->status & 1))
					player->apCounter--;

				if (player->apCounter <= 0)
				{
					if (player->ap < 100)
					{
						player->ap = CLIP(player->ap + 1, 0, 100);
						player->updateProp(PALIGNMENT);
					}

					player->apCounter = (player->ap < 20 ? aptime[0] : (player->ap < 40 ? aptime[1] : (player->ap < 60 ? aptime[2] : (player->ap < 80 ? aptime[3] : aptime[4]))));
				}
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
	printf( "[%s] Incoming connection: [%s]\n", getTimeStr(1).text(), newSock->tcpIp() );
}

bool updateFile(char *pFile)
{
	char *ext = strrchr(pFile, '.');
	CString file = pFile;

	if (strcmp(ext, ".graal") == 0 || strcmp(ext, ".nw") == 0 || strcmp(ext, ".zelda") == 0)
		CLevel::updateLevel(file);
	else if (strcmp(pFile, "rchelp.txt") == 0)
		RCHelpMessage.load(pFile);
	else if (strcmp(pFile, "rcmessage.txt") == 0)
		RCMessage.load(pFile);
	else if (strcmp(pFile, "serverflags.txt") == 0)
		serverFlags.load(pFile);
	else if (strcmp(pFile, "servermessage.html") == 0)
		loadServerMessage();
	else if (strcmp(pFile, "rules.txt") == 0)
		WordFilter.load("rules.txt");
	else if ( strcmp(pFile, "foldersconfig.txt") == 0 )
		folderConfig.load( "foldersconfig.txt" );
	else
		return false;

	return true;
}

bool loadSettings(char* pFile)
{
	CStringList settings;
	if (!settings.load(pFile))
		return false;

	for (int i = 0; i < settingList.count(); i++)
	{
		delete (SettingKey *)settingList[i];
		i--;
	}
	settingList.clear();

	for (int i = 0; i < settings.count(); i++)
	{
		if (settings[i][0] == '#' || settings[i][0] == '\'')
			continue;

		SettingKey *key = new SettingKey();
		key->name = settings[i].copy(0, settings[i].find('=')).trim();
		key->value = settings[i].copy(settings[i].find('=') + 1).trim();
	}

	/* ARRAY Server-Options */
	globalGuildList.load(findKey("allowedglobalguilds"), ",");
	cheatwindows.load(findKey("cheatwindows"), ",");
	jailLevels.load(findKey("jaillevels"), ",");
	mapNames.load(findKey("maps"), ",");
	profileList.load(findKey("profilevars", "Kills:=playerkills,Deaths:=playerdeaths,Maxpower:=playerfullhearts,Rating:=playerrating,Alignment:=playerap,Gralat:=playerrupees,Swordpower:=playerswordpower,Spin Attack:=canspin"), ",");
	staffGuilds.load(findKey("staffguilds", "Server,Manager,Owner,Admin,FAQ,LAT,NAT,GAT,GP,GP Chief,Bugs Admin,NPC Admin,Gani Team,GFX Admin,Events Team,Events Admin,Guild Admin"), ",");
	staffList.load(findKey("staff"), ",");
	statusList.load(findKey("playerlisticons", "Online,Away,DND"), ",");

	/* BOOL Server-Options */
	apSystem = CHECK_BOOL(findKey("apsystem", "true"));
	bushesDrop = CHECK_BOOL(findKey("bushitems", "true"));
	cheatwindowsban = CHECK_BOOL(findKey("cheatwindowsban", "false"));
	clientsidePushPull = CHECK_BOOL(findKey("clientsidepushpull", "true"));
	defaultweapons = CHECK_BOOL(findKey("defaultweapons", "true"));
	detailedconsole = CHECK_BOOL(findKey("detailedconsole", "false"));
	dontaddserverflags = CHECK_BOOL(findKey("dontaddserverflags", "false"));
	dontchangekills = CHECK_BOOL(findKey("dontchangekills", "false"));
	dropItemsDead = CHECK_BOOL(findKey("dropitemsdead", "true"));
	globalGuilds = CHECK_BOOL(findKey("globalguilds", "true"));
	idleDisconnect = CHECK_BOOL(findKey("disconnectifnotmoved", "true"));
	noExplosions = CHECK_BOOL(findKey("noexplosions", "false"));
	setbodyallowed = CHECK_BOOL(findKey("setbodyallowed", "true"));
	setheadallowed = CHECK_BOOL(findKey("setheadallowed", "true"));
	setswordallowed = CHECK_BOOL(findKey("setswordallowed", "true"));
	setshieldallowed = CHECK_BOOL(findKey("setshieldallowed", "true"));
	showConsolePackets = CHECK_BOOL(findKey("showconsolepackets", "false"));
	staffOnly = CHECK_BOOL(findKey("staffonly", "false"));
	vasesDrop = CHECK_BOOL(findKey("vasesdrop", "true"));
	warptoforall  = CHECK_BOOL(findKey("warptoforall", "false"));

	/* INT Server-Options */
	aptime[0] = atoi(findKey("aptime0", "30"));
	aptime[1] = atoi(findKey("aptime1", "90"));
	aptime[2] = atoi(findKey("aptime2", "300"));
	aptime[3] = atoi(findKey("aptime3", "600"));
	aptime[4] = atoi(findKey("aptime4", "1200"));
	baddyRespawn = atoi(findKey("baddyrespawntime"));
	cheatwindowstime = atoi(findKey("cheatwindowstime", "60"));
	heartLimit = atoi(findKey("heartlimit", "20"));
	horseLife = atoi(findKey("horselifetime"));
	listServerFields[3] = toString(GSERVER_BUILD);
	listServerPort = atoi(findKey("listport", "14900"));
	maxNoMovement = atoi(findKey("maxnomovement", "1200"));
	maxPlayers = atoi(findKey("maxplayers", "128"));
	serverPort = atoi(findKey("serverport", "14802"));
	shieldLimit = atoi(findKey("shieldlimit", "3"));
	swordLimit = atoi(findKey("swordlimit", "4"));
	tileRespawn = atoi(findKey("tilerespawn"));
	unstickmeX = (float)atof(findKey("unstickmex", "30"));
	unstickmeY = (float)atof(findKey("unstickmey", "30.5"));

	/* TEXT Server-Options */
	unstickmeLevel = findKey("unstickmelevel", "onlinestartlocal.nw");
	listServerIp = findKey("listip", "listserver.graal.in");
	listServerFields[0] = findKey("name", "My Server");
	listServerFields[1] = findKey("description", "My Server");
	listServerFields[2] = findKey("language", "English");
	listServerFields[4] = findKey("url", "http://www.graal.in");
	listServerFields[5] = findKey("myip", "AUTO");
	shareFolder = findKey("sharefolder");
	staffHead = findKey("staffhead", "head25.png");
	worldName = findKey("worldname", "main");

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
		} while (FindNextFile(hFind, &filedata));
	}
	FindClose(hFind);
}

void getSubFiles(char* pDir, CStringList& pOut, CString* search)
{
	// Assemble the search wildcards.
	CString searchdir( pDir );
	if ( search == 0 ) searchdir << "*";
	else
	{
		searchdir << search->replaceAll( "%", "*" );
		searchdir << ".txt";
	}

	WIN32_FIND_DATA filedata;
	HANDLE hFind = FindFirstFile(searchdir.text(), &filedata);
	if(hFind!=NULL)
	{
		do
		{
			if(!(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				pOut.add(filedata.cFileName);
		} while (FindNextFile(hFind, &filedata));
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

void getSubFiles(char* pDir, CStringList& pOut, CString* search)
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
		{
			if ( search != 0 )
			{
				CString s( *search );
				CString m( ent->d_name );
				s.replaceAll( "%", "*" );
				s << ".txt";
				if ( m.match( s.text() ) == false ) continue;
			}
			pOut.add( ent->d_name );
		}
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

			// Special case with the weapon image.
			if ( parameters[1].trim() == "-" ) weapon->image = CString();
			else weapon->image = parameters[1].trim();

			weapon->modTime = (long long)atol(parameters[2].trim().text());
			weapon->code = "";
			for(i++; i < weaponData.count() && weaponData[i] != "ENDWEAPON"; i++)
				weapon->code << weaponData[i] << "\xa7";
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

		// Save name.
		weaponData[index] << weapon->name << ",";

		// If the NPC doesn't have an image, write a hyphen.
		if ( weapon->image.length() == 0 ) weaponData[index] << "-" << ",";
		else weaponData[index] << weapon->image << ",";

		// Write the modification time.
		weaponData[index] << modTime;

		code = weapon->code;
		char* line = strtok(code.text(), "\xa7");
		while(line != NULL)
		{
			weaponData.add(line);
			line = strtok(NULL, "\xa7");
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

CPlayer* findPlayerId(CString pAccountName, bool pOnly)
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

	return (pOnly ? NULL : rcFound);
}

bool isValidFile(CBuffer& file, int type)
{
	for ( int i = 0; i < folderConfig.count(); ++i )
	{
		CString ftype( folderConfig[i].readString( " " ) );
		CString fmask( folderConfig[i].readString( "" ) );
		ftype.trim();
		fmask.trim();

		switch ( type )
		{
			case 11:	// HEADGIF
				if ( ftype == "head" )
					if ( file.match( fmask.text() ) )
						return true;
			break;

			case 35:	// BODYIMG
				if ( ftype == "body" )
					if ( file.match( fmask.text() ) )
						return true;
			break;

			case 8:		// SWORDPOWER
				if ( ftype == "sword" )
					if ( file.match( fmask.text() ) )
						return true;
			break;

			case 9:		// SHIELDPOWER
				if ( ftype == "shield" )
					if ( file.match( fmask.text() ) )
						return true;
			break;

			case 1:		// level
				if ( ftype == "level" )
					if ( file.match( fmask.text() ) )
						return true;
			break;

			default:
			case 0:		// file
				if ( ftype == "file" )
					if ( file.match( fmask.text() ) )
						return true;
			break;
		}
	}

	return false;
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
		ListServer_End();
	errorOut("rclog.txt", "Server shutdown..");
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
}

void errorOut(char *pFile, CBuffer pError, bool pWrite)
{
	if (pWrite)
	{
		printf("[%s] %s\n", getTimeStr(1).text(), pError.text());
	}

	char buffer[60] = "logs/";
	strcpy(buffer+5, pFile);
	FILE *file = fopen(buffer, "a");
	fprintf(file, "[%s] %s\r\n", getTimeStr(0).text(), pError.text());
	fclose(file);
}

CPacket listFiles(char *pDir, char *pRights)
{
	CPacket retVal;
	CStringList files;
	struct stat fileStat;
	getSubFiles(pDir, files);

	for (int i = 0; i < files.count(); i++)
	{
		CPacket dir;
		CString fullName;
		fullName << pDir << files[i];
		stat(fullName.text(), &fileStat);
		dir << (char)files[i].length() << files[i] << (char)strlen(pRights) << pRights << (long long)fileStat.st_size << (long long)fileStat.st_mtime;
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

CString getTimeStr(int pType)
{
	char timestr[60];
	time_t curtime = time(NULL);

	switch (pType)
	{
		case 1:
			strftime(timestr, sizeof(timestr), "%I:%M %p", localtime(&curtime));
		break;

		default:
			strftime(timestr, sizeof(timestr), "%a %b %d %X %Y", localtime(&curtime));
		break;
	}

	return CString(timestr);
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

/* Setting-Key */
SettingKey::SettingKey()
{
	settingList.add(this);
}

SettingKey::~SettingKey()
{
	settingList.remove(this);
}

char *findKey(CString pName, char *pDefault)
{
	for (int i = 0; i < settingList.count(); i++)
	{
		SettingKey *key = (SettingKey *)settingList[i];
		if (key->name == pName)
			return key->value.text();
	}

	// Workaround an odd bug.
	if ( pDefault )
		return pDefault;
	else
		return "0";
}
