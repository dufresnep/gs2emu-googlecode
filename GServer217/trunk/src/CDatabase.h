/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include "CPacket.h"
#include "CStringList.h"
#include "CPlayer.h"
#include "CAccount.h"

#ifdef STORE_SQL
	extern sqlite3 *gserverDB;

	bool openDatabase(char* pFileName);
	CPacket getProfile(CString& pPlayerName);
	bool doQuery(CString& pQuery);
	int processQuery(void *pParam, int argc, char **argv, char **azColName);
	void setProfile(CString& pAccountName, CPacket& pProfileData);
#else

#endif

bool removeGuildMember(CString& pGuildName, CString& pPlayerName);
bool guildExists(CString& pGuildName);
bool guildMemberExists(CString& pGuildName, CString& pAccountName, CString& pNickName);
bool addGuildMember(CString& pGuildName, CString& pPlayerName);
bool addGuild(CString& pGuildName);
bool removeGuild(CString& pGuildName);
CPacket getAccountList(CString& pName, CString& pCondition);
