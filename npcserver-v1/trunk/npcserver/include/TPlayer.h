#include <iostream>
using namespace std;
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "CString.h"
#include "IUtil.h"
#include <map>
#include "CSocket.h"
#include "CList.h"
#include "TNPCServer.h"
#include <stdio.h>
#include "CFileQueue.h"
#include "gmThread.h"

#ifndef TGRAALPLAYER_H
#define TGRAALPLAYER_H

#define propscount	83

// -- Minor Macros -- //
#define ATTRLIST(GATTR, ID) case GATTR: mPlayerAttr[ID] = pPropPacket.readChars(pPropPacket.readGUChar()); break;

class TNPCServer;
class TScriptClass;
class TScriptWeapon;

// -- Class Definitions -- //
class TPlayer
{
	public:
		// -- Static Initialization -- //
		static void RegisterObject();
		static bool created;

		// -- Constructor | Destructor -- //
		TPlayer(TNPCServer *pServer, int pId);
		~TPlayer();

		// -- Functions -- //
		CString GetPlayerProp(int pPropId);
		void SetProps(CString& pPropPacket, bool npcServer);
		void SendProp(int pPropId);
		void UpdateWeaponList(std::vector<CString> weapons);
		void AddWeapon(CString weapon);
		void DeleteWeapon(CString weapon);
									
		// -- Returns Values -- //
		inline const CString& GetAccount()			const { return mPlayerAccount; }
		inline const CString& GetLevel()			const { return mPlayerLevel; }
		inline const CString& GetNick()				const { return mPlayerNick; }
		inline int GetId()							const { return mPlayerId; }
		inline const std::vector<CString> getWeaponList() const {return mWeaponList;}
		std::map<CString, CString> GetPropList()	const { return mPropList; }		

		TNPCServer * GetServer()	{return mServer;}

		void SetFlag(const CString& pFlagName, const CString& pFlagValue);
		void SendFlag(const CString& pFlagName);
		void SendPM(const CString& pMessage);

		//Game Monkey Access
		gmTableObject * m_table;
		static gmType s_typeId;

		gmMachine machine;

		float mX,mY;
		int mPlayerDir;

		std::map<int,bool> touchingnpcs;

	protected:
		// Variables -> Player Properties
		
		bool mPlayerInit;
		CString mPlayerAccount, mPlayerAni, mPlayerAttr[30], mPlayerChat, mPlayerLanguage, mPlayerNick;
		int  mPlayerId;
		
		CString mBodyImage, mHeadImage, mShieldImage, mSwordImage;
		float mCurPower;
		
		int mApCount, mArrowCount, mBombCount, mDeathsCount, mGralatCount, mKillsCount, mMagicCount;
		int mBombPower, mGlovePower, mMaxPower, mShieldPower, mSwordPower;
		int mAdditFlags, mCarrySprite, mOnlineTime, mStatus;
		std::map<CString, CString> mFlagList;
		std::map<CString, CString> mPropList;
		std::vector<CString> mWeaponList;

		// Variables -> Properties
		CString mPlayerLevel;

		// Variables -> Server
		TNPCServer * mServer;
};



#endif