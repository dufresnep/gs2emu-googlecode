/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include "main.h"
#include "CWordFilter.h"

/* Never was great formulating =P */
bool CWordFilter::apply(CPlayer *pPlayer, CBuffer &pBuffer, int pCheck)
{
	bool logsave = false, rctell = false;
	CBuffer start;
	CStringList found;
	int pos = 0, wc = 0;

	if ( pPlayer == 0 ) return false;

	for (int i = 0; i < WordList.count(); i++)
	{
		WordMatch *word = (WordMatch *)WordList[i];
		if ( word == 0 ) continue;
		if (!word->check[pCheck]) continue;

		for (int j = 0; j < pBuffer.length(); j++)
		{
			for (int k = 0; k < word->match.length(); k++)
			{
				char c1 = pBuffer[j + k];
				char c2 = word->match[k];
				if (c2 != '?' && (isUpper(c2) && c2 != c1) || (isLower(c2) && toLower(c2) != toLower(c1)))
				{
					if (wc >= word->precision)
					{
						found.add(start);

						for (int l = 0; l < (int)sizeof(word->action); l++)
						{
							if (!word->action[l])
								continue;

							switch (l)
							{
								case FILTERA_LOG:
									if (logsave)
										break;

									logsave = true;
									if (pPlayer != NULL)
										errorOut("wordfilter.txt", CBuffer() << pPlayer->accountName << " has used rude words while chatting: " << start);
								break;

								case FILTERA_REPLACE:
									pos = pBuffer.find(' ', j);
									pos = (pos == -1 ? start.length() : pos-j+1);
									for (int m = 0; m < pos; m++)
										pBuffer.replace(j + m, '*');
								break;

								case FILTERA_TELLRC:
									if (rctell)
										break;

									rctell = true;
									if (pPlayer != NULL)
										sendRCPacket(CPacket() << (char)DRCLOG << pPlayer->accountName << " has used rude words while chatting: " << start);
								break;

								case FILTERA_WARN:
									pBuffer = (word->warnmessage.length() > 0 ? word->warnmessage : warnmessage);
								break;

								case FILTERA_JAIL: // kinda useless...?
								break;

								case FILTERA_BAN:
									if (pPlayer != NULL)
									{
										CBuffer pLog = CBuffer() << "\n" << getTimeStr(0) << "\n" << pPlayer->accountName << " has used rude words while chatting: " << start;
										pPlayer->setBan(pLog, true);
									}
								break;
							}
						}
					}

					start.clear();
					wc = 0;
					break;
				}

				start.writeChar(c1);
				wc++;
			}
		}
	}

	return (found.count() > 0);
}

bool CWordFilter::load(char *pFile)
{
	for (int i = 0; i < WordList.count(); i++)
		delete (WordMatch *)WordList[i];
	WordList.clear();
	CStringList lines;
	lines.load(pFile);
	if(lines.count() < 1)
		return false;

	for (int i = 0; i < lines.count(); i++)
	{
		CStringList words;
		words.load(lines[i].text(), " ");
		if (words.count() <= 0)
			continue;

		if (words[0] == "WARNMESSAGE")
		{
			words.remove(0);
			warnmessage = words.join(" ");
		}
			else if (words[0] == "SHOWWORDSTORC")
		{
			showtorc = CHECK_BOOL(words[1].text());
		}
			else if (words[0] == "RULE")
		{
			WordMatch *word = new WordMatch();
			memset(word->check, false, sizeof(word->check));
			memset(word->action, false, sizeof(word->action));

			for (i++; i < lines.count() && lines[i] != "RULEEND"; i++)
			{
				words.load(lines[i].text(), " ");
				if (words.count() <= 0)
					continue;

				if (words[0] == "ACTION" && words.count() >= 2)
				{
					for (int ii = 1; ii < words.count(); ii++)
					{
						if (words[ii] == "ban") word->action[FILTERA_BAN] = true;
						else if (words[ii] == "jail") word->action[FILTERA_JAIL] = true;
						else if (words[ii] == "log") word->action[FILTERA_LOG] = true;
						else if (words[ii] == "replace") word->action[FILTERA_REPLACE] = true;
						else if (words[ii] == "tellrc") word->action[FILTERA_TELLRC] = true;
						else if (words[ii] == "warn") word->action[FILTERA_WARN] = true;
					}
				}
					else if (words[0] == "CHECK" && words.count() >= 2)
				{
					for (int ii = 1; ii < words.count(); ii++)
					{
						if (words[ii] == "chat") word->check[FILTERC_CHAT] = true;
						else if (words[ii] == "nick") word->check[FILTERC_NICK] = true;
						else if (words[ii] == "pm") word->check[FILTERC_PM] = true;
						else if (words[ii] == "toall") word->check[FILTERC_TOALL] = true;
					}
				}
					else if (words[0] == "MATCH" && words.count() >= 2)
				{
					words.remove(0);
					word->match = words.join(" ");
				}
					else if (words[0] == "PRECISION" && words.count() == 2)
				{
					word->precision = (words[1].find('%') >= 0 ? word->match.length() * atoi(words[1].text()) / 100 : atoi(words[1].text()));
				}
					else if (words[0] == "WORDPOSITION" && words.count() == 2)
				{
					if (words[1] == "full") word->position = FILTERP_FULL;
					else if (words[1] == "part") word->position = FILTERP_PART;
					else if (words[1] == "start") word->position = FILTERP_START;
				}
					else if (words[0] == "WARNMESSAGE" && words.count() >= 2)
				{
					words.remove(0);
					word->warnmessage = words.join(" ");
				}
			}

			if (word->precision <= 0)
				word->precision = word->match.length();

			WordList.add(word);
		}
	}

	return true;
}
