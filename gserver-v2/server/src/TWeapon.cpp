#include "TWeapon.h"

CString TWeapon::getWeaponPacket() const
{
	return CString() >> (char)getName().length() << getName()
		>> (char)0 >> (char)getImage().length() << getImage()
		>> (char)1 >> (short)getScript().length() << getScript();
}
