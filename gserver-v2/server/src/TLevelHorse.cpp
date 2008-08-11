#include "TLevelHorse.h"

CString TLevelHorse::getHorseStr() const
{
	return CString() >> (char)x >> (char)y << image;
}
