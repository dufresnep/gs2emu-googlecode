// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef MAINH
#define MAINH
#include "CList.h"
#include "CString.h"
#include "CStringList.h"
#include "CPlayer.h"
#include "CNpc.h"
#include <time.h>

extern bool apSystem, bushesDrop, dropItemsDead, globalGuilds, lsConnected, noExplosions, serverRunning, showConsolePackets, showQuery, staffOnly, vasesDrop;
extern char fSep[], dataDir[];
extern CLevel* NOLEVEL;
extern CList playerList, playerIds, newPlayers, weaponList, npcList, npcIds, levelList;
extern CSocket listServer;
extern CString databaseFile, listServerFields[6], serverMessage, staffHead, unstickmeLevel, worldName, rev;
extern CStringList clothCommands, colourNames, globalGuildList, jailLevels, mapNames, profileList, RCBans, rcFolders, RCMessage, RCHelpMessage, serverFlags, staffGuilds, staffList, statusList, subDirs;
extern float unstickmeX, unstickmeY;
extern int apInterval, baddyRespawn, gameTime, heartLimit, horseLife, maxNoMovement, maxPlayers, nwTime, serverPort, shieldLimit, swordLimit, tileRespawn;

bool isIpBanned(CString& pIp);
bool loadWeapons(char* pFile);
char* getDataFile(char* pFile);
char* removeGifExtension(CString& pFileName);
CPacket listFiles(char* pDir);
CPlayer* findPlayerId(CString& pAccountName);
CString getFileExtension(CString& pFileName);
int createPlayerId(CPlayer* pPlayer);
int createNpcId(CNpc* pNpc);
int getFileSize(char* pFile);
long long getFileModTime(char* pFile);
void CalculateMD5(char *buffer, int length, char *checksum);
void errorOut(char* pError);
void getSubDirs(char* dir);
void getSubFiles(char* pDir, CStringList& pOut);
void loadServerMessage();
void saveWeapons(char* pFile);
void sendRCPacket(CPacket& pPacket);
void shutdownServer();

inline void errorOut(CString& pError)
{
	errorOut(pError.text());
}

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

enum
{
	SLSNAME,
	SLSDESC,
	SLSLANG,
	SLSVER,
	SLSURL,
	SLSIP,
	SLSPORT,
	SLSCOUNT,
	SLVACCOUNT,
	SLVGUILD,
	SLVFILE,
	SLVNICKNAME
};

enum
{
	GSVACCOUNT,
	GSVGUILD,
	GSVFILEC,
	GSVFILED,
	GSVFILES,
    GSVOLD,
    GSVCURRENT
};

#endif // MAINH
