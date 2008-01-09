// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "sqlite3.h"
#include "CPacket.h"
#include "CStringList.h"
#include "CPlayer.h"
#include "CAccount.h"
extern sqlite3 *gserverDB;

CString escapeStr(CString& pInput);
bool openDatabase(char* pFileName);
CPacket getProfile(CString& pPlayerName);
CPacket getAccountList(CString& pName, CString& pCondition);
bool doQuery(CString& pQuery);
int processQuery(void *pParam, int argc, char **argv, char **azColName);
void setProfile(CString& pAccountName, CPacket& pProfileData);

bool removeGuildMember(CString& pGuildName, CString& pPlayerName);
bool guildExists(CString& pGuildName);
bool guildMemberExists(CString& pGuildName, CString& pAccountName, CString& pNickName);
bool addGuildMember(CString& pGuildName, CString& pPlayerName);
bool addGuild(CString& pGuildName);
bool removeGuild(CString& pGuildName);
