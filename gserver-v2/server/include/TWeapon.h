#ifndef TWEAPON_H
#define TWEAPON_H

#include <time.h>
#include "TNPC.h"
#include "CString.h"

class TWeapon
{
public:
	TWeapon(const char id) : npc(0), modTime(0), defaultWeapon(true), defaultWeaponId(id) {}
	TWeapon(TNPC* pNPC) : npc(pNPC), modTime(time(0)), defaultWeapon(false), defaultWeaponId(-1) {}

	CString getWeaponPacket() const;

	char getWeaponId() const			{ return defaultWeaponId; }
	time_t getModTime() const			{ return modTime; }
	CString getName() const;
	CString getImage() const;
	CString getScript() const;

private:
	TNPC* npc;
	time_t modTime;
	bool defaultWeapon;
	char defaultWeaponId;
};

#endif
