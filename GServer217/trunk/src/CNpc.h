// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef CNPCH
#define CNPCH

#include <stdarg.h>
#include "CString.h"
#include "CLevel.h"
#include "CPacket.h"

enum {
	NPCGIF,       //  0
	ACTIONSCRIPT, //  1
	NPCX,         //  2
	NPCY,         //  3
	NPCPOWER,     //  4
	NPCRUPEES,    //  5
	NPCARROWS,    //  6
	NPCBOMBS,     //  7
	NGLOVEPOWER,  //  8
	NBOMBPOWER,   //  9
	NSWORDGIF,       // 10
	NSHIELDGIF,   // 11
	NPCANI,      // 12  ??? used to be NPCBOWGIF but seems to set GANI
	VISFLAGS,     // 13
	BLOCKFLAGS,   // 14
	NPCMESSAGE,   // 15
	NPCHURTDXDY,  // 16
	NPCID,        // 17
	NPCSPRITE,    // 18
	NPCCOLORS,    // 19
	NPCNICKNAME,  // 20
	NPCHORSEGIF,  // 21
	NPCHEADGIF,   // 22
	NPCSAVE0,     // 23
	NPCSAVE1,     // 24
	NPCSAVE2,     // 25
	NPCSAVE3,     // 26
	NPCSAVE4,     // 27
	NPCSAVE5,     // 28
	NPCSAVE6,     // 29
	NPCSAVE7,     // 30
	NPCSAVE8,     // 31
	NPCSAVE9,     // 32
	NALIGNMENT,   // 33
	NPCGIFPART,   // 34
	NPCBODY,		//35
	NGATTRIB1,	//36
	NGATTRIB2,	//37
	NGATTRIB3,	//38
	NGATTRIB4,	//39
	NGATTRIB5		//40
};

#define npcpropcount 41
class CNpc
{
	private:
		void con_print( const char* format, ... );

	public:
		int modTime[npcpropcount];
		CNpc(CString& pImage, CString& pCode, float pX, float pY, CLevel* pLevel);
		~CNpc();
		CString serverCode, clientCode, gAttribs[5];
		CLevel* level;
		CString image, gAni, swordImage, shieldImage, bowImage, chatMsg, nickName, horseImage, headImage, weaponName, imagePart, bodyImage;
		char saves[10];
		int id, rupees, darts, bombs, glovePower, visFlags, blockFlags, sprite, colors[8], bombPower, ap, swordPower, shieldPower, power;
		float x, y, hurtX, hurtY;
		void removeComments();
		CPacket getProperty(int pId);
		CPacket getPropertyList(int newTime);
		CString getFunctionParameter(char* pName);
		void processJoinScripts();
		void setProps(CPacket& pProps);
};

#endif // CNPCH
