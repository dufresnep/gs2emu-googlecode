// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CWeapon.h"

CPacket CWeapon::getSendData()
{
    CPacket pOut;
    pOut << (char)name.length() << name << (char)0 << (char)image.length() << image << (char)1 << (short)code.length() << code;
    return pOut;
}
