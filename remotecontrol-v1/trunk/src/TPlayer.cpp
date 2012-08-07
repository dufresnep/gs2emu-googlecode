#include "main.h"
#include "TPlayer.h"

/*
	Constructor / Destructor
*/

TPlayer::TPlayer(int pId)
{
	// Set Id
	plyrId = pId;
	playerItem = new QTreeWidgetItem();

	// Account-Attributes
	isBanned = isLoadOnly = false;
	acctRights = 0;
	acctFolders.clear();

	// Player-Attributes
	memset(plyrColors, 0, sizeof(plyrColors));

	plyrAp = 50;
	plyrBombPower = plyrGlovePower = plyrShieldPower = plyrSwordPower = 1;
	plyrBombs = 5;
	plyrDarts = 10;
	plyrDeaths = plyrGralats = plyrKills = plyrOnline = plyrStatus = 0;
	plyrMagic = 100;

	plyrMaxPower = plyrPower = 5.0f;
	plyrX = 30.5f;
	plyrY = 30.0f;
	plyrZ =  0.0f;
	plyrRating = 1500.0f;
	plyrDev = 350.0f;

	plyrChests.clear();
	plyrFlags.clear();
	plyrWeapons.clear();
}

TPlayer::~TPlayer()
{
	delete playerItem;
}

/*
	Functions: ...
*/

QTreeWidgetItem* TPlayer::getItem()
{
	return playerItem;
}

void TPlayer::updateItem()
{
	playerItem->setText(0, acctNick.text());
	playerItem->setText(1, acctName.text());
	playerItem->setText(2, plyrLevel.text());
	playerItem->setText(3, toString(plyrId).text());

	printf("update item\n");
}

/*
	Functions: Properties
*/

CPacket TPlayer::getProp(int pProp)
{
	CPacket retVal;

	switch (pProp)
	{
		case NICKNAME:
			retVal << (char)acctNick.length() << acctNick;
		break;

		case MAXPOWER:
		break;

		case CURPOWER:
		break;

		case RUPEESCOUNT:
		break;

		case ARROWSCOUNT:
		break;

		case BOMBSCOUNT:
		break;

		case GLOVEPOWER:
		break;

		case BOMBPOWER:
		break;

		case SWORDPOWER:
		break;

		case SHIELDPOWER:
		break;

		case PLAYERANI:
		break;

		case HEADGIF:
		break;

		case CURCHAT:
		break;

		case PLAYERCOLORS:
		break;

		case PLAYERID:
		break;

		case PLAYERX:
		break;

		case PLAYERY:
		break;

		case PLAYERSPRITE:
		break;

		case STATUS:
		break;

		case CARRYSPRITE:
		break;

		case CURLEVEL:
		break;

		case HORSEGIF:
		break;

		case HORSEBUSHES:
		break;

		case EFFECTCOLORS:
		break;

		case CARRYNPC:
		break;

		case APCOUNTER:
		break;

		case MAGICPOINTS:
		break;

		case KILLSCOUNT:
		break;

		case DEATHSCOUNT:
		break;

		case ONLINESECS:
		break;

		case LASTIP:
		break;

		case UDPPORT:
		break;

		case PALIGNMENT:
		break;

		case PADDITFLAGS:
		break;

		case PACCOUNTNAME:
		break;

		case BODYIMG:
		break;

		case RATING:
		break;

		case GATTRIB1:
		break;

		case GATTRIB2:
		break;

		case GATTRIB3:
		break;

		case GATTRIB4:
		break;

		case GATTRIB5:
		break;

		case PEMPTY42:
		break;

		case PEMPTY43:
		break;

		case PEMPTY44:
		break;

		case PLAYERZ:
		break;

		case GATTRIB6:
		break;

		case GATTRIB7:
		break;

		case GATTRIB8:
		break;

		case GATTRIB9:
		break;

		case PEMPTY50:
		break;

		case PCONNECTED:
		break;

		case PLANGUAGE:
		break;

		case PSTATUSMSG:
		break;

		case GATTRIB10:
		break;

		case GATTRIB11:
		break;

		case GATTRIB12:
		break;

		case GATTRIB13:
		break;

		case GATTRIB14:
		break;

		case GATTRIB15:
		break;

		case GATTRIB16:
		break;

		case GATTRIB17:
		break;

		case GATTRIB18:
		break;

		case GATTRIB19:
		break;

		case GATTRIB20:
		break;

		case GATTRIB21:
		break;

		case GATTRIB22:
		break;

		case GATTRIB23:
		break;

		case GATTRIB24:
		break;

		case GATTRIB25:
		break;

		case GATTRIB26:
		break;

		case GATTRIB27:
		break;

		case GATTRIB28:
		break;

		case GATTRIB29:
		break;

		case GATTRIB30:
		break;

		case UNKNOWN1:
		break;

		default:
		break;
	}

	return retVal;
}

void TPlayer::setProps(CPacket& pPacket)
{
	while (pPacket.bytesLeft())
	{
		int pProp = pPacket.readByteU1();

		switch (pProp)
		{
			case NICKNAME:
				acctNick = pPacket.readChars(pPacket.readByteU1());
			break;

			case MAXPOWER:
				plyrMaxPower = (float)pPacket.readByte1();
			break;

			case CURPOWER:
				plyrPower = (float)pPacket.readByte1()/2;
			break;

			case RUPEESCOUNT:
				plyrGralats = pPacket.readByteU3();
			break;

			case ARROWSCOUNT:
				plyrDarts = pPacket.readByte1();
			break;

			case BOMBSCOUNT:
				plyrBombs = pPacket.readByte1();
			break;

			case GLOVEPOWER:
				plyrGlovePower = pPacket.readByte1();
			break;

			case BOMBPOWER:
				plyrBombPower = pPacket.readByte1();
			break;

			case SWORDPOWER:
				plyrSwordPower = pPacket.readByte1()-30;
				plyrSword = pPacket.readChars(pPacket.readByteU1());
			break;

			case SHIELDPOWER:
				plyrShieldPower = pPacket.readByte1()-10;
				plyrShield = pPacket.readChars(pPacket.readByteU1());
			break;

			case PLAYERANI:
				plyrGani = pPacket.readChars(pPacket.readByteU1());
			break;

			case HEADGIF:
				plyrHead = pPacket.readChars(pPacket.readByteU1()-100);
			break;

			case CURCHAT:
				pPacket.readChars(pPacket.readByteU1());
			break;

			case PLAYERCOLORS:
				for (int i = 0; i < 5; i ++)
					plyrColors[i] = pPacket.readByte1();
			break;

			case PLAYERID:
				pPacket.readByte2();
			break;

			case PLAYERX:
				plyrX = (float)pPacket.readByte1()/2;
			break;

			case PLAYERY:
				plyrY = (float)pPacket.readByte1()/2;
			break;

			case PLAYERSPRITE:
				pPacket.readByte1();
			break;

			case STATUS:
				plyrStatus = pPacket.readByte1();
			break;

			case CARRYSPRITE:
			break;

			case CURLEVEL:
				plyrLevel = pPacket.readChars(pPacket.readByteU1());
			break;

			case HORSEGIF:
				plyrHorse = pPacket.readChars(pPacket.readByteU1());
			break;

			case HORSEBUSHES:
				pPacket.readByte1();
			break;

			case EFFECTCOLORS:
				pPacket.readByte1();
			break;

			case CARRYNPC:
				pPacket.readByte3();
			break;

			case APCOUNTER:
				pPacket.readByte2();
			break;

			case MAGICPOINTS:
				plyrMagic = pPacket.readByte1();
			break;

			case KILLSCOUNT:
				plyrKills = pPacket.readByte3();
			break;

			case DEATHSCOUNT:
				plyrDeaths = pPacket.readByte3();
			break;

			case ONLINESECS:
				plyrOnline = pPacket.readByte3();
			break;

			case LASTIP:
				pPacket.readByte5();
			break;

			case UDPPORT:
				pPacket.readByte3();
			break;

			case PALIGNMENT:
				plyrAp = pPacket.readByte1();
			break;

			case PADDITFLAGS:
				pPacket.readByte1();
			break;

			case PACCOUNTNAME:
				pPacket.readChars(pPacket.readByteU1());
			break;

			case BODYIMG:
				plyrBody = pPacket.readChars(pPacket.readByteU1());
			break;

			case RATING:
				pPacket.readByte3();
			break;

			case GATTRIB1:  plyrAttr[0] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB2:  plyrAttr[1] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB3:  plyrAttr[2] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB4:  plyrAttr[3] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB5:  plyrAttr[4] = pPacket.readChars(pPacket.readByteU1()); break;

			case GATTRIB6:  plyrAttr[5] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB7:  plyrAttr[6] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB8:  plyrAttr[7] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB9:  plyrAttr[8] = pPacket.readChars(pPacket.readByteU1()); break;

			case GATTRIB10: plyrAttr[9] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB11: plyrAttr[10] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB12: plyrAttr[11] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB13: plyrAttr[12] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB14: plyrAttr[13] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB15: plyrAttr[14] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB16: plyrAttr[15] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB17: plyrAttr[16] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB18: plyrAttr[17] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB19: plyrAttr[18] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB20: plyrAttr[19] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB21: plyrAttr[20] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB22: plyrAttr[21] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB23: plyrAttr[22] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB24: plyrAttr[23] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB25: plyrAttr[24] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB26: plyrAttr[25] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB27: plyrAttr[26] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB28: plyrAttr[27] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB29: plyrAttr[28] = pPacket.readChars(pPacket.readByteU1()); break;
			case GATTRIB30: plyrAttr[29] = pPacket.readChars(pPacket.readByteU1()); break;

			case PEMPTY42:
			break;

			case PEMPTY43:
			break;

			case PEMPTY44:
			break;

			case PLAYERZ:
				plyrZ = (float)pPacket.readByte1()/2;;
			break;

			case PEMPTY50:
			break;

			case PCONNECTED:
			break;

			case PLANGUAGE:
				pPacket.readByte1();
			break;

			case PSTATUSMSG:
				pPacket.readByteU1();
			break;

			case UNKNOWN1:
			break;

			default:
			return;
		}
	}

	updateItem();
}
