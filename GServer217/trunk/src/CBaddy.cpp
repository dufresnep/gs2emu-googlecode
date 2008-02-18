// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CBaddy.h"
#include "main.h"

char baddyStartMode[] = {WALK,WALK,WALK,WALK,SWAMPSHOT,HAREJUMP,WALK,WALK,WALK,WALK};
bool baddyPropsReinit[] = {false, true, true, true, true, true, true, true, false, false, false};
int baddyPower[] = {2,3,4,3,2,1,1,6,12,8};

char* baddyImages[] = {
	"baddygray.png", "baddyblue.png", "baddyred.png", "baddyblue.png",
	"baddygray.png", "baddyhare.png", "baddyoctopus.png", "baddygold.png", "baddylizardon.png",
	"baddydragon.png"
	};

void CBaddy::reset()
{
	mode = baddyStartMode[(int)type];
	x = startX;
	y = startY;
	power = baddyPower[(int)type];
	image = baddyImages[(int)type];
	dir = (2 << 2) + 2;
	aniCount = 0;
}

CPacket CBaddy::getProperty(int pId)
{
	CPacket retVal;
	switch(pId)
	{
		case BADDYID:
			retVal << (char)id;
		break;
		case BADDYX:
			retVal << (char)(x*2);
		break;
		case BADDYY:
			retVal << (char)(y*2);
		break;
		case BADDYTYPE:
			retVal << (char)type;
		break;
		case BADDYGIF:
			retVal << (char)power << (char)image.length() << image;
		break;
		case BADDYMODE:
			retVal << (char)mode;
		break;
		case BADDYANISTEP:
			retVal << (char)aniCount;
		break;
		case BADDYDIR:
			retVal << (char)dir;
		break;

		case BADDYVERSE1:
		case BADDYVERSE2:
		case BADDYVERSE3:
		{
			int verseId = pId-BADDYVERSE1;
			if(verseId < verses.count())
				retVal << (char)verses[verseId].length() << verses[verseId];
		}
		break;
	}
	return retVal;
}

void CBaddy::setProps(CPacket& pPacket)
{
	int len;
	while(pPacket.bytesLeft())
	{
		int index = pPacket.readByte1();
		switch(index)
		{
			case BADDYID:
				id = pPacket.readByte1();
			break;
			case BADDYX:
				x = CLIP(pPacket.readByte1()/2, 0, 63);
			break;
			case BADDYY:
				y = CLIP(pPacket.readByte1()/2, 0, 63);
			break;
			case BADDYTYPE:
				type = pPacket.readByte1();
			break;
			case BADDYGIF:
				if(pPacket.bytesLeft()>=2)
				{
					power = pPacket.readByte1();
					len = pPacket.readByte1();
					if(len >= 0)
						image = pPacket.readChars(len);
				}
			break;
			case BADDYMODE:
				mode = pPacket.readByte1();
				if ( mode == DIE )
				{
					if(respawn)
						respawnCount = baddyRespawn;
					else respawnCount = -1;
				}
			break;
			case BADDYANISTEP:
				aniCount = pPacket.readByte1();
			break;
			case BADDYDIR:
				dir = pPacket.readByte1();
			break;
			case BADDYVERSE1:
			case BADDYVERSE2:
			case BADDYVERSE3:
			{
				len = pPacket.readByte1();
				int verseId = index-BADDYVERSE1;
				if(len>=0)
				{
					if(verseId < verses.count())
						verses[index-BADDYVERSE1] = pPacket.readChars(len);
				}
			}
			break;

			default:
				return;
		}
	}
}

CPacket CBaddy::getPropList()
{
	CPacket retVal;
	for(int i = 1; i < baddypropcount; i ++)
		retVal << (char)i << getProperty(i);
	return retVal;
}
