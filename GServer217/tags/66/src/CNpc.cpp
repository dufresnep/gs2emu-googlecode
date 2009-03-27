/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include <stdarg.h>
#include "CNpc.h"
#include "main.h"

CNpc::CNpc(CString& pImage, CString& pCode, float pX, float pY, CLevel* pLevel)
{
	CNpc( pImage, pCode, pX, pY, pLevel, false );
}

CNpc::CNpc(CString& pImage, CString& pCode, float pX, float pY, CLevel* pLevel, bool levelNPC)
: x(pX), y(pY), hurtX(0), hurtY(0), level(pLevel), image(pImage),
levelNPC(levelNPC), clientCode(pCode),
rupees(0), darts(0), bombs(0), glovePower(0), blockFlags(0), bombPower(0),
power(0), sprite(2), shieldPower(0), swordPower(0),
visFlags(1)
{
	removeComments();
	weaponName = getFunctionParameter("toweapons ");
	if ( clientCode.length() < 20 )
	{
		if ( level->sparZone == false && clientCode.find("sparringzone") >= 0 )
			level->sparZone = true;
	}

	id = createNpcId(this);
	npcList.add(this);
	memset(saves, 0, sizeof(saves));
	memset(colors, 0, sizeof(colors));
	memset(modTime, 0, sizeof(modTime));
	gAni = "idle";
	for (int i = 0; i < 6; i++)
		imagePart.writeByte(32);

	modTime[NPCGIF] = getSysTime();
	modTime[ACTIONSCRIPT] = getSysTime();
	modTime[NPCX] = getSysTime();
	modTime[NPCY] = getSysTime();
	modTime[VISFLAGS] = getSysTime();
	modTime[NPCSPRITE] = getSysTime();
}

CNpc::~CNpc()
{
	npcIds.replace(id, NULL);
}

CPacket CNpc::getPropertyList(time_t newTime)
{
	CPacket retVal;

	for ( int i = 0; i < npcpropcount; i++ )
	{
		if ( modTime[i] >= newTime && modTime[i] > 0 )
		{
			if (oldcreated && i == VISFLAGS && newTime == 0)
				retVal << (char)i << (char)1;
			else
				retVal << (char)i << getProperty(i);
		}
		if (modTime[NPCANI] == 0 && image == "#c#")
			retVal << (char)NPCANI << (char)4 << "idle";
	}
	return retVal;
}

CString CNpc::getFunctionParameter(char* pName)
{
	CString retVal;
	int pos = clientCode.find(pName);
	if(pos >= 0)
	{
		pos += (int)strlen(pName);
		int len = clientCode.findToken(";\xa7}", pos) - pos;
		if(len > 0)
			retVal = clientCode.copy(pos, len);
	}
	retVal.trim();
	return retVal;
}

void CNpc::removeComments()
{
	int pos=0;

	while((pos = clientCode.find("//")) >= 0)
	{
		//check for urls (http://)
		if(pos > 0 && clientCode[pos-1] == ':')
			break;
		int len = clientCode.find((char)0xa7, pos)-pos;
		if(len > 0)
			clientCode.remove(pos, len);
		else break;
	}

	while((pos = clientCode.find("/*")) >= 0)
	{
		int len = clientCode.find("*/", pos)-pos;
		if(len > 0)
			clientCode.remove(pos, len+2);
		else break;
	}

}
CPacket CNpc::getProperty(int pId)
{
	CPacket retVal;
	switch(pId)
	{
		case NPCGIF:
			retVal << (char)image.length() << image;
		break;

		case ACTIONSCRIPT:
			retVal << (short)clientCode.length() << clientCode.copy(0, MIN(clientCode.length(),0x3FFF));
		break;

		case NPCX:
			retVal << (char)(x*2);
		break;

		case NPCY:
			retVal << (char)(y*2);
		break;

		case NPCPOWER:
			retVal << (char)power;
		break;

		case NPCRUPEES:
			retVal.writeByte3(rupees);
		break;

		case NPCARROWS:
			retVal << (char)darts;
		break;

		case NPCBOMBS:
			retVal << (char)bombs;
		break;

		case NGLOVEPOWER:
			retVal << (char)glovePower;
		break;

		case NBOMBPOWER:
			retVal << (char)bombPower;
		break;

		case NSWORDGIF:
		{
			int sp = swordPower+30;
			if ( sp == 30 ) sp = 0;
			retVal << (char)sp;
			if ( sp > 30 )
				retVal << (char)swordImage.length() << swordImage;
		}
		break;

		case NSHIELDGIF:
		{
			int sp = shieldPower+10;
			if ( sp == 10 ) sp = 0;
			retVal << (char)sp;
			if ( sp > 10 )
				retVal << (char)shieldImage.length() << shieldImage;
		}
		break;

		case NPCANI:
			retVal << (char)gAni.length() << gAni;
		break;

		case VISFLAGS:
			retVal << (char)visFlags;
		break;

		case BLOCKFLAGS:
			retVal << (char)blockFlags;
		break;

		case NPCMESSAGE:
			if ( chatMsg.length() > 200 )
				chatMsg = chatMsg.copy(0,200);
			retVal << (char)chatMsg.length() << chatMsg;
		break;

		case NPCHURTDXDY:
			retVal << (char)(hurtX*32)+32 << (char)(hurtY*32)+32;
		break;

		case NPCID:
			retVal.writeByte3(id);
		break;

		case NPCSPRITE:
			retVal << (char)(sprite % 4);
		break;

		case NPCCOLORS:
			retVal << (char)colors[0] << (char)colors[1] << (char)colors[2] << (char)colors[3] << (char)colors[4];// << (char)colors[5];
		break;

		case NPCNICKNAME:
			retVal << (char)nickName.length() << nickName;
		break;

		case NPCHORSEGIF:
			retVal << (char)horseImage.length() << horseImage;
		break;

		case NPCHEADGIF:
			retVal << (char)(headImage.length()+100) << headImage;
		break;

		case NALIGNMENT:
			retVal.writeByte1(ap);
		break;

		case NPCSAVE0:
		case NPCSAVE1:
		case NPCSAVE2:
		case NPCSAVE3:
		case NPCSAVE4:
		case NPCSAVE5:
		case NPCSAVE6:
		case NPCSAVE7:
		case NPCSAVE8:
		case NPCSAVE9:
			retVal << saves[pId-NPCSAVE0];
		break;

		case NPCGIFPART:
			retVal << imagePart;
		break;

		case NPCBODY:
			retVal << (char)bodyImage.length() << bodyImage;
		break;

		case NGATTRIB1:
		case NGATTRIB2:
		case NGATTRIB3:
		case NGATTRIB4:
		case NGATTRIB5:
		{
			int index = pId - NGATTRIB1;
			retVal << (char)gAttribs[index].length() << gAttribs[index];
		}
		break;

		case NGMAPLEVELX:
			retVal << (char)0;
			break;

		case NGMAPLEVELY:
			retVal << (char)0;
			break;

		case NEMPTY43:
			errorOut( "debuglog.txt", CString() << "CNpc::getProperty() requested NEMPTY" << toString(pId) );
			CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"CNpc::getProperty() requested NEMPTY" << toString(pId) << "\"" );
			break;

		case NGATTRIB6:
		case NGATTRIB7:
		case NGATTRIB8:
		case NGATTRIB9:
/* Does the client not send gani attribs > 9?
		case NGATTRIB10:
		case NGATTRIB11:
		case NGATTRIB12:
		case NGATTRIB13:
		case NGATTRIB14:
		case NGATTRIB15:
		case NGATTRIB16:
		case NGATTRIB17:
		case NGATTRIB18:
		case NGATTRIB19:
		case NGATTRIB20:
		case NGATTRIB21:
		case NGATTRIB22:
		case NGATTRIB23:
		case NGATTRIB24:
		case NGATTRIB25:
		case NGATTRIB26:
		case NGATTRIB27:
		case NGATTRIB28:
		case NGATTRIB29:
		case NGATTRIB30:
*/
		{
			int index = 6 + pId - NGATTRIB6;
			retVal << (char)gAttribs[index].length() << gAttribs[index];
		}
		break;

		default:
			errorOut( "debuglog.txt", CString() << "CNpc::getProperty() requested unknown prop " << toString(pId) );
			CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"CNpc::getProperty() requested unknown prop " << toString(pId) << "\"" );
			break;

	}
	return retVal;
}

void CNpc::setProps(CPacket& pProps)
{
	int len;
	int previousMessage = 0;
	while(pProps.bytesLeft())
	{
		int index = pProps.readByte1();
		CPacket oldProp = getProperty(index);
		switch(index)
		{
			case NPCGIF:
				len = (unsigned char)pProps.readByte1();
				image = pProps.readChars(len);
			break;

			case ACTIONSCRIPT:
				len = (unsigned int)pProps.readByte2();
				clientCode = pProps.readChars(len);
			break;

			case NPCX:
				x = (float)(pProps.readByte1())/2;
			break;

			case NPCY:
				y = (float)(pProps.readByte1())/2;
			break;

			case NPCPOWER:
				power = (unsigned char)pProps.readByte1();
			break;

			case NPCRUPEES:
				rupees = (unsigned int)pProps.readByte3();
			break;

			case NPCARROWS:
				darts = (unsigned char)pProps.readByte1();
			break;

			case NPCBOMBS:
				bombs = (unsigned char)pProps.readByte1();
			break;

			case NGLOVEPOWER:
				glovePower = (unsigned char)pProps.readByte1();
			break;

			case NBOMBPOWER:
				bombPower = (unsigned char)pProps.readByte1();
			break;

			case NSWORDGIF:
			{
				int sp = (unsigned char)pProps.readByte1();
				if ( sp >= 30 )
				{
					sp -= 30;
					len = (unsigned char)pProps.readByte1();
					if ( len >= 0 )
						swordImage = pProps.readChars(len);
				}
				else
				{
					if ( sp >= 1 && sp <= 4 )
						swordImage = CString() << "sword" << toString(sp) << ".png";
					else
						swordImage = "";
				}
				swordPower = (sp >= 30) ? (sp - 30) : sp;
			}

			break;

			case NSHIELDGIF:
			{
				int sp = (unsigned char)pProps.readByte1();
				if(sp >= 10)
				{
					len = (unsigned char)pProps.readByte1();
					if(len >= 0)
						shieldImage = pProps.readChars(len);
				} else
				{
					if(sp >= 1 && sp <= 3)
						shieldImage = CString() << "shield" << toString(sp) << ".png";
					else shieldImage = "";
				}
				shieldPower = (sp >= 10) ? (sp - 10) : sp;
			}
			break;

			case NPCANI:
				len = (unsigned char)pProps.readByte1();
				gAni = pProps.readChars(len);
			break;

			case VISFLAGS:
				visFlags = (unsigned char)pProps.readByte1();
			break;

			case BLOCKFLAGS:
				blockFlags = (unsigned char)pProps.readByte1();
			break;

			case NPCMESSAGE:
				len = (unsigned char)pProps.readByte1();
				chatMsg = pProps.readChars(len);
			break;

			case NPCHURTDXDY:
				hurtX = ((float)(pProps.readByte1()-32))/32;
				hurtY = ((float)(pProps.readByte1()-32))/32;
			break;

			case NPCID:
				pProps.readByte3();
			break;

			case NPCSPRITE:
				sprite = (unsigned char)pProps.readByte1();
				if ( sprite >= 132 ) sprite = 0;
			break;

			case NPCCOLORS:
				for (int i = 0; i < 5; i++)
					colors[i] = (unsigned char)pProps.readByte1();
			break;

			case NPCNICKNAME:
				len = (unsigned char)pProps.readByte1();
				nickName = pProps.readChars(len);
			break;

			case NPCHORSEGIF:
				len = (unsigned char)pProps.readByte1();
				horseImage = pProps.readChars(len);
			break;

			case NPCHEADGIF:
				len = (unsigned char)pProps.readByte1();
				if (len < 100)
					headImage = CString() << "head" << toString(len) << ".png";
				else
					headImage = pProps.readChars(len-100);
			break;

			case NPCSAVE0:
			case NPCSAVE1:
			case NPCSAVE2:
			case NPCSAVE3:
			case NPCSAVE4:
			case NPCSAVE5:
			case NPCSAVE6:
			case NPCSAVE7:
			case NPCSAVE8:
			case NPCSAVE9:
				saves[index-NPCSAVE0] = (unsigned char)pProps.readByte1();
			break;

			case NALIGNMENT:
				ap = CLIP(pProps.readByte1(), 0, 100);
			break;

			case NPCGIFPART:
				imagePart = pProps.readChars(6);
			break;

			case NPCBODY:
				len = (unsigned char)pProps.readByte1();
				bodyImage = pProps.readChars(len);
			break;

			case NGATTRIB1:
			case NGATTRIB2:
			case NGATTRIB3:
			case NGATTRIB4:
			case NGATTRIB5:
				len = (unsigned char)pProps.readByte1();
				gAttribs[index-NGATTRIB1] = pProps.readChars(len);
			break;

			case NGMAPLEVELX:
				pProps.readByte1();
				break;

			case NGMAPLEVELY:
				pProps.readByte1();
				break;

			case NEMPTY43:
				errorOut( "debuglog.txt", CString() << "CNpc::setProps() tried to set NEMPTY" << toString(index) );
				CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"CNpc::setProps tried to set NEMPTY" << toString(index) << "\"" );
				break;

			case NGATTRIB6:
			case NGATTRIB7:
			case NGATTRIB8:
			case NGATTRIB9:
/* Does the client not send gani attribs > 9?
			case NGATTRIB10:
			case NGATTRIB11:
			case NGATTRIB12:
			case NGATTRIB13:
			case NGATTRIB14:
			case NGATTRIB15:
			case NGATTRIB16:
			case NGATTRIB17:
			case NGATTRIB18:
			case NGATTRIB19:
			case NGATTRIB20:
			case NGATTRIB21:
			case NGATTRIB22:
			case NGATTRIB23:
			case NGATTRIB24:
			case NGATTRIB25:
			case NGATTRIB26:
			case NGATTRIB27:
			case NGATTRIB28:
			case NGATTRIB29:
			case NGATTRIB30:
*/
				len = (unsigned char)pProps.readByte1();
				if(len >= 0)
					gAttribs[6+index-NGATTRIB6] = pProps.readChars(len);
			break;

			default:
//				errorOut( "debuglog.txt", CString() << "CNpc::setProps() tried to set unknown " << toString(index) );
//				CPlayer::sendGlobally( CPacket() << (char)SRPGWINDOW << "\"CNpc::setProps tried to set unknown " << toString(index) << "\"" );
				printf("NPC %d (%.2f, %.2f): ", id, x, y);
				printf("Unknown prop: %ud, readPos: %d\n", index, pProps.getRead());
				for (int i = 0; i < pProps.length(); ++i)
					printf("%02x ", (unsigned char)pProps[i]);
				printf("\n");

//				if ( detailedconsole )
//					printf("[%s] UNKNOWN NPC PROP: %i, Prev: %i, Value: %s\n", getTimeStr(1).text(), index, previousMessage, (pProps.text() + pProps.getRead()));
			return;
		}
		previousMessage = index;

		// If a prop changed, adjust its mod time.
		if ( index >= 0 && index < npcpropcount )
		{
			if ( oldProp != getProperty(index) )
				modTime[index] = getSysTime();
		}
	}
}
