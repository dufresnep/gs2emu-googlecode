#include "TScriptClass.h"

// -- Constructor -- //
TScriptClass::TScriptClass(const CString& pClassName, const CString& pClassCode)
{
	this->mClassName = pClassName;
	this->mClassCode = pClassCode;
}

// - Set Class Code -- //
void TScriptClass::SetClassCode(const CString& pClassCode)
{
	this->mClassCode = pClassCode;
}

// -- Set Class Name -- //
void TScriptClass::SetClassName(const CString& pClassName)
{
	this->mClassName = pClassName;
}
