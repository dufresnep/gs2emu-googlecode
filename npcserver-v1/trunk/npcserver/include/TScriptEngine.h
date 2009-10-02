#ifndef TSCRIPTENGINE_H
#define TSCRIPTENGINE_H

#include <math.h>
#include <vector>
#include "CString.h"
#include "gmThread.h"
#include "MiscFunctions.h"
#include "TNPCServer.h"
#include "TPlayer.h"
#include "gmCall.h"

class TNPCServer;

class TScriptEngine
{
	public:

		///Functions///
		void callFunction(CString function, TPlayer * pPlayer, CString params);

	protected:

		//Functions
		void compileScript();
		void parseToGM(CString script);

		//Set/Add
		void setThisVar(CString prop, gmVariable value) {thisVariables[prop] = value;}
		void addFunction(CString funcName, gmFunctionObject * opcode) {thisFunctions[funcName] = opcode;}

		//GM Set and Get
		static void GM_CDECL TScriptGetDot(gmThread * a_thread, gmVariable * a_operands);
		static void GM_CDECL TScriptSetDot(gmThread * a_thread, gmVariable * a_operands);

		//Get
		gmVariable getThisVar(CString prop) {return thisVariables[prop];}
		gmFunctionObject * getFunction(CString funcName){return thisFunctions[funcName];}
		
		//Variables
		gmType scriptType;

	private:
		
		///Functions///
		//Misc
		void Reset();

		///Variables///
		CString mGameMonkeyScript;
		TNPCServer * mServer;
		std::map<CString,gmVariable> thisVariables;
		std::map<CString, gmFunctionObject *> thisFunctions;
};

#endif
