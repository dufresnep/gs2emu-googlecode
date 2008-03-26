/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#include <stdarg.h>
#include "CNpc.h"
#include "main.h"

CNpc::CNpc(CString& pImage, CString& pCode, float pX, float pY, CLevel* pLevel)
{
	x = pX;
	y = pY;

	level = pLevel;
	hurtX = hurtY = 0;
	image = pImage;
	clientCode = pCode;
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
	rupees = darts = bombs = glovePower = blockFlags = bombPower = power = 0;
	sprite = 2;
	shieldPower = swordPower = 0;
	shieldImage = "";
	swordImage = "";
	visFlags = 1;
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
			retVal << (char)i << getProperty(i);
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
	con_print( "CNpc::getProperty - id: %d\n", id );
	switch(pId)
	{
		case NPCGIF:
			retVal << (char)image.length() << image;
			con_print( "NPCGIF\n" );
		break;

		case ACTIONSCRIPT:
			retVal << (short)clientCode.length() << clientCode;
			con_print( "ACTIONSCRIPT\n" );
		break;

		case NPCX:
			retVal << (char)(x*2);
			con_print( "NPCX\n" );
		break;

		case NPCY:
			retVal << (char)(y*2);
			con_print( "NPCY\n" );
		break;

		case NPCPOWER:
			retVal << (char)power;
			con_print( "NPCPOWER\n" );
		break;

		case NPCRUPEES:
			retVal.writeByte3(rupees);
			con_print( "NPCRUPEES\n" );
		break;

		case NPCARROWS:
			retVal << (char)darts;
			con_print( "NPCARROWS\n" );
		break;

		case NPCBOMBS:
			retVal << (char)bombs;
			con_print( "NPCBOMBS\n" );
		break;

		case NGLOVEPOWER:
			retVal << (char)glovePower;
			con_print( "NGLOVEPOWER\n" );
		break;

		case NBOMBPOWER:
			retVal << (char)bombPower;
			con_print( "NBOMBPOWER\n" );
		break;

		case NSWORDGIF:
		{
			int sp = swordPower+30;
			if ( sp == 30 ) sp = 0;
			retVal << (char)sp;
			if ( sp > 30 )
				retVal << (char)swordImage.length() << swordImage;

			con_print( "NSWORDGIF\n" );
		}
		break;

		case NSHIELDGIF:
		{
			int sp = shieldPower+10;
			if ( sp == 10 ) sp = 0;
			retVal << (char)sp;
			if ( sp > 10 )
				retVal << (char)shieldImage.length() << shieldImage;

			con_print( "NSHIELDGIF\n" );
		}
		break;

		case NPCANI:
			retVal << (char)gAni.length() << gAni;
			con_print( "NPCANI\n" );
		break;

		case VISFLAGS:
			retVal << (char)visFlags;
			con_print( "VISFLAGS\n" );
		break;

		case BLOCKFLAGS:
			retVal << (char)blockFlags;
			con_print( "BLOCKFLAGS\n" );
		break;

		case NPCMESSAGE:
			if ( chatMsg.length() > 200 )
				chatMsg = chatMsg.copy(0,200);
			retVal << (char)chatMsg.length() << chatMsg;
			con_print( "NPCMESSAGE\n" );
		break;

		case NPCHURTDXDY:
			retVal << (char)(hurtX*32)+32 << (char)(hurtY*32)+32;
			con_print( "NPCHURTDXDY\n" );
		break;

		case NPCID:
			retVal.writeByte3(id);
			con_print( "NPCID\n" );
		break;

		case NPCSPRITE:
			retVal << (char)(sprite % 4);
			con_print( "NPCSPRITE\n" );
		break;

		case NPCCOLORS:
			retVal << (char)colors[0] << (char)colors[1] << (char)colors[2] << (char)colors[3] << (char)colors[4];// << (char)colors[5];
			con_print( "NPCCOLORS\n" );
		break;

		case NPCNICKNAME:
			retVal << (char)nickName.length() << nickName;
			con_print( "NPCNICKNAME\n" );
		break;

		case NPCHORSEGIF:
			retVal << (char)horseImage.length() << horseImage;
			con_print( "NPCHORSEGIF\n" );
		break;

		case NPCHEADGIF:
			retVal << (char)(headImage.length()+100) << headImage;
			con_print( "NPCHEADGIF\n" );
		break;

		case NALIGNMENT:
			retVal.writeByte1(ap);
			con_print( "NALIGNMENT\n" );
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
			con_print( "NPCSAVE%d\n", pId-NPCSAVE0 );
		break;

		case NPCGIFPART:
			retVal << imagePart;
			con_print( "NPCGIFPART\n" );
		break;

		case NPCBODY:
			retVal << (char)bodyImage.length() << bodyImage;
			con_print( "NPCBODY\n" );
		break;

		case NGATTRIB1:
		case NGATTRIB2:
		case NGATTRIB3:
		case NGATTRIB4:
		case NGATTRIB5:
		{
			int index = pId - NGATTRIB1;
			retVal << (char)gAttribs[index].length() << gAttribs[index];
			con_print( "NGATTRIB%d\n", index );
		}
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
			con_print( "NGATTRIB%d\n", index );
		}
		break;
	}
	con_print( "Packet: %s\n\n", retVal.text() );
	return retVal;
}

void CNpc::setProps(CPacket& pProps)
{
	int len;
	int previousMessage = 0;
	con_print( "CNpc::setProps - id: %d\nPacket: %s\n", id, pProps.text() );
	while(pProps.bytesLeft())
	{
		int index = pProps.readByte1();
		CPacket oldProp = getProperty(index);
		switch(index)
		{
			case NPCGIF:
				len = pProps.readByte1();
				con_print( "NPCGIF: len: %d ", len );
				if(len >= 0)
					image = pProps.readChars(len);
				con_print( "image: %s\n", image.text() );
			break;

			case ACTIONSCRIPT:
				len = pProps.readByte2();
				con_print( "ACTIONSCRIPT: len: %d\n", len );
				if(len >= 0)
					clientCode = pProps.readChars(len);
			break;

			case NPCX:
				x = (float)(pProps.readByte1())/2;
				con_print( "NPCX: %d\n", x );
			break;

			case NPCY:
				y = (float)(pProps.readByte1())/2;
				con_print( "NPCY: %d\n", y );
			break;

			case NPCPOWER:
				power = pProps.readByte1();
				con_print( "NPCPOWER: %d\n", power );
			break;

			case NPCRUPEES:
				rupees = pProps.readByte3();
				con_print( "NPCRUPEES: %d\n", rupees );
			break;

			case NPCARROWS:
				darts = pProps.readByte1();
				con_print( "NPCARROWS: %d\n", darts );
			break;

			case NPCBOMBS:
				bombs = pProps.readByte1();
				con_print( "NPCBOMBS: %d\n", bombs );
			break;

			case NGLOVEPOWER:
				glovePower = pProps.readByte1();
				con_print( "NGLOVEPOWER: %d\n", glovePower );
			break;

			case NBOMBPOWER:
				bombPower = pProps.readByte1();
				con_print( "NBOMBPOWER: %d\n", bombPower );
			break;

			case NSWORDGIF:
			{
				con_print( "NSWORDGIF: " );
				int sp = pProps.readByte1();
				con_print( "sp: %d, ", sp );
				if ( sp >= 30 )
				{
					sp -= 30;
					len = pProps.readByte1();
					if ( len >= 0 )
						swordImage = pProps.readChars(len);
					con_print( "image: %s\n", swordImage.text() );
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
				con_print( "NSHIELDGIF: " );
				int sp = pProps.readByte1();
				con_print( "sp: %d, ", sp );
				if(sp >= 10)
				{
					len = pProps.readByte1();
					if(len >= 0)
						shieldImage = pProps.readChars(len);
					con_print( "image: %s\n", shieldImage.text() );
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
				len = pProps.readByte1();
				if(len >= 0)
					gAni = pProps.readChars(len);
				con_print( "NPCANI: len: %d, gani: %s\n", len, gAni.text() );
			break;

			case VISFLAGS:
				visFlags = pProps.readByte1();
				con_print( "VISFLAGS: %d\n", visFlags );
			break;

			case BLOCKFLAGS:
				blockFlags = pProps.readByte1();
				con_print( "BLOCKFLAGS %d\n", blockFlags );
			break;

			case NPCMESSAGE:
				len = pProps.readByte1();
				if(len >= 0)
					chatMsg = pProps.readChars(len);
				con_print( "NPCMESSAGE: len: %d, chatMsg: %s\n", len, chatMsg.text() );
			break;

			case NPCHURTDXDY:
				hurtX = ((float)(pProps.readByte1()-32))/32;
				hurtY = ((float)(pProps.readByte1()-32))/32;
				con_print( "NPCHURTDXDY: hurtX: %d hurtY: %d\n", hurtX, hurtY );
			break;

			case NPCID:
				pProps.readByte3();
				con_print( "NPCID: %d\n", len );
			break;

			case NPCSPRITE:
				sprite = pProps.readByte1();
				if ( sprite < 0 || sprite >= 132 ) sprite = 0;
				con_print( "NPCSPRITE: %d\n", sprite );
			break;

			case NPCCOLORS:
				con_print( "NPCCOLORS: " );
				for (int i = 0; i < 5; i++)
				{
					colors[i] = pProps.readByte1();
					con_print( "%d, ", colors[i] );
				}
				con_print( "\n" );
			break;

			case NPCNICKNAME:
				len = pProps.readByte1();
				if(len >= 0)
					nickName = pProps.readChars(len);
				con_print( "NPCNICKNAME: len: %d name: %s\n", len, nickName.text() );
			break;

			case NPCHORSEGIF:
				len = pProps.readByte1();
				if(len >= 0)
					horseImage = pProps.readChars(len);
				con_print( "NPCHORSEGIF: len: %d image: %s\n", len, horseImage.text() );
			break;

			case NPCHEADGIF:
				len = pProps.readByte1();
				if (len < 100)
				{
					if (len >= 0)
						headImage = CString() << "head" << toString(len) << ".png";
				}
				else
				{
					if (len > 100)
						headImage = pProps.readChars(len-100);
				}
				con_print( "NPCHEADGIF: len: %d image: %s\n", len, headImage.text() );
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
				saves[index-NPCSAVE0] = pProps.readByte1();
				con_print( "NPCSAVE%d: %d\n", index-NPCSAVE0, saves[index-NPCSAVE0] );
			break;

			case NALIGNMENT:
				ap = CLIP(pProps.readByte1(), 0, 100);
				con_print( "NALIGNMENT: %d\n", ap );
			break;

			case NPCGIFPART:
				imagePart = pProps.readChars(6);
				con_print( "NPCGIFPART:\n" );
			break;

			case NPCBODY:
				len = pProps.readByte1();
				if(len >= 0)
					bodyImage = pProps.readChars(len);
				con_print( "NPCBODY: len: %d image: %s\n", len, bodyImage.text() );
			break;

			case NGATTRIB1:
			case NGATTRIB2:
			case NGATTRIB3:
			case NGATTRIB4:
			case NGATTRIB5:
				len = pProps.readByte1();
				if(len >= 0)
					gAttribs[index-NGATTRIB1] = pProps.readChars(len);
				con_print( "NGATTRIB%d: len: %d attrib: %s\n", index-NGATTRIB1, len, gAttribs[index-NGATTRIB1].text() );
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
				len = pProps.readByte1();
				if(len >= 0)
					gAttribs[6+index-NGATTRIB6] = pProps.readChars(len);
				con_print( "NGATTRIB%d: len: %d attrib: %s\n", 6+index-NGATTRIB6, len, gAttribs[6+index-NGATTRIB6].text() );
			break;

			default:
				if ( detailedconsole )
					printf("[%s] UNKNOWN NPC PROP: %i, Prev: %i, Value: %s\n", getTimeStr(1).text(), index, previousMessage, (pProps.text() + pProps.getRead()));
			return;
		}
		previousMessage = index;

		// If a prop changed, adjust its mod time.
		if(index >= 0 && index < npcpropcount)
		{
			if ( oldProp != getProperty(index) )
				modTime[index] = getSysTime();
		}
	}
	con_print( "\n" );
}

void CNpc::con_print( const char* format, ... )
{
	//if ( !showConsolePackets )// || (id != 1 && id != 2 && id != 5 && id != 32) )
		return;

	va_list args;
	va_start( args, format );
	vprintf( format, args );
	va_end( args );
}
