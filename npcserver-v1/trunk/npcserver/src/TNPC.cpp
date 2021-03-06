#include "TNPC.h"
#include <vector>
#include <time.h>
#include "ICommon.h"
#include "IUtil.h"
#include "CString.h"
#include "TNPC.h"
#include "TLevel.h"
#include "TMap.h"
#include "TNPCServer.h"
#include "MiscFunctions.h"
#include "IUtil.h"

const char __nSavePackets[10] = { 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
const char __nAttrPackets[30] = { 36, 37, 38, 39, 40, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68 };


gmType TNPC::s_typeId = GM_STRING;
gmType TNPC::s_typeIdForVariables = GM_STRING;


TNPC::TNPC(int npcId,TNPCServer* pServer, TLevel* pLevel,gmMachine * npcMachine, bool pLevelNPC)
:
levelNPC(pLevelNPC),
x(0), y(0), hurtX(32.0f), hurtY(32.0f),
x2((int)(0*16)), y2((int)(0*16)),
gmaplevelx(0), gmaplevely(0),
id(npcId), rupees(0),
darts(0), bombs(0), glovePower(0), bombPower(0), swordPower(0), shieldPower(0),
visFlags(1), blockFlags(0), sprite(2), power(0), ap(50),
image(""), gani("idle"),
level(pLevel), server(pServer)
{
	width = 0;
	height = 0;

	machine = npcMachine;

	memset((void*)colors, 0, sizeof(colors));
	memset((void*)saves, 0, sizeof(saves));
	memset((void*)modTime, 0, sizeof(modTime));

	// bowImage for pre-2.x clients.
	bowImage >> (char)0;

	// imagePart needs to be Graal-packed.
	for (int i = 0; i < 6; i++)
		imagePart.writeGChar(0);

	// Set the gmap levels.
	//TMap* gmap = level->getMap();
	//if (gmap && gmap->getType() == MAPTYPE_GMAP)
	//{
	//	gmaplevelx = (unsigned char)gmap->getLevelX(level->getLevelName());
	//	gmaplevely = (unsigned char)gmap->getLevelY(level->getLevelName());
	//}

	timeout = -1;

	// We need to alter the modTime of the following props as they should be always sent.
	// If we don't, they won't be sent until the prop gets modified.
	modTime[NPCPROP_IMAGE] = modTime[NPCPROP_SCRIPT] = modTime[NPCPROP_X] = modTime[NPCPROP_Y]
		= modTime[NPCPROP_VISFLAGS] = modTime[NPCPROP_ID] = modTime[NPCPROP_SPRITE]
		= modTime[NPCPROP_GMAPLEVELX] = modTime[NPCPROP_GMAPLEVELY]
		= modTime[NPCPROP_X2] = modTime[NPCPROP_Y2] = time(0);

	
}

TNPC::~TNPC()
{

}

CString TNPC::getProp(unsigned char pId, int clientVersion) const
{
	switch(pId)
	{
		case NPCPROP_IMAGE:
		return CString() >> (char)image.length() << image;

		case NPCPROP_SCRIPT:
		{
			//SCRIPTENGINE INSERT
			return fullScript;
			//SCRIPTENGINE INSERT
		}

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
		if (swordPower == 0)
			return CString() >> (char)0;
		else
			return CString() >> (char)(swordPower + 30) >> (char)swordImage.length() << swordImage;

		case NPCPROP_SHIELDIMAGE:
		if (shieldPower + 10 > 10)
			return CString() >> (char)(shieldPower + 10) >> (char)shieldImage.length() << shieldImage;
		else
			return CString() >> (char)0;

		case NPCPROP_GANI:
		if (clientVersion < CLVER_2_1)
			return bowImage;
		return CString() >> (char)gani.length() << gani;

		case NPCPROP_VISFLAGS:
		return CString() >> (char)visFlags;

		case NPCPROP_BLOCKFLAGS:
		return CString() >> (char)blockFlags;

		case NPCPROP_MESSAGE:
		return CString() >> (char)chatMsg.subString(0, 200).length() << chatMsg.subString(0, 200);

		case NPCPROP_HURTDXDY:
		return CString() >> (char)((hurtX*32)+32) >> (char)((hurtY*32)+32);

		case NPCPROP_ID:
		return CString() >> (int)id;

		// Sprite is deprecated and has been replaced by def.gani.
		// Sprite now holds the direction of the npc.  sprite % 4 gives backwards compatibility.
		case NPCPROP_SPRITE:
		{
			if (clientVersion < CLVER_2_1) return CString() >> (char)sprite;
			else return CString() >> (char)(sprite % 4);
		}

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

		case NPCPROP_GMAPLEVELX:
		return CString() >> (char)gmaplevelx;

		case NPCPROP_GMAPLEVELY:
		return CString() >> (char)gmaplevely;

		case NPCPROP_CLASS:
		return CString() >> (short)0;

		case NPCPROP_X2:
		{
			unsigned short val = (x2 < 0 ? -x2 : x2);
			val <<= 1;
			if (x2 < 0) val |= 0x0001;
			return CString().writeGShort(val);
		}

		case NPCPROP_Y2:
		{
			unsigned short val = (y2 < 0 ? -y2 : y2);
			val <<= 1;
			if (y2 < 0) val |= 0x0001;
			return CString().writeGShort((short)val);
		}
		case NPCLEVEL:
		{
			return CString() >> (char)level->getLevelName().length() << level->getLevelName();
		}
	}

	// Saves.
	if (inrange(pId, 23, 32))
	{
		for (unsigned int i = 0; i < sizeof(__nSavePackets); i++)
		{
			if (__nSavePackets[i] == pId)
				return CString() >> (char)saves[i];
		}
	}

	// Gani attributes.
	if (inrange(pId, 36, 40) || inrange(pId, 44, 68))
	{
		for (unsigned int i = 0; i < sizeof(__nAttrPackets); i++)
		{
			if (__nAttrPackets[i] == pId)
				return CString() >> (char)gAttribs[i].length() << gAttribs[i];
		}
	}

	return CString();
}

CString TNPC::getProps(time_t newTime, int clientVersion) const
{
	bool oldcreated = false;
	CString retVal;
	int pmax = npcpropcount;
	if (clientVersion < CLVER_2_1) pmax = 36;

	for (int i = 0; i < pmax; i++)
	{
		if (modTime[i] != 0 && modTime[i] >= newTime)
		{
			if (oldcreated && i == NPCPROP_VISFLAGS && newTime == 0)
				retVal >> (char)i >> (char)1;
			else
				retVal >> (char)i << getProp(i, clientVersion);
		}
	}
	if (clientVersion > CLVER_1_411)
	{
		if (modTime[NPCPROP_GANI] == 0 && image == "#c#")
			retVal >> (char)NPCPROP_GANI >> (char)4 << "idle";
	}

	return retVal;
}
void TNPC::SendProp(int pPropId)
{
	CString packet = CString() >> (char)GO_NC_QUERY >> (char)NCO_NPCPROPSSET >> (int)id >> (char)pPropId << getProp(pPropId);
	server->sendToGserver(packet);
}

void TNPC::setProps(CString& pProps, int clientVersion)
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
				if (!image.isEmpty() && clientVersion < CLVER_2_1 && getExtension(image).isEmpty())
					image << ".gif";
			break;

			case NPCPROP_SCRIPT:
				fullScript = pProps.readChars(pProps.readGUInt5());
				parseServerside();
				callFunction("onCreated",NULL,"");
			break;

			case NPCPROP_X:
				x = (float)(pProps.readGChar()) / 2.0f;
				x2 = (int)(x * 16);
			break;

			case NPCPROP_Y:
				y = (float)(pProps.readGChar()) / 2.0f;
				y2 = (int)(y * 16);
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
					swordImage = CString() << "sword" << CString(sp) << (clientVersion < CLVER_2_1 ? ".gif" : ".png");
				else
				{
					sp -= 30;
					len = pProps.readGUChar();
					if (len > 0)
					{
						swordImage = pProps.readChars(len);
						if (!swordImage.isEmpty() && clientVersion < CLVER_2_1 && getExtension(swordImage).isEmpty())
							swordImage << ".gif";
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
					shieldImage = CString() << "shield" << CString(sp) << (clientVersion < CLVER_2_1 ? ".gif" : ".png");
				else
				{
					sp -= 10;
					len = pProps.readGUChar();
					if (len > 0)
					{
						shieldImage = pProps.readChars(len);
						if (!shieldImage.isEmpty() && clientVersion < CLVER_2_1 && getExtension(shieldImage).isEmpty())
							shieldImage << ".gif";
					}
					else shieldImage = "";
				}
				shieldPower = sp;
			}
			break;

			case NPCPROP_GANI:
				if (clientVersion < CLVER_2_1)
				{
					// Older clients don't use ganis.  This is the bow power and image instead.
					int sp = pProps.readGUChar();
					if (sp < 10)
						bowImage = CString() >> (char)sp;
					else
					{
						sp -= 10;
						if (sp < 0) break;
						bowImage = pProps.readChars(sp);
						if (!bowImage.isEmpty() && clientVersion < CLVER_2_1 && getExtension(bowImage).isEmpty())
							bowImage << ".gif";
						bowImage = CString() >> (char)(10 + bowImage.length()) << bowImage;
					}
					break;
				}
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
				if (!horseImage.isEmpty() && clientVersion < CLVER_2_1 && getExtension(horseImage).isEmpty())
					horseImage << ".gif";
			break;

			case NPCPROP_HEADIMAGE:
				len = pProps.readGUChar();
				if (len < 100)
					headImage = CString() << "head" << CString(len) << (clientVersion < CLVER_2_1 ? ".gif" : ".png");
				else
				{
					headImage = pProps.readChars(len - 100);
					if (!headImage.isEmpty() && clientVersion < CLVER_2_1 && getExtension(headImage).isEmpty())
						headImage << ".gif";
				}
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

			case NPCPROP_GMAPLEVELX:
				gmaplevelx = pProps.readGUChar();
			break;

			case NPCPROP_GMAPLEVELY:
				gmaplevely = pProps.readGUChar();
			break;

			case NPCPROP_CLASS:
				pProps.readChars(pProps.readGShort());
			break;

			// Location, in pixels, of the npc on the level in 2.3+ clients.
			// Bit 0x0001 controls if it is negative or not.
			// Bits 0xFFFE are the actual value.
			case NPCPROP_X2:
				x2 = len = pProps.readGUShort();

				// If the first bit is 1, our position is negative.
				x2 >>= 1;
				if ((short)len & 0x0001) x2 = -x2;

				// Let pre-2.3+ clients see 2.3+ movement.
				x = (float)x2 / 16.0f;
				break;

			case NPCPROP_Y2:
				y2 = len = pProps.readGUShort();

				// If the first bit is 1, our position is negative.
				y2 >>= 1;
				if ((short)len & 0x0001) y2 = -y2;

				// Let pre-2.3+ clients see 2.3+ movement.
				y = (float)y2 / 16.0f;
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
			case NPCPROP_GATTRIB10: gAttribs[9]  = pProps.readChars(pProps.readGUChar()); break;
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

			default:
			{
				printf("NPC %d (%.2f, %.2f): ", id, x, y);
				printf("Unknown prop: %ud, readPos: %d\n", propId, pProps.readPos());
				for (int i = 0; i < pProps.length(); ++i)
					printf("%02x ", (unsigned char)pProps[i]);
				printf("\n");
			}
			return;
		}

		// If a prop changed, adjust its mod time.
		if (propId < npcpropcount)
		{
			if (oldProp != getProp(propId))
				modTime[propId] = time(0);
		}
	}
}

void TNPC::decTimer()
{
	timeout-=.1f;
}

void TNPC::callFunction(CString function,TPlayer * pPlayer,CString params)
{        
	
	gmVariable returnVar;
	if (gameMonkeyScript.isEmpty()) return; 

	int errors = machine->CheckSyntax(gameMonkeyScript.text());

	if(errors) return;
	
	gmTableObject * m_table;

	//If parameters, then populate a gmtable
	if (params != "")
	{
		std::vector<CString> paramList = params.tokenize(",");

		m_table = machine->AllocTableObject();
		
		int count = 0;
		for (std::vector<CString>::iterator i = paramList.begin(); i != paramList.end(); ++i)
		{
			gmVariable param;
			if (i->isNumber()) 
			{
				float val = (float)atof(i->text());
				param.SetFloat(val);
			}
			else 
			{
				param.SetString(pPlayer->machine.AllocStringObject(i->text()));
			}
			m_table->Set(machine,count,param);
			count++;
		}
	}

	int resultInt = 0;  

	//Attach npc to this.
	gmUserObject * npcObj = machine->AllocUserObject(this,TNPC::s_typeId);
	gmVariable npc = gmVariable(npcObj);

	gmCall call;
	
	
	if (thisVariables[function] == 0) return;
	gmFunctionObject * funcObj = thisVariables[function]->GetFunctionObjectSafe();
	if (call.BeginFunction(machine, funcObj,npc))
	{
		if (pPlayer) call.AddParamUser(pPlayer,TPlayer::s_typeId);
		if (params != "") call.AddParamTable(m_table);
		
		call.End();
	} 
	//machine->Execute(0);
	
}

void TNPC::compileScript()
{
	if (serverScript.isEmpty()) return;

	int errors = machine->CheckSyntax(gameMonkeyScript.text());
	server->sendPlayerPM("Agret",gameMonkeyScript.text()); // DEBUG: REMOVE THIS LINE

	//We dont want to display any error information for level npcs
	// if(errors) return;
// OR DO WE
	if(errors)
	{
		bool first = true;
		const char * message;
	    CString output;
		bool done = false;

		std::vector<CString> errorList;
		server->ncChat(CString() << "Script compiler output for level " << level->getLevelName().text());
		
		while((message = machine->GetLog().GetEntry(first))) 
		{
			output << message;

			if (!done) errorList.push_back(CString() << "Error: " << output.subString(output.find(",")+1,output.length()-output.find(",")+1).trim());
			//server->ncChat(CString() << "Error: " << output.subString(output.find(",")+1,output.length()-output.find(",")+1).trim());
			done= true;
		}

		server->ncChat(errorList[0]);

		delete message;
		machine->GetLog().Reset();
	 }

	else
	{
		gmCall call;
		//Good Stuff
		//Compile
		gmFunctionObject * initFunction = machine->CompileStringToFunction(gameMonkeyScript.text());

		//Attach npc to this.
		gmUserObject * npcObj = machine->AllocUserObject(this,TNPC::s_typeId);
		gmVariable npc = gmVariable(npcObj);

		//call functions to popullate function and var list.
		machine->ExecuteFunction(initFunction,0,true,&npc);
		//if (call.BeginFunction(machine,initFunction,npc))
		//{
		//	call.End();
		//}
	}
}

void TNPC::parseServerside()
{
	CString script = fullScript.readString("//#CLIENTSIDE");
	serverScript = removeComments(script,"\xa7");
	serverScript.removeAllI("/*");
	serverScript.removeAllI("*/");

	//Need to parse before removing newLine delimiters
	gameMonkeyScript = gs2ToGameMonkey(serverScript);

	//Store cleaned gs2
	serverScript.removeAllI("\xa7");

	//Kill Threads
	for (std::vector<int>::iterator i = threads.begin(); i != threads.end(); ++i)
	{
		machine->KillThread(*i);
	}

	//Clear Variables
	thisVariables.clear();

	

	//Clear Threads
	threads.clear();

	machine->CollectGarbage();

	compileScript();
	
}
void TNPC::setVar(CString prop, gmVariable value)
{

	thisVariables[prop] = new gmVariable();
	*thisVariables[prop] = value;
}

//Function: npcobj.var
//Returns: value of var
void GM_CDECL TNPC::TNPCGetDot(gmThread * a_thread, gmVariable * a_operands)
{
	//O_GETDOT = 0,       // object, "member"          (tos is a_operands + 2)
	GM_ASSERT(a_operands[0].m_type == TNPC::s_typeId);
	gmUserObject * user = (gmUserObject *) GM_OBJECT(a_operands[0].m_value.m_ref);
	TNPC * pNPC = (TNPC *) user->m_user;

	CString prop = a_operands[1].GetCStringSafe();

	TNPCServer * pServer = pNPC->GetServer();

	//Check to see if incoming variable doesnt exist in npc props
	if (pServer->npcPropAccess[prop].valueType == PROP_UNKOWN)
	{
		gmVariable * thisVar = pNPC->getThisVar(prop);

		if (thisVar == 0) 
		{
			a_operands[0].Nullify();
			return;
		}
		
		a_operands[0] = *thisVar;
		return;
	}
	
	CString propGet;
	gmVariable propReturn; 
	CString propData = pNPC->getProp(pServer->npcPropAccess[prop].propEnumValue);

	switch(pServer->npcPropAccess[prop].packetType)
	{
	case PROP_STRING:
		propGet = propData.readChars(propData.readGUChar());
		propReturn.SetString(pNPC->GetMachine()->AllocStringObject(propGet.text(), propGet.length()));
		a_operands[0] = propReturn;
		break;
	case PROP_CHAR: //prop type is int/float
		switch (pServer->npcPropAccess[prop].valueType)
		{
			case PROP_FLOAT:
			{
				if (prop == "x" || prop == "y") 
				{
					// float posVal = (float)strtofloat(propData);
					int posVal = propData.readGUChar();
					posVal /= 2;
					printf("\n\nPOS POS POS %i\n\n",posVal);
					propReturn.SetInt(posVal);
					a_operands[0] = propReturn;
					// printf("Got known SPECIALFLOAT prop %s, value %f\n",prop.text(),propReturn);
					printf("Got known SPECIALFLOAT prop %s, value %i\n",prop.text(),propReturn.GetInt());
					break;
				}
				propReturn.SetFloat((float)strtofloat(propData));
				a_operands[0] = propReturn;
				printf("Got known FLOAT prop %s, value %f\n",prop.text(),propReturn);
				break;
			}
			case PROP_INT:
				propReturn.SetInt(propData.readGUInt());
				a_operands[0] = propReturn;
				printf("Got known INT prop %s\n",prop.text());
				break;
		}
		printf("Got known CHAR prop %s\n",prop.text());
		break;
		/*propGet = propData.readGUChar();
		propReturn.SetString(pNPC->GetMachine()->AllocStringObject(propGet.text(), propGet.length()));
		a_operands[0] = propReturn;
		break;*/
	}
}

//Function: npcrobj.var=val;
//Returns:non, sets var to val
void GM_CDECL TNPC::TNPCSetDot(gmThread * a_thread, gmVariable * a_operands)
{
	//O_SETDOT,           // object, value, "member"   (tos is a_operands + 3)
	GM_ASSERT(a_operands[0].m_type == TNPC::s_typeId);
	gmUserObject * user = (gmUserObject *) GM_OBJECT(a_operands[0].m_value.m_ref);
	TNPC * pNPC = (TNPC *) user->m_user;

	CString propName = a_operands[2].GetCStringSafe();

	if (pNPC->GetServer()->npcPropAccess[propName].valueType == PROP_UNKOWN) 
	{
		CString propName = a_operands[2].GetCStringSafe();
		pNPC->setVar(propName,a_operands[1]);
//		printf("Trying to set UNKNOWN prop %s to %s...\n",propName.text(),a_operands[1]);
		return;
	}

	CString propStringValue;
	float propFloatValue;

	if (a_operands[1].IsFloat()) propStringValue = propFloatValue = a_operands[1].GetFloat();
	
	else if (a_operands[1].IsInt()) 
	{	
		propFloatValue = (float)a_operands[1].GetInt();
		propStringValue = a_operands[1].GetInt();
		printf("Trying to set INT value of prop %s to %i...\n",propName.text(),a_operands[1].GetInt());
	}

	else propStringValue = a_operands[1].GetCStringSafe();

	TNPCServer * pServer = pNPC->GetServer();

	//SPECIAL NEEDS
	if (propName == "x" || propName == "y") 
	{
		if (!propStringValue.isEmpty())
			propFloatValue = (float)strtofloat(propStringValue);
		propFloatValue *= 2;
	}
	
	//END SPECIAL NEEDS

	if (!pServer->npcPropAccess[propName].readOnly)
	{
		printf("Trying to set prop %s...\n",propName.text());
		//What type of value are we trying to set?
		switch (pServer->npcPropAccess[propName].valueType)
		{
		case PROP_STRING://Prop type is String
			printf("Trying to set STRING prop %s, value %s\n",propName.text(),propStringValue.text());
			pNPC->setProps(CString() >> (char)pServer->npcPropAccess[propName].propEnumValue >> (char)propStringValue.length() << propStringValue.text());
			break;
		case PROP_FLOAT:
			printf("Trying to set FLOAT prop %s, value %f\n",propName.text(),propFloatValue);
			// pNPC->setProps(CString() >> (float)pServer->npcPropAccess[propName].propEnumValue >> (float)propFloatValue;
			pNPC->setProps(CString() >> (char)pServer->npcPropAccess[propName].propEnumValue >> (char)propFloatValue);
			break;
		case PROP_INT: //prop type is int/float
			//How should we send the packet?
			printf("Trying to set INT prop %s...\n",propName.text());
			switch (pServer->npcPropAccess[propName].packetType)
			{
			case PROP_CHAR://prop packet type is char
				printf("Trying to set CHAR prop %s to %f\n",propName.text(),propFloatValue);
				pNPC->setProps(CString() >> (char)pServer->npcPropAccess[propName].propEnumValue >> (char)propFloatValue);
				break;
			case PROP_INT://prop packet type is int
				printf("Trying to set INT prop %s, value %i\n",propName.text(),(int)propFloatValue);
				pNPC->setProps(CString() >> (char)pServer->npcPropAccess[propName].propEnumValue >> (int)propFloatValue);
				break;
			}
			break;
		}
		printf("No matches.\n",propName.text());
		pNPC->SendProp(pServer->npcPropAccess[propName].propEnumValue);
	}
}

/*
CString doJoins(const CString& code, CFileSystem* fs)
{
	CString ret;
	CString c(code);
	std::vector<CString> joinList;

	// Parse out all the joins.
	while (c.bytesLeft())
	{
		ret << c.readString("join ");
		if (c.bytesLeft())
		{
			ret << ";\xa7";
			joinList.push_back(CString() << c.readString(";") << ".txt");
		}
	}

	// Add the files now.
	for (std::vector<CString>::iterator i = joinList.begin(); i != joinList.end(); ++i)
	{
		//printf("file: %s\n", (*i).text());
		c = fs->load(*i);
		c.removeAllI("\r");
		c.replaceAllI("\n", "\xa7");
		ret << removeComments(c, "\xa7");
	}

	return ret;
}*/
