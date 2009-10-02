/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#ifndef CLEVELH
#define CLEVELH
#include "CString.h"
#include "CList.h"
#include "CStringList.h"
#include "CPacket.h"
#include "COther.h"
#include "CBaddy.h"
#include "CPlayer.h"
extern int horseLife;

class CPlayer;
class CMap;
class CHorse;
class CLevel
{
	public:
	CString playerWorld, fileName;
	CList links, baddies, npcs, players, horses, chests, items, boardChanges;
	CList baddyIds;
	CStringList signs;
	CMap* map;
	int levelIndex;
	time_t modTime;
	short saveCounter;
	bool opened, sparZone;
	short tiles[64*64];
	CLevel(CString& pFileName);
	CLevel()
	{
		map = NULL;
		levelIndex = 0;
		sparZone = opened = false;
		baddyIds.add(0);
	}
	~CLevel();
	static CLevel* openMap(CString& pFileName);
	static CLevel* find(CString& pFileName);
	static void updateLevel(CString& pFileName);

	bool loadNW(CString& pFileName);
	bool loadGraal(CString& pFileName);
	bool loadZelda(CString& pFileName);
	void loadTiles(CPacket& levelData, CString& pVersion);
	void loadLinks(CPacket& levelData);
	void loadBaddies(CPacket& levelData);
	void loadNpcs(CPacket& levelData);
	void loadChests(CPacket& levelData);
	void loadSigns(CPacket& levelData);
	void reset();
	void animate();
	void saveNpcs();
	int addNewNpc(CString& pImage, CString& pCodeFile, float pX, float pY);
	void addHorse(CHorse* pHorse);
	void removeHorse(float pX, float pY);
	CString getSignCode(CString& pText);
	static CString processNpcLine(CString& pLine);
	bool changeBoard(CPacket& pTileData, int pX, int pY, int pWidth, int pHeight, CPlayer* player);
	CPacket applyChange(CPacket& pTileData, int pX, int pY, int pWidth, int pHeight);
	int createBaddyId(CBaddy*pBaddy);
	void dropBaddyItem( char iX, char iY );
};

//Level objects
class CLink
{
	public:
	CString nextMap;
	int x, y, width, height;
	CString warpX, warpY;
	const char* getLinkString();
	CLink(CString& pNextMap, int pX, int pY, int pWidth, int pHeight, CString& pNextX, CString& pNextY)
	{
		nextMap = pNextMap;
		x = CLIP(pX, 0, 63);
		y = CLIP(pY, 0, 63);
		width = CLIP(pWidth, 0, 63);
		height = CLIP(pHeight, 0, 63);
		warpX = pNextX;
		warpY = pNextY;
	}
};

class CChest
{
	public:
	int x, y, signIndex, item;
	CChest(int pX, int pY, int pSign, int pItem)
	{
		x = CLIP(pX, 0, 63);
		y = CLIP(pY, 0, 63);
		signIndex = pSign;
		item = pItem;
	}
};

class CItem
{
public:
	float x, y;
	int counter, itemId;
	CItem(float pX, float pY, int pItemId)
	{
		x = pX;
		y = pY;
		itemId = pItemId;
		counter = 10;
	}
};

class CBoardChange
{
public:
	CPacket tileData, prevData;
	short counter, x, y, width, height;
	time_t modifyTime;
	CLevel* level;
	CBoardChange(CPacket& pTiles, int pX, int pY, int pWidth, int pHeight);
	CPacket getSendData();
};

class CHorse
{
public:
	float x, y;
	CString imageName;
	int dir, counter;
	int bushes;
	CHorse(CString& pImageName, float pX, float pY, int pDir, int pBushes)
	{
		imageName = pImageName;
		x = pX;
		y = pY;
		dir = pDir;
		bushes = pBushes;
		counter = horseLife;
	}
};
#endif // CLEVELH
