/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include "CDatabase.h"
#include "main.h"

/*
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
		if(sqlite3_exec(gserverDB, query.text(), processQuery, &result, &zErrMsg)
			|| result.count() <= 0)
			retVal << (char)pPlayerName.length() << pPlayerName << "		 ";
		else
		{
			retVal << (char)pPlayerName.length() << pPlayerName;
			for(int i = 1; i < result.count(); i++)
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
		query << " realname='" << items[0].escapeStr() << "',";
		query << " age=" << items[1].escapeStr() << ",";
		query << " sex='" << items[2].escapeStr() << "',";
		query << " country='" << items[3].escapeStr() << "',";
		query << " icq='" << items[4].escapeStr() << "',";
		query << " email='" << items[5].escapeStr() << "',";
		query << " webpage='" << items[6].escapeStr() << "',";
		query << " favhangout='" << items[7].escapeStr() << "',";
		query << " favquote='" << items[8].escapeStr() << "'";
		query << " WHERE accname LIKE '" << pAccountName << "'";
		if(sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg))
			printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
		return;
	}

	bool doQuery(CString& pQuery)
	{
		char *zErrMsg =0;
		if(sqlite3_exec(gserverDB, pQuery.text(), NULL, NULL, &zErrMsg))
		{
			printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
			return false;
		}
		return true;
	}
*/

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

CPacket getAccountList(CString& pName, CString& pCondition)
{
	CPacket retVal;
	CStringList fileList;
	if ( pName.length() > 0 )
		getSubFiles("accounts/", fileList, &pName);
	else
		getSubFiles("accounts/", fileList);

	for (int i = 0; i < fileList.count(); i++)
	{
		CString acc = fileList[i].copy(0, fileList[i].length() - 4);
		if (acc.length() < 1)
			continue;
		if ( pCondition.length() > 0 )
		{
			if ( CAccount::meetsConditions( acc, pCondition ) == true )
				retVal << (char)acc.length() << acc;
		} else retVal << (char)acc.length() << acc;
	}

	return retVal;
}
