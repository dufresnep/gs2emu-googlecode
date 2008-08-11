#ifndef TNPC_H
#define TNPC_H

#include "ICommon.h"
//#include "TLevel.h"

enum
{
	NPCPROP_IMAGE			= 0,
	NPCPROP_SCRIPT			= 1,
	NPCPROP_X				= 2,
	NPCPROP_Y				= 3,
	NPCPROP_POWER			= 4,
	NPCPROP_RUPEES			= 5,
	NPCPROP_ARROWS			= 6,
	NPCPROP_BOMBS			= 7,
	NPCPROP_GLOVEPOWER		= 8,
	NPCPROP_BOMBPOWER		= 9,
	NPCPROP_SWORDIMAGE		= 10,
	NPCPROP_SHIELDIMAGE		= 11,
	NPCPROP_GANI			= 12,
	NPCPROP_VISFLAGS		= 13,
	NPCPROP_BLOCKFLAGS		= 14,
	NPCPROP_MESSAGE			= 15,
	NPCPROP_HURTDXDY		= 16,
	NPCPROP_ID				= 17,
	NPCPROP_SPRITE			= 18,
	NPCPROP_COLORS			= 19,
	NPCPROP_NICKNAME		= 20,
	NPCPROP_HORSEIMAGE		= 21,
	NPCPROP_HEADIMAGE		= 22,
	NPCPROP_SAVE0			= 23,
	NPCPROP_SAVE1			= 24,
	NPCPROP_SAVE2			= 25,
	NPCPROP_SAVE3			= 26,
	NPCPROP_SAVE4			= 27,
	NPCPROP_SAVE5			= 28,
	NPCPROP_SAVE6			= 29,
	NPCPROP_SAVE7			= 30,
	NPCPROP_SAVE8			= 31,
	NPCPROP_SAVE9			= 32,
	NPCPROP_ALIGNMENT		= 33,
	NPCPROP_IMAGEPART		= 34,
	NPCPROP_BODYIMAGE		= 35,
	NPCPROP_GATTRIB1		= 36,
	NPCPROP_GATTRIB2		= 37,
	NPCPROP_GATTRIB3		= 38,
	NPCPROP_GATTRIB4		= 39,
	NPCPROP_GATTRIB5		= 40,
	NPCPROP_EMPTY41			= 41,
	NPCPROP_EMPTY42			= 42,
	NPCPROP_EMPTY43			= 43,
	NPCPROP_GATTRIB6		= 44,
	NPCPROP_GATTRIB7		= 45,
	NPCPROP_GATTRIB8		= 46,
	NPCPROP_GATTRIB9		= 47,

// Does the client not send gani attribs > 9?
	NPCPROP_GATTRIB10		= 48,
	NPCPROP_GATTRIB11		= 49,
	NPCPROP_GATTRIB12		= 50,
	NPCPROP_GATTRIB13		= 51,
	NPCPROP_GATTRIB14		= 52,
	NPCPROP_GATTRIB15		= 53,
	NPCPROP_GATTRIB16		= 54,
	NPCPROP_GATTRIB17		= 55,
	NPCPROP_GATTRIB18		= 56,
	NPCPROP_GATTRIB19		= 57,
	NPCPROP_GATTRIB20		= 58,
	NPCPROP_GATTRIB21		= 59,
	NPCPROP_GATTRIB22		= 60,
	NPCPROP_GATTRIB23		= 61,
	NPCPROP_GATTRIB24		= 62,
	NPCPROP_GATTRIB25		= 63,
	NPCPROP_GATTRIB26		= 64,
	NPCPROP_GATTRIB27		= 65,
	NPCPROP_GATTRIB28		= 66,
	NPCPROP_GATTRIB29		= 67,
	NPCPROP_GATTRIB30		= 68,
};

#define npcpropcount 48
//#define npcpropcount 69

class TLevel;

class TNPC
{
	public:
		TNPC(const CString& pImage, const CString& pScript, float pX, float pY, TLevel* pLevel, bool pLevelNPC = true);
		~TNPC();

		CString getProp(int pId) const;
		CString getProps(time_t newTime) const;
		void setProps(CString& pProps);

		void setId(int pId)			{ id = pId; }
		int getId()					{ return id; }
		TLevel* getLevel()			{ return level; }

	private:
		bool levelNPC;
		time_t modTime[npcpropcount];
		float x, y, hurtX, hurtY;
		int id, rupees;
		unsigned char darts, bombs, glovePower, bombPower, swordPower, shieldPower;
		unsigned char visFlags, blockFlags, sprite, colors[5], power, ap;
		CString gAttribs[30];
		CString image, swordImage, shieldImage, headImage, bodyImage, horseImage, gani;
		CString nickName, imagePart, chatMsg, weaponName;
		CString serverCode, clientCode;
		unsigned char saves[10];
		TLevel* level;
};

#endif