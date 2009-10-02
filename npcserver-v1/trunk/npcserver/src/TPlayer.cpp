#include "TPlayer.h"
#include "TNPCServer.h"


class TWeaponScript;
class gmMachine;
// -- Static Initialization -- //
bool TPlayer::created = false;
typedef TPlayer * (TPlayer::*P2F)(const CString&);
std::map<CString, P2F> functions;

void TPlayer::RegisterObject()
{
	// Pointer-Functions
	if (!TPlayer::created)
	{
		TPlayer::created = true;
	}
}
gmType TPlayer::s_typeId = GM_STRING;

// -- Constructor -- //
TPlayer::TPlayer(TNPCServer *pServer, int pId)
{
	this->mServer		= pServer;
	this->mPlayerId		= pId;
	this->mPlayerInit	= false;
	m_table =  machine.AllocTableObject();
}

void TPlayer::UpdateWeaponList(std::vector<CString> weapons)
{
	mWeaponList = weapons;
}

void TPlayer::AddWeapon(CString weapon)
{
	TScriptWeapon * checkWeapon = mServer->getWeapon(weapon);

	if (checkWeapon == 0) return;

	mWeaponList.push_back(weapon);
}

void TPlayer::DeleteWeapon(CString weapon)
{
	TScriptWeapon * checkWeapon = mServer->getWeapon(weapon);

	if (checkWeapon == 0) return;

	vecRemove<CString>(mWeaponList, weapon);
}

// -- Function: Get Prop -- //
CString TPlayer::GetPlayerProp(int pPropId)
{
	switch (pPropId)
	{
		case PLPROP_NICKNAME: // 0
			return CString() >> (char)mPlayerNick.length() << mPlayerNick;

		case PLPROP_MAXPOWER: // 1
			return CString() >> (char)mMaxPower;

		case PLPROP_CURPOWER: // 2
			return CString() >> (char)(mCurPower * 2);

		case PLPROP_RUPEESCOUNT: // 3
			return CString() >> (int)mGralatCount;

		case PLPROP_ARROWSCOUNT: // 4
			return CString() >> (char)mArrowCount;

		case PLPROP_BOMBSCOUNT: // 5
			return CString() >> (char)mBombCount;

		case PLPROP_GLOVEPOWER: // 6
			return CString() >> (char)mGlovePower;

		case PLPROP_BOMBPOWER: // 7
			return CString() >> (char)mBombPower;

		case PLPROP_SWORDPOWER: // 8
			return CString() >> (char)(mSwordPower+30) >> (char)mSwordImage.length() << mSwordImage;

		case PLPROP_SHIELDPOWER: // 9
			return CString() >> (char)(mShieldPower+10) >> (char)mShieldImage.length() << mShieldImage;

		case PLPROP_GANI: // 10
			return CString() >> (char)mPlayerAni.length() << mPlayerAni;

		case PLPROP_HEADGIF: // 11
			return CString() >> (char)(mHeadImage.length()+100) << mHeadImage;

		case PLPROP_CURCHAT: // 12
			return CString() >> (char)mPlayerChat.length() << mPlayerChat;
			
		case PLPROP_X: //15
			return CString() >> (char)(mX * 2);
		case PLPROP_Y: //16
			return CString() >> (char)(mY * 2);

		case PLPROP_SPRITE: // 17
			return CString() >> (char)(mPlayerDir % 4);
		case PLPROP_CURLEVEL:
			return CString() >> (char)mPlayerLevel.length() << mPlayerLevel;

		case PLPROP_KILLSCOUNT: // 27
			return CString() >> (int)mKillsCount;

		case PLPROP_DEATHSCOUNT: // 28
			return CString() >> (int)mDeathsCount;
		
		case PLPROP_ONLINESECS: // 29
			return CString() >> (int)mOnlineTime;

		case PLPROP_ALIGNMENT: // 32
			return CString() >> (char)mApCount;
		case PLPROP_ACCOUNTNAME: //34
			return CString() >> (char)mPlayerAccount.length() << mPlayerAccount;

		case PLPROP_BODYIMG: // 35
			return CString() >> (char)mBodyImage.length() << mBodyImage;
		
	}

	return CString();
}

// -- Function: Set Props -- //
void TPlayer::SetProps(CString& pPropPacket, bool npcServer)
{
	// Definitions
	int len = 0, sp = 0;

	// Parse Properties Packet
	while (pPropPacket.bytesLeft() > 0)
	{
		unsigned char propId = pPropPacket.readGUChar();

		switch (propId)
		{
			// Player Nickname
			case PLPROP_NICKNAME: // 0
				mPlayerNick = pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// Max Power
			case PLPROP_MAXPOWER: // 1
				mMaxPower = pPropPacket.readGUChar();
				break;

			// Current Power
			case PLPROP_CURPOWER: // 2
				mCurPower = (float)pPropPacket.readGUChar() / 2;
				break;

			// Current Rupees
			case PLPROP_RUPEESCOUNT: // 3
				mGralatCount = pPropPacket.readGUInt();
				break;

			// Arrows Count
			case PLPROP_ARROWSCOUNT: // 4
				mArrowCount = pPropPacket.readGUChar();
				break;

			// Bombs Count
			case PLPROP_BOMBSCOUNT: // 5
				mBombCount = pPropPacket.readGUChar();
				break;

			// Glove Power
			case PLPROP_GLOVEPOWER: // 6
				mGlovePower = pPropPacket.readGUChar();
				break;

			// Bomb Power
			case PLPROP_BOMBPOWER: // 7
				mBombPower = pPropPacket.readGUChar();
				break;

			// Sword Power
			case PLPROP_SWORDPOWER: // 8
				if ((sp = pPropPacket.readGUChar()) > 4)
				{
					sp -= 30;
					len = pPropPacket.readGUChar();
					mSwordImage = (len > 0 ? pPropPacket.readChars(len) : "");
				}
				else mSwordImage = CString() << "sword" << sp << ".png";
				mSwordPower = sp;
				break;

			// Shield Power
			case PLPROP_SHIELDPOWER: // 9
				if ((sp = pPropPacket.readGUChar()) > 3)
				{
					sp -= 10;
					if (sp < 0)
						break;
					len = pPropPacket.readGUChar();
					mShieldImage = (len > 0 ? pPropPacket.readChars(len) : "");
				}
				else mShieldImage = CString() << "shield" << sp << ".png";

				mShieldPower = sp;
				break;

			// Gani
			case PLPROP_GANI: // 10
				pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// Head Image
			case PLPROP_HEADGIF: // 11
				len = pPropPacket.readGUChar();
				mHeadImage = (len < 100 ? CString("head") << len << ".png" : pPropPacket.readChars(len - 100));
				break;

			// Chat Text
			case PLPROP_CURCHAT: // 12
				{
					mPlayerChat = pPropPacket.readChars(pPropPacket.readGUChar());
					if (npcServer) break;
					if (mPlayerChat.isEmpty()) break;

					if (!mPlayerLevel.isEmpty())
					{
						TLevel * level = mServer->getLevel(mPlayerLevel);

						if (level != 0) level->callNPCS("onPlayerchats",this,"");

						std::vector<CString> weaponList = getWeaponList();

						for (std::vector<CString>::iterator i = weaponList.begin(); i != weaponList.end(); ++i)
						{
							TScriptWeapon * pWeapon = mServer->getWeapon(*i);

							if (pWeapon == 0) continue;

							pWeapon->callFunction("onPlayerchats",this,"");
						}
					}
				}
				break;

			// Colors
			case PLPROP_COLORS: // 13
				for (unsigned int i = 0; i < 5; i++)
					pPropPacket.readGUChar();
				break;

			// Player Id
			case PLPROP_ID: // 14
				pPropPacket.readGUShort();
				break;

			// Player X
			case PLPROP_X: // 15
				mX = (float)(pPropPacket.readGUChar() / 2.0f);
				
				break;

			// Player Y
			case PLPROP_Y: // 16
				mY = (float)(pPropPacket.readGUChar() / 2.0f);
				break;

			// Player Sprite
			case PLPROP_SPRITE: // 17
				mPlayerDir = pPropPacket.readGUChar() % 4;
				break;

			// Player Status
			case PLPROP_STATUS: // 18
				mStatus = pPropPacket.readGUChar();
				break;

			// Player Carry Sprite?
			case PLPROP_CARRYSPRITE: // 19
				mCarrySprite = pPropPacket.readGUChar();
				break;

			// Player Level
			case PLPROP_CURLEVEL: // 20
				{
					if (!mPlayerLevel.isEmpty() && !npcServer)
					{
						TLevel * level = mServer->getLevel(mPlayerLevel);
						if (level != 0) level->removePlayer(this);
					}

					mPlayerLevel = pPropPacket.readChars(pPropPacket.readGUChar()).trim();

					if (npcServer) break;

					if (mPlayerLevel.isEmpty()) break;

					TLevel * newLevel = mServer->getLevel(mPlayerLevel);

					if (newLevel == 0) 
					{
						mServer->RequestLevel(mPlayerLevel);
					}
					else newLevel->addPlayer(this);
				}
				break;

			// Player Horse
			case PLPROP_HORSEGIF: // 21
				pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// Player Horse Power
			case PLPROP_HORSEBUSHES: // 22
				pPropPacket.readGUChar();
				break;

			// ?
			case PLPROP_EFFECTCOLORS: // 23
				len = pPropPacket.readGUChar();
				if (len > 0)
					pPropPacket.readGInt4();
				break;

			// Carry NPC Id
			case PLPROP_CARRYNPC: // 24
				pPropPacket.readGInt();
				break;

			// Player AP Counter
			case PLPROP_APCOUNTER: // 25
				pPropPacket.readGUShort();
				break;

			// Player MP
			case PLPROP_MAGICPOINTS: // 26
				mMagicCount = pPropPacket.readGUChar();
				break;

			// Player Kills
			case PLPROP_KILLSCOUNT: // 27
				mKillsCount = pPropPacket.readGInt();
				break;

			// Player Deaths
			case PLPROP_DEATHSCOUNT: // 28
				mDeathsCount = pPropPacket.readGInt();
				break;

			// Online Time
			case PLPROP_ONLINESECS: // 29
				mOnlineTime = pPropPacket.readGUInt();
				break;

			// Player IP
			case PLPROP_IPADDR: // 30
				pPropPacket.readGInt5();
				break;

			// Player UDP
			case PLPROP_UDPPORT: // 31
				pPropPacket.readGInt();
				break;

			// Player AP
			case PLPROP_ALIGNMENT: // 32
				mApCount = pPropPacket.readGUChar();
				break;

			// Player Additional Flags
			case PLPROP_ADDITFLAGS: // 33
				mAdditFlags = pPropPacket.readGChar();
				break;

			// Player Account
			case PLPROP_ACCOUNTNAME: // 34
				{
					mPlayerAccount = pPropPacket.readChars(pPropPacket.readGUChar());
					mPropList["account"] = mPlayerAccount;
				}
				break;

			// Player Body Image
			case PLPROP_BODYIMG: // 35
				mBodyImage = pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// Player Rating
			case PLPROP_RATING: // 36
				pPropPacket.readGInt();
				break;

			ATTRLIST(PLPROP_GATTRIB1, 0);
			ATTRLIST(PLPROP_GATTRIB2, 1);
			ATTRLIST(PLPROP_GATTRIB3, 2);
			ATTRLIST(PLPROP_GATTRIB4, 3);
			ATTRLIST(PLPROP_GATTRIB5, 4);
			ATTRLIST(PLPROP_GATTRIB6, 5);
			ATTRLIST(PLPROP_GATTRIB7, 6);
			ATTRLIST(PLPROP_GATTRIB8, 7);
			ATTRLIST(PLPROP_GATTRIB9, 8);
			ATTRLIST(PLPROP_GATTRIB10, 9);
			ATTRLIST(PLPROP_GATTRIB11, 10);
			ATTRLIST(PLPROP_GATTRIB12, 11);
			ATTRLIST(PLPROP_GATTRIB13, 12);
			ATTRLIST(PLPROP_GATTRIB14, 13);
			ATTRLIST(PLPROP_GATTRIB15, 14);
			ATTRLIST(PLPROP_GATTRIB16, 15);
			ATTRLIST(PLPROP_GATTRIB17, 16);
			ATTRLIST(PLPROP_GATTRIB18, 17);
			ATTRLIST(PLPROP_GATTRIB19, 18);
			ATTRLIST(PLPROP_GATTRIB20, 19);
			ATTRLIST(PLPROP_GATTRIB21, 20);
			ATTRLIST(PLPROP_GATTRIB22, 21);
			ATTRLIST(PLPROP_GATTRIB23, 22);
			ATTRLIST(PLPROP_GATTRIB24, 23);
			ATTRLIST(PLPROP_GATTRIB25, 24);
			ATTRLIST(PLPROP_GATTRIB26, 25);
			ATTRLIST(PLPROP_GATTRIB27, 26);
			ATTRLIST(PLPROP_GATTRIB28, 27);
			ATTRLIST(PLPROP_GATTRIB29, 28);
			ATTRLIST(PLPROP_GATTRIB30, 29);

			// unknown packet 42
			case PLPROP_UNKNOWN42: // 42
				pPropPacket.readGInt4();
				break;

			// GMap Level X
			case PLPROP_GMAPLEVELX: // 43
				pPropPacket.readGUChar();
				break;

			// GMap Level Y
			case PLPROP_GMAPLEVELY: // 44
				pPropPacket.readGUChar();
				break;

			// Player Z
			case PLPROP_Z: // 45
				pPropPacket.readGUChar();
				break;

			// don't know..
			case PLPROP_JOINLEAVELVL: // 50
				pPropPacket.readGUChar();
				break;

			// Disconnect Player..?
			case PLPROP_PCONNECTED: // 51
				//mServer->NC_DelPlayer(mPlayerId);
				break;

			// Player Language
			case PLPROP_PLANGUAGE: // 52
				mPlayerLanguage = pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// 'Status Msg?'
			case PLPROP_PSTATUSMSG: // 53
				pPropPacket.readGUChar();
				break;

			// Operating System
			case PLPROP_OSTYPE: // 75
				pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// Code Page
			case PLPROP_TEXTCODEPAGE: // 76
				pPropPacket.readGInt();
				break;

			// Unknown Packet
			case PLPROP_UNKNOWN77: // 77
				break;

			// Player X
			case PLPROP_X2: // 78
				pPropPacket.readGShort();
				break;

			// Player Y
			case PLPROP_Y2: // 79
				pPropPacket.readGShort();
				break;

			// Player Z
			case PLPROP_Z2: // 80
				pPropPacket.readGShort();
				break;

			// Unknown Packet
			case PLPROP_UNKNOWN81: // 81
				break;

			// 'Community Name'
			case PLPROP_COMMUNITYNAME: // 82
				pPropPacket.readChars(pPropPacket.readGUChar());
				break;

			// Invalid Property
			default:
				printf("Invalid Property: %i\n", propId);
				return;
		}
	}

	// Call onPlayerLogin Event..
	if (!mPlayerInit && !mPlayerAccount.isEmpty())
	{
		//mServer->callNPCEvent("onPlayerLogin", this);
		mPlayerInit = true;
	}
}

// -- Function: Send Prop -- //
void TPlayer::SendProp(int pPropId)
{
	mServer->sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_PLAYERPROPSSET >> (short)mPlayerId >> (char)pPropId << GetPlayerProp(pPropId));
}

void TPlayer::SetFlag(const CString& pFlagName, const CString& pFlagValue)
{
	// Delete or Set
	if (pFlagValue.isEmpty())
		mFlagList.erase(pFlagName);
	else
		mFlagList[pFlagName] = pFlagValue;
}

void TPlayer::SendFlag(const CString& pFlagName)
{
	//mServer->npc_control.sendPacket(CString() >> (char)NCO_NC_QUERY >> (char)NCREQ_SETFLAG >> (short)mPlayerId << pFlagName << "=" << mFlagList[pFlagName]);
}

void TPlayer::SendPM(const CString& pMessage)
{
	//mServer->sendPM(mPlayerId, pMessage);
}