#include "CString.h"

#ifdef _WIN32
	#define strncasecmp _strnicmp
	#define snprintf _snprintf
#endif

/*
	Constructor ~ Deconstructor
*/

CString::CString(const char *pString)
{
	buffer = 0;

	int length = strlen(pString);
	clear(length);
	write(pString, length);
}

CString::CString(const CString& pString)
{
	buffer = 0;

	clear(pString.length());
	write(pString.text(), pString.length());
}

CString::CString(char pChar)
{
	buffer = 0;

	clear(sizeof(char));
	writeChar(pChar);
}

CString::CString(double pDouble)
{
	buffer = 0;

	char tempBuff[32];
	sprintf(tempBuff, "%f", pDouble);
	*this = tempBuff;
}

CString::CString(float pFloat)
{
	buffer = 0;

	char tempBuff[32];
	sprintf(tempBuff, "%f", pFloat);
	*this = tempBuff;
}

CString::CString(int pInteger)
{
	buffer = 0;

	char tempBuff[32];
	sprintf(tempBuff, "%i", pInteger);
	*this = tempBuff;
}

CString::CString(unsigned int pUInteger)
{
	buffer = 0;

	char tempBuff[32];
	sprintf(tempBuff, "%u", pUInteger);
	*this = tempBuff;
}

CString::CString(unsigned long int pUInteger)
{
	buffer = 0;

	char tempBuff[32];
	sprintf(tempBuff, "%lu", pUInteger);
	*this = tempBuff;
}

CString::~CString()
{
	free(buffer);
}

/*
	Data-Management
*/

bool CString::load(const CString& pString)
{
	char buff[65535];
	FILE *file = 0;
	if ((file = fopen(pString.text(), "rb")) == 0)
		return false;

	int size = 0;
	clear();
	while ((size = (int)fread(buff, 1, sizeof(buff), file)) > 0)
		write(buff, size);
	fclose(file);
	return true;
}

bool CString::save(const CString& pString) const
{
	FILE *file = 0;
	if ((file = fopen(pString.text(), "wb")) == 0)
		return false;
	fwrite(buffer, 1, sizec, file);
	fclose(file);
	return true;
}

CString CString::readChars(int pLength)
{
	retVal.clear();
	pLength = clip(pLength, 0, sizec - readc);
	retVal.write(&buffer[readc], pLength);
	readc += pLength;
	return retVal;
}

CString CString::readString(const CString& pString)
{
	retVal.clear();
	if (readc > sizec) return retVal;

	int len;
	len = (pString.isEmpty() ? -1 : find(pString, readc) - readc);
	len = (len < 0 ? bytesLeft() : len);
	len = (len > bytesLeft() ? bytesLeft() : len); //
	retVal.write(&buffer[readc], len);

	// If len was set to bytesLeft(), it MIGHT have pString.
	int len2;
	if ((len2 = retVal.find(pString)) != -1)
		retVal.removeI(len2, pString.length());

	readc += len + pString.length();
	//readc = (readc > sizec ? sizec : readc); // GO
	return retVal;
}

int CString::read(char *pDest, int pSize)
{
	int length = (sizec - readc < pSize ? sizec - readc : pSize);
	if (length <= 0)
	{
		memset((void*)pDest, 0, pSize);
		return 0;
	}
	memcpy((void*)pDest, (void*)&buffer[readc], length);
	readc += length;
	return length;
}

int CString::write(const char *pSrc, int pSize)
{
	if (!pSize)
		return 0;

	if (writec + pSize >= buffc)
	{
		buffc = writec + pSize + 10 + (sizec / 3);
		buffer = (char*)realloc(buffer, buffc);
	}

	memcpy(&buffer[writec], pSrc, pSize);
	writec += pSize;
	sizec = (writec > sizec ? writec : sizec);
	buffer[sizec] = 0;
	return pSize;
}

int CString::write(const CString& pString)
{
	return write(pString.text(), pString.length());
}

void CString::clear(int pSize)
{
	free(buffer);
	sizec = readc = writec = 0;
	buffc = (pSize > 0 ? pSize : 1) + 1;
	buffer = (char*)malloc(buffc);
	buffer[0] = 0;
}

/*
	Functions
*/

CString CString::escape() const
{
	retVal.clear();

	for (int i = 0; i < length(); i++)
	{
		if (buffer[i] == '\\')
			retVal << "\\\\";
		else if (buffer[i] == '\"')
			retVal << "\"\"";
		else if (buffer[i] == '\'')
			retVal << "\'\'";
		else
			retVal << buffer[i];
	}

	return retVal;
}

CString CString::left(int pLength) const
{
	return subString(0, pLength);
}

CString CString::right(int pLength) const
{
	return subString(length() - pLength, pLength);
}

CString CString::remove(int pStart, int pLength) const
{
	retVal = *this;
	if (pLength < 1 || pStart < 0)
		return retVal;
	if (pStart >= sizec)
		return retVal;

	pLength = clip(pLength, 0, retVal.length()-pStart);
	memmove(retVal.text() + pStart, retVal.text() + pStart + pLength, retVal.length() - pStart - (pLength - 1));
	retVal.setSize(retVal.length() - pLength);
	retVal[retVal.length()] = 0;
	return retVal;
}

CString CString::subString(int pStart, int pLength) const
{
	// Read Entire String?
	if (pLength == -1)
		pLength = length();

	retVal.clear();
	pStart = clip(pStart, 0, length());
	pLength = clip(pLength, 0, length()-pStart);

	if (pLength > 0)
		retVal.write(&buffer[pStart], pLength);
	return retVal;
}

CString CString::toLower() const
{
	retVal = *this;
	for (int i = 0; i < retVal.length(); i++)
	{
		if (in(retVal[i], 'A', 'Z'))
			retVal[i] += 32;
	}

	return retVal;
}

CString CString::toUpper() const
{
	retVal = *this;
	for (int i = 0; i < retVal.length(); i++)
	{
		if (in(retVal[i], 'a', 'z'))
			retVal[i] -= 32;
	}

	return retVal;
}

CString CString::trim() const
{
	return trimLeft().trimRight();
}

CString CString::trimLeft() const
{
	for (int i = 0; i < length(); ++i)
	{
		if (buffer[i] > ' ')
			return subString(i, length() - i);
	}

	return CString(*this);
}

CString CString::trimRight() const
{
	for (int i = length() - 1; i >= 0; --i)
	{
		if (buffer[i] > ' ')
			return subString(0, i+1);
	}

	return CString(*this);
}

CString CString::bzcompress() const
{
	retVal.clear();
	char buf[65536];
	int error = 0;
	unsigned int clen = sizeof(buf);

	if ((error = BZ2_bzBuffToBuffCompress(buf, &clen, buffer, length(), 1, 0, 30)) != BZ_OK)
		return retVal;

	retVal.write(buf, clen);
	return retVal;
}

CString CString::bzuncompress() const
{
	retVal.clear();
	char buf[65536];
	int error = 0;
	unsigned int clen = sizeof(buf);

	if ((error = BZ2_bzBuffToBuffDecompress(buf, &clen, buffer, length(), 0, 0)) != BZ_OK)
		return retVal;

	retVal.write(buf, clen);
	return retVal;
}

CString CString::zcompress() const
{
	retVal.clear();
	char buf[65536];
	int error = 0;
	unsigned long clen = sizeof(buf);

	if ((error = compress((Bytef *)buf, (uLongf *)&clen, (const Bytef *)buffer, length())) != Z_OK)
		return retVal;

	retVal.write(buf, clen);
	return retVal;
}

CString CString::zuncompress() const
{
	retVal.clear();
	char buf[65536];
	int error = 0;
	unsigned long clen = sizeof(buf);

	if ((error = uncompress((Bytef *)buf, (uLongf *)&clen, (const Bytef *)buffer, length())) != Z_OK)
		return retVal;

	retVal.write(buf, clen);
	return retVal;
}

int CString::find(const CString& pString, int pStart) const
{
	char* loc = strstr(buffer + pStart, pString.text());
	if (loc == 0) return -1;
	return (int)(loc - buffer);
}

int CString::findi(const CString& pString, int pStart) const
{
	for (int i = pStart; i <= length() - pString.length(); ++i)
	{
		if (strncasecmp(&buffer[i], pString.text(), pString.length()) == 0)
			return i;
	}
	return -1;
}

int CString::findl(char pChar) const
{
	char* loc = strrchr(buffer, (int)pChar);
	if (loc == 0) return -1;
	return (int)(loc - buffer);
}

std::vector<CString> CString::tokenize(const CString& pString) const
{
	retVal = *this;
	std::vector<CString> strList;
	char *tok = strtok(retVal.text(), pString.text());

	while (tok != 0)
	{
		strList.push_back(tok);
		tok = strtok(0, pString.text());
	}

	return strList;
}

std::vector<CString> CString::loadToken(const CString& pFile, const CString& pToken)
{
	CString fileData;
	if (!fileData.load(pFile))
		return std::vector<CString>();
	return fileData.tokenize(pToken);
}

/*
	Operators
*/
CString& CString::operator=(const CString& pString)
{
	if (this == &pString)
		return *this;

	clear(pString.length());
	write((char*)pString.text(), pString.length());
	return *this;
}

CString& CString::operator<<(const CString& pString)
{
	if (this == &pString)
		return *this << CString(pString);

	write(pString.text(), pString.length());
	return *this;
}

CString& CString::operator+=(const CString& pString)
{
	return *this << pString;
}

CString CString::operator+(const CString& pString)
{
	return CString(*this) << pString;
}

char& CString::operator[](int pIndex)
{
	return buffer[pIndex];
}

bool operator==(const CString& pString1, const CString& pString2)
{
	if (pString1.length() == pString2.length())
	{
		if (memcmp(pString1.text(), pString2.text(), pString1.length()) == 0)
			return true;
	}

	return false;
}

bool operator!=(const CString& pString1, const CString& pString2)
{
	return !(pString1 == pString2);
}

bool operator<(const CString& pString1, const CString& pString2)
{
	int len = (pString1.length() > pString2.length() ? pString2.length() : pString1.length());
	return memcmp(pString1.text(), pString2.text(), len) < 0;
}

bool operator>(const CString& pString1, const CString& pString2)
{
	int len = (pString1.length() > pString2.length() ? pString2.length() : pString1.length());
	return memcmp(pString1.text(), pString2.text(), len) > 0;
}

bool operator<=(const CString& pString1, const CString& pString2)
{
	int len = (pString1.length() > pString2.length() ? pString2.length() : pString1.length());
	return memcmp(pString1.text(), pString2.text(), len) <= 0;
}

bool operator>=(const CString& pString1, const CString& pString2)
{
	int len = (pString1.length() > pString2.length() ? pString2.length() : pString1.length());
	return memcmp(pString1.text(), pString2.text(), len) >= 0;
}

CString operator+(const CString& pString1, const CString& pString2)
{
	return CString(pString1) << pString2;
}

/*
	Additional Functions for Data-Packing
*/
CString& CString::writeChar(const char pData)
{
	write((char*)&pData, 1);
	return *this;
}

CString& CString::writeShort(const short pData)
{
	char val[2];
	val[0] = ((pData >> 8) & 0xFF);
	val[1] = (pData & 0xFF);
	write((char*)&val, 2);
	return *this;
}

CString& CString::writeInt(const int pData)
{
	char val[4];
	val[0] = ((pData >> 24) & 0xFF);
	val[1] = ((pData >> 16) & 0xFF);
	val[2] = ((pData >> 8) & 0xFF);
	val[3] = (pData & 0xFF);
	write((char *)&val, 4);
	return *this;
}

char CString::readChar()
{
	char val;
	read(&val, 1);
	return val;
}

short CString::readShort()
{
	unsigned char val[2];
	read((char*)val, 2);
	return (val[0] << 8) + val[1];
}

int CString::readInt()
{
	unsigned char val[4];
	read((char*)val, 4);
	return (val[0] << 24) + (val[1] << 16) + (val[2] << 8) + val[3];
}

/*
	Additional Functions for Data-Packing (GRAAL)
*/
CString& CString::writeGChar(const char pData)
{
	char val = pData+32;
	write((char*)&val, 1);
	return *this;
}

CString& CString::writeGShort(const short pData)
{
	char val[2];
	val[0] = ((pData >> 7) & 0x7F)+32;
	val[1] = (pData & 0x7F)+32;
	write((char*)&val, 2);
	return *this;
}

CString& CString::writeGInt(const int pData)
{
	char val[3];
	val[0] = ((pData >> 14) & 0x7F)+32;
	val[1] = ((pData >> 7) & 0x7F)+32;
	val[2] = (pData & 0x7F)+32;
	write((char *)&val, 3);
	return *this;
}

CString& CString::writeGInt4(const int pData)
{
	char val[4];
	val[0] = ((pData >> 21) & 0x7F)+32;
	val[1] = ((pData >> 14) & 0x7F)+32;
	val[2] = ((pData >> 7) & 0x7F)+32;
	val[3] = (pData & 0x7F)+32;
	write((char *)&val, 4);
	return *this;
}

CString& CString::writeGInt5(const long long pData)
{
	char val[5];
	val[0] = ((pData >> 28) & 0x7F)+32;
	val[1] = ((pData >> 21) & 0x7F)+32;
	val[2] = ((pData >> 14) & 0x7F)+32;
	val[3] = ((pData >> 7) & 0x7F)+32;
	val[4] = (pData & 0x7F)+32;
	write((char *)&val, 5);
	return *this;
}

char CString::readGChar()
{
	char val;
	read(&val, 1);
	return val-32;
}

short CString::readGShort()
{
	unsigned char val[2];
	read((char*)val, 2);
	return ((val[0]-32) << 7) + val[1]-32;
}

int CString::readGInt()
{
	unsigned char val[3];
	read((char*)val, 3);
	return ((val[0]-32) << 14) + ((val[1]-32) << 7) + val[2]-32;
}

int CString::readGInt4()
{
	unsigned char val[4];
	read((char*)val, 4);
	return ((val[0]-32) << 21) + ((val[1]-32) << 14) + ((val[2]-32) << 7) + val[3]-32;
}

int CString::readGInt5()
{
	unsigned char val[5];
	read((char*)val, 5);
	return ((val[0]-32) << 28) + ((val[1]-32) << 21) + ((val[2]-32) << 14) + ((val[3]-32) << 7) + val[4]-32;
}

// 2002-05-07 by Markus Ewald
CString CString::B64_Encode()
{
	static const char *EncodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//	CString retVal;
	retVal.clear();

	for (int i = 0; i < sizec; i++)
	{
		char pCode;

		pCode = (buffer[i] >> 2) & 0x3f;
		retVal.writeChar(EncodeTable[pCode]);

		pCode = (buffer[i] << 4) & 0x3f;
		if (i++ < sizec)
			pCode |= (buffer[i] >> 4) & 0x0f;
		retVal.writeChar(EncodeTable[pCode]);

		if (i < sizec)
		{
			pCode = (buffer[i] << 2) & 0x3f;
			if (i++ < sizec)
				pCode |= (buffer[i] >> 6) & 0x03;
			retVal.writeChar(EncodeTable[pCode]);
		}
		else
		{
			i++;
			retVal.writeChar('=');
		}

		if (i < sizec)
		{
			pCode = buffer[i] & 0x3f;
			retVal.writeChar(EncodeTable[pCode]);
		}
		else
		{
			retVal.writeChar('=');
		}
	}

	return retVal;
}

CString CString::B64_Decode()
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

	//CString retVal;
	retVal.clear();

	for (int i = 0; i < sizec; i++)
	{
		unsigned char c1, c2;

		c1 = (char)DecodeTable[(unsigned char)buffer[i]];
		i++;
		c2 = (char)DecodeTable[(unsigned char)buffer[i]];
		c1 = (c1 << 2) | ((c2 >> 4) & 0x3);
		retVal.writeChar(c1);

		if (i++ < sizec)
		{
			c1 = buffer[i];
			if (c1 == '=')
				break;

			c1 = (char)DecodeTable[(unsigned char)buffer[i]];
			c2 = ((c2 << 4) & 0xf0) | ((c1 >> 2) & 0xf);
			retVal.writeChar(c2);
		}

		if (i++ < sizec)
		{
			c2 = buffer[i];
			if (c2 == '=')
				break;

			c2 = (char)DecodeTable[(unsigned char)buffer[i]];
			c1 = ((c1 << 6) & 0xc0) | c2;
			retVal.writeChar(c1);
		}
	}

	return retVal;
}

/*
	Friend Functions
*/
CString getExtension(const CString& pStr)
{
	int pos = pStr.findl('.');
	if (pos >= 0)
		return pStr.subString(pos);
	return CString();
}
