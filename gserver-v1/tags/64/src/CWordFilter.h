/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#ifndef CWORDFILTER_H
#define CWORDFILTER_H

enum
{
	FILTERC_CHAT,
	FILTERC_NICK,
	FILTERC_PM,
	FILTERC_TOALL
};

enum
{
	FILTERA_BAN,
	FILTERA_JAIL,
	FILTERA_LOG,
	FILTERA_REPLACE,
	FILTERA_TELLRC,
	FILTERA_WARN
};

enum
{
	FILTERP_FULL,
	FILTERP_PART,
	FILTERP_START
};


inline bool isLower(char pChar)
{
	return (pChar >= 97 && pChar <= 122);
}

inline bool isUpper(char pChar)
{
	return (pChar >= 65 && pChar <= 90);
}

inline char toLower(char pChar)
{
	if (pChar >= 65 && pChar <= 90)
		return pChar+32;
	return pChar;
}

inline char toUpper(char pChar)
{
	if (pChar >= 97 && pChar <= 122)
		return pChar-32;
	return pChar;
}


struct WordMatch
{
	bool action[6], check[4];
	CBuffer match, warnmessage;
	int position, precision;
};

class CWordFilter
{
	public:
		bool apply(CPlayer *pPlayer, CBuffer &pBuffer, int pCheck);
		bool load(char *pFile);

	private:
		bool showtorc;
		CBuffer warnmessage;
		CList WordList;
};

#endif
