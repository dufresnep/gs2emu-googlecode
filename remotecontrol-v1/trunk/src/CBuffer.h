/* GraalReborn Server
    $Id: CBuffer.h 203 2008-03-31 22:36:26Z loneboco $
 (C) GraalReborn 2007 */

#ifndef HCBUFFER
#define HCBUFFER

#include <cstdlib>
#include <cstring>
#include <cstdio>
#define __MIN(a, b) (a < b ? a : b)
#define __MAX(a, b) (a > b ? a : b)

#if !(defined(WIN32) || defined(WIN64))
	#define stricmp		strcasecmp
#endif

class CBuffer
{
	char* data;
	int dataLength, count;
	int readPos, writePos;

	void clipCounters();

	public:
	static CBuffer retBuffer;
	CBuffer(const char* pStr);
	CBuffer(int pSize = 10);
	CBuffer(const CBuffer& pBuffer)
	{
		dataLength = pBuffer.count;
		data = (char*)malloc(dataLength+1);
		data[0] = count = readPos = writePos = 0;
		writeBuffer(pBuffer);
	}
	~CBuffer();

	//Writing
	int writeBytes(const char* pBytes, int pLength);
	int writeByte(unsigned char pByte);
	int writeChar(char pChar);
	int writeShort(short pShort);
	int writeuShort(unsigned short pShort);
	int writeInt(int pInt);
	int writeuInt(unsigned int pInt);
	int writeFloat(float pFloat);
	int writeLong(long long pLong);
	int writeuLong(unsigned long long pLong);
	int writeDouble(double pDouble);
	int writeChars(const char* pChars);
	int writeString(const char* pString);
	int writeBuffer(const CBuffer& pBuffer);

	//Reading
	void readBytes(const char* pOut, int pCount);
	unsigned char readByte();
	char readChar();
	short readShort();
	unsigned short readuShort();
	int readInt();
	unsigned int readuInt();
	float readFloat();
	long long readLong();
	unsigned long long readuLong();
	double readDouble();
	char* readChars(int pLength);
	char* readLine();
	char* readString();
	char* readString(const char* pSep);

	//Misc
	void clear();
	void resize(int pSize);
	int compare(const CBuffer& pBuffer);
	int compare(const char* pStr);
	int find(char pChar, int pStart = 0);
	int find(const char* pString, int pStart = 0);
	int findl(char pChar);
	int findToken(const char* pTokens, int pStart = 0);
	bool save(const char* pFileName);
	bool load(const char* pFileName);
	void setWrite(int pPos);
	void setRead(int pPos);
	int getRead();
	bool match(const char* pMask);
	unsigned int checkSum();
	CBuffer& trimLeft();
	CBuffer& trimRight();
	CBuffer& trim();
	CBuffer& toLower();
	CBuffer& toUpper();
	CBuffer& remove(int pStart, int pCount);
	CBuffer& remove(const char* pStr);
	CBuffer& removeAll(const char* pStr);
	CBuffer& insert(int pPos, const char* pStr);
	CBuffer& replace(int pPos, char pChar);
	CBuffer& replace(const char* pString, const char* pNewString);
	CBuffer& replaceAll(const char* pString, const char* pNewString);
	CBuffer& tokenize();
	CBuffer& untokenize();
	CBuffer copy(int pStart, int pLength);
	CBuffer copy(int pStart);
	CBuffer escapeStr();
	CBuffer B64_Encode();
	CBuffer B64_Decode();
	char* text();
	int bytesLeft();
	int length();
	bool isNumber();

	//Operators
	CBuffer& operator=(const CBuffer& pBuffer);
	CBuffer& operator=(const char* pStr);
	CBuffer& operator+=(const CBuffer& pBuffer);
	CBuffer& operator+=(const char* pStr);
	CBuffer& operator<<(const CBuffer& pIn);
	CBuffer& operator<<(const char* pIn);
	CBuffer& operator+(const CBuffer& pOther);
	bool operator==(const CBuffer& pBuffer);
	bool operator==(const char* pStr);
	bool operator<(const CBuffer& pBuffer);
	bool operator<=(const CBuffer& pBuffer);
	bool operator<(const char* pStr);
	bool operator<=(const char* pStr);
	bool operator>(const CBuffer& pBuffer);
	bool operator>=(const CBuffer& pBuffer);
	bool operator>(const char* pStr);
	bool operator>=(const char* pStr);
	bool operator!=(const CBuffer& pBuffer);
	bool operator!=(const char* pStr);
	char operator[](int pPos);

};

typedef CBuffer CString;

inline int CBuffer::getRead()
{
	return readPos;
}

inline int CBuffer::writeByte(unsigned char pByte)
{
	return writeBytes((char*)&pByte, sizeof(unsigned char));
}

inline int CBuffer::writeChar(char pChar)
{
	return writeBytes((char*)&pChar, sizeof(char));
}

inline int CBuffer::writeShort(short pShort)
{
	return writeBytes((char*)&pShort, sizeof(short));
}

inline int CBuffer::writeuShort(unsigned short pShort)
{
	return writeBytes((char*)&pShort, sizeof(unsigned short));
}

inline int CBuffer::writeInt(int pInt)
{
	return writeBytes((char*)&pInt, sizeof(int));
}

inline int CBuffer::writeuInt(unsigned int pInt)
{
	return writeBytes((char*)&pInt, sizeof(unsigned int));
}

inline int CBuffer::writeFloat(float pFloat)
{
	return writeBytes((char*)&pFloat, sizeof(float));
}

inline int CBuffer::writeLong(long long pLong)
{
	return writeBytes((char*)&pLong, sizeof(long long));
}

inline int CBuffer::writeuLong(unsigned long long pLong)
{
	return writeBytes((char*)&pLong, sizeof(unsigned long long));
}

inline int CBuffer::writeDouble(double pDouble)
{
	return writeBytes((char*)&pDouble, sizeof(double));
}

inline int CBuffer::writeChars(const char* pChars)
{
	return writeBytes(pChars, (int)strlen(pChars));
}

inline int CBuffer::writeString(const char* pString)
{
	return writeBytes(pString, (int)strlen(pString)+1);
}


inline void CBuffer::setWrite(int pPos)
{
	writePos = pPos;
}

inline void CBuffer::setRead(int pPos)
{
	readPos = pPos;
}

inline int CBuffer::bytesLeft()
{
	if(count-readPos < 0)
		return 0;
	return count-readPos;
}

inline char* CBuffer::text()
{
	return data;
}

inline bool CBuffer::operator!=(const CBuffer& pBuffer)
{
	return (compare(pBuffer) != 0);
}

inline bool CBuffer::operator==(const CBuffer& pBuffer)
{
	return (compare(pBuffer) == 0);
}

inline bool CBuffer::operator!=(const char* pStr)
{
	return (compare(pStr) != 0);
}

inline int CBuffer::compare(const CBuffer& pBuffer)
{
	if(count < pBuffer.count)
		return -1;
	if(count > pBuffer.count)
		return 1;
	return memcmp(data, pBuffer.data, count);
}

inline int CBuffer::compare(const char* pStr)
{
	return strcmp(data, pStr);
}

inline bool CBuffer::operator==(const char* pStr)
{
	return (compare(pStr) == 0);
}

inline bool CBuffer::operator<(const CBuffer& pBuffer)
{
	return (compare(pBuffer) < 0);
}

inline bool CBuffer::operator<=(const CBuffer& pBuffer)
{
	return (compare(pBuffer) <= 0);
}

inline bool CBuffer::operator<(const char* pStr)
{
	return (compare(pStr) < 0);
}

inline bool CBuffer::operator<=(const char* pStr)
{
	return (compare(pStr) <= 0);
}

inline bool CBuffer::operator>(const CBuffer& pBuffer)
{
	return (compare(pBuffer) > 0);
}

inline bool CBuffer::operator>=(const CBuffer& pBuffer)
{
	return (compare(pBuffer) >= 0);
}

inline bool CBuffer::operator>(const char* pStr)
{
	return (compare(pStr) > 0);
}

inline bool CBuffer::operator>=(const char* pStr)
{
	return (compare(pStr) >= 0);
}

inline char CBuffer::operator[](int pPos)
{
	if(pPos < 0 || pPos >= count)
		return 0;
	return data[pPos];
}

inline int CBuffer::length()
{
	return count;
}

inline int CBuffer::writeBuffer(const CBuffer& pBuffer)
{
	return writeBytes(pBuffer.data, pBuffer.count);
}

inline char* CBuffer::readLine()
{
	return readString("\n");
}

inline CBuffer& CBuffer::remove(const char* pStr)
{
	return remove(find(pStr), (int)strlen(pStr));
}

inline CBuffer::~CBuffer()
{
	free(data);
}

inline CString toString(int pValue)
{
	CString retVal;
	char buffer[30];
	sprintf(buffer, "%i", pValue);
	retVal = buffer;
	return retVal;
}

inline CString toString(float pValue)
{
	CString retVal;
	char buffer[30];
	sprintf(buffer, "%2.2f", pValue);
	retVal = buffer;
	return retVal;
}

inline CString toString(unsigned long pValue)
{
	CString retVal;
	char buffer[30];
	sprintf(buffer, "%lu", pValue);
	retVal = buffer;
	return retVal;
}


#endif // HCBUFFER
