/* GraalReborn Server
    $Id: CBuffer.cpp 287 2008-05-01 06:18:55Z loneboco $
 (C) GraalReborn 2007 */

#include <vector>
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

int CBuffer::findl(char pChar)
{
	for(int i = count; i > 0; i--)
	{
		if(data[i] == pChar)
			return i;
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
	while((size = (int)fread(buffer, 1, sizeof(buffer), fHandle)) > 0)
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
	for(int i = 0; i < (int)strlen(data); i++)
	{
		char letter = data[i];
		if(letter == '\'')
			retVal << "\'\'";
		else
			retVal.writeChar(letter);
	}
	return retVal;
}

// 2002-05-07 by Markus Ewald
CBuffer CBuffer::B64_Encode()
{
	static const char *EncodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	CBuffer retVal;

	for (int i = 0; i < count; i++)
	{
		char pCode;

		pCode = (data[i] >> 2) & 0x3f;
		retVal.writeChar(EncodeTable[pCode]);

		pCode = (data[i] << 4) & 0x3f;
		if (i++ < count)
			pCode |= (data[i] >> 4) & 0x0f;
		retVal.writeChar(EncodeTable[pCode]);

		if (i < count)
		{
			pCode = (data[i] << 2) & 0x3f;
			if (i++ < count)
				pCode |= (data[i] >> 6) & 0x03;
			retVal.writeChar(EncodeTable[pCode]);
		}
			else
		{
			i++;
			retVal.writeChar('=');
		}

		if (i < count)
		{
			pCode = data[i] & 0x3f;
			retVal.writeChar(EncodeTable[pCode]);
		}
			else
		{
			retVal.writeChar('=');
		}
	}

	return retVal;
}

CBuffer CBuffer::B64_Decode()
{
	static const int DecodeTable[] = {
	// 0   1   2   3   4   5   6   7   8   9
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //   0 -   9
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //  10 -  19
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //  20 -  29
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //  30 -  39
	-1, -1, -1, 62, -1, -1, -1, 63, 52, 53,  //  40 -  49
	54, 55, 56, 57, 58, 59, 60, 61, -1, -1,  //  50 -  59
	-1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  //  60 -  69
	 5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  //  70 -  79
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  //  80 -  89
	25, -1, -1, -1, -1, -1, -1, 26, 27, 28,  //  90 -  99
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38,  // 100 - 109
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48,  // 110 - 119
	49, 50, 51, -1, -1, -1, -1, -1, -1, -1,  // 120 - 129
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 130 - 139
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 140 - 149
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 150 - 159
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 160 - 169
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 170 - 179
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 180 - 189
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 190 - 199
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 200 - 209
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 210 - 219
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 220 - 229
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 230 - 239
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 240 - 249
	-1, -1, -1, -1, -1, -1				   // 250 - 256
	};

	CBuffer retVal;

	for (int i = 0; i < count; i++)
	{
		unsigned char c1, c2;

		c1 = (char)DecodeTable[(unsigned char)data[i]];
		i++;
		c2 = (char)DecodeTable[(unsigned char)data[i]];
		c1 = (c1 << 2) | ((c2 >> 4) & 0x3);
		retVal.writeChar(c1);

		if (i++ < count)
		{
			c1 = data[i];
			if (c1 == '=')
				break;

			c1 = (char)DecodeTable[(unsigned char)data[i]];
			c2 = ((c2 << 4) & 0xf0) | ((c1 >> 2) & 0xf);
			retVal.writeChar(c2);
		}

		if (i++ < count)
		{
			c2 = data[i];
			if (c2 == '=')
				break;

			c2 = (char)DecodeTable[(unsigned char)data[i]];
			c1 = ((c1 << 6) & 0xc0) | c2;
			retVal.writeChar(c1);
		}
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
	memmove(data + pStart, data + pStart + pCount, count - pStart - pCount);
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

CBuffer& CBuffer::replace(int pPos, char pChar)
{
	if(pPos < 0 || pPos > count)
		return *this;
	data[pPos-1] = pChar;
	return *this;
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

/* Best I could do to immitate graal's stupid tokenize - Joey */
/* Don't need to immitate it perfectly.  Rely on Graal's stupidness.  Put quotations around everything! */
CBuffer& CBuffer::tokenize()
{
	CBuffer retVal;
	int pos[] = { 0, 0 };

	// Add a trailing \n to the line if one doesn't already exist.
	if ( data[ count - 1 ] != '\n' ) writeChar( '\n' );

	// Do the tokenization stuff.
	while ( (pos[0] = find( "\n", pos[1] )) != -1 )
	{
		CBuffer temp;
		temp << copy( pos[1], pos[0] - pos[1] );
		temp.replaceAll( "\"", "\"\"" );	// Change all " to ""
		temp.removeAll( "\r" );
		if ( temp.length() > 0 )
			retVal << "\"" << temp << "\",";
		else
			retVal << ",";
		pos[1] = pos[0] + 1;
	}

	// Write it back to the buffer now killing the trailing comma.
	clear();
	writeBuffer(retVal.remove(retVal.length() - 1, 1));
	return *this;
}

CBuffer& CBuffer::untokenize()
{
	std::vector<CBuffer> temp;
	int pos[] = { 0, 1 };

	// Copy the buffer data to a working copy and trim it.
	CBuffer nData( data );
	nData.trim();

	// Check to see if it starts with a quotation mark.  If not, set pos[1] to 0.
	if ( nData[0] != '\"' ) pos[1] = 0;

	// Untokenize.
	while (	(pos[0] = nData.find( ",", pos[1] )) != -1  )
	{
		// Empty blocks are blank lines.
		if ( pos[0] == pos[1] )
		{
			pos[1]++;
			temp.push_back( "\r" );		// Ugly as hell.  Workaround strtok() limitation.
			continue;
		}

		// Check for ,,"""blah"
		if ( nData[pos[1]] == '\"' && nData[pos[1]+1] != '\"' )
		{
			// Check to make sure it isn't ,"",
			if ( !(pos[1] + 2 < nData.length() && nData[pos[1]+2] == ',') )
				pos[1]++;
		}

		// Check and see if the comma is outside or inside of the thing string.
		// If pos[1] points to a quotation mark we have to find the closing quotation mark.
		if ( pos[1] > 0 && nData[pos[1] - 1] == '\"' )
		{
			while ( true )
			{
				if ( pos[0] == -1 ) break;
				if ((nData[pos[0]-1] != '\"') ||
					(nData[pos[0]-1] == '\"' && nData[pos[0]-2] == '\"') )
					pos[0] = nData.find( ",", pos[0] + 1 );
				else
					break;
			}
		}

		// Exit out if we previously failed to find the end.
		if ( pos[0] == -1 ) break;

		// "test",test
		CString t2;
		if ( pos[0] > 0 && nData[ pos[0] - 1 ] == '\"' )
			t2 = nData.copy(pos[1], pos[0] - pos[1] - 1);
		else
			t2 = nData.copy(pos[1], pos[0] - pos[1]);

		// Check if the string is valid and if it is, copy it.
		t2.replaceAll( "\"\"", "\"" );
		t2.removeAll( "\n" );
		t2.removeAll( "\r" );

		// Add it.
		temp.push_back( t2.text() );

		// Move forward the correct number of spaces.
		if ( pos[0] + 1 != nData.length() && nData[pos[0] + 1] == '\"' )
			pos[1] = pos[0] + (int)strlen( ",\"" );	// test,"test
		else
			pos[1] = pos[0] + (int)strlen( "," );	// test,test
	}

	// Try and grab the very last element.
	if ( pos[1] < nData.length() )
	{
		// If the end is a quotation mark, remove it.
		if ( nData[ nData.length() - 1 ] == '\"' )
			nData.remove( nData.length() - 1, 1 );

		// Sanity check.
		if ( pos[1] != nData.length() )
		{
			temp.push_back( nData.copy( pos[1] ).text() );
			int tempEnd = temp.size() - 1;
			temp[ tempEnd ].replaceAll( "\"\"", "\"" );	// Replace "" with "
			temp[ tempEnd ].removeAll( "\n" );
			temp[ tempEnd ].removeAll( "\r" );
		}
	}

	// Write the correct string out.
	clear();
	for ( int i = 0; i < (int)temp.size(); ++i )
		*this << (CBuffer)temp[i] << "\n";

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

bool CBuffer::isNumber()
{
	const char numbers[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };
	char periodCount = 0;
	for ( int i = 0; i < count; ++i )
	{
		bool isNum = false;
		for ( int j = 0; j < 11; ++j )
		{
			if ( data[i] == numbers[j] )
			{
				if ( j == 10 ) periodCount++;
				isNum = true;
				j = 11;
			}
		}
		if ( isNum == false || periodCount > 1 )
			return false;
	}
	return true;
}
