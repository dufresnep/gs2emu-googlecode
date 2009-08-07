#include "IUtil.h"
#include "CString.h"

CString removeExtension(const CString& file)
{
	int ePos = file.findl('.');
	if (ePos == -1) return file;

	return file.subString(0, ePos);
}
