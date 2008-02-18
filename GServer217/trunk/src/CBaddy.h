// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef CBADDYH
#define CBADDYH
#define baddytypes 10

#include "CString.h"
#include "CStringList.h"
#include "CPacket.h"
#include "COther.h"

extern char baddyStartMode[];
extern bool baddyPropsReinit[];

#define baddypropcount 10
// Baddy properties types
enum {
	BADDYID,	  //  0
	BADDYX,	   //  1
	BADDYY,	   //  2
	BADDYTYPE,	//  3
	BADDYGIF,	 //  4
	BADDYMODE,	//  5
	BADDYANISTEP, //  6
	BADDYDIR,	 //  7
	BADDYVERSE1,  //  8
	BADDYVERSE2,  //  9
	BADDYVERSE3   // 10
};

// Baddy modes
enum {
	WALK,		// 0
	LOOK,		// 1
	HUNT,		// 2
	HURTED,	  // 3
	BUMPED,	  // 4
	DIE,		 // 5
	SWAMPSHOT,   // 6
	HAREJUMP,	// 7
	OCTOSHOT,	// 8
	DEAD		 // 9
};

class CBaddy
{
	public:
	CString image;
	CStringList verses;
	char type;
	char x, y, power, startX, startY, respawnCount, mode, aniCount, dir;
	bool respawn;
	short id;
	CBaddy(int pX, int pY, char pType);
	void loadVerses(char* pLine);
	CPacket getPropList();
	CPacket getProperty(int pId);
	void setProps(CPacket &pProps);
	void reset();
};

inline CBaddy::CBaddy(int pX, int pY, char pType)
{
	x = startX = CLIP(pX, 0, 63);
	y = startY = CLIP(pY, 0, 63);
	type = CLIP(pType, 0, baddytypes-1);
	respawn = true;
	reset();
}

inline void CBaddy::loadVerses(char* pLine)
{
	verses.load(pLine, "\\");
}
#endif // CBADDYH

