/* GraalReborn Server
    $Id: CPacket.h 167 2008-03-04 06:32:02Z agret@graalforge.com $
 (C) GraalReborn 2007 */

#ifndef CPACKETH
#define CPACKETH
#include "CBuffer.h"

//This class is an extension of CBuffer and allows
//You to read and write values in the format that
//the graal protocol and file types associated with
//graal (eg .graal files) use

class CPacket:
	public CBuffer
{
	public:
	int writeByte1(char pByte);
	int writeByte2(short pByte2);
	int writeByte3(int pByte3);
	int writeByte4(int pByte4);
	int writeByte5(long long pByte5);

	char readByte1();
	short readByte2();
	int readByte3();
	int readByte4();
	long long readByte5();

	unsigned char readByteU1();
	unsigned short readByteU2();
	unsigned int readByteU3();
	unsigned int readByteU4();
	unsigned long long readByteU5();

	CPacket& operator<<(char pByte);
	CPacket& operator<<(short pByte2);
	CPacket& operator<<(int pByte3);
	CPacket& operator<<(long long pByte5);

	CPacket& operator=(const CBuffer& pBuffer);
	CPacket& operator=(const char* pStr);
	CPacket& operator+=(const CBuffer& pBuffer);
	CPacket& operator+=(const char* pStr);
	CPacket& operator<<(const CBuffer& pIn);
	CPacket& operator<<(const char* pIn);
	CPacket& operator+(const CBuffer& pOther);
};

// Signed
inline char CPacket::readByte1()
{
	char retVal;
	readBytes(&retVal, 1);
	return retVal - 32;
}

inline short CPacket::readByte2()
{
	unsigned char retVal[2];
	readBytes((char*)retVal, 2);
	return ((retVal[0]-32) << 7) + (retVal[1]-32);
}

inline int CPacket::readByte3()
{
	unsigned char retVal[3];
	readBytes((char*)retVal, 3);
	return ((retVal[0]-32) << 14) + ((retVal[1]-32) << 7) + (retVal[2]-32);
}

inline int CPacket::readByte4()
{
	unsigned char retVal[4];
	readBytes((char*)retVal, 4);
	return ((retVal[0]-32) << 21) + ((retVal[1]-32) << 14) +((retVal[2]-32) << 7) + (retVal[3]-32);
}

inline long long CPacket::readByte5()
{
	unsigned char retVal[5];
	readBytes((char*)retVal, 5);
	return ((retVal[0]-32) << 28) + ((retVal[1]-32) << 21) + ((retVal[2]-32) << 14) +((retVal[3]-32) << 7) + (retVal[4]-32);
}

// Unsigned
inline unsigned char CPacket::readByteU1()
{
	char retVal;
	readBytes(&retVal, 1);
	return (unsigned char)(retVal - 32);
}

inline unsigned short CPacket::readByteU2()
{
	unsigned char retVal[2];
	readBytes((char*)retVal, 2);
	return (unsigned short)(((retVal[0]-32) << 7) + (retVal[1]-32));
}

inline unsigned int CPacket::readByteU3()
{
	unsigned char retVal[3];
	readBytes((char*)retVal, 3);
	return (unsigned int)(((retVal[0]-32) << 14) + ((retVal[1]-32) << 7) + (retVal[2]-32));
}

inline unsigned int CPacket::readByteU4()
{
	unsigned char retVal[4];
	readBytes((char*)retVal, 4);
	return (unsigned int)(((retVal[0]-32) << 21) + ((retVal[1]-32) << 14) +((retVal[2]-32) << 7) + (retVal[3]-32));
}

inline unsigned long long CPacket::readByteU5()
{
	unsigned char retVal[5];
	readBytes((char*)retVal, 5);
	return (unsigned long long)(((retVal[0]-32) << 28) + ((retVal[1]-32) << 21) + ((retVal[2]-32) << 14) +((retVal[3]-32) << 7) + (retVal[4]-32));
}

inline CPacket& CPacket::operator<<(char pByte)
{
	writeByte1(pByte);
	return *this;
}
inline CPacket& CPacket::operator<<(short pByte2)
{
	writeByte2(pByte2);
	return *this;
}
inline CPacket& CPacket::operator<<(int pByte3)
{
	writeByte3(pByte3);
	return *this;
}
inline CPacket& CPacket::operator<<(long long pByte5)
{
	writeByte5((int)pByte5);
	return *this;
}

inline CPacket& CPacket::operator=(const CBuffer& pBuffer)
{
	return (CPacket&)CBuffer::operator=(pBuffer);
}
inline CPacket& CPacket::operator=(const char* pStr)
{
	return (CPacket&)CBuffer::operator=(pStr);
}
inline CPacket& CPacket::operator+=(const CBuffer& pBuffer)
{
	return (CPacket&)CBuffer::operator+=(pBuffer);
}
inline CPacket& CPacket::operator+=(const char* pStr)
{
	return (CPacket&)CBuffer::operator+=(pStr);
}
inline CPacket& CPacket::operator<<(const CBuffer& pIn)
{
	return (CPacket&)CBuffer::operator<<(pIn);
}
inline CPacket& CPacket::operator<<(const char* pIn)
{
	return (CPacket&)CBuffer::operator<<(pIn);
}
inline CPacket& CPacket::operator+(const CBuffer& pOther)
{
	return (CPacket&)CBuffer::operator+(pOther);
}
#endif // CPACKETH

