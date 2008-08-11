#include "TWeapon.h"
#include "TLevelItem.h"
#include "TPlayer.h"

CString TWeapon::getWeaponPacket() const
{
	if (defaultWeapon)
		return CString() >> (char)PLO_DEFAULTWEAPON >> (char)defaultWeaponId;

	return CString() >> (char)PLO_NPCWEAPONADD
		>> (char)getName().length() << getName()
		>> (char)0 >> (char)getImage().length() << getImage()
		>> (char)1 >> (short)getScript().length() << getScript();
}

CString TWeapon::getName() const
{
	if (defaultWeapon)
		return TLevelItem::getItemName(defaultWeaponId);
	return npc->getWeaponName();
}

CString TWeapon::getImage() const
{
	if (npc == 0) return CString();
	return npc->getProp(NPCPROP_IMAGE).removeI(0, 1);
}

CString TWeapon::getScript() const
{
	if (npc == 0) return CString();
	return npc->getClientCode();
}
