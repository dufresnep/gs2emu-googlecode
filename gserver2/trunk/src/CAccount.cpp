// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CAccount.h"
#include "CDatabase.h"

#include "CDatabase.h"
#include "CIni.h"
#include "main.h"

CAccount::CAccount()
{
    banned = loadOnly = false;
    adminLevel = adminRights = 0;
    adminIp = "0.0.0.0";
}

bool CAccount::loadDBAccount(CString& pAccount)
{
	CString query;
	CStringList words;
	char *zErrMsg =0;
	query << "SELECT accname, banned, onlyload, adminrights, adminlevel, comments, adminip"
		" FROM accounts WHERE accname LIKE '" << pAccount << "'";
	if(sqlite3_exec(gserverDB, query.text(), processQuery, &words, &zErrMsg))
	{
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
		return false;
	}

	if(words.count() <= 6)
		return false;

	accountName = words[0];
	banned = (atoi(words[1].text())>0);
	loadOnly = (atoi(words[2].text())>0);
	adminRights = atoi(words[3].text());
	adminLevel = atoi(words[4].text());
	comments = words[5];
	adminIp = words[6];
	return true;
}

bool CAccount::loadWorldProps(CString& pAccount)
{
	CString query;
	CStringList words;
	char *zErrMsg =0;
	query << "SELECT * FROM " << worldName;
	query << " WHERE accname LIKE '" << pAccount << "'";
	if(showQuery)
	printf("QUERY: %s\n", query.text());
	if(sqlite3_exec(gserverDB, query.text(), processQuery, &words, &zErrMsg))
	{
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
		return false;
	}

	if(words.count() <= 31)
		return false;

	accountName = words[0];
	nickName = words[1];
    x = (float)atof(words[2].text());
    y = (float)atof(words[3].text());
    levelName = words[4];
    maxPower = CLIP((float)atof(words[5].text()), 0.0f, (float)heartLimit);
    power = CLIP((float)atof(words[6].text()), 0.0f, maxPower);
    rubins = atoi(words[7].text());
    darts = atoi(words[8].text());
    bombCount = atoi(words[9].text());
    glovePower = atoi(words[10].text());
    swordPower = CLIP(atoi(words[11].text()), 0, swordLimit);
    shieldPower = CLIP(atoi(words[12].text()), 0, shieldLimit);
    headImage = words[13];
    bodyImage = words[14];
    swordImage = words[15];
    shieldImage = words[16];
    CString colorStr = words[17].text();
    for (int i = 0; i < colorStr.length() && i < 5; i++)
        colors[i] = colorStr[i]-'a';
    sprite = atoi(words[18].text());
    status = atoi(words[19].text());
    horseImage = words[20];
    horseBushes = atoi(words[21].text());
    magicPoints = atoi(words[22].text());
    kills = atoi(words[23].text());
    deaths = atoi(words[24].text());
    onlineSecs = atoi(words[25].text());
    lastIp = atoi(words[26].text());
    ap = atoi(words[27].text());
    myWeapons.load(words[28].text(), ",");
    myChests.load(words[29].text(), ",");
    myFlags.load(words[30].text(), "§");
    apCounter = atoi(words[31].text());
	return true;
}

bool CAccount::loadWorldPropsIni()
{
	CIni ini;
    if (!ini.load("newaccount.ini"))
        return false;

    nickName = ini.readString("main", "nickName", "Unknown");
    x = (float)ini.readReal("main", "x", 32);
    y = (float)ini.readReal("main", "y", 32);
    levelName = ini.readString("main", "level", "startlevel.graal");
    maxPower = (float)ini.readReal("main", "maxHP", 3);
    power = (float)ini.readReal("main", "hp", maxPower);
    rubins = (int)ini.readReal("main", "rupees", 0);
    darts = (int)ini.readReal("main", "arrows", 10);
    bombCount = (int)ini.readReal("main", "bombs", 10);
    glovePower = (int)ini.readReal("main", "glovePower", 0);
    swordPower = (int)ini.readReal("main", "swordPower", 0);
    shieldPower = (int)ini.readReal("main", "shieldPower", 0);
    headImage = ini.readString("main", "headImage", "head0.png");
    bodyImage = ini.readString("main", "bodyImage", "body.png");
    swordImage = ini.readString("main", "swordImage", "sword0.png");
    shieldImage = ini.readString("main", "shieldImage", "shield0.png");
    CString colorStr = ini.readString("main", "colors", "cakes");
    for (int i = 0; i < colorStr.length() && i < 5; i++)
        colors[i] = (int)colorStr[i]-'a';

    sprite = (int)ini.readReal("main", "sprite", 0);
    status = (int)ini.readReal("main", "status", 0);
    horseImage = ini.readString("main", "horseImage", "");
    horseBushes = (int)ini.readReal("main", "horseBushes", 0);
    magicPoints = (int)ini.readReal("main", "magic", 0);
    kills = (int)ini.readReal("main", "kills", 0);
    deaths = (int)ini.readReal("main", "deaths", 0);
    onlineSecs = (int)ini.readReal("main", "onlineTime", 0);
    lastIp = (int)ini.readReal("main", "lastIp", 0);
    ap = (int)ini.readReal("main", "alignment", 50);
    myWeapons.load(ini.readString("main", "weapons", ""), ",");
    myChests.load(ini.readString("main", "chests", ""), ",");
    myFlags.load(ini.readString("main", "flags", ""), "§");
    apCounter = (int)ini.readReal("main", "apCounter", 0);
    return true;
}

void CAccount::saveAccount()
{
	CString query;
	char *zErrMsg =0;
	CAccount account;
	if(!account.loadDBAccount(accountName))
	{
        query = "INSERT INTO accounts (accname)";
        query << " VALUES('" << accountName << "')";
        sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg);
	}
	query = "UPDATE accounts SET";
	query << " banned=" << toString((int)banned) << ",";
	query << " onlyload=" << toString((int)loadOnly) << ",";
	query << " adminrights=" << toString(adminRights) << ",";
	query << " adminlevel=" << toString(adminLevel) << ",";
	query << " comments='" << escapeStr(comments) << "',";
	query << " adminip='" << adminIp << "'";
	query << " WHERE accname LIKE '" << accountName << "'";
	if(showQuery)
	printf("QUERY: %s\n", query.text());
	if(sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg))
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
	return;
}

void CAccount::saveWorldProps()
{
	CString query;
	char *zErrMsg =0;
	if(loadOnly)
        return;
    CAccount test;
    if(!test.loadWorldProps(accountName))
    {
        query << "INSERT INTO " << worldName << " (accname) VALUES('" << accountName  << "')";
        sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg);
    }

	query = "";
	query << "UPDATE " << worldName << " SET ";
	query << " nickname='" << escapeStr(nickName) << "',";
	query << " x=" << toString(x) << ",";
	query << " y=" << toString(y) << ",";
	query << " level='" << escapeStr(levelName) << "',";
	query << " maxhp=" << toString(maxPower) << ",";
	query << " hp=" << toString(power) << ",";
	query << " rupees=" << toString(rubins) << ",";
	query << " arrows=" << toString(darts) << ",";
	query << " bombs=" << toString(bombCount) << ",";
	query << " glovepower=" << toString(glovePower) << ",";
	query << " swordpower=" << toString(swordPower) << ",";
	query << " shieldpower=" << toString(shieldPower) << ",";
	query << " headimg='" << escapeStr(headImage) << "',";
	query << " bodyimg='" << escapeStr(bodyImage) << "',";
	query << " swordimg='" << escapeStr(swordImage) << "',";
	query << " shieldimg='" << escapeStr(shieldImage) << "',";
	CString colorStr;
	for (int i=0; i<5; i++)
		colorStr.writeChar(colors[i]+'a');
	query << " colors='" << escapeStr(colorStr) << "',";
	query << " sprite=" << toString(sprite) << ",";
	query << " status=" << toString(status) << ",";
	query << " horseimg='" << escapeStr(horseImage) << "',";
	query << " horsebushes=" << toString(horseBushes) << ",";
	query << " magic=" << toString(magicPoints) << ",";
	query << " kills=" << toString(kills) << ",";
	query << " deaths=" << toString(deaths) << ",";
	query << " onlinetime=" << toString(onlineSecs) << ",";
	query << " lastip=" << toString(lastIp) << ",";
	query << " alignment=" << toString(ap) << ",";
	query << " weapons='" << myWeapons.join(",") << "',";
	query << " chests='" << myChests.join(",") << "',";
	query << " flags='" << myFlags.join("§") << "',";
	query << " apcounter=" << toString(apCounter);

	query << " WHERE accname LIKE '" << accountName << "'";
	if(showQuery)
	printf("QUERY: %s\n", query.text());
	if(sqlite3_exec(gserverDB, query.text(), NULL, NULL, &zErrMsg))
		printf("SQL Error: %s\n", sqlite3_errmsg(gserverDB));
	return;
}
