/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#ifndef CNPCH
#define CNPCH

#include <stdarg.h>
#include "CString.h"
#include "CLevel.h"
#include "CPacket.h"

enum {
	NPCGIF,			//  0
	ACTIONSCRIPT,	//  1
	NPCX,			//  2
	NPCY,			//  3
	NPCPOWER,		//  4
	NPCRUPEES,		//  5
	NPCARROWS,		//  6
	NPCBOMBS,		//  7
	NGLOVEPOWER,	//  8
	NBOMBPOWER,		//  9
	NSWORDGIF,		// 10
	NSHIELDGIF,		// 11
	NPCANI,			// 12  ??? used to be NPCBOWGIF but seems to set GANI
	VISFLAGS,		// 13
	BLOCKFLAGS,		// 14
	NPCMESSAGE,		// 15
	NPCHURTDXDY,	// 16
	NPCID,			// 17
	NPCSPRITE,		// 18
	NPCCOLORS,		// 19
	NPCNICKNAME,	// 20
	NPCHORSEGIF,	// 21
	NPCHEADGIF,		// 22
	NPCSAVE0,		// 23
	NPCSAVE1,		// 24
	NPCSAVE2,		// 25
	NPCSAVE3,		// 26
	NPCSAVE4,		// 27
	NPCSAVE5,		// 28
	NPCSAVE6,		// 29
	NPCSAVE7,		// 30
	NPCSAVE8,		// 31
	NPCSAVE9,		// 32
	NALIGNMENT,		// 33
	NPCGIFPART,		// 34
	NPCBODY,		// 35
	NGATTRIB1,		// 36
	NGATTRIB2,		// 37
	NGATTRIB3,		// 38
	NGATTRIB4,		// 39
	NGATTRIB5,		// 40
	NEMPTY41,		// 41
	NEMPTY42,		// 42
	NEMPTY43,		// 43
	NGATTRIB6,		// 44
	NGATTRIB7,		// 45
	NGATTRIB8,		// 46
	NGATTRIB9,		// 47

/* Does the client not send gani attribs > 9?
	NGATTRIB10,		// 48
	NGATTRIB11,		// 49
	NGATTRIB12,		// 50
	NGATTRIB13,		// 51
	NGATTRIB14,		// 52
	NGATTRIB15,		// 53
	NGATTRIB16,		// 54
	NGATTRIB17,		// 55
	NGATTRIB18,		// 56
	NGATTRIB19,		// 57
	NGATTRIB20,		// 58
	NGATTRIB21,		// 59
	NGATTRIB22,		// 60
	NGATTRIB23,		// 61
	NGATTRIB24,		// 62
	NGATTRIB25,		// 63
	NGATTRIB26,		// 64
	NGATTRIB27,		// 65
	NGATTRIB28,		// 66
	NGATTRIB29,		// 67
	NGATTRIB30		// 68
*/
};

#define npcpropcount 48
//#define npcpropcount 69
class CNpc
{
	private:
		void con_print( const char* format, ... );

	public:
		time_t modTime[npcpropcount];
		CNpc(CString& pImage, CString& pCode, float pX, float pY, CLevel* pLevel);
		CNpc(CString& pImage, CString& pCode, float pX, float pY, CLevel* pLevel, bool levelNPC);
		~CNpc();
		CString serverCode, clientCode, gAttribs[9];
		CLevel* level;
		CString image, gAni, swordImage, shieldImage, bowImage, chatMsg, nickName, horseImage, headImage, weaponName, imagePart, bodyImage;
		char saves[10];
		int id, rupees, darts, bombs, glovePower, visFlags, blockFlags, sprite, colors[8], bombPower, ap, swordPower, shieldPower, power;
		bool levelNPC;
		float x, y, hurtX, hurtY;
		void removeComments();
		CPacket getProperty(int pId);
		CPacket getPropertyList(time_t newTime);
		CString getFunctionParameter(char* pName);
		void processJoinScripts();
		void setProps(CPacket& pProps);
};

#endif // CNPCH
