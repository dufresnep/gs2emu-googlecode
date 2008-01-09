#include "CClasses.h"
#include "CRCLive.h"
#include "main.h"

CPlayer::CPlayer(CRCChat *tab)
{
	this->tab  = tab;
	this->type = -1;
}

CPlayer::~CPlayer()
{
	tab->Players.remove(this);
	
	if (type != CLIENTRC)
		return;
	
	for (int i = 0; i < tab->listWidget->count(); i++)
	{
		if (tab->sock->type == IRC)
		{
			if (tab->listWidget->item(i)->text() == nickname.text())
				delete tab->listWidget->item(i);
		}
			else
		{
			if (tab->listWidget->item(i)->statusTip().toInt() == id)
				delete tab->listWidget->item(i);
		}
	}
}

void CPlayer::setProps(CPacket &pProps)
{
	int len = 0;
	
	while (pProps.bytesLeft())
	{
		int startpos = pProps.getRead();
		int index    = pProps.readByte1();
		
        if (index < 0 || index > propscount)
            continue;
		
		switch (index)
		{
			case NICKNAME:
				nickname = pProps.readChars(pProps.readByte1());
			break;

			case MAXPOWER:
				maxPower = CLIP((float)pProps.readByte1(), 0.0f, 30.0f);
			break;

			case CURPOWER:
				power = CLIP((float)pProps.readByte1()/2, 0.0f, maxPower);
			break;
			
			case RUPEESCOUNT:
				rubins = CLIP(pProps.readByte3(), 0, 999999);
			break;

			case ARROWSCOUNT:
				darts = CLIP(pProps.readByte1(), 0, 99);
			break;

			case BOMBSCOUNT:
				bombCount = CLIP(pProps.readByte1(), 0, 99);
			break;

			case GLOVEPOWER:
				glovePower = CLIP(pProps.readByte1(), 0, 3);
			break;

			case BOMBPOWER:
				bombPower = CLIP(pProps.readByte1(), 0, 3);
			break;

			case PLAYERANI:
				gAni = pProps.readChars(pProps.readByte1());
			break;

			case SWORDPOWER:
				swordPower = CLIP(pProps.readByte1() - 30, 0, 3);
				swordImage = pProps.readChars(pProps.readByte1());
			break;
			
			case SHIELDPOWER:
				shieldPower = CLIP(pProps.readByte1() - 10, 0, 3);
				shieldImage = pProps.readChars(pProps.readByte1());
			break;
			
			case HEADGIF:
				headimg = pProps.readChars(pProps.readByte1() - 100);
			break;

			case CURCHAT:
				chatMsg = pProps.readChars(pProps.readByte1());
			break;

			case PLAYERCOLORS:
				for (int i = 0; i < 5; i ++)
					colors[i] = pProps.readByte1();
			break;

			case PLAYERID:
				id = pProps.readByte2();
			break;

			case PLAYERX:
				x = (float)pProps.readByte1() / 2;
				status &= (-1-1);
				noMovement = 0;
			break;

			case PLAYERY:
				y = (float)pProps.readByte1() / 2;
				status &= (-1-1);
				noMovement = 0;
			break;

			case PLAYERSPRITE:
				sprite = pProps.readByte1();
			break;

			case STATUS:
				status = pProps.readByte1();
			break;

			case CARRYSPRITE:
				carrySprite = pProps.readByte1();
			break;

			case CURLEVEL:
				level = pProps.readChars(pProps.readByte1());
			break;

			case HORSEGIF:
				horseimg = pProps.readChars(pProps.readByte1());
			break;

			case HORSEBUSHES:
				horseBushes = pProps.readByte1();
			break;

			case EFFECTCOLORS:
				pProps.readByte5();
			break;

			case CARRYNPC:
				pProps.readByte3();
			break;

			case APCOUNTER:
				apCounter = pProps.readByte2();
			break;

			case MAGICPOINTS:
				magicPoints = CLIP(pProps.readByte1(), 0, 100);
			break;

			case KILLSCOUNT:
			    kills = pProps.readByte3();
			break;

			case DEATHSCOUNT:
				deaths = pProps.readByte3();
			break;

			case ONLINESECS:
				onlineSecs = pProps.readByte3();
			break;

			case LASTIP:
				lastIp = pProps.readByte5();
			break;

			case UDPPORT:
				udpPort = pProps.readByte3();
			break;

			case PALIGNMENT:
				ap = pProps.readByte1();
			break;

			case PADDITFLAGS:
				additionalFlags = pProps.readByte1();
			break;

			case PACCOUNTNAME:
				account = pProps.readChars(pProps.readByte1());
			break;

			case BODYIMG:
				bodyimg = pProps.readChars(pProps.readByte1());
			break;

			case PSTATUSMSG:
				statusMsg = pProps.readByte1();
			break;

			case PLANGUAGE:
				language = pProps.readChars(pProps.readByte1());
			break;

			case GATTRIB1:
			case GATTRIB2:
			case GATTRIB3:
			case GATTRIB4:
			case GATTRIB5:
				ganiAttribs[index-GATTRIB1] = pProps.readChars(pProps.readByte1());
			break;
			
			case RATING:
				rating = pProps.readByte2();
			break;
			
			default:
			return;
		}
	}
}