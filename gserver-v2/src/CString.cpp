#include "CString.h"

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
	FILE *file = NULL;
	if ((file = fopen(pString.text(), "rb")) == NULL)
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
	FILE *file = NULL;
	if ((file = fopen(pString.text(), "wb")) == NULL)
		return false;
	fwrite(buffer, 1, sizec, file);
	fclose(file);
	return true;
}

CString CString::readChars(int pLength)
{
	retVal.clear();
	pLength = clip(pLength, 0, sizec);
	retVal.write(&buffer[readc], pLength);
	readc += pLength;
	return retVal;
}

CString CString::readString(const CString& pString)
{
	retVal.clear();
	int len;
	len = (pString.isEmpty() ? -1 : find(pString, readc) - readc);
	len = (len < 0 ? bytesLeft() : len);
	retVal.write(&buffer[readc], len);
	readc += len + pString.length();
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
	if (pStart + pLength > retVal.length()  || pLength < 1 || pStart < 0)
		return retVal;

	memmove(retVal.text() + pStart, retVal.text() + pStart + pLength, retVal.length() - pStart - pLength);
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
	CString retVal = *this;
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
	if (pString.length() == 1)
	{
		for (int i = pStart; i < length(); i++)
		{
			if (buffer[i] == pString.text()[0])
				return i;
		}
	}
	else
	{
		for (int i = pStart; i < length() - pString.length(); ++i)
		{
			if (strncmp(&buffer[i], pString.text(), pString.length()) == 0)
				return i;
		}
	}

	return -1;
}


int CString::findl(char pChar) const
{
	for (int i = length(); i > 0; i--)
	{
		if (buffer[i] == pChar)
			return i;
	}

	return -1;
}

std::vector<CString> CString::tokenize(const CString& pString) const
{
	retVal = *this;
	std::vector<CString> strList;
	char *tok = strtok(retVal.text(), pString.text());

	while (tok != NULL)
	{
		strList.push_back(tok);
		tok = strtok(NULL, pString.text());
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
	char retVal;
	read(&retVal, 1);
	return retVal;
}

short CString::readShort()
{
	unsigned char retVal[2];
	read((char*)retVal, 2);
	return (retVal[0] << 8) + retVal[1];
}

int CString::readInt()
{
	unsigned char retVal[4];
	read((char*)retVal, 4);
	return (retVal[0] << 24) + (retVal[1] << 16) + (retVal[2] << 8) + retVal[3];
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
	char val[4];
	val[0] = ((pData >> 21) & 0x7F)+32;
	val[1] = ((pData >> 14) & 0x7F)+32;
	val[2] = ((pData >> 7) & 0x7F)+32;
	val[3] = (pData & 0x7F)+32;
	write((char *)&val, 4);
	return *this;
}

CString& CString::writeGInt3(const int pData)
{
	char val[3];
	val[0] = ((pData >> 14) & 0x7F)+32;
	val[1] = ((pData >> 7) & 0x7F)+32;
	val[2] = (pData & 0x7F)+32;
	write((char *)&val, 3);
	return *this;
}

CString& CString::writeGInt5(const int pData)
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
	char retVal;
	read(&retVal, 1);
	return retVal-32;
}

short CString::readGShort()
{
	unsigned char retVal[2];
	read((char*)retVal, 2);
	return ((retVal[0]-32) << 7) + retVal[1]-32;
}

int CString::readGInt()
{
	unsigned char retVal[4];
	read((char*)retVal, 4);
	return ((retVal[0]-32) << 21) + ((retVal[1]-32) << 14) + ((retVal[2]-32) << 7) + retVal[3]-32;
}

int CString::readGInt3()
{
	unsigned char retVal[3];
	read((char*)retVal, 3);
	return ((retVal[0]-32) << 14) + ((retVal[1]-32) << 7) + retVal[2]-32;
}

int CString::readGInt5()
{
	unsigned char retVal[5];
	read((char*)retVal, 5);
	return ((retVal[0]-32) << 28) + ((retVal[1]-32) << 21) + ((retVal[2]-32) << 14) + ((retVal[3]-32) << 7) + retVal[4]-32;
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
