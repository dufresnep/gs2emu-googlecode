// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef CWEAPONH
#define CWEAPONH
#include "CPacket.h"
#include "CString.h"

enum
{
	GREENRUPEE,
	BLUERUPEE,
	REDRUPEE,
	BOMBS,
	DARTS,
	HEART,
	GLOVE1,
	BOW,
	BOMB,
	SHIELD,
	SWORD,
	FULLHEART,
	SUPERBOMB,
	BATTLEAXE,
	GOLDENSWORD,
	MIRRORSHIELD,
	GLOVE2,
	LIZARDSHIELD,
	LIZARDSWORD,
	GOLDRUPEE,
	FIREBALL,
	FIREBLAST,
	NUKESHOT,
	JOLTBOMB,
	SPINATTACK
};
class CWeapon
{
	public:
	CString code, name, image;
	long long modTime;
	CPacket getSendData();
};

#endif // CWEAPONH
