/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#include "CAccount.h"
#include "CDatabase.h"
#include "CIni.h"
#include "main.h"

CAccount::CAccount()
: banned(false), ftpOn(false), loadOnly(false), lastIp(0), onlineSecs(0),
adminRights(0), sprite(2), status(20), type(CLIENTPLAYER),
bombPower(1), glovePower(1), shieldPower(1), swordPower(1),
deaths(0), kills(0), horseBushes(0), ap(50), apCounter(100),
magicPoints(0), rubins(0), bombs(5), darts(10),
x(32.0f), y(32.0f), z(0.0f), power(3.0f), maxPower(3.0f),
rating(1500.0f), deviation(350.0f), oldDeviation(350.0f), lastSparTime(0)
{
	adminIp = "0.0.0.0";
	memset(colors, 0, sizeof(colors));

	levelName = "";
	gAni = "idle";
	nickName = "unknown";
	headImage = "head0.png";
	bodyImage = "body.png";
	shieldImage = "shield1.png";
	swordImage = "sword1.png";
	language = "English";
}

bool CAccount::loadDBAccount(CString pAccount, bool fromAccount)
{
	CStringList file;
	CBuffer fileName;

	// File loading logic.
	if ( fromAccount == true )
	{
		accountName = pAccount;
		fileName = CString() << "accounts" << fSep << accountName << ".txt";
	}
	else
		fileName = CString() << "accounts" << fSep << pAccount << ".txt";

	// Load and check if the file is valid.
	if (!file.load(fileName.text()) || file[0] != "GRACC001")
		return false;

	for (int i = 0; i < file.count(); i++)
	{
		int sep = file[i].find(' ');
		CBuffer section = file[i].copy(0, sep);
		CBuffer val = file[i].copy(sep + 1);
		section.trim();
		val.trim();

		if (section == "NAME") continue; //accountName = val;
		else if (section == "NICK") continue; //nickName = val;
		else if (section == "LEVEL") levelName = val;
		else if (section == "X") x = (float)atof(val.text());
		else if (section == "Y") y = (float)atof(val.text());
		else if (section == "MAXHP") maxPower = (float)atoi(val.text());
		else if (section == "HP") power = (float)atoi(val.text());
		else if (section == "RUPEES") rubins = atoi(val.text());
		else if (section == "ANI") gAni = val;
		else if (section == "ARROWS") darts = atoi(val.text());
		else if (section == "BOMBS") bombs = atoi(val.text());
		else if (section == "GLOVEP") glovePower = atoi(val.text());
		else if (section == "SHIELDP") shieldPower = atoi(val.text());
		else if (section == "SWORDP") swordPower = atoi(val.text());
		else if (section == "HEAD") headImage = val;
		else if (section == "BODY") bodyImage = val;
		else if (section == "SWORD") swordImage = val;
		else if (section == "SHIELD") shieldImage = val;
		else if (section == "COLORS") { CStringList t; t.load(val.text(), ","); for (int i = 0; i < t.count() && i < 5; i++) colors[i] = atoi(t[i].text()); }
		else if (section == "SPRITE") sprite = atoi(val.text());
		else if (section == "STATUS") status = atoi(val.text());
		else if (section == "MP") magicPoints = atoi(val.text());
		else if (section == "AP") ap = atoi(val.text());
		else if (section == "APCOUNTER") apCounter = atoi(val.text());
		else if (section == "ONSECS") onlineSecs = atoi(val.text());
		else if (section == "IP") lastIp = atoi(val.text());
		else if (section == "LANGUAGE") language = val;
		else if (section == "KILLS") kills = atoi(val.text());
		else if (section == "DEATHS") deaths = atoi(val.text());
		else if (section == "RATING") rating = (float)atof(val.text());
		else if (section == "DEVIATION") deviation = (float)atof(val.text());
		else if (section == "OLDDEVIATION") oldDeviation = (float)atof(val.text());
		else if (section == "LASTSPARTIME") lastSparTime = (time_t)atol(val.text());
		else if (section == "FLAG") myFlags.add(val);
		else if (section == "ATTR1") myAttr[0] = val; // could trim these 30 lines into one.. but it'd probably go slower then a simple compare.. who knows.
		else if (section == "ATTR2") myAttr[1] = val;
		else if (section == "ATTR3") myAttr[2] = val;
		else if (section == "ATTR4") myAttr[3] = val;
		else if (section == "ATTR5") myAttr[4] = val;
		else if (section == "ATTR6") myAttr[5] = val;
		else if (section == "ATTR7") myAttr[6] = val;
		else if (section == "ATTR8") myAttr[7] = val;
		else if (section == "ATTR9") myAttr[8] = val;
		else if (section == "ATTR10") myAttr[9] = val;
		else if (section == "ATTR11") myAttr[10] = val;
		else if (section == "ATTR12") myAttr[11] = val;
		else if (section == "ATTR13") myAttr[12] = val;
		else if (section == "ATTR14") myAttr[13] = val;
		else if (section == "ATTR15") myAttr[14] = val;
		else if (section == "ATTR16") myAttr[15] = val;
		else if (section == "ATTR17") myAttr[16] = val;
		else if (section == "ATTR18") myAttr[17] = val;
		else if (section == "ATTR19") myAttr[18] = val;
		else if (section == "ATTR20") myAttr[19] = val;
		else if (section == "ATTR21") myAttr[20] = val;
		else if (section == "ATTR22") myAttr[21] = val;
		else if (section == "ATTR23") myAttr[22] = val;
		else if (section == "ATTR24") myAttr[23] = val;
		else if (section == "ATTR25") myAttr[24] = val;
		else if (section == "ATTR26") myAttr[25] = val;
		else if (section == "ATTR27") myAttr[26] = val;
		else if (section == "ATTR28") myAttr[27] = val;
		else if (section == "ATTR29") myAttr[28] = val;
		else if (section == "ATTR30") myAttr[29] = val;
		else if (section == "WEAPON") myWeapons.add(val);
		else if (section == "CHEST") myChests.add(val);
		else if (section == "BANNED") banned = (atoi(val.text()) == 0) ? false : true;
		else if (section == "BANREASON") banReason = val;
		else if (section == "COMMENTS") comments = val;
		else if (section == "LOCALRIGHTS") adminRights = atoi(val.text());
		else if (section == "IPRANGE") adminIp = val;
		else if (section == "FOLDERRIGHT") myFolders.add(val);
		else if (section == "LASTFOLDER") lastFolder = val;
	}

	// Check for illegal folders
	for (int i = 0; i < myFolders.count(); i++)
	{
		if (myFolders[i].find(':') >= 0 || myFolders[i].find("..") >= 0 || myFolders[i].find(" /*") >= 0)
		{
			myFolders.remove(i);
			i--;
		}
	}

	return true;
}

bool CAccount::loadWorldPropsIni()
{
	// accounts/defaultaccount.txt
	return loadDBAccount( "defaultaccount", false );
}

void CAccount::saveAccount(bool pAttributes)
{
	if (loadOnly || accountName.length() < 1 || type == CLIENTRC)
		return;

	CAccount oldAccount;
	CBuffer fileName = CString() << "accounts" << fSep << accountName << ".txt";
	CBuffer newFile;

	if (pAttributes)
		if ( oldAccount.loadDBAccount(accountName) == false )
			return;

	newFile << "GRACC001\n";
	newFile << "NAME " << accountName << "\n";
	newFile << "NICK " << nickName << "\n";
	newFile << "LEVEL " << (pAttributes ? oldAccount.levelName : levelName) << "\n";
	newFile << "X " << toString((pAttributes ? oldAccount.x : x)) << "\n";
	newFile << "Y " << toString((pAttributes ? oldAccount.y : y)) << "\n";
	newFile << "MAXHP " << toString((pAttributes ? oldAccount.maxPower : maxPower)) << "\n";
	newFile << "HP " << toString((pAttributes ? oldAccount.power : power)) << "\n";
	newFile << "RUPEES " << toString((pAttributes ? oldAccount.rubins : rubins)) << "\n";
	newFile << "ANI " << (pAttributes ? oldAccount.gAni : gAni) << "\n";
	newFile << "ARROWS " << toString((pAttributes ? oldAccount.darts : darts)) << "\n";
	newFile << "BOMBS " << toString((pAttributes ? oldAccount.bombs : bombs)) << "\n";
	newFile << "GLOVEP " << toString((pAttributes ? oldAccount.glovePower : glovePower)) << "\n";
	newFile << "SHIELDP " << toString((pAttributes ? oldAccount.shieldPower : shieldPower)) << "\n";
	newFile << "SWORDP " << toString((pAttributes ? oldAccount.swordPower : swordPower)) << "\n";
	newFile << "HEAD " << (pAttributes ? oldAccount.headImage : headImage) << "\n";
	newFile << "BODY " << (pAttributes ? oldAccount.bodyImage : bodyImage) << "\n";
	newFile << "SWORD " << (pAttributes ? oldAccount.swordImage : swordImage) << "\n";
	newFile << "SHIELD " << (pAttributes ? oldAccount.shieldImage : shieldImage) << "\n";
	newFile << "COLORS " << toString((pAttributes ? oldAccount.colors[0] : colors[0])) << "," << toString((pAttributes ? oldAccount.colors[1] : colors[1])) << "," << toString((pAttributes ? oldAccount.colors[2] : colors[2])) << "," << toString((pAttributes ? oldAccount.colors[3] : colors[3])) << "," << toString((pAttributes ? oldAccount.colors[4] : colors[4])) << "\n";
	newFile << "SPRITE " << toString((pAttributes ? oldAccount.sprite : sprite)) << "\n";
	newFile << "STATUS " << toString((pAttributes ? oldAccount.status : status)) << "\n";
	newFile << "MP " << toString((pAttributes ? oldAccount.magicPoints : magicPoints)) << "\n";
	newFile << "AP " << toString((pAttributes ? oldAccount.ap : ap)) << "\n";
	newFile << "APCOUNTER " << toString((pAttributes ? oldAccount.apCounter : apCounter)) << "\n";
	newFile << "ONSECS " << toString((pAttributes ? oldAccount.onlineSecs : onlineSecs)) << "\n";
	newFile << "IP " << toString(lastIp) << "\n";
	newFile << "LANGUAGE " << (pAttributes ? oldAccount.language : language) << "\n";
	newFile << "KILLS " << toString(kills) << "\n";
	newFile << "DEATHS " << toString(deaths) << "\n";
	newFile << "RATING " << toString(rating) << "\n";
	newFile << "DEVIATION " << toString(deviation) << "\n";
	newFile << "OLDDEVIATION " << toString(oldDeviation) << "\n";
	newFile << "LASTSPARTIME " << toString((unsigned long)lastSparTime) << "\n";
//	newFile << "PLATFORM " << platform << "\n";
//	newFile << "CODEPAGE " << name << "\n";
	for (int i = 0; i < (int)(sizeof((pAttributes ? oldAccount.myAttr : myAttr)) / 20); i++)
		if ((pAttributes ? oldAccount.myAttr[i] : myAttr[i]).length() > 0) newFile << "ATTR" << toString(i + 1) << " " << (pAttributes ? oldAccount.myAttr[i] : myAttr[i]) << "\n";
	for (int i = 0; i < (pAttributes ? oldAccount.myChests : myChests).count(); i++)
		newFile << "CHEST " << (pAttributes ? oldAccount.myChests[i] : myChests[i]) << "\n";
	for (int i = 0; i < (pAttributes ? oldAccount.myWeapons : myWeapons).count(); i++)
		newFile << "WEAPON " << (pAttributes ? oldAccount.myWeapons[i] : myWeapons[i]) << "\n";
	for (int i = 0; i < (pAttributes ? oldAccount.myFlags : myFlags).count(); i++)
		newFile << "FLAG " << (pAttributes ? oldAccount.myFlags[i] : myFlags[i]) << "\n";
	newFile << "\n";
	newFile << "BANNED " << toString(banned) << "\n";
	newFile << "BANREASON " << banReason << "\n";
	newFile << "COMMENTS " << comments << "\n";
	newFile << "LOCALRIGHTS " << toString(adminRights) << "\n";
	newFile << "IPRANGE " << adminIp << "\n";
	for (int i = 0; i < myFolders.count(); i++)
		newFile << "FOLDERRIGHT " << myFolders[i] << "\n";
	newFile << "LASTFOLDER " << lastFolder << "\n";
	newFile.save(fileName.text());
	return;
}

bool CAccount::meetsConditions( CString pAccount, CString conditions )
{
	const char* conditional[] = { ">=", "<=", "!=", "=", ">", "<" };
	CStringList file;
	CString fileName;
	fileName = CString() << "accounts" << fSep << pAccount << ".txt";

	// Load and check if the file is valid.
	if (!file.load(fileName.text()) || file[0] != "GRACC001")
		return false;

	// Load the conditions into a string list.
	CStringList cond;
	conditions.removeAll( "'" );
	conditions.replaceAll( "%", "*" );
	cond.load( conditions.text(), "," );
	bool* conditionsMet = new bool[cond.count()];
	memset( (void*)conditionsMet, 0, sizeof(bool) * cond.count() );

	// Go through each line of the loaded file.
	for (int i = 0; i < file.count(); i++)
	{
		int sep = file[i].find(' ');
		CBuffer section = file[i].copy(0, sep);
		CBuffer val = file[i].copy(sep + 1);
		section.trim();
		val.trim();

		// Check each line against the conditions specified.
		for ( int j = 0; j < cond.count(); ++j )
		{
			int cond_num = -1;

			// Read out the name and value.
			cond[j].setRead(0);

			// Find out what conditional we are using.
			for ( int k = 0; k < 6; ++k )
			{
				if ( cond[j].find( conditional[k] ) != -1 )
				{
					cond_num = k;
					k = 6;
				}
			}
			if ( cond_num == -1 ) continue;

			CString cname = cond[j].readString( conditional[cond_num] );
			CString cvalue = cond[j].readString( "" );
			cname.trim();
			cvalue.trim();
			cond[j].setRead(0);

			// Now, do a case-insensitive comparison of the section name.
#ifdef WIN32
			if ( stricmp( section.text(), cname.text() ) == 0 )
#else
			if ( strcasecmp( section.text(), cname.text() ) == 0 )
#endif
			{
				switch ( cond_num )
				{
					case 0:
					case 1:
					{
						// 0: >=
						// 1: <=
						// Check if it is a number.  If so, do a number comparison.
						bool condmet = false;
						if ( val.isNumber() )
						{
							double vNum[2] = { atof( val.text() ), atof( cvalue.text() ) };
							if ( ((cond_num == 1) ? (vNum[0] <= vNum[1]) : (vNum[0] >= vNum[1])) )
							{
								conditionsMet[j] = true;
								condmet = true;
							}
						}
						else
						{
							// If not a number, do a string comparison.
							int ret = strcmp( val.text(), cvalue.text() );
							if ( ((cond_num == 1) ? (ret <= 0) : (ret >= 0)) )
							{
								conditionsMet[j] = true;
								condmet = true;
							}
						}

						// No conditions met means we see if we can fail.
						if ( condmet == false )
						{
							CBuffer cnameUp = cname.toUpper();
							if ( !(cnameUp == "CHEST" || cnameUp == "WEAPON" ||
								cnameUp == "FLAG" || cnameUp == "FOLDERRIGHT") )
								goto condAbort;
						}
						break;
					}

					case 4:
					case 5:
					{
						// 4: >
						// 5: <
						bool condmet = false;
						if ( val.isNumber() )
						{
							double vNum[2] = { atof( val.text() ), atof( cvalue.text() ) };
							if ( ((cond_num == 5) ? (vNum[0] < vNum[1]) : (vNum[0] > vNum[1])) )
							{
								conditionsMet[j] = true;
								condmet = true;
							}
						}
						else
						{
							int ret = strcmp( val.text(), cvalue.text() );
							if ( ((cond_num == 5) ? (ret < 0) : (ret > 0)) )
							{
								conditionsMet[j] = true;
								condmet = true;
							}
						}

						if ( condmet == false )
						{
							CBuffer cnameUp = cname.toUpper();
							if ( !(cnameUp == "CHEST" || cnameUp == "WEAPON" ||
								cnameUp == "FLAG" || cnameUp == "FOLDERRIGHT") )
								goto condAbort;
						}
						break;
					}

					case 2:
					{
						// 2: !=
						// If we find a match, return false.
						if ( val.isNumber() )
						{
							double vNum[2] = { atof( val.text() ), atof( cvalue.text() ) };
							if ( vNum[0] == vNum[1] ) goto condAbort;
							conditionsMet[j] = true;
						}
						else
						{
							if ( val.match( cvalue.text() ) == true ) goto condAbort;
							conditionsMet[j] = true;
						}
						break;
					}

					case 3:
					default:
					{
						// 0 - equals
						// If it returns false, don't include this account in the search.
						bool condmet = false;
						if ( val.isNumber() )
						{
							double vNum[2] = { atof( val.text() ), atof( cvalue.text() ) };
							if ( vNum[0] == vNum[1] )
							{
								conditionsMet[j] = true;
								condmet = true;
							}
						}
						else
						{
							if ( val.match( cvalue.text() ) == true )
							{
								conditionsMet[j] = true;
								condmet = true;
							}
						}

						if ( condmet == false )
						{
							CBuffer cnameUp = cname.toUpper();
							if ( !(cnameUp == "CHEST" || cnameUp == "WEAPON" ||
								cnameUp == "FLAG" || cnameUp == "FOLDERRIGHT") )
								goto condAbort;
						}
						break;
					}
				}
			}
		}
	}

	// Check if all the conditions were met.
	for ( int i = 0; i < cond.count(); ++i )
		if ( conditionsMet[i] == false ) goto condAbort;

	// Clean up.
	delete [] conditionsMet;

	return true;

condAbort:
	delete [] conditionsMet;
	return false;
}
