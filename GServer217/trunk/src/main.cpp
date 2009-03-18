/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include "main.h"
#include "CIni.h"
#include "CPlayer.h"
#include "CSocket.h"
#include "CWeapon.h"
#include "CDatabase.h"
#include "CLevel.h"
#include "CBuffer.h"
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <sys/utime.h>
	#define _utime utime
	#define _utimbuf utimbuf;
#elif defined(PSPSDK)
	#include <pspkernel.h>
	#include <pspdebug.h>
	#include <pspsdk.h>
	PSP_MODULE_INFO("GServer217", 0, 1, 1);
#endif

#ifndef WIN32
	#include <unistd.h>
	#include <dirent.h>
	#include <utime.h>

	#ifndef SIGBREAK
		#define SIGBREAK SIGQUIT
	#endif
#endif

// Function pointer for signal handling.
typedef void (*sighandler_t)(int);

bool apSystem, bushesDrop, cheatwindowsban, dontaddserverflags, dontchangekills, dropItemsDead, globalGuilds, hasShutdown = false, lsConnected = false, noExplosions, serverRunning, setbodyallowed, setheadallowed, setswordallowed, setshieldallowed, showConsolePackets, staffOnly, vasesDrop, warptoforall, defaultweapons;
bool clientsidePushPull, detailedconsole, baddyDropItems, noFoldersConfig;
bool healswords, putnpcenabled, adminCanChangeGralat;
int mindeathgralats, maxdeathgralats, tiledroprate;
char fSep[] = "/";
const char* __admin[]   = {"description", "detailedconsole", "language", "listport", "listip", "maxplayers", "myip", "name", "nofoldersconfig", "onlystaff", "serverport", "sharefolder", "showconsolepackets", "url", "worldname"};
const char* __colours[] = {"white", "yellow", "orange", "pink", "red", "darkred", "lightgreen", "green", "darkgreen", "lightblue", "blue", "darkblue", "brown", "cynober", "purple", "darkpurple", "lightgray", "gray", "black", "transparent"};
const char* __cloths[]  = {"setskin", "setcoat", "setsleeve", "setshoe", "setbelt", "setsleeves", "setshoes"};
const char* __defaultfiles[] = {
	"carried.gani", "carry.gani", "carrystill.gani", "carrypeople.gani", "dead.gani", "def.gani", "ghostani.gani", "grab.gani", "gralats.gani", "hatoff.gani", "haton.gani", "hidden.gani", "hiddenstill.gani", "hurt.gani", "idle.gani", "kick.gani", "lava.gani", "lift.gani", "maps1.gani", "maps2.gani", "maps3.gani", "pull.gani", "push.gani", "ride.gani", "rideeat.gani", "ridefire.gani", "ridehurt.gani", "ridejump.gani", "ridestill.gani", "ridesword.gani", "shoot.gani", "sit.gani", "skip.gani", "sleep.gani", "spin.gani", "swim.gani", "sword.gani", "walk.gani", "walkslow.gani",
	"sword1.png", "sword2.png", "sword3.png", "sword4.png",
	"shield1.png", "shield2.png", "shield3.png"
};
CLevel* NOLEVEL = new CLevel();
CList newPlayers, playerList, playerIds, settingList, weaponList, npcList, npcIds, levelList;
CSocket responseSock, serverSock;
CString dataDir, listServerFields[6], listServerIp, serverMessage, shareFolder, staffHead, worldName, unstickmeLevel;
CString programDir;
CStringList adminNames, cheatwindows, clothCommands, colourNames, globalGuildList, jailLevels, mapNames, profileList, RCBans, RCMessage, RCHelpMessage, serverFlags, staffGuilds, staffList, statusList, subDirs;
CStringList folderConfig, defaultFiles;
CWordFilter WordFilter;
bool oldcreated;
float unstickmeX, unstickmeY;
int aptime[5], baddyRespawn, cheatwindowstime, gameTime = 1, heartLimit, horseLife, idleDisconnect, listServerPort, maxNoMovement, maxPlayers, nwTime, serverTime = 0, shieldLimit, swordLimit, tileRespawn;
CString serverPort, localip;
CString serverhq_pass;
int serverhq_level;

void acceptNewPlayers(CSocket &pSocket);
void doTimer();
void sendRCPacket(CPacket& pPacket);
void shutdownServer( int signal );

#ifdef PSPSDK
	/* Exit callback */
	int exit_callback(int arg1, int arg2, void *common)
	{
		shutdownServer(0);
		return 0;
	}

	/* Callback thread */
	int CallbackThread(SceSize args, void *argp)
	{
		int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
		sceKernelRegisterExitCallback(cbid);
		sceKernelSleepThreadCB();
		return 0;
	}

	/* Sets up the callback thread and returns its thread id */
	int SetupCallbacks()
	{
		int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
		if (thid >= 0)
			sceKernelStartThread(thid, 0, 0);
		return thid;
	}
#endif

int main(int argc, char *argv[])
{
	#ifdef PSPSDK
		pspDebugScreenInit();
		SetupCallbacks();
	#else
		// Shut down the server if we get a kill signal.
		signal( SIGINT, (sighandler_t) shutdownServer );
		signal( SIGTERM, (sighandler_t) shutdownServer );
		signal( SIGBREAK, (sighandler_t) shutdownServer );
		signal( SIGABRT, (sighandler_t) shutdownServer );
	#endif

	/* Setup Data-Directory */
	dataDir = CBuffer(argv[0]).replaceAll("\\", "/");
	dataDir = dataDir.copy(0, dataDir.findl('/') + 1);
	programDir = dataDir;
	dataDir << "world/";

	/* Main Initiating */
	adminNames.load( __admin, sizeof(__admin) / sizeof(const char*) );
	colourNames.load( __colours, sizeof(__colours) / sizeof(const char*) );
	clothCommands.load( __cloths, sizeof(__cloths) / sizeof(const char*) );
	defaultFiles.load( __defaultfiles, sizeof(__defaultfiles) / sizeof(const char*) );
	playerIds.add(0);
	playerIds.add(0);
	npcIds.add(0);
	srand((int)time(NULL));

	/* Load Important Files */
	updateFile("rchelp.txt");
	updateFile("rcmessage.txt");
	updateFile("rules.txt");
	updateFile("serverflags.txt");
	updateFile("servermessage.html");
	updateFile("foldersconfig.txt");
	updateFile("serverhq.txt");

	/* Load Settings */
	if (!loadSettings("serveroptions.txt"))
	{
		errorOut("errorlog.txt", "Unable to load server settings..");
		return 1;
	}

	/* Load Weapons */
	if (!loadWeapons("weapons.txt"))
	{
		errorOut("errorlog.txt", "Unable to load weapons from weapons.txt..");
		return 1;
	}

	/* Initialize Sockets */
	serverSock.setType( SOCKET_TYPE_SERVER );
	serverSock.setProtocol( SOCKET_PROTOCOL_TCP );
	serverSock.setOptions( SOCKET_OPTION_NONBLOCKING );
	serverSock.setDescription( "serverSock" );
	CString empty;
	if ( serverSock.init( empty, serverPort ) )
		return 1;

	// Connect server socket.
	if ( serverSock.connect() )
	{
		errorOut("errorlog.txt", CString() << "SOCK ERROR: Unable to listen on port: " << serverPort);
		return 1;
	}

	/* Server Finished Loading */
	printf("GServer 2 by 39ster\nSpecial thanks to Marlon, Agret, Pac300, 39ster and others for porting the \noriginal 1.39 gserver to 2.1\nServer listening on port: %s\nServer version: Build %s\n\n", serverPort.text(), listServerFields[3].text());
	errorOut("serverlog.txt", "Server started");

	if ( listServerFields[5] == "localhost" )
		errorOut("serverlog.txt", "[DEBUG_LOCALHOSTMODE] Localhost mode is activated.\nListserver communication & account authentication are disabled.", true);

	serverRunning = true;

	if ( !(listServerFields[5] == "localhost") )
		if (!lsConnected)
			ListServer_Connect();

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
			wait(10);
		}

		doTimer();
		gameTime ++;
		NOLEVEL->reset();

		// Every 30 seconds
		if (gameTime % 30 == 0)
		{
			ListServer_Send(CPacket() << (char)SLSPING << "\n");
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

		// Every 5 seconds.
		if (gameTime % 5 == 0)
		{
			/* Reconnect Listserver if Disconnected */
			if ( !(listServerFields[5] == "localhost") )
				if (!lsConnected)
					ListServer_Connect();
		}

		// Send the current server time to the client.
		// Happens every 5 seconds.
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
				if ((getTime() - player->lastMovement > maxNoMovement) &&
					(time(NULL) - player->lastChat > maxNoMovement) )
				{
					errorOut("errorlog.txt", CString() << "Client " << player->accountName << " had no activity for over " << toString(maxNoMovement) << " seconds.");
					player->sendPacket(CPacket() << (char)DISMESSAGE << "You have been disconnected because of inactivity.");
					player->deleteMe = true;
					continue;
				}
			}

			if (time(NULL) - player->lastData > 300)
			{
				errorOut("errorlog.txt", CString() << "No data from " << player->accountName << " for 300 secs");
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
				if ( !(player->status & 1) && player->level->sparZone == false )
					player->apCounter--;

				if ( player->apCounter <= 0 )
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
	{
		folderConfig.load( "foldersconfig.txt" );

		// Don't allow .. in the folder path.
		for ( int i = 0; i < folderConfig.count(); ++i )
		{
			if ( ((CBuffer)folderConfig[i]).find( ".." ) != -1 )
			{
				folderConfig.remove(i);
				--i;
			}
		}

		getSubDirs();
	}
	else if (strcmp(pFile, "serverhq.txt") == 0)
	{
		CStringList settings;
		if (!settings.load("serverhq.txt"))
			return false;

		serverhq_pass.clear();
		serverhq_level = 1;

		for (int i = 0; i < settings.count(); i++)
		{
			if (settings[i][0] == '#' || settings[i][0] == '\'')
				continue;

			CString name = settings[i].copy(0, settings[i].find('=')).trim();
			CString value = settings[i].copy(settings[i].find('=') + 1).trim();

			if (name == "password")
				serverhq_pass = value;
			else if (name == "level")
				serverhq_level = atoi(value.text());
		}
	}
	else
		return false;

	return true;
}

bool loadSettings(char* pFile)
{
	CString prevName = listServerFields[0];
	CString prevDesc = listServerFields[1];
	CString prevLang = listServerFields[2];
	CString prevURL = listServerFields[4];
	CString prevIP = listServerFields[5];
	CString prevLIP = localip;
	CString prevPort = serverPort;

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
	staffList.load(findKey("staff"), ",", true);
	statusList.load(findKey("playerlisticons", "Online,Away,DND"), ",");

	/* BOOL Server-Options */
	apSystem = CHECK_BOOL(findKey("apsystem", "true"));
	baddyDropItems = CHECK_BOOL(findKey("baddyitems", "false"));
	bushesDrop = CHECK_BOOL(findKey("bushitems", "true"));
	cheatwindowsban = CHECK_BOOL(findKey("cheatwindowsban", "false"));
	clientsidePushPull = CHECK_BOOL(findKey("clientsidepushpull", "true"));
	defaultweapons = CHECK_BOOL(findKey("defaultweapons", "true"));
	detailedconsole = CHECK_BOOL(findKey("detailedconsole", "false"));
	dontaddserverflags = CHECK_BOOL(findKey("dontaddserverflags", "false"));
	dontchangekills = CHECK_BOOL(findKey("dontchangekills", "false"));
	dropItemsDead = CHECK_BOOL(findKey("dropitemsdead", "true"));
	globalGuilds = CHECK_BOOL(findKey("globalguilds", "true"));
	healswords = CHECK_BOOL(findKey("healswords", "false"));
	idleDisconnect = CHECK_BOOL(findKey("disconnectifnotmoved", "true"));
	noExplosions = CHECK_BOOL(findKey("noexplosions", "false"));
	noFoldersConfig = CHECK_BOOL(findKey("nofoldersconfig", "false"));
	putnpcenabled = CHECK_BOOL(findKey("putnpcenabled", "true"));
	adminCanChangeGralat = CHECK_BOOL(findKey("normaladminscanchangegralats", "true"));
	setbodyallowed = CHECK_BOOL(findKey("setbodyallowed", "true"));
	setheadallowed = CHECK_BOOL(findKey("setheadallowed", "true"));
	setswordallowed = CHECK_BOOL(findKey("setswordallowed", "true"));
	setshieldallowed = CHECK_BOOL(findKey("setshieldallowed", "true"));
	showConsolePackets = CHECK_BOOL(findKey("showconsolepackets", "false"));
	staffOnly = CHECK_BOOL(findKey("onlystaff", "false"));
	vasesDrop = CHECK_BOOL(findKey("vasesdrop", "true"));
	warptoforall = CHECK_BOOL(findKey("warptoforall", "false"));
	oldcreated = CHECK_BOOL(findKey("oldcreated", "false"));

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
	mindeathgralats = atoi(findKey("mindeathgralats","1"));
	maxdeathgralats = atoi(findKey("maxdeathgralats","50"));
	shieldLimit = atoi(findKey("shieldlimit", "3"));
	swordLimit = CLIP(atoi(findKey("swordlimit", "4")), -25, 25);
	tiledroprate = CLIP(atoi(findKey("tiledroprate", "50")), 0, 100);
	tileRespawn = atoi(findKey("respawntime"));
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
	localip = findKey("localip", "AUTO");
	serverPort = findKey("serverport", "14802");
	shareFolder = findKey("sharefolder");
	staffHead = findKey("staffhead", "head25.png");
	worldName = findKey("worldname", "main");

	// Send changes to the serverlist.
	if (prevDesc != listServerFields[1])
		ListServer_Send(CPacket() << (char)SLSDESC << listServerFields[1] << "\n");
	if (prevLang != listServerFields[2])
		ListServer_Send(CPacket() << (char)SLSLANG << listServerFields[2] << "\n");
	if (prevURL != listServerFields[4])
		ListServer_Send(CPacket() << (char)SLSURL << listServerFields[4] << "\n");
	if (prevIP != listServerFields[5])
		ListServer_Send(CPacket() << (char)SLSIP << listServerFields[5] << "\n");
	if (prevLIP != localip)
		ListServer_Send(CPacket() << (char)SLSSETLOCALIP << localip << "\n");
	if (prevPort != serverPort)
		ListServer_Send(CPacket() << (char)SLSPORT << serverPort << "\n");

	/* Load Maps */
	for(int i = 0; i < CMap::mapList.count(); i++)
		delete((CMap*)CMap::mapList[i]);
	CMap::mapList.clear();
	for(int i = 0; i < mapNames.count(); i++)
		CMap::openMap(mapNames[i].trim());

	// Now fix all the levels.
	for ( int i = 0; i < levelList.count(); ++i )
	{
		CLevel* level = (CLevel*)levelList[i];

		//Find our map id
		level->map = 0;
		for ( int j = 0; j < CMap::mapList.count(); ++j )
		{
			CMap* m = (CMap*)CMap::mapList[j];
			if ( (level->levelIndex = m->getLevelpos(level->fileName)) >= 0 )
			{
				level->map = m;
				break;
			}
		}
	}

	// In case we turned off folders config, get the new sub dirs.
	getSubDirs();

	return true;
}

void getSubDirs()
{
	// If foldersconfig.txt is turned off, use the old style.
	if ( noFoldersConfig )
	{
		subDirs.clear();
		getSubDirs_os( dataDir.text() );
		if ( shareFolder.length() > 1 )
			getSubDirs_os( shareFolder.text() );
	}
	else
	{
		subDirs.clear();
		subDirs.add(dataDir);
		for (int i = 0; i < folderConfig.count(); i++)
		{
			if (folderConfig[i][0] == '#')
				continue;

			CBuffer fmask, fname;

			// Get rid of all \t and replace with ' '.
			// Also, trim.
			folderConfig[i].replaceAll( "\t", " " );

			// Read past the identifier.
			folderConfig[i].setRead(folderConfig[i].find(' '));

			// Read the mask
			CBuffer temp = folderConfig[i].readString( "" );
			temp.trim();

			// If it starts with ./, use . instead of world/ as the search dir.
			if ( temp.find( "./" ) == 0 )
				fmask = CBuffer() << programDir << temp;
			else
				fmask = CBuffer() << dataDir << temp;

			// Pull off the file mask and only save the directory.
			fname = CBuffer() << fmask.readChars(fmask.findl(fSep[0])) << fSep;
			if (subDirs.find(fname) == -1)
				subDirs.add(fname);
		}
	}
}

#ifdef WIN32
void getSubDirs_os(char *pDir)
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
					getSubDirs_os(directory.text());
				}
			}
		} while (FindNextFile(hFind, &filedata));
	}
	FindClose(hFind);
}
#else
void getSubDirs_os(char *pDir)
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
				getSubDirs_os(directory.text());
		}
	}
	closedir(dir);
}
#endif

#if defined(WIN32)
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
#elif defined(PSPSDK)
	void getSubFiles(char* pDir, CStringList& pOut, CString* search)
	{
		SceUID dir;
		if ((dir = sceIoDopen(pDir)) <= 0)
			return;

		SceIoDirent ent;
		SceIoStat statx;

		while (sceIoDread(dir, &ent) > 0)
		{
			CString fullName = CString() << pDir << ent.d_name;
			sceIoGetstat(fullName.text(), &statx);
			if (!(statx.st_mode & S_IFDIR))
			{
				if (search != 0)
				{
					CString s( *search );
					CString m( ent.d_name );
					s.replaceAll( "%", "*" );
					s << ".txt";
					if (m.match( s.text()) == false) continue;
				}
				pOut.add( ent.d_name );
			}
		}

		sceIoDclose(dir);
	}
#else
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

time_t getFileModTime(char* pFile)
{
	struct stat fileStat;
	if(strlen(pFile) <=0)
		return 0;
	if(stat(pFile, &fileStat) != -1)
		return (time_t)fileStat.st_mtime;
	return 0;
}

bool setFileModTime(char* pFile, time_t modTime)
{
	// Get the full path to the file.
	if (pFile == 0) return false;

	// Set the times.
	struct utimbuf ut;
	ut.actime = modTime;
	ut.modtime = modTime;

	// Change the file.
	if (utime(pFile, &ut) == 0) return true;
	return false;
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
	for(int i = 2; i < playerIds.count(); i++)
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

CPlayer* findRCId(CString pAccountName)
{
	for(int i = 0; i < playerList.count(); i++)
	{
		CPlayer* other = (CPlayer*)playerList[i];
		if(stricmp(other->accountName.text(), pAccountName.text()) == 0)
		{
			if(other->type == CLIENTRC)
				return other;
		}
	}

	return 0;
}

bool isValidFile(CBuffer& file, int type)
{
	for ( int i = 0; i < folderConfig.count(); ++i )
	{
		folderConfig[i].setRead(0);
		CString ftype( folderConfig[i].readString( " " ) );
		CString fmask = CBuffer() << dataDir <<
			CBuffer(folderConfig[i].readString( "" )).trim().text();
		folderConfig[i].setRead(0);

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

			case -1:	// Any
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

void shutdownServer( int signal )
{
	if(hasShutdown)
		return;
	hasShutdown = true;
	serverRunning = false;
	serverSock.disconnect();
	saveWeapons("weapons.txt");
	serverFlags.save("serverflags.txt");
	for(int i = playerList.count()-1; i >= 0; i--)
		delete ((CPlayer*)playerList[i]);

	if(lsConnected)
		ListServer_End();

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

	errorOut("serverlog.txt", "Server shutdown.");

	#ifdef PSPSDK
		sceKernelExitGame();
	#endif
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
	if ( file )
	{
		fprintf(file, "[%s] %s\r\n", getTimeStr(0).text(), pError.text());
		fclose(file);
	}
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
