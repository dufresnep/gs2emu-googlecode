// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef MAINH
#define MAINH
#include "CList.h"
#include "CListServer.h"
#include "CString.h"
#include "CStringList.h"
#include "CNpc.h"
#include "CPlayer.h"
#include "CWordFilter.h"
#include <dir.h>
#include <time.h>

#define GSERVER_REVISION 52
#define STORE_SQL false

extern bool apSystem, bushesDrop, cheatwindowsban, dontaddserverflags, dontchangekills, dropItemsDead, globalGuilds, lsConnected, noExplosions, serverRunning, setbodyallowed, setheadallowed, setswordallowed, setshieldallowed, showConsolePackets, showQuery, staffOnly, vasesDrop, warptoforall;
extern char fSep[], dataDir[];
extern CLevel *NOLEVEL;
extern CList newPlayers, playerList, playerIds, settingList, weaponList, npcList, npcIds, levelList;
extern CString listServerFields[6], serverMessage, staffHead, unstickmeLevel, worldName, rev;
extern CStringList adminNames, cheatwindows, clothCommands, colourNames, globalGuildList, jailLevels, mapNames, profileList, RCBans, RCMessage, RCHelpMessage, serverFlags, staffGuilds, staffList, statusList, subDirs;
extern float unstickmeX, unstickmeY;
extern int aptime[5], baddyRespawn, cheatwindowstime, gameTime, heartLimit, horseLife, maxNoMovement, maxPlayers, nwTime, serverPort, shieldLimit, swordLimit, tileRespawn;
extern CWordFilter WordFilter;

bool isIpBanned(CString& pIp);
bool loadSettings(char* pFile);
bool loadWeapons(char* pFile);
bool updateFile(char *pFile);
char *findKey(CString pName, char *pDefault = NULL);
char *getDataFile(char* pFile);
char *removeGifExtension(CString& pFileName);
CPacket listFiles(char *pDir, char *pRights);
CPlayer* findPlayerId(CString pAccountName, bool pOnly = false);
CString getFileExtension(CString& pFileName);
CString getTimeStr(int pType = 0);
int createPlayerId(CPlayer* pPlayer);
int createNpcId(CNpc* pNpc);
int getFileSize(char* pFile);
long int getFileModTime(char* pFile);
void CalculateMD5(char *buffer, int length, char *checksum);
void errorOut(char *pFile, CBuffer pError, bool pWrite = false);
void getSubDirs(char* dir);
void getSubFiles(char* pDir, CStringList& pOut);
void loadServerMessage();
void saveWeapons(char* pFile);
void sendRCPacket(CPacket& pPacket);
void shutdownServer();

inline char* removeGifExtension(char* pFileName)
{
    CString tmp = pFileName;
    return removeGifExtension(tmp);
}

inline int getTime()
{
    return gameTime;
}

inline int getNWTime()
{
	return ((int)time(NULL) - 11078 * 24 * 60 * 60) * 2 / 10;
}

inline char* getMd5(char* pString)
{
	static char buffer[50];
	CalculateMD5(pString, (int)strlen(pString), buffer);
	return buffer;
}

/* Didn't feel like flooding main.cpp anymore <.<. */
class SettingKey
{
	public:
		SettingKey();
		~SettingKey();

		CString name, value;
};

#endif // MAINH
