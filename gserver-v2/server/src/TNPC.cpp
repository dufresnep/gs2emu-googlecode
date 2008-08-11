#include <vector>
#include <time.h>
#include "ICommon.h"
#include "IUtil.h"
#include "CString.h"
#include "TNPC.h"
#include "TLevel.h"

char __savePackets[10] = { 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
char __attrPackets[30] = { 36, 37, 38, 39, 40, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68 };

static CString toWeaponName(const CString& code);
static std::vector<CString> removeComments(const CString& code);

TNPC::TNPC(const CString& pImage, const CString& pScript, float pX, float pY, TLevel* pLevel, bool pLevelNPC)
:
levelNPC(pLevelNPC),
x(pX), y(pY), hurtX(32.0f), hurtY(32.0f),
id(-1), rupees(0),
darts(0), bombs(0), glovePower(0), bombPower(0), swordPower(0), shieldPower(0),
visFlags(1), blockFlags(0), sprite(2), power(0), ap(50),
image(pImage), gani("idle"),
level(pLevel)
{
	memset((void*)colors, 0, sizeof(colors));
	memset((void*)saves, 0, sizeof(saves));
	memset((void*)modTime, 0, sizeof(modTime));

	// imagePart needs to be Graal-packed.
	for (int i = 0; i < 6; i++)
		imagePart.writeGChar(0);

	// We need to alter the modTime of the following props as they should be always sent.
	// If we don't, they won't be sent until the prop gets modified.
	modTime[NPCPROP_IMAGE] = modTime[NPCPROP_SCRIPT] = modTime[NPCPROP_X] = modTime[NPCPROP_Y]
		= modTime[NPCPROP_VISFLAGS] = modTime[NPCPROP_SPRITE] = time(0);

	// Search if the NPC is a sparringzone NPC.
	if (pScript.subString(0, 12) == "sparringzone") pLevel->setSparringZone(true);

	// Remove comments and separate clientside and serverside scripts.
	std::vector<CString> parsedCode = removeComments(pScript);
	if (parsedCode.size() == 1) clientCode = parsedCode[0];
	else if (parsedCode.size() > 1)
	{
		serverCode = parsedCode[0];
		for (unsigned int i = 1; i < parsedCode.size(); ++i)
			clientCode << parsedCode[i];
	}
	//if (serverCode.length() > 0) printf("serverCode: %s\n", serverCode.text());
	//if (clientCode.length() > 0) printf("clientCode: %s\n", clientCode.text());

	// Search for toweapons in the clientside code and extract the name of the weapon.
	weaponName = toWeaponName(clientCode);

	// Just a little warning for people who don't know.
	if (clientCode.length() > 0x3FFF)
		printf("WARNING: Clientside script of NPC (%s) exceeds the limit of 16383 bytes.\n", (weaponName.length() != 0 ? weaponName.text() : image.text()));

	// TODO: Create plugin hook so NPCServer can acquire/format code.
}

TNPC::~TNPC()
{
	// TODO: Remove from npcIds
}

CString TNPC::getProp(int pId) const
{
	switch(pId)
	{
		case NPCPROP_IMAGE:
		return CString() >> (char)image.length() << image;

		case NPCPROP_SCRIPT:
		return CString() >> (short)clientCode.length() << clientCode;

		case NPCPROP_X:
		return CString() >> (char)(x * 2);

		case NPCPROP_Y:
		return CString() >> (char)(y * 2);

		case NPCPROP_POWER:
		return CString() >> (char)power;

		case NPCPROP_RUPEES:
		return CString() >> (int)rupees;

		case NPCPROP_ARROWS:
		return CString() >> (char)darts;

		case NPCPROP_BOMBS:
		return CString() >> (char)bombs;

		case NPCPROP_GLOVEPOWER:
		return CString() >> (char)glovePower;

		case NPCPROP_BOMBPOWER:
		return CString() >> (char)bombPower;

		case NPCPROP_SWORDIMAGE:
		if (swordPower + 30 > 30)
			return CString() >> (char)(swordPower + 30) >> (char)swordImage.length() << swordImage;
		else
			return CString() >> (char)0;

		case NPCPROP_SHIELDIMAGE:
		if (shieldPower + 10 > 10)
			return CString() >> (char)(shieldPower + 10) >> (char)shieldImage.length() << shieldImage;
		else
			return CString() >> (char)0;

		case NPCPROP_GANI:
		return CString() >> (char)gani.length() << gani;

		case NPCPROP_VISFLAGS:
		return CString() >> (char)visFlags;

		case NPCPROP_BLOCKFLAGS:
		return CString() >> (char)blockFlags;

		case NPCPROP_MESSAGE:
		return CString() >> (char)chatMsg.subString(0, 200).length() << chatMsg.subString(0, 200);

		case NPCPROP_HURTDXDY:
		return CString() >> (char)(hurtX*32)+32 >> (char)(hurtY*32)+32;

		case NPCPROP_ID:
		return CString() >> (int)id;

		// Sprite is deprecated and has been replaced by def.gani.
		// Sprite now holds the direction of the npc.  sprite % 4 gives backwards compatibility.
		case NPCPROP_SPRITE:
		return CString() >> (char)(sprite % 4);

		case NPCPROP_COLORS:
		return CString() >> (char)colors[0] >> (char)colors[1] >> (char)colors[2] >> (char)colors[3] >> (char)colors[4];

		case NPCPROP_NICKNAME:
		return CString() >> (char)nickName.length() << nickName;

		case NPCPROP_HORSEIMAGE:
		return CString() >> (char)horseImage.length() << horseImage;

		case NPCPROP_HEADIMAGE:
		return CString() >> (char)(headImage.length() + 100) << headImage;

		case NPCPROP_ALIGNMENT:
		return CString() >> (char)ap;

		case NPCPROP_IMAGEPART:
		return CString() << imagePart;

		case NPCPROP_BODYIMAGE:
		return CString() >> (char)bodyImage.length() << bodyImage;
	}

	// Saves.
	if (inrange(pId, 23, 32))
	{
		for (unsigned int i = 0; i < sizeof(__savePackets); i++)
		{
			if (__savePackets[i] == pId)
				return CString() >> (char)saves[i];
		}
	}

	// Gani attributes.
	if (inrange(pId, 36, 40) || inrange(pId, 44, 68))
	{
		for (unsigned int i = 0; i < sizeof(__attrPackets); i++)
		{
			if (__attrPackets[i] == pId)
				return CString() >> (char)gAttribs[i].length() << gAttribs[i];
		}
	}

	return CString();
}

CString TNPC::getProps(time_t newTime) const
{
	CString retVal;
	for (int i = 0; i < npcpropcount; i++)
	{
		if ( modTime[i] >= newTime && modTime[i] > 0 )
			retVal >> (char)i << getProp(i);
	}
	return retVal;
}

void TNPC::setProps(CString& pProps)
{
	int len = 0;
	while (pProps.bytesLeft() > 0)
	{
		unsigned char propId = pProps.readGUChar();
		CString oldProp = getProp(propId);
		//printf( "propId: %d\n", propId );
		switch (propId)
		{
			case NPCPROP_IMAGE:
				image = pProps.readChars(pProps.readGUChar());
			break;

			case NPCPROP_SCRIPT:
				clientCode = pProps.readChars(pProps.readGUShort());
			break;

			case NPCPROP_X:
				x = (float)(pProps.readGChar())/2;
			break;

			case NPCPROP_Y:
				y = (float)(pProps.readGChar())/2;
			break;

			case NPCPROP_POWER:
				power = pProps.readGUChar();
			break;

			case NPCPROP_RUPEES:
				rupees = pProps.readGUInt();
			break;

			case NPCPROP_ARROWS:
				darts = pProps.readGUChar();
			break;

			case NPCPROP_BOMBS:
				bombs = pProps.readGUChar();
			break;

			case NPCPROP_GLOVEPOWER:
				glovePower = pProps.readGUChar();
			break;

			case NPCPROP_BOMBPOWER:
				bombPower = pProps.readGUChar();
			break;

			case NPCPROP_SWORDIMAGE:
			{
				int sp = pProps.readGUChar();
				if (sp <= 4)
					swordImage = CString() << "sword" << CString(sp) << ".png";
				else
				{
					sp -= 30;
					len = pProps.readGUChar();
					if (len > 0)
					{
						// TODO: foldersconfig stuff.
						swordImage = pProps.readChars(len);
					}
					else swordImage = "";
					//swordPower = clip(sp, ((settings->getBool("healswords", false) == true) ? -(settings->getInt("swordlimit", 3)) : 0), settings->getInt("swordlimit", 3));
				}
				swordPower = sp;
			}
			break;

			case NPCPROP_SHIELDIMAGE:
			{
				int sp = pProps.readGUChar();
				if (sp <= 3)
					shieldImage = CString() << "shield" << CString(sp) << ".png";
				else
				{
					sp -= 10;
					len = pProps.readGUChar();
					if (len > 0)
					{
						// TODO: foldersconfig stuff.
						shieldImage = pProps.readChars(len);
					}
					else shieldImage = "";
				}
				shieldPower = sp;
			}
			break;

			case NPCPROP_GANI:
				gani = pProps.readChars(pProps.readGUChar());
			break;

			case NPCPROP_VISFLAGS:
				visFlags = pProps.readGUChar();
			break;

			case NPCPROP_BLOCKFLAGS:
				blockFlags = pProps.readGUChar();
			break;

			case NPCPROP_MESSAGE:
				chatMsg = pProps.readChars(pProps.readGUChar());
			break;

			case NPCPROP_HURTDXDY:
				// TODO: Might have the same formatting as the player GMAPX/Y props.
				hurtX = ((float)(pProps.readGUChar()-32))/32;
				hurtY = ((float)(pProps.readGUChar()-32))/32;
			break;

			case NPCPROP_ID:
				pProps.readGUInt();
			break;

			case NPCPROP_SPRITE:
				sprite = pProps.readGUChar();
			break;

			case NPCPROP_COLORS:
				for (int i = 0; i < 5; i++)
					colors[i] = pProps.readGUChar();
			break;

			case NPCPROP_NICKNAME:
				nickName = pProps.readChars(pProps.readGUChar());
			break;

			case NPCPROP_HORSEIMAGE:
				horseImage = pProps.readChars(pProps.readGUChar());
			break;

			case NPCPROP_HEADIMAGE:
				len = pProps.readGUChar();
				if (len < 100)
					headImage = CString() << "head" << CString(len) << ".png";
				else
					headImage = pProps.readChars(len - 100);
			break;

			case NPCPROP_ALIGNMENT:
				ap = pProps.readGUChar();
				ap = clip(ap, 0, 100);
			break;

			case NPCPROP_IMAGEPART:
				imagePart = pProps.readChars(6);
			break;

			case NPCPROP_BODYIMAGE:
				bodyImage = pProps.readChars(pProps.readGUChar());
			break;

			case NPCPROP_SAVE0: saves[0] = pProps.readGUChar(); break;
			case NPCPROP_SAVE1: saves[1] = pProps.readGUChar(); break;
			case NPCPROP_SAVE2: saves[2] = pProps.readGUChar(); break;
			case NPCPROP_SAVE3: saves[3] = pProps.readGUChar(); break;
			case NPCPROP_SAVE4: saves[4] = pProps.readGUChar(); break;
			case NPCPROP_SAVE5: saves[5] = pProps.readGUChar(); break;
			case NPCPROP_SAVE6: saves[6] = pProps.readGUChar(); break;
			case NPCPROP_SAVE7: saves[7] = pProps.readGUChar(); break;
			case NPCPROP_SAVE8: saves[8] = pProps.readGUChar(); break;
			case NPCPROP_SAVE9: saves[9] = pProps.readGUChar(); break;

			case NPCPROP_GATTRIB1:  gAttribs[0]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB2:  gAttribs[1]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB3:  gAttribs[2]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB4:  gAttribs[3]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB5:  gAttribs[4]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB6:  gAttribs[5]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB7:  gAttribs[6]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB8:  gAttribs[7]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB9:  gAttribs[8]  = pProps.readChars(pProps.readGUChar()); break;
/*			case NPCPROP_GATTRIB10: gAttribs[9]  = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB11: gAttribs[10] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB12: gAttribs[11] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB13: gAttribs[12] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB14: gAttribs[13] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB15: gAttribs[14] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB16: gAttribs[15] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB17: gAttribs[16] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB18: gAttribs[17] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB19: gAttribs[18] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB20: gAttribs[19] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB21: gAttribs[20] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB22: gAttribs[21] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB23: gAttribs[22] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB24: gAttribs[23] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB25: gAttribs[24] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB26: gAttribs[25] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB27: gAttribs[26] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB28: gAttribs[27] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB29: gAttribs[28] = pProps.readChars(pProps.readGUChar()); break;
			case NPCPROP_GATTRIB30: gAttribs[29] = pProps.readChars(pProps.readGUChar()); break;
*/
			default:
			{
				printf("Unidentified NPCPROP: %i, readPos: %d\n", propId, pProps.readPos());
				for (int i = 0; i < pProps.length(); ++i)
					printf("%02x ", (unsigned char)pProps[i]);
				printf("\n");
			}
			return;
		}

		// If a prop changed, adjust its mod time.
		if ( propId >= 0 && propId < npcpropcount )
		{
			if ( oldProp != getProp(propId) )
				modTime[propId] = time(0);
		}
	}
}

CString toWeaponName(const CString& code)
{
	int name_start = code.find("toweapons ");
	if (name_start == -1) return CString();
	name_start += 10;	// 10 = strlen("toweapons ")

	int name_end = code.find(";", name_start);
	if (name_end == -1) return CString();

	return code.subString(name_start, name_end - name_start).trim();
}

std::vector<CString> removeComments(const CString& code)
{
	CString outLine;
	std::vector<CString> retVal;
	std::vector<CString> script = code.tokenize("\xa7");
	bool multiLine = false;
	for (std::vector<CString>::iterator i = script.begin(); i != script.end(); ++i)
	{
		CString line = *i;

		// Loop until every comment is removed from the line.
		bool doLoop = true;
		while (doLoop)
		{
			// First, we check for multi-line comments.
			// If multiLine is true, search for the end of the multi-line comment.
			if (multiLine)
			{
				int mlc_end = line.find("*/");
		
				// If not found, we can discard this entire line.
				if (mlc_end == -1)
				{
					doLoop = false;
					line.clear();
				}
				else
				{
					// If found, erase up to it.
					line.removeI(0, mlc_end + 2);
					multiLine = false;
				}
				continue;
			}

			// Check for the start of a multi-line comment.
			int mlc_start = line.find("/*");
			if (mlc_start != -1)
			{
				multiLine = true;
				int mlc_end = line.find("*/");
				if (mlc_end == -1)
				{
					// If no end was found, remove the rest of the line and break the loop.
					line.removeI(mlc_start, line.length());
					doLoop = false;
				}
				else
				{
					multiLine = false;
					line.removeI(mlc_start, (mlc_end + 2) - mlc_start);
				}
				continue;
			}

			// Check for a single line comment.
			int slc_start = line.find("//");
			if (slc_start != -1)
			{
				// If //#CLIENTSIDE is found, add the current code read as serverside script.
				if (line.subString(slc_start, 13) == "//#CLIENTSIDE")
				{
					retVal.push_back(outLine);
					outLine.clear();
				}
				line.removeI(slc_start, line.length());
				doLoop = false;
				continue;
			}

			doLoop = false;
		}

		// If line has any data left in it, add it to the retVal;
		if (line.length() != 0)
			outLine << line.trimI() << "\xa7";
	}

	// Add our code to the vector.  If serverside code was added, the size should now be 2.
	retVal.push_back(outLine);
	return retVal;
}
