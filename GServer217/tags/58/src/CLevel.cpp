/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#include "CLevel.h"
#include "main.h"
#include "CNpc.h"
#include "CBaddy.h"
#include "CMap.h"
#include "CPlayer.h"
#include "CList.h"
#include "CWeapon.h"

CString base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
CString signText = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
				"0123456789!?-.,#>()#####\"####':/~&### <####;\n";
CString signSymbols = "ABXYudlrhxyz#4.";
int ctablen[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1};
int ctabindex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16};
int ctab[] = {91, 92, 93, 94, 77, 78, 79, 80, 74, 75, 71, 72, 73, 86, 87, 88, 67};
extern char* itemNames[];

// Store join command stuff.
CList joinList;

CLevel::CLevel(CString& pFileName)
{
	map = NULL;
	baddyIds.add(0);
	levelIndex = 0;
	sparZone = opened = false;
	saveCounter = 5;
	memset(tiles, 0, sizeof(tiles));

	if(pFileName.find(".nw") >= 0)
	{
		if(!loadNW(pFileName))
			return;
	} else
	{
		if(!loadGraal(pFileName))
			return;
	}

	//Find our map id
	for(int i = 0; i < CMap::mapList.count(); i++)
	{
		CMap* m = (CMap*)CMap::mapList[i];
		if((levelIndex = m->getLevelpos(pFileName)) >= 0)
		{
			map = m;
			break;
		}
	}
	opened = true;
}

bool CLevel::loadNW(CString& pFileName)
{
	CStringList levelData;
	CString version;
	char* dataFile = getDataFile(pFileName.text());
	if(!strlen(dataFile))
		return false;

	if(!levelData.load(dataFile))
		return false;

	if(levelData.count() < 1)
		return false;

	version = levelData[0];
	modTime = getFileModTime(dataFile);
	fileName = pFileName;
	if(version == "GLEVNW01" || version == "GSERVL01")
	{
		for(int i = 1; i < levelData.count(); i ++)
		{
			CStringList words;
			words.load(levelData[i].text(), " ");
			if(words.count() <= 0)
				continue;
			if(words[0] == "BOARD")
			{
				if(words.count() <= 5)
					continue;

				int x = atoi(words[1].text());
				int y = atoi(words[2].text());
				int w = atoi(words[3].text());
				CString& data = words[5];
				if(x >= 0 && x <= 64 && y >= 0 && y <= 64 && w > 0 && x + w <= 64)
				{
					if(data.length() >= w*2)
					{
						for(int ii = x; ii < x + w; ii++)
						{
							char left = data.readChar();
							char top = data.readChar();
							short tile = base64.find(left) << 6;
							tile += base64.find(top);
							tiles[ii + y*64] = tile;
						}
					}
				}
			} else if(words[0] == "LINK")
			{
				if(words.count() <= 7)
					continue;
				if(strlen(getDataFile(words[1].text())))
				{
					links.add(new CLink(words[1], atoi(words[2].text()), atoi(words[3].text()),
						atoi(words[4].text()), atoi(words[5].text()), words[6], words[7]));
				}
			} else if(words[0] == "CHEST")
			{
				if(words.count() <= 4)
					continue;
				for(int ii = 0; ii < itemcount; ii++)
				{
					if(words[3] == itemNames[ii])
					{
						chests.add(new CChest(atoi(words[1].text()), atoi(words[2].text()),
							atoi(words[4].text()), ii));
						break;
					}
				}

			} else if(words[0] == "NPC")
			{
				if(words.count() <= 3)
					continue;
				CString image, code, code2;
				float x, y;
				if(words[1] != "-")
					image = words[1];

				x = (float)atof(words[2].text());
				y = (float)atof(words[3].text());
				for(i++; i < levelData.count() && levelData[i] != "NPCEND"; i++)
					code << levelData[i] << "\xa7";

				// Create the new NPC.  Do this before parsing the join commands.
				// The CNpc constructor will remove all comments.
				CNpc* jnpc = new CNpc( image, code, x, y, this, true );

				// Now filter out the join commands.
				CStringList npcData;
				npcData.load( jnpc->clientCode.text(), "\xa7" );
				for ( int j = 0; j < npcData.count(); ++j )
					code2 << processNpcLine( npcData[j] ) << "\xa7";
				jnpc->clientCode = code2;

				// Now, add all the joined files to the code.
				if ( joinList.count() > 0 )
				{
					CString* file = 0;
					while ( (file = (CString*)joinList[0]) != 0 )
					{
						// Load the source file into memory.
						CString dataFile = getDataFile(file->text());
						if(dataFile.length())
						{
							// Append to the end of the script.
							CString retVal;
							retVal.load(dataFile.text());
							retVal.replaceAll("\r\n", "\xa7");
							retVal.replaceAll("\n", "\xa7");
							jnpc->clientCode << retVal << "\xa7";
						}
						delete (CString*)joinList[0];
						joinList.remove(0);
					}
				}
				joinList.clear();

				npcs.add( jnpc );
			} else if(words[0] == "BADDY")
			{
				if(words.count() <= 3)
					continue;
				int x = atoi(words[1].text());
				int y = atoi(words[2].text());
				int type = atoi(words[3].text());

				CBaddy* baddy = new CBaddy(x, y, type);
				int baddyId = createBaddyId(baddy);
				baddy->id = baddyId;
				for(i++; i < levelData.count() && levelData[i] != "BADDYEND"; i++)
					baddy->verses.add(levelData[i].text());
				if(baddies.count() < 50)
					baddies.add(baddy);
				else delete baddy;
			} else if(words[0] == "SIGN")
			{
				if(words.count() <= 2)
					continue;

				CString sign;
				int x = atoi(words[1].text());
				int y = atoi(words[2].text());
				sign.writeChar(x+32);
				sign.writeChar(y+32);

				for (i++; i < levelData.count() && levelData[i] != "SIGNEND"; i++)
					sign << getSignCode(CString() << levelData[i] << "\n");
				signs.add(sign);
			} else if(words[0] == "REPLACENPC")
			{
				int npcId = atoi(words[1].text());
				CNpc* npc = (CNpc*)npcs[npcId];
				if(npc == NULL)
					continue;

				for(i++; i < levelData.count() && levelData[i] != "REPLACENPCEND"; i++)
					npc->setProps((CPacket&)levelData[i]);
			}
		}
	} else return false;

	return true;
}

CString CLevel::processNpcLine(CString& pLine)
{
	//Find join codes
	CString retVal(pLine);
	int ret = 0;
	while ( (ret = retVal.find("join ", ret) + 1) > 0 )
	{
		// Trim the line.
		retVal.trimLeft();

		// See if join is now the first character on the line.
		if ( retVal.find("join ") == 0 )
		{
			// Find the semi-colon and add the text file name to 'script'
			int s_pos = retVal.find(';');
			CString* script = new CString();
			*script = "";
			*script << retVal.copy(5, s_pos-5);

			// See if the file was already added or not.
			bool found = false;
			for ( int i = 0; i < joinList.count(); ++i )
			{
				if ( *((CString *)joinList[i]) == *script )
				{
					found = true;
					break;
				}
			}
			if ( !found )
			{
				// Save the name of the text file and replace join with a semi-colon.
				retVal.remove( (CString() << "join " << *script).text() );
				*script << ".txt";
				joinList.add( (void *)script );
			}
			else
				delete script;
		}
	}
	return retVal;
}

void CLevel::saveNpcs()
{
	return;

	CStringList npcData;
	//printf("SAVING NPCS\n");
	for(int i = 0; i < npcs.count(); i++)
	{
		CNpc* npc = (CNpc*)npcs[i];
		if(npc == NULL)
			continue;
		npcData.add(CString() << "REPLACENPC " << toString(i));
		CPacket props;
		for(int ii = 0; ii < npcpropcount; ii++)
		{
			if(ii != ACTIONSCRIPT)
				props << (char) ii << npc->getProperty(ii);
		}
		npcData.add(props);
		npcData.add("REPLACENPCEND\r\n");
	}
	CString fName;
	fName << "npcprops" << fSep << fileName << ".code";
	npcData.save(fName.text());
}

bool CLevel::loadGraal(CString& pFileName)
{
	CPacket levelData;
	CString version;
	char* dataFile = getDataFile(pFileName.text());
	if(!strlen(dataFile))
		return false;
	if(!levelData.load(dataFile))
		return false;
	fileName = pFileName;
	modTime = getFileModTime(dataFile);
	version = levelData.readChars(8);

	bool v0 = (version == "GR-V1.00");
	bool v1 = (version == "GR-V1.01");
	bool v2 = (version == "GR-V1.02");
	bool v3 = (version == "GR-V1.03");
	//printf("Loading map %s\n", pFileName.text());
	//printf("Loading tiles..\n");
	loadTiles(levelData, version);
	//printf("Loading links..\n");
	loadLinks(levelData);
	//printf("Loading baddies..\n");
	loadBaddies(levelData);
	if (v0 || v1 || v2 || v3)
	{
		//printf("Loading npcs..\n");
		loadNpcs(levelData);
	}
	if (v1 || v2 || v3)
	{
		//printf("Loading chests..\n");
		loadChests(levelData);
	}
	//printf("Loading signs..\n");
	loadSigns(levelData);
	return true;
}

void CLevel::loadTiles(CPacket& levelData, CString& pVersion)
{
	int boardindex = 0;
	int bitsread = 0;
	int codebits = 12;
	if(pVersion == "GR-V1.02" || pVersion == "GR-V1.03")
		codebits = 13;

	int buffer = 0;
	int count = 1;
	int firstcode = -1;
	bool twicerepeat = false;
	unsigned char databyte;

	while(boardindex < 64*64 && levelData.bytesLeft() > 0)
	{
		while(bitsread < codebits)
		{
			databyte = levelData.readByte();
			buffer += (databyte << bitsread);
			bitsread += 8;
		}

		int code = buffer & ((1 << codebits) - 1);
		buffer >>= codebits;
		bitsread -= codebits;

		if ((code & (1 << (codebits-1))) > 0)
		{
			if((code & 0x100) > 0)
				twicerepeat = true;
			count = code & 0xFF;
		} else if(count == 1)
		{
			tiles[boardindex] = (short)code;
			boardindex++;
		} else
		{
			if(twicerepeat)
			{
				if(firstcode < 0)
					firstcode = code;
				else
				{
					for(int i = 1; i <= count && boardindex < 64*64-1; i++)
					{
						tiles[boardindex] = (short)firstcode;
						tiles[boardindex+1] = (short)code;
						boardindex += 2;
					}
					firstcode = -1;
					twicerepeat = false;
					count = 1;
				}
			} else
			{
				for(int i = 1; i <= count && boardindex < 64*64; i++)
				{
					tiles[boardindex] = (short)code;
					boardindex++;
				}
				count = 1;
			}
		}
	}
}

void CLevel::loadLinks(CPacket& levelData)
{
	while(levelData.bytesLeft())
	{
		CBuffer line = levelData.readLine();
		if(line.length() <= 0 || line == "#")
			break;

		CString nextMap = line.readString(" ");
		if(!strlen(getDataFile(nextMap.text())))
			continue;
		int x = atoi(line.readString(" "));
		int y = atoi(line.readString(" "));
		int width = atoi(line.readString(" "));
		int height = atoi(line.readString(" "));
		CString warpX = line.readString(" ");
		CString warpY = line.readString(" ");
		links.add(new CLink(nextMap, x, y, width, height, warpX, warpY));
	}
}

int CLevel::createBaddyId(CBaddy* pBaddy)
{
	for(int i = 1; i < baddyIds.count(); i++)
	{
		if(baddyIds[i] == NULL)
		{
			baddyIds.replace(i, pBaddy);
			return i;
		}
	}
	return baddyIds.add(pBaddy);
}

void CLevel::loadBaddies(CPacket& levelData)
{
	while(levelData.bytesLeft())
	{
		CPacket line;
		while(levelData.bytesLeft())
		{
			char byte;
			byte = levelData.readChar();
			if(byte == '\n' && line.length() >= 3)
				break;
			line.writeChar(byte);
		}

		if(line.length() < 3 || line[0] < 0)
			break;

		if(baddies.count() > 50)
			continue;
		int x = line.readByte();
		int y = line.readByte();
		char type = line.readByte();
		CBaddy* baddy = new CBaddy(x, y, type);
		baddy->id = createBaddyId(baddy);
		baddy->loadVerses(line.readChars(line.bytesLeft()));
		baddies.add(baddy);
	}
}

void CLevel::loadNpcs(CPacket& levelData)
{
	while(levelData.bytesLeft())
	{
		CPacket line;
		line << levelData.readLine();
		if(line.length() < 1 || line == "#")
			break;

		float x = line.readByte1();
		float y = line.readByte1();
		CString image = line.readString("#");
		CString code = line.readChars(line.bytesLeft());

		// Create the new NPC.  Do this before parsing the join commands.
		// The CNpc constructor will remove all comments.
		CString code2;
		CNpc* jnpc = new CNpc( image, code, x, y, this, true );

		// Now filter out the join commands.
		CStringList npcData;
		npcData.load( jnpc->clientCode.text(), "\xa7" );
		for ( int j = 0; j < npcData.count(); ++j )
			code2 << processNpcLine( npcData[j] ) << "\xa7";
		jnpc->clientCode = code2;

		// Now, add all the joined files to the code.
		if ( joinList.count() > 0 )
		{
			CString* file = 0;
			while ( (file = (CString*)joinList[0]) != 0 )
			{
				// Load the source file into memory.
				CString dataFile = getDataFile(file->text());
				if(dataFile.length())
				{
					// Append to the end of the script.
					CString retVal;
					retVal.load(dataFile.text());
					retVal.replaceAll("\r\n", "\xa7");
					retVal.replaceAll("\n", "\xa7");
					jnpc->clientCode << retVal << "\xa7";
				}
				delete (CString*)joinList[0];
				joinList.remove(0);
			}
		}
		joinList.clear();
		npcs.add( jnpc );
	}
}

void CLevel::loadChests(CPacket& levelData)
{
	while(levelData.bytesLeft())
	{
		CPacket line;
		line << levelData.readLine();
		if(line.length() < 1 || line == "#")
			break;

		int x = line.readByte1();
		int y = line.readByte1();
		int item = line.readByte1();
		int signIndex = line.readByte1();
		chests.add(new CChest(x, y, signIndex, item));
	}
}

void CLevel::loadSigns(CPacket& levelData)
{
	while(levelData.bytesLeft())
	{
		CBuffer line = levelData.readLine();
		if(line.length() <= 0)
			break;

		signs.add(line.text());
	}
}

CLevel* CLevel::find(CString& pFileName)
{
	for(int i = 0; i < levelList.count(); i ++)
	{
		CLevel* level = (CLevel*)levelList[i];
		if(level->fileName == pFileName)
			return level;
	}
	return NULL;
}

CLevel* CLevel::openMap(CString& pFileName)
{
	CLevel* level = find(pFileName);
	if(level != NULL)
		return level;

	level = new CLevel(pFileName);
	if(!level->opened)
	{
		delete level;
		return NOLEVEL;
	}

	levelList.add(level);
	if ( detailedconsole ) printf("[%s] MAP COUNT: %i\n", getTimeStr(1).text(), levelList.count());
	return level;
}

CString CLevel::getSignCode(CString& pText)
{
	CString retVal;
	int txtLen = pText.length();
	for(int i = 0; i < txtLen; i++)
	{
		char letter = pText[i];
		if(letter == '#')
		{
			i ++;
			if(i < txtLen)
			{
				letter = pText[i];
				int code = signSymbols.find(letter);
				if(code >= 0)
				{
					for(int ii = 0; ii < ctablen[code]; ii++)
						retVal.writeChar(ctab[ctabindex[code] + ii] + 32);
				}
			}
		} else
		{
			int code = signText.find(letter);
			if(code >= 0)
				retVal.writeChar(code+32);
		}
	}
	return retVal;
}

void CLevel::addNewNpc(CString& pImage, CString& pCodeFile, float pX, float pY)
{
	CStringList codeData;
	CString code;
	char* dataFile = getDataFile(pCodeFile.text());
	if(!strlen(dataFile))
		return;
	if(!codeData.load(dataFile))
		return;
	for(int i = 0; i < codeData.count(); i++)
		code << codeData[i] << "\xa7";

	// Create the new NPC.  Do this before parsing the join commands.
	// The CNpc constructor will remove all comments.
	CString code2;
	CNpc* npc = new CNpc( pImage, code, pX, pY, this, false );

	// Now filter out the join commands.
	CStringList npcData;
	npcData.load( npc->clientCode.text(), "\xa7" );
	for ( int j = 0; j < npcData.count(); ++j )
		code2 << processNpcLine( npcData[j] ) << "\xa7";
	npc->clientCode = code2;

	// Now, add all the joined files to the code.
	if ( joinList.count() > 0 )
	{
		CString* file = 0;
		while ( (file = (CString*)joinList[0]) != 0 )
		{
			// Load the source file into memory.
			CString dataFile = getDataFile(file->text());
			if(dataFile.length())
			{
				// Append to the end of the script.
				CString retVal;
				retVal.load(dataFile.text());
				retVal.replaceAll("\r\n", "\xa7");
				retVal.replaceAll("\n", "\xa7");
				npc->clientCode << retVal << "\xa7";
			}
			delete (CString*)joinList[0];
			joinList.remove(0);
		}
	}
	joinList.clear();
	npcs.add( npc );

	for(int i = 0; i < players.count(); i++)
	{
		CPlayer* player = (CPlayer*)players[i];
		player->sendPacket(CPacket() << (char)SNPCPROPS << (int)npc->id <<
			npc->getPropertyList(0));
	}
}

void CLevel::updateLevel(CString& pFileName)
{
	CLevel* level = find(pFileName);
	if(level == NULL)
		return;

	level->reset();
	if(getFileExtension(pFileName) == ".nw")
	{
		if(!level->loadNW(pFileName))
			return;
	} else
	{
		if(!level->loadGraal(pFileName))
			return;
	}
	CList playerTemp;
	while(level->players.count())
	{
		CPlayer* player = (CPlayer*)level->players[0];
		player->leaveLevel();
		level->players.remove(player);
		playerTemp.add(player);
	}

	for ( int i = 0; i < playerList.count(); ++i )
	{
		CPlayer* player = (CPlayer*)playerList[i];
		for ( int j = player->enteredLevels.count() - 1; j >= 0; --j )
		{
			CEnteredLevel* entered = (CEnteredLevel*)player->enteredLevels[j];
			if ( entered == 0 || entered->level == level )
			{
				entered->time = 0;
				//delete entered;
				//player->enteredLevels.remove( j );
				break;
			}
		}
	}

	for ( int i = 0; i < playerTemp.count(); i++ )
	{
		CPlayer* player = (CPlayer*)playerTemp[i];
		player->sendPacket(CPacket() << (char)LEVELNAME << pFileName);
		player->sendLevel(pFileName, player->x, player->y, 0);
	}
}

void CLevel::reset()
{
	for(int i = 0; i < npcs.count(); i++)
	{
		CNpc* npc = (CNpc*)npcs[i];
		for(int ii = 0; ii < playerList.count(); ii++)
		{
			CPlayer* player = (CPlayer*)playerList[ii];

			// Terrible hack to fix the destroy; command on putnpc npcs.
			if ( npc->levelNPC == false )
			{
				// SDELNPC doesn't work, so this terrible hack will do.
				player->sendPacket( CPacket() << (char)SNPCPROPS << (int)npc->id <<
					(char)ACTIONSCRIPT << (short)0 << (char)VISFLAGS << (char)0 <<
					(char)BLOCKFLAGS << (char)0 << (char)NPCMESSAGE << (char)0 );
			}
			player->sendPacket(CPacket() << (char)SDELNPC << (int)npc->id);
			player->compressAndSend();
		}
		delete npc;
	}
	npcs.clear();
	for(int i = 0; i < baddies.count(); i++)
	{
		CBaddy* baddy = (CBaddy*)baddies[i];
		delete baddy;
	}
	baddies.clear();

	for(int i = 0; i < chests.count(); i++)
	{
		CChest* chest = (CChest*)chests[i];
		delete chest;
	}
	chests.clear();
	for(int i = 0; i < boardChanges.count(); i++)
	{
		CBoardChange* change = (CBoardChange*)boardChanges[i];
		delete change;
	}
	boardChanges.clear();

	for(int i = 0; i < links.count(); i++)
	{
		CLink* link = (CLink*)links[i];
		delete link;
	}
	links.clear();
	for(int i = 0; i < items.count(); i++)
	{
		CItem* item = (CItem*)items[i];
		delete item;
	}
	items.clear();
	signs.clear();
	baddyIds.clear();
	opened = sparZone = false;
	map = NULL;
	saveCounter = 300;
}

void CLevel::animate()
{
	for ( int i = 0; i < boardChanges.count(); i++ )
	{
		CBoardChange* change = (CBoardChange*)boardChanges[i];
		if ( change->counter > 0 )
		{
			change->counter--;
			if ( change->counter <= 0 )
			{
				// Put the old data back in.  DON'T DELETE THE CHANGE.
				// The client remembers board changes and if we delete the
				// change, the client won't get the new data.
				applyChange(change->prevData, change->x, change->y, change->width, change->height);
				change->tileData = change->prevData;
				change->modifyTime = getSysTime();
				change->counter = -1;
			}
		}
	}

	for ( int i = 0; i < items.count(); i++ )
	{
		CItem* item = (CItem*)items[i];
		item->counter--;
		if(item->counter == 0)
		{
			items.remove(i);
			delete item;
			i--;
		}
	}

	for ( int i = 0; i < baddies.count(); i++ )
	{
		CBaddy* baddy = (CBaddy*)baddies[i];
		if(baddy->mode == DIE || !players.count())
		{
			if(baddy->respawn)
			{
				// Drop items.
				if ( baddy->respawnCount == baddyRespawn && baddyDropItems == true )
					dropBaddyItem( baddy->x, baddy->y );

				baddy->respawnCount--;
				if(!baddy->respawnCount)
				{
					baddy->reset();
					CPacket baddyProps;
					baddyProps << (char)SBADDYPROPS << (char)baddy->id << baddy->getPropList();
					for(int ii = 0; ii < players.count(); ii ++)
					{
						CPlayer*player = (CPlayer*)players[ii];
						player->sendPacket(baddyProps);
					}
				}
			}
			if(!baddy->respawn && !players.count())
			{
				baddyIds.replace(baddy->id, NULL);
				baddies.remove(baddy);
				delete baddy;
				i--;
			}
		}
	}

	for(int i = 0; i < horses.count(); i++)
	{
		CHorse* horse = (CHorse*)horses[i];
		horse->counter--;
		if(horse->counter == 0)
		{
			CPacket horseData;
			horseData << (char)SDELHORSE << (char)(horse->x*2) <<
				(char)(horse->y*2);
			for (int ii = 0; ii < players.count(); ii++)
				((CPlayer*)players[ii])->sendPacket(horseData);
			delete horse;
			horses.remove(i);
			i--;
		}
	}
	saveCounter--;
	if(saveCounter <= 0)
	{
		saveNpcs();
		saveCounter = 300;
	}
}

void CLevel::dropBaddyItem( char iX, char iY )
{
	// 41.66...% chance of a green gralat.
	// 41.66...% chance of something else.
	// 16.66...% chance of nothing.
	int itemId = rand()%12;
	bool valid = true;

	switch (itemId)
	{
		case GREENRUPEE:
		case BLUERUPEE:
		case REDRUPEE:
		case BOMBS:
		case DARTS:
		case HEART:
			break;
		break;

		default:
			if ( itemId > 5 && itemId < 10 ) itemId = GREENRUPEE;
			else valid = false;
			break;
	}

	if ( valid )
	{
		items.add(new CItem(iX, iY, itemId));
		for (int i = 0; i < players.count(); i++)
			((CPlayer*)players[i])->sendPacket(CPacket() << (char)SADDEXTRA << (char)(iX*2) << (char)(iY*2) << (char)itemId);
	}
}

const char* CLink::getLinkString()
{
	static char retVal[500];
	sprintf(retVal, "%s %i %i %i %i %s %s", nextMap.text(), x, y, width, height,
				warpX.text(), warpY.text());
	return retVal;
}

CLevel::~CLevel()
{
	reset();
}

CBoardChange::CBoardChange(CPacket& pTiles, int pX, int pY, int pWidth, int pHeight)
{
	x = pX;
	y = pY;
	width = pWidth;
	height = pHeight;
	counter = tileRespawn;
	tileData << pTiles;
	modifyTime = getSysTime();
}

CPacket CBoardChange::getSendData()
{
	CPacket retVal;
	retVal << (char)x << (char)y << (char)width << (char)height << tileData;
	return retVal;
}

short tileObjects[] = {
	0x1ff,0x3ff,0x2ac,2,0x200,0x22,
	0x3de,0x1a4,0x14a,0x674
};

bool CLevel::changeBoard(CPacket& pTileData, int pX, int pY, int pWidth, int pHeight, CPlayer* player)
{
	if( pX < 0 || pY < 0 || pX > 63 || pY > 63 ||
		pWidth < 1 || pHeight < 1 ||
		pX + pWidth > 64 || pY + pHeight > 64 )
		return false;

	// Do the check for the push-pull block.
	if ( pWidth == 4 && pHeight == 4 && clientsidePushPull )
	{
		// Try to find the top-left corner tile.
		int i;
		for ( i = 0; i < 16; ++i )
		{
			short stoneCheck = pTileData.readByte2();
			if ( stoneCheck == 0x6E4 || stoneCheck == 0x7CE )
				break;
		}

		// Check if we found a possible push-pull block.
		if ( i != 16 && i < 11 )
		{
			// Go back one full short so the first readByte2() returns the top-left corner.
			pTileData.setRead( i * 2 );

			int foundCount = 0;
			for ( int j = 0; j < 6; ++j )
			{
				// Read a piece.
				short stoneCheck = pTileData.readByte2();

				// A valid stone will have pieces at the following j locations.
				if ( j == 0 || j == 1 || j == 4 || j == 5 )
				{
					switch ( stoneCheck )
					{
						// red
						case 0x6E4:
						case 0x6E5:
						case 0x6F4:
						case 0x6F5:
						// blue
						case 0x7CE:
						case 0x7CF:
						case 0x7DE:
						case 0x7DF:
							foundCount++;
							break;
					}
				}
			}
			pTileData.setRead(0);

			// Check if we found a full tile.  If so, don't accept the change.
			if ( foundCount == 4 )
			{
				player->sendPacket( CPacket() << (char)SBOARDMODIFY << (char)pX << (char)pY << (char)pWidth << (char)pHeight << pTileData );
				return false;
			}
		}
	}

	//Delete any existing changes within the same region
	for ( int i = 0; i < boardChanges.count(); i++ )
	{
		CBoardChange* change = (CBoardChange*)boardChanges[i];
		if(change->x >= pX && change->y >= pY && change->x + change->width <= pX + pWidth &&
			change->y + change->height <= pY + pHeight)
		{
			delete change;
			boardChanges.remove(i);
			i--;
		}
	}

	CBoardChange* change = new CBoardChange(pTileData, pX, pY, pWidth, pHeight);
	change->prevData << applyChange(pTileData, pX, pY, pWidth, pHeight);
	if ( change->prevData.length() > 0 )
		boardChanges.add(change);
	else delete change;
	return true;
}

CPacket CLevel::applyChange(CPacket& pTileData, int pX, int pY, int pWidth, int pHeight)
{
	CPacket retVal;
	CPacket sendBuff;
	sendBuff << (char)SBOARDMODIFY << (char)pX << (char)pY << (char)pWidth << (char)pHeight << pTileData;

	short oldTile = tiles[pX+(pY*64)];
	for(int i = 0; i < 10; i++)
	{
		if(oldTile == tileObjects[i])
		{
			for(int y = 0; y< pHeight; y++)
			{
				for(int x = 0; x < pWidth; x++)
				{
					retVal << tiles[pX+x+((pY+y)*64)];
				}
			}
			break;
		}
	}

	for (int i = 0; i < players.count(); i++)
	{
		CPlayer* other = (CPlayer*)players[i];
		other->sendPacket(sendBuff);
	}

	return retVal;
}

void CLevel::addHorse(CHorse* pHorse)
{
	horses.add(pHorse);
}

void CLevel::removeHorse(float pX, float pY)
{
	for(int i = 0; i < horses.count(); i++)
	{
		CHorse* horse = (CHorse*)horses[i];
		if(horse->x == pX && horse->y == pY)
		{
			delete horse;
			horses.remove(i);
			break;
		}
	}
}
