// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CDatabase.h"
#include "main.h"

sqlite3 *gserverDB;
bool profileExists(CString& pPlayerName);
int processQuery(void *pParam, int argc, char **argv, char **azColName)
{
	CStringList& list = *(CStringList*)pParam;
	for(int i = 0; i < argc; i++)
		list.add(argv[i]);
	return 0;
}

bool openDatabase(char* pFileName)
{
	int result;
	result = sqlite3_open(pFileName, &gserverDB);
	if(result)
		return false;
	return true;
}
bool profileExists(CString& pPlayerName)
{
	CStringList result;
	char *zErrMsg = 0;
	CString query = "SELECT accname FROM profiles WHERE accname LIKE ";
	query << "'" << pPlayerName << "'";
	if(sqlite3_exec(gserverDB, query.text(), processQuery, &result, &zErrMsg)
		|| result.count() <= 0)
        return false;
	return true;
}

CPacket getProfile(CString& pPlayerName)
{
	CPacket retVal;
	CStringList result;
	char *zErrMsg = 0;
	CString query = "SELECT * FROM profiles WHERE accname LIKE ";
	query << "'" << pPlayerName << "'";
	if(showQuery)
    printf("QUERY: %s\n", query.text());
	if(sqlite3_exec(gserverDB, query.text(), processQuery, &result, &zErrMsg)
		|| result.count() <= 0)
		retVal << (char)pPlayerName.length() << pPlayerName << "         ";
	else
	{
	    retVal << (char)pPlayerName.length() << pPlayerName;
		for(int i = 1; i < result.count(); i++)
			retVal << (char)result[i].length() << result[i];
	}
	if(showQuery)
	printf("RETVAL: %s\n", retVal.text());
	return retVal;
}
CPacket getAccountList(CString& pName, CString& pCondition)
{
	CPacket retVal;
	CStringList result;
	CString query;
	char *zErrMsg = 0;
	if(!pName.length())
		pName = "%";
	query << "SELECT accname"
		" FROM accounts WHERE accname LIKE '" << pName << "'";
	if(pCondition.length())
		query << " and " << pCondition;
	query << " LIMIT 0, 5000";
	if(sqlite3_exec(gserverDB, query.text(), processQuery, &result, &zErrMsg)
		|| result.count() <= 0)
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
	else
	{
		for(int i = 0; i < result.count(); i++)
			retVal << (char)result[i].length() << result[i];
	}
	return retVal;
}
void setProfile(CString& pAccountName, CPacket& pProfileData)
{
	CString query;
	CStringList items;
	char *zErrMsg =0;
	if(!profileExists(pAccountName))
    {
        query = "INSERT INTO profiles (accname)";
        query << " VALUES('" << pAccountName << "')";
        sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg);
    }

	for(int i = 0; i < 9; i++)
		items.add(pProfileData.readChars(pProfileData.readByte1()));

	if(items.count() <= 8)
		return;

	query = "UPDATE profiles SET";
	query << " realname='" << escapeStr(items[0]) << "',";
	query << " age=" << escapeStr(items[1]) << ",";
	query << " sex='" << escapeStr(items[2]) << "',";
	query << " country='" << escapeStr(items[3]) << "',";
	query << " icq='" << escapeStr(items[4]) << "',";
	query << " email='" << escapeStr(items[5]) << "',";
	query << " webpage='" << escapeStr(items[6]) << "',";
	query << " favhangout='" << escapeStr(items[7]) << "',";
	query << " favquote='" << escapeStr(items[8]) << "'";
	query << " WHERE accname LIKE '" << pAccountName << "'";
	if(showQuery)
	printf("QUERY: %s\n", query.text());
	if(sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg))
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
	return;
}

bool doQuery(CString& pQuery)
{
	char *zErrMsg =0;
	if(showQuery)
	printf("QUERY: %s\n", pQuery.text());
	if(sqlite3_exec(gserverDB, pQuery.text(), NULL, NULL, &zErrMsg))
	{
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
		return false;
	}
	return true;
}

bool removeGuildMember(CString& pGuildName, CString& pPlayerName)
{
	CString guildName = pGuildName;
	guildName.replaceAll(" ", "_");
	CString path = CString() << "guilds" << fSep << "guild" << guildName << ".txt";
    CStringList guildPlayers;
    if(!guildPlayers.load(path.text()))
        return false;
    int pos = guildPlayers.findI(pPlayerName);
    if(pos >= 0)
        guildPlayers.remove(pos);
    guildPlayers.save(path.text());
    return true;
}

bool guildExists(CString& pGuildName)
{
    CString tmp;
	CString guildName = pGuildName;
	guildName.replaceAll(" ", "_");
    CString path = CString() << "guilds" << fSep << "guild" << guildName << ".txt";
    return tmp.load(path.text());
}

bool guildMemberExists(CString& pGuildName, CString& pAccountName, CString& pNickName)
{
    CStringList guildPlayers;
	CString guildName = pGuildName;
	guildName.replaceAll(" ", "_");
	CString path = CString() << "guilds" << fSep << "guild" << guildName << ".txt";
    if(!guildPlayers.load(path.text()))
        return false;

	if (guildPlayers[0] == "TYPE=NICK")
	{
		CString find = CString() << pAccountName << ":" << pNickName;
		for (int i = 1; i < guildPlayers.count(); i++)
		{
			if (guildPlayers[i] == find)
				return true;
		}

		return false;
	}

	return (guildPlayers.findI(pAccountName) >= 0);
}

bool addGuildMember(CString& pGuildName, CString& pPlayerName)
{
    CStringList guildPlayers;
	CString guildName = pGuildName;
	guildName.replaceAll(" ", "_");
	CString path = CString() << "guilds" << fSep << "guild" << guildName << ".txt";
    if(!guildPlayers.load(path.text()))
        return false;

    if(guildPlayers.findI(pPlayerName) < 0)
        guildPlayers.add(pPlayerName);
    guildPlayers.save(path.text());
    return true;
}

bool addGuild(CString& pGuildName)
{
    FILE* file;
    CString guildName = pGuildName;
	guildName.replaceAll(" ", "_");
	CString path = CString() << "guilds" << fSep << "guild" << guildName << ".txt";
    if((file = fopen(path.text(), "r")) != 0)
    {
        fclose(file);
        return false;
    }
    if((file = fopen(path.text(), "w")) != 0)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool removeGuild(CString& pGuildName)
{
	CString guildName = pGuildName;
	guildName.replaceAll(" ", "_");
    CString path = CString() << "guilds" << fSep << "guild" << guildName << ".txt";
    if(remove(path.text()) != 0)
        return false;
	return true;
}

CString escapeStr(CString& pInput)
{
    CString retVal;
    for(int i = 0; i < pInput.length(); i++)
    {
        char letter = pInput[i];
        if(letter == '\'')
            retVal << "\'\'";
        else
            retVal.writeChar(letter);
    }
    return retVal;
}


