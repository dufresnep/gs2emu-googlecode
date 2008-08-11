#ifndef TWEAPON_H
#define TWEAPON_H

#include <time.h>
#include "TNPC.h"
#include "CString.h"

class TWeapon
{
public:
	TWeapon(TNPC* pNPC) : npc(pNPC), modTime(time(0)) {}

	CString getWeaponPacket() const;

	CString getName() const				{ return npc->getWeaponName(); }
	CString getImage() const			{ return npc->getProp(NPCPROP_IMAGE).removeI(0, 1); }
	CString getScript() const			{ return npc->getClientCode(); }
	time_t getModTime() const			{ return modTime; }

private:
	TNPC* npc;
	time_t modTime;
};

#endif
