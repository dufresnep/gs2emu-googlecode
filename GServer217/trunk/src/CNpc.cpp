// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

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
	if(clientCode.length() < 20)
		level->sparZone = (clientCode.find("sparringzone") >= 0);

    id = createNpcId(this);
    npcList.add(this);
	memset(saves, 0, sizeof(saves));
	memset(colors, 0, sizeof(colors));
	memset(modTime, 0, sizeof(modTime));
	gAni = "idle";
	rupees = darts = bombs = glovePower = blockFlags = sprite = bombPower = power = 0;
	visFlags = 1;
	for(int i = 0; i < 6; i++)
		imagePart.writeByte(32);

    modTime[NPCGIF] = getTime();
	modTime[ACTIONSCRIPT] = getTime();
	modTime[NPCX] = getTime();
	modTime[NPCY] = getTime();
	modTime[VISFLAGS] = getTime();
}

CNpc::~CNpc()
{
    npcIds.replace(id, NULL);
}

CPacket CNpc::getPropertyList(int newTime)
{
    CPacket retVal;
	for(int i =0; i <npcpropcount; i++)
	{
		if(modTime[i] >= newTime && modTime[i]>0)
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
		int len = clientCode.findToken(";§}", pos) - pos;
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
		int len = clientCode.find('§', pos)-pos;
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
            retVal << (short)clientCode.length() << clientCode;
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

		case NPCANI2:
		case NPCANI:
			retVal << (char)gAni.length() << gAni;
		break;

		case NSHIELDGIF:
			retVal << (char)shieldPower+10 << (char)shieldImage.length() <<
                shieldImage;
		break;

		case VISFLAGS:
			retVal << (char)visFlags;
		break;

		case BLOCKFLAGS:
			retVal << (char)blockFlags;
		break;

		case NPCMESSAGE:
			retVal << (char)chatMsg.length() << chatMsg;
		break;

		case NPCHURTDXDY:
			retVal << (char)(hurtX*32)+32 << (char)(hurtY*32)+32;
		break;

		case NPCID:
			retVal.writeByte3(id);
		break;

		case NPCSPRITE:
			retVal << (char)(sprite%4);
		break;

		case NPCCOLORS:
			retVal << (char)colors[0] << (char)colors[1] << (char)colors[2] << (char)colors[3] << (char)colors[4] <<
				(char)colors[5];
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
		switch(index)
		{
			case NPCGIF:
				len = pProps.readByte1();
				if(len >= 0)
					image = pProps.readChars(len);
			break;

			case ACTIONSCRIPT:
				len = pProps.readByte2();
				if(len >= 0)
					clientCode = pProps.readChars(len);
			break;

			case NPCX:
				x = (float)(pProps.readByte1())/2;
			break;

			case NPCY:
				y = (float)(pProps.readByte1())/2;
			break;

			case NPCPOWER:
				power = pProps.readByte1();
			break;

			case NPCRUPEES:
				rupees = pProps.readByte3();
			break;

			case NPCARROWS:
				darts = pProps.readByte1();
			break;

			case NPCBOMBS:
                bombs = pProps.readByte1();
            break;

			case NGLOVEPOWER:
                glovePower = pProps.readByte1();
			break;

			case NBOMBPOWER:
                bombPower = pProps.readByte1();
			break;

			case NPCANI2:
			case NPCANI:
                len = pProps.readByte1();
                if(len >= 0)
                    gAni = pProps.readChars(len);
			break;

            case NSHIELDGIF:
            {
                int sp = pProps.readByte1();
                if(sp >= 10)
                {
                    len = pProps.readByte1();
                    if(len >= 0)
                        shieldImage = pProps.readChars(len);
                } else
                {
                    if(sp >= 1 && sp <= 3)
                        shieldImage = CString() << "shield" << toString(sp) << ".png";
                    else shieldImage = "";
                }
            }
            break;

			case VISFLAGS:
                visFlags = pProps.readByte1();
            break;

            case BLOCKFLAGS:
                blockFlags = pProps.readByte1();
            break;

            case NPCMESSAGE:
                len = pProps.readByte1();
                if(len >= 0)
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
                sprite = pProps.readByte1();
            break;

            case NPCCOLORS:
                for(int i = 0; i < 5; i++)
                    colors[i] = pProps.readByte1();
            break;

            case NPCNICKNAME:
                len = pProps.readByte1();
                if(len >= 0)
                    nickName = pProps.readChars(len);
            break;

            case NPCHORSEGIF:
                len = pProps.readByte1();
                if(len >= 0)
                    horseImage = pProps.readChars(len);
            break;

            case NPCHEADGIF:
                len = pProps.readByte1();
                if (len < 100)
                {
                    if (len >= 0)
                        headImage = CString() << "head" << toString(len) << ".png";
                } else
                {
                    if (len > 100)
                        headImage = pProps.readChars(len-100);
                }
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
            break;

            case NALIGNMENT:
                ap = CLIP(pProps.readByte1(), 0, 100);
            break;

            case NPCGIFPART:
                imagePart = pProps.readChars(6);
            break;
			case NPCBODY:
				len = pProps.readByte1();
				if(len >= 0)
					bodyImage = pProps.readChars(len);
			break;

			case NGATTRIB1:
			case NGATTRIB2:
			case NGATTRIB3:
			case NGATTRIB4:
			case NGATTRIB5:
				len = pProps.readByte1();
				if(len >= 0)
					gAttribs[index-NGATTRIB1] = pProps.readChars(len);
			break;
            default:
				printf("UNKNOWN NPC PROP: %i\n"
					"VALUE: %s\n"
					"PREVIOUS: %i\n", index, (pProps.text() + pProps.getRead()),
						previousMessage);
            return;
		}
		previousMessage = index;
		if(index >= 0 && index < npcpropcount)
            modTime[index] = getTime();
	}
}
