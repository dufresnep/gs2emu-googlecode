#include "CBuffer.h"


CBuffer CBuffer::retBuffer(20);
CBuffer::CBuffer(const char* pStr)
{
	if(pStr != NULL)
		dataLength = (int)strlen(pStr)+10;
    data = (char*)malloc(dataLength+1);
    data[0] = count = readPos = writePos = 0;
	if(pStr != NULL)
		writeChars(pStr);
}

CBuffer::CBuffer(int pSize)
{
    if(pSize <= 0)
        pSize = 10;
    dataLength = pSize;
    data = (char*)malloc(dataLength+1);
    data[0] = count = readPos = writePos = 0;
}

void CBuffer::clear()
{
    if(dataLength > 20)
	{
		dataLength = 20;
        free(data);
        data = (char*)malloc(dataLength+1);
    }
    data[0] = count = readPos = writePos = 0;
}

CBuffer& CBuffer::operator=(const CBuffer& pBuffer)
{
    if(this != &pBuffer)
    {
        clear();
        writeBuffer(pBuffer);
    }
    return *this;
}

CBuffer& CBuffer::operator=(const char* pStr)
{
    clear();
	if(pStr != NULL)
		writeChars(pStr);
    return *this;
}

CBuffer& CBuffer::operator+=(const char* pStr)
{
    writeChars(pStr);
    return *this;
}

CBuffer& CBuffer::operator+=(const CBuffer& pBuffer)
{
    writeBuffer(pBuffer);
    return *this;
}

CBuffer& CBuffer::operator<<(const CBuffer& pIn)
{
    writeBuffer(pIn);
    return *this;
}

CBuffer& CBuffer::operator<<(const char* pIn)
{
    writeChars(pIn);
    return *this;
}

CBuffer& CBuffer::operator+(const CBuffer& pOther)
{
    retBuffer.clear();
    retBuffer.writeBuffer(*this);
    retBuffer.writeBuffer(pOther);
    return retBuffer;
}

int CBuffer::find(char pChar, int pStart)
{
    if(pStart < 0)
        pStart = 0;
    for(int i = pStart; i < count; i++)
    {
        if(data[i] == pChar)
            return i;
    }
    return -1;
}

int CBuffer::find(const char* pString, int pStart)
{
    if(pStart < 0)
        pStart = 0;
    int len = (int)strlen(pString);
    for(int i = pStart; i < count; i++)
    {
        if(i + len > count)
            break;
        for(int ii = 0; ii < len; ii++)
        {
            if(data[i + ii] != pString[ii])
                break;
            if(ii == len-1)
                return i;
        }
    }
    return -1;
}

int CBuffer::findToken(const char* pTokens, int pStart)
{
	CString tokens = pTokens;
	for(int i = pStart; i < count; i++)
    {
        if(tokens.find(data[i]) >= 0)
			return i;
    }
    return -1;
}

void CBuffer::resize(int pSize)
{
    if(pSize >= dataLength)
    {
        dataLength = pSize + 20;
		data = (char*)realloc((void*)data, dataLength+1);
    }

}

bool CBuffer::save(const char* pFileName)
{
    FILE* fHandle = fopen(pFileName, "wb");
    if(fHandle == 0)
        return false;
    fwrite(data, 1, count, fHandle);
    fclose(fHandle);
    return true;
}
bool CBuffer::load(const char* pFileName)
{
	char buffer[60000];
    FILE* fHandle = fopen(pFileName, "rb");
    if(fHandle == 0)
        return false;
	clear();
	int size = 0;
	while((size = fread(buffer, 1, sizeof(buffer), fHandle)) > 0)
		writeBytes(buffer, size);
    fclose(fHandle);
    return true;
}

int CBuffer::writeBytes(const char* pBytes, int pLength)
{
    if(pLength <= 0)
        return 0;

    resize(writePos + pLength);
    memcpy(&data[writePos], pBytes, pLength);
    writePos += pLength;
    count = __MAX(writePos, count);
    clipCounters();
    data[count] = 0;
    return pLength;
}

void CBuffer::readBytes(const char* pOut, int pCount)
{
    int len =__MIN(bytesLeft(), pCount);
    if(len <= 0)
	{
		memset((void*)pOut, 0, pCount);
        return;
	}

    memcpy((void*)pOut, (void*)&data[readPos],len);
    readPos += len;
}

unsigned char CBuffer::readByte()
{
    unsigned char retVal;
    readBytes((char*)&retVal, sizeof(unsigned char));
    return retVal;
}

char CBuffer::readChar()
{
    char retVal;
    readBytes((char*)&retVal, sizeof(char));
    return retVal;
}

short CBuffer::readShort()
{
    short retVal;
    readBytes((char*)&retVal, sizeof(short));
    return retVal;
}

unsigned short CBuffer::readuShort()
{
    unsigned short retVal;
    readBytes((char*)&retVal, sizeof(unsigned short));
    return retVal;
}

int CBuffer::readInt()
{
    int retVal;
    readBytes((char*)&retVal, sizeof(int));
    return retVal;
}

unsigned int CBuffer::readuInt()
{
    unsigned int retVal;
    readBytes((char*)&retVal, sizeof(unsigned int));
    return retVal;
}

float CBuffer::readFloat()
{
    float retVal;
    readBytes((char*)&retVal, sizeof(float));
    return retVal;
}

long long CBuffer::readLong()
{
    long long retVal;
    readBytes((char*)&retVal, sizeof(long long));
    return retVal;
}

unsigned long long CBuffer::readuLong()
{
    unsigned long long retVal;
    readBytes((char*)&retVal, sizeof(unsigned long long));
    return retVal;
}

double CBuffer::readDouble()
{
    double retVal;
    readBytes((char*)&retVal, sizeof(double));
    return retVal;
}

char* CBuffer::readString()
{
    retBuffer.clear();
    int len = 0;
    for(int i = readPos; i < count; i++)
    {
        len++;
        if(data[i] == '\0')
        {
            retBuffer.writeBytes(&data[readPos], len);
            readPos += len;
            clipCounters();
            break;
        }
    }
    return retBuffer.text();
}

char* CBuffer::readChars(int pLength)
{
    retBuffer.clear();
    if(pLength > 0)
    {
        pLength = __MIN(bytesLeft(), pLength);
        retBuffer.writeBytes(&data[readPos], pLength);
        readPos += pLength;
		clipCounters();
    }
    return retBuffer.text();
}

char* CBuffer::readString(const char* pSep)
{
    retBuffer.clear();
    int len;
    if(pSep[0] != 0)
    {
        len = find(pSep, readPos) - readPos;
        if(len < 0)
            len = bytesLeft();
    } else len = bytesLeft();

    retBuffer.writeBytes(&data[readPos], len);
    readPos += len + (int)strlen(pSep);
    clipCounters();
    return retBuffer.text();
}

CBuffer CBuffer::copy(int pStart, int pLength)
{
	CBuffer retVal;
    if(pStart < count)
        retVal.writeBytes(data + pStart, pLength);
    return retVal;
}

CBuffer CBuffer::copy(int pStart)
{
	CBuffer retVal;
    if(pStart < count)
        retVal.writeBytes(data + pStart, count - pStart );
    return retVal;
}

CBuffer CBuffer::escapeStr()
{
    CBuffer retVal;
    for(int i = 0; i < strlen(data); i++)
    {
        char letter = data[i];
        if(letter == '\'')
            retVal << "\'\'";
        else
            retVal.writeChar(letter);
    }
    return retVal;
}

CBuffer& CBuffer::trimLeft()
{
    for(int i = 0; i < count; i++)
    {
        if(data[i] > ' ')
        {
            if(i > 0)
            {
                count -= i;
                memmove(data, &data[i], count);
                clipCounters();
                data[count] = 0;
            }
            break;
        }
    }

    return *this;
}

CBuffer& CBuffer::trimRight()
{
    for(int i = count-1; i >= 0; i--)
    {
        if(data[i] <= ' ')
            count--;
        else
        {
            data[count] = 0;
            break;
        }
    }
    clipCounters();
    return *this;
}

CBuffer& CBuffer::trim()
{
    trimRight();
    trimLeft();
    return *this;
}

CBuffer& CBuffer::toLower()
{
    for(int i = 0; i < count; i++)
    {
        if(data[i] >= 65 && data[i] <= 90)
            data[i] += 32;
    }
    return *this;
}

CBuffer& CBuffer::toUpper()
{
    for(int i = 0; i < count; i++)
    {
        if(data[i] >= 97 && data[i] <= 122)
            data[i] -= 32;
    }
    return *this;
}

CBuffer& CBuffer::remove(int pStart, int pCount)
{
    if(pStart + pCount > count || pStart < 0 || pCount <= 0)
        return *this;
    memcpy(data + pStart, data + pStart + pCount, count - pStart - pCount);
    count -= pCount;
    clipCounters();
    data[count] = 0;
    return *this;
}

CBuffer& CBuffer::removeAll(const char* pStr)
{
    int pos = 0;
    while(1)
    {
        pos = find(pStr, pos);
        if(pos >= 0)
            remove(pos, (int)strlen(pStr));
        else break;
    }
    return *this;
}

CBuffer& CBuffer::insert(int pPos, const char* pStr)
{
    CBuffer buff = copy(0, pPos);
    buff += pStr;
    buff.writeBytes(data + pPos, count - pPos);
    return (*this = buff);
}

CBuffer& CBuffer::replace(const char* pString, const char* pNewString)
{
    int pos = find(pString, 0);
    if(pos < 0)
        return *this;
    remove(pos, (int)strlen(pString));
    insert(pos, pNewString);
    return *this;
}

CBuffer& CBuffer::replaceAll(const char* pString, const char* pNewString)
{
    int pos = 0;
    int len = (int)strlen(pString);
    int len2 = (int)strlen(pNewString);
    while(1)
    {
        pos = find(pString, pos);
        if(pos < 0)
            break;
        remove(pos, len);
        insert(pos, pNewString);
        pos += len2;
    }
    return *this;
}


bool CBuffer::match(const char* pMask)
{
	char *cp = 0, *mp = 0;
	char* string = data;
	char* wild = (char*)pMask;
	while ((*string) && (*wild != '*')) {
		if((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}
	while (*string) {
		if (*wild == '*') {
			if(!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}
	while (*wild == '*') {
		wild++;
	}
	return !*wild;
}

void CBuffer::clipCounters()
{
    count = __MIN(dataLength, count);
    readPos = __MIN(readPos, count);
    writePos = __MIN(writePos, count);
}

//Adler32
unsigned int CBuffer::checkSum()
{
    char*p = data;
	unsigned int len = count;
	unsigned int a = 1, b = 0;
	while(len)
	{
		unsigned tlen = len > 5550 ? 5550 : len;
		len -= tlen;
		do
		{
			a += *p++;
			b += a;
		} while (--tlen);
		a = (a & 0xffff) + (a >> 16) * (65536-65521);
		b = (b & 0xffff) + (b >> 16) * (65536-65521);
	}
	if(a >= 65521)
		a -= 65521;
	b = (b & 0xffff) + (b >> 16) * (65536-65521);
	if(b >= 65521)
		b -= 65521;
	return b << 16 | a;
}


