#include <vector>
#include "ICommon.h"
#include "main.h"
#include "TPlayer.h"
#include "TAccount.h"
#include "TLevel.h"

#define serverlog	server->getServerLog()
#define rclog		server->getRCLog()
extern bool __sendLocal[propscount];
extern int __attrPackets[30];

/*
	TPlayer: Prop-Manipulation
*/
CString TPlayer::getProp(int pPropId)
{
	CSettings* settings = &(server->getSettings());
	switch (pPropId)
	{
		case PLPROP_NICKNAME:
		return CString() >> (char)nickName.length() << nickName;

		case PLPROP_MAXPOWER:
		return CString() >> (char)maxPower;

		case PLPROP_CURPOWER:
		return CString() >> (char)(power * 2);

		case PLPROP_RUPEESCOUNT:
		return CString() >> (int)gralatc;

		case PLPROP_ARROWSCOUNT:
		return CString() >> (char)arrowc;

		case PLPROP_BOMBSCOUNT:
		return CString() >> (char)bombc;

		case PLPROP_GLOVEPOWER:
		return CString() >> (char)glovePower;

		case PLPROP_BOMBPOWER:
		return CString() >> (char)bombPower;

		case PLPROP_SWORDPOWER:
		return CString() >> (char)(swordPower+30) >> (char)swordImg.length() << swordImg;

		case PLPROP_SHIELDPOWER:
		return CString() >> (char)(shieldPower+10) >> (char)shieldImg.length() << shieldImg;

		case PLPROP_GANI:
		return CString() >> (char)gAni.length() << gAni;

		case PLPROP_HEADGIF:
		return CString() >> (char)(headImg.length()+100) << headImg;

		case PLPROP_CURCHAT:
		return CString() >> (char)chatMsg.length() << chatMsg;

		case PLPROP_COLORS:
		return CString() >> (char)colors[0] >> (char)colors[1] >> (char)colors[2] >> (char)colors[3] >> (char)colors[4];

		case PLPROP_ID:
		return CString() >> (short)id;

		case PLPROP_X:
		return CString() >> (char)(x * 2);

		case PLPROP_Y:
		return CString() >> (char)(y * 2);

		case PLPROP_Z:
		return CString() >> (char)(z * 2);

		case PLPROP_SPRITE:
		return CString() >> (char)sprite;

		case PLPROP_STATUS:
		return CString() >> (char)status;

		case PLPROP_CARRYSPRITE:
		return CString() >> (char)carrySprite;

		case PLPROP_CURLEVEL:
		if (type == CLIENTTYPE_CLIENT)
			return CString() >> (char)levelName.length() << levelName;
		else
			return CString() >> (char)1 << " ";

		case PLPROP_HORSEGIF:
		return CString() >> (char)horseImg.length() << horseImg;

		case PLPROP_HORSEBUSHES:
		return CString() >> (char)horsec;

		case PLPROP_EFFECTCOLORS:
		return CString() >> (char)0;

		case PLPROP_CARRYNPC:
		return CString() >> (int)0;

		case PLPROP_APCOUNTER:
		return CString() >> (short)(apCounter+1);

		case PLPROP_MAGICPOINTS:
		return CString() >> (char)mp;

		case PLPROP_KILLSCOUNT:
		return CString() >> (int)kills;

		case PLPROP_DEATHSCOUNT:
		return CString() >> (int)deaths;

		case PLPROP_ONLINESECS:
		return CString() >> (int)onlineTime;

		case PLPROP_LASTIP:
		return CString().writeGInt5(accountIp);

		case PLPROP_UDPPORT:
		return CString() >> (int)udpport;

		case PLPROP_ALIGNMENT:
		return CString() >> (char)ap;

		case PLPROP_ADDITFLAGS:
		return CString() >> (char)additionalFlags;

		case PLPROP_ACCOUNTNAME:
		return CString() >> (char)accountName.length() << accountName;

		case PLPROP_BODYIMG:
		return CString() >> (char)bodyImg.length() << bodyImg;

		case PLPROP_RATING:
		{
			int temp = (((int)rating & 0xFFF) << 9) | ((int)deviation & 0x1FF);
			return CString() >> (int)temp;
		}

		// Simplifies login.
		// Manually send prop if you are leaving the level.
		// 1 = join level, 0 = leave level.
		case PLPROP_JOINLEAVELVL:
		return CString() >> (char)1;

		case PLPROP_PCONNECTED:
		return CString();

		case PLPROP_PLANGUAGE:
		return CString() >> (char)language.length() << language;

		case PLPROP_PSTATUSMSG:
			// TODO: statusList.
		//if (statusMsg > statusList.count() - 1)  retVal.writeByte1(0);
		return CString() >> (char)0;

		// OS type.
		// Windows: wind
		case PLPROP_OSTYPE:
		return CString() >> (char)os.length() << os;

		// Text codepage.
		// Example: 1252
		case PLPROP_TEXTCODEPAGE:
		return CString().writeGInt(codepage);

		case PLPROP_GMAPX:
		{
			unsigned short val = (gmapx < 0 ? -gmapx : gmapx);
			val <<= 1;
			if (gmapx < 0) val |= 0x0001;
			return CString().writeGShort(val);
		}

		case PLPROP_GMAPY:
		{
			unsigned short val = (gmapy < 0 ? -gmapy : gmapy);
			val <<= 1;
			if (gmapy < 0) val |= 0x0001;
			return CString().writeGShort((short)val);
		}

		case PLPROP_GMAPLEVELX:
		return CString() >> (char)gmaplevelx;

		case PLPROP_GMAPLEVELY:
		return CString() >> (char)gmaplevely;
	}

	if (inrange(pPropId, 37, 41) || inrange(pPropId, 46, 49) || inrange(pPropId, 54, 74))
	{
		for (unsigned int i = 0; i < sizeof(__attrPackets) / sizeof(int); i++)
		{
			if (__attrPackets[i] == pPropId)
				return CString() >> (char)attrList[i].length() << attrList[i];
		}
	}

	return CString();
}

void TPlayer::setProps(CString& pPacket, bool pForward, bool pForwardToSelf)
{
	CSettings* settings = &(server->getSettings());
	CString globalBuff, levelBuff, selfBuff;
	int len = 0;
/*
	printf("\n");
	printf("%s\n", pPacket.text());
	for (int i = 0; i < pPacket.length(); ++i)
		printf("%02x ", (unsigned char)((pPacket.text())[i]));
	printf("\n");
*/
	while (pPacket.bytesLeft() > 0)
	{
		unsigned char propId = pPacket.readGUChar();
		//printf("ID: %u\n", propId);
		switch (propId)
		{
			case PLPROP_NICKNAME:
				len = pPacket.readGUChar();
				len = clip(len, 0, 224);
				setNick(pPacket.readChars(len));
				globalBuff >> (char)propId << getProp(propId);
			break;

			case PLPROP_MAXPOWER:
				maxPower = pPacket.readGUChar();
				maxPower = clip(maxPower, 0, settings->getInt("heartlimit", 20));
				maxPower = clip(maxPower, 0, 20);
			break;

			case PLPROP_CURPOWER:
				power = (float)pPacket.readGUChar() / 2;
				power = clip(power, 0, (float)maxPower);
			break;

			case PLPROP_RUPEESCOUNT:
				gralatc = pPacket.readGUInt();
				gralatc = clip(gralatc, 0, 9999999);
			break;

			case PLPROP_ARROWSCOUNT:
				arrowc = pPacket.readGUChar();
				arrowc = clip(arrowc, 0, 99);
			break;

			case PLPROP_BOMBSCOUNT:
				bombc = pPacket.readGUChar();
				bombc = clip(bombc, 0, 99);
			break;

			case PLPROP_GLOVEPOWER:
				glovePower = pPacket.readGUChar();
				glovePower = clip(glovePower, 0, 3);
			break;

			case PLPROP_BOMBPOWER:
				bombPower = pPacket.readGUChar();
				bombPower = clip(bombPower, 0, 3);
			break;

			case PLPROP_SWORDPOWER:
			{
				int sp = pPacket.readGUChar();
				if (sp <= 4)
					swordImg = CString() << "sword" << CString(sp) << ".png";
				else
				{
					sp -= 30;
					len = pPacket.readGUChar();
					if (len > 0)
					{
						// TODO: foldersconfig stuff.
						swordImg = pPacket.readChars(len);
					}
					else swordImg = "";
				}
				swordPower = clip(sp, ((settings->getBool("healswords", false) == true) ? -(settings->getInt("swordlimit", 3)) : 0), settings->getInt("swordlimit", 3));
			}
			break;

			case PLPROP_SHIELDPOWER:
			{
				int sp = pPacket.readGUChar();
				if (sp <= 3)
					shieldImg = CString() << "shield" << CString(sp) << ".png";
				else
				{
					sp -= 10;
					if (sp < 0) break;
					len = pPacket.readGUChar();
					if (len > 0)
					{
						// TODO: foldersconfig stuff.
						shieldImg = pPacket.readChars(len);
					}
					else shieldImg = "";
				}
				shieldPower = clip(sp, 0, settings->getInt("shieldlimit", 3));
			}
			break;

			case PLPROP_GANI:
				gAni = pPacket.readChars(pPacket.readGUChar());
			break;

			case PLPROP_HEADGIF:
				len = pPacket.readGUChar();
				if (len < 100)
				{
					headImg = CString() << "head" << CString(len) << ".png";
					globalBuff >> (char)propId << getProp(propId);
				}
				else if (len > 100)
				{
					// TODO: Foldersconfig stuff.
					headImg = pPacket.readChars(len-100);
					globalBuff >> (char)propId << getProp(propId);
				}
			break;

			case PLPROP_CURCHAT:
				len = pPacket.readGUChar();
				len = clip(len, 0, 220);
				chatMsg = pPacket.readChars(len);
				//processChat(chatMsg);
			break;

			case PLPROP_COLORS:
				for (unsigned int i = 0; i < sizeof(colors) / sizeof(unsigned char); ++i)
					colors[i] = pPacket.readGUChar();
			break;

			case PLPROP_ID:
				pPacket.readGUShort();
			break;

			case PLPROP_X:
				x = (float)(pPacket.readGUChar() / 2);
				status &= (-1-1);
				lastMovement = time(0);
			break;

			case PLPROP_Y:
				y = (float)(pPacket.readGUChar() / 2);
				status &= (-1-1);
				lastMovement = time(0);
			break;

			case PLPROP_Z:
				z = (float)(pPacket.readGUChar() / 2);
				status &= (-1-1);
				lastMovement = time(0);
			break;

			case PLPROP_SPRITE:
				sprite = pPacket.readGUChar();
			break;

			case PLPROP_STATUS:
			{
				int oldStatus = status;
				status = pPacket.readGUChar();

				if (id == -1) break;

				// When they come back to life, give them hearts.
				if ((oldStatus & 8) > 0 && (status & 8) == 0)
				{
					power = clip((ap < 20 ? 3 : (ap < 40 ? 5 : maxPower)), 0.0f, maxPower);
					// TODO: send level
					//if (level->players.count() == 1)
						//sendLevel(level->fileName, this->x, this->y, getSysTime());
				}

				// When they die, increase deaths and make somebody else level leader.
				if ((oldStatus & 8) == 0 && (status & 8) > 0)
				{
					// TODO: all this.
					/*
					if(!level->sparZone)
						deaths++;
					if (level->players.count() > 1 && level->players[0] == this)
					{
						level->players.remove(0);
						level->players.add(this);
						((CPlayer*)level->players[0])->sendPacket(CPacket() << (char)ISLEADER);
					}
					*/
				}
			}
			break;

			case PLPROP_CARRYSPRITE:
				carrySprite = pPacket.readGUChar();
			break;

			case PLPROP_CURLEVEL:
				len = pPacket.readGUChar();
				if (len >= 0)
					levelName = pPacket.readChars(len);
			break;

			case PLPROP_HORSEGIF:
				len = pPacket.readGUChar();
				if (len >= 0)
				{
					CString temp(pPacket.readChars(len));
					// TODO: foldersconfig
					//if (noFoldersConfig || isValidFile(temp, -1))
						horseImg = temp;
				}
			break;

			case PLPROP_HORSEBUSHES:
				horsec = pPacket.readGUChar();
			break;

			case PLPROP_EFFECTCOLORS:
				len = pPacket.readGUChar();
				if (len > 0)
					pPacket.readGInt4();
			break;

			case PLPROP_CARRYNPC:
				pPacket.readGInt();
			break;

			case PLPROP_APCOUNTER:
				apCounter = pPacket.readGUShort();
			break;

			case PLPROP_MAGICPOINTS:
				mp = pPacket.readGUChar();
				mp = clip(mp, 0, 100);
			break;

			case PLPROP_KILLSCOUNT:
				pPacket.readGInt();
			break;

			case PLPROP_DEATHSCOUNT:
				pPacket.readGInt();
			break;

			case PLPROP_ONLINESECS:
				pPacket.readGInt();
			break;

			case PLPROP_LASTIP:
				pPacket.readGInt5();
			break;

			case PLPROP_UDPPORT:
				udpport = pPacket.readGInt();
				// TODO: udp support.
			break;

			case PLPROP_ALIGNMENT:
				ap = pPacket.readGUChar();
				ap = clip(ap, 0, 100);
			break;

			case PLPROP_ADDITFLAGS:
				additionalFlags = pPacket.readGUChar();
			break;

			case PLPROP_ACCOUNTNAME:
				len = pPacket.readGUChar();
				if (len >= 0)
					pPacket.readChars(len);
			break;

			case PLPROP_BODYIMG:
				len = pPacket.readGUChar();
				if (len >= 0)
					bodyImg = pPacket.readChars(len);
				// TODO: foldersconfig
			break;

			case PLPROP_RATING:
				pPacket.readGInt();
			break;
/*
			case PLPROP_UNKNOWN42:
				break;

			case PLPROP_UNKNOWN50:
				break;
*/
			case PLPROP_PCONNECTED:
			break;

			case PLPROP_PLANGUAGE:
				len = pPacket.readGUChar();
				if (len >= 0)
					language = pPacket.readChars(len);
			break;

			case PLPROP_PSTATUSMSG:
				statusMsg = pPacket.readGUChar();
				if (id == -1)
					break;

				server->sendPacketToAll(CString() >> (char)PLO_OTHERPLPROPS >> (short)id >> (char)PLPROP_PSTATUSMSG >> (char)statusMsg, this);
			break;

			case PLPROP_GATTRIB1:  attrList[0]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB2:  attrList[1]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB3:  attrList[2]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB4:  attrList[3]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB5:  attrList[4]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB6:  attrList[5]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB7:  attrList[6]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB8:  attrList[7]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB9:  attrList[8]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB10: attrList[9]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB11: attrList[10] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB12: attrList[11] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB13: attrList[12] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB14: attrList[13] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB15: attrList[14] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB16: attrList[15] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB17: attrList[16] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB18: attrList[17] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB19: attrList[18] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB20: attrList[19] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB21: attrList[20] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB22: attrList[21] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB23: attrList[22] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB24: attrList[23] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB25: attrList[24] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB26: attrList[25] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB27: attrList[26] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB28: attrList[27] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB29: attrList[28] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB30: attrList[29] = pPacket.readChars(pPacket.readGUChar()); break;

			// OS type.
			// Windows: wind
			case PLPROP_OSTYPE:
				len = pPacket.readGUChar();
				os = pPacket.readChars(len);
				break;

			// Text codepage.
			// Example: 1252
			case PLPROP_TEXTCODEPAGE:
				codepage = pPacket.readGInt();
				break;

			// Location, in pixels, of the player on the GMap.
			// Bit 0x0001 controls if it is negative or not.
			// Bits 0xFFFE are the actual value.
			case PLPROP_GMAPX:
				gmapx = len = pPacket.readGUShort();
				lastMovement = time(0);

				// If the first bit is 1, our position is negative.
				gmapx >>= 1;
				if ((short)len & 0x0001)
					gmapx = -gmapx;
				printf( "gmap x: %d\n", gmapx );
				break;

			case PLPROP_GMAPY:
				gmapy = len = pPacket.readGUShort();
				lastMovement = time(0);

				// If the first bit is 1, our position is negative.
				gmapy >>= 1;
				if ((short)len & 0x0001)
					gmapy = -gmapy;
				printf( "gmap y: %d\n", gmapy );
				break;

			case PLPROP_GMAPLEVELX:
				gmaplevelx = pPacket.readGUChar();
				printf( "gmap level x: %d\n", gmaplevelx );
				break;

			case PLPROP_GMAPLEVELY:
				gmaplevely = pPacket.readGUChar();
				printf( "gmap level y: %d\n", gmaplevely );
				break;

			default:
			{
				printf("Unidentified PLPROP: %i, readPos: %d\n", propId, pPacket.readPos());
				for (int i = 0; i < pPacket.length(); ++i)
					printf("%02x ", (unsigned char)pPacket[i]);
				printf("\n");
			}
			return;
		}

		if (pForward && __sendLocal[propId] == true)
			levelBuff >> (char)propId << getProp(propId);

		if (pForwardToSelf)
			selfBuff >> (char)propId << getProp(propId);
	}

	// Send Buffers Out
	if (isLoggedIn())
	{
		if (globalBuff.length() > 0)
			server->sendPacketToAll(CString() >> (char)PLO_OTHERPLPROPS >> (short)this->id << globalBuff, this);
		if (levelBuff.length() > 0)
			server->sendPacketToLevel(CString() >> (char)PLO_OTHERPLPROPS >> (short)this->id << levelBuff, getLevel(), this);
		if (selfBuff.length() > 0)
			sendPacket(CString() >> (char)PLO_PLAYERPROPS << selfBuff);
		sendCompress();
	}
}

void TPlayer::sendProps(bool *pProps, int pCount)
{
	// Definition
	CString propPacket;

	// Create Props
	for (int i = 0; i < pCount; ++i)
	{
		if (pProps[i])
			propPacket >> (char)i << getProp(i);
	}

	// Send Packet
	sendPacket(CString() >> (char)PLO_PLAYERPROPS << propPacket);
}

CString TPlayer::getProps(bool *pProps, int pCount)
{
	CString propPacket;

	// Start the prop packet.
	propPacket >> (char)PLO_OTHERPLPROPS >> (short)this->id;

	if (pCount > 0)
	{
		// Check if PLPROP_JOINLEAVELVL is set.
		if (pProps[PLPROP_JOINLEAVELVL])
			propPacket >> (char)PLPROP_JOINLEAVELVL >> (char)1;

		// Create Props
		for (int i = 0; i < pCount; ++i)
		{
			if (i == PLPROP_JOINLEAVELVL) continue;
			if (pProps[i])
				propPacket >> (char)i << getProp(i);
		}
	}

	return propPacket;
}
