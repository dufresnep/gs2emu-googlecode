#ifndef TSCRIPTCLASS_H
#define TSCRIPTCLASS_H

#include "CString.h"

/*
	- join:
	  This normally loads a text file (join bomys; will
	  load bomys.txt from the configured scripts folder)
	  and adds it to the npc script, so it works
	  similar to an include;
	  the internal representation is slightly different:
	  each npc has a list of scripts, the own script
	  is parsed and added as the first script in this list;
	  when you do join then another script is parsed
	  and added to the script list;
	  when the script for the requested class is already
	  in the memory (used by another npc) then it is not
	  copied, it is just added to the script list;
	  so when several npcs are using the same class
	  then it is only hold one time in the memory and
	  the script object is put into the script lists
	  of the npcs;
	  functions can be overwritten

  Some Notes:
	Compile the script once, and use the 'script-table'
	or object in all other references? If that's possible
	with whatever scripting system we use, I guess.. I'm
	just assuming that is what he means btw. - Joey
*/

class TScriptClass
{
	public:
		// -- Constructor | Destructor -- //
		TScriptClass(const CString& pClassName, const CString& pClassCode);
		~TScriptClass() { }

		// -- Functions -- //
		void SetClassCode(const CString& pClassCode);
		void SetClassName(const CString& pClassName);

		// -- Returns Class Code -- //
		inline const CString& GetClassCode() const
		{
			return mClassCode;
		}

		// -- Returns Class Name -- //
		CString GetScriptClassName() 
		{
			return mClassName;
		}

		// -- Return Script Table -- //
		inline void * GetScriptTable()
		{
			return mScriptTable;
		}

	protected:
		CString mClassCode, mClassName;
		void *mScriptTable; // ?
};

#endif
