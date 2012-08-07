/* GraalReborn Server
    $Id: CPacket.cpp 167 2008-03-04 06:32:02Z agret@graalforge.com $
 (C) GraalReborn 2007 */

#include "CPacket.h"

int CPacket::writeByte1(char pByte)
{
	char val = pByte + 32;
	return writeBytes(&val, 1);
}

int CPacket::writeByte2(short pByte2)
{
	char val[2];
	val[0] = (((pByte2 >> 7) & 0x7F)+32);
	val[1] = ((pByte2 & 0x7F)+32);
	return writeBytes(val, 2);
}

int CPacket::writeByte3(int pByte3)
{
	char val[3];
	val[0] = (((pByte3 >> 14)&0x7F)+32);
	val[1] =(((pByte3>>7) & 0x7F)+32);
	val[2] =((pByte3 & 0x7F)+32);

	return writeBytes(val, 3);
}

int CPacket::writeByte4(int pByte4)
{
	char val[4];
	val[0] =(((pByte4 >> 21) & 0x7F)+32);
	val[1] =(((pByte4 >> 14) & 0x7F)+32);
	val[2] = (((pByte4 >> 7) & 0x7F)+32);
	val[3] =((pByte4 & 0x7F)+32);
	return writeBytes(val, 4);
}

int CPacket::writeByte5(long long pByte5)
{
	char val[5];
	val[0] = (char)(((pByte5 >> 28) & 0x7F)+32);
	val[1] = (char)(((pByte5 >> 21) & 0x7F)+32);
	val[2] = (char)(((pByte5 >> 14) & 0x7F)+32);
	val[3] = (char)(((pByte5 >> 7) & 0x7F)+32);
	val[4] = (char)((pByte5 & 0x7F)+32);
	return writeBytes(val, 5);
}

