#ifndef TLEVEL_H
#define TLEVEL_H

#include <vector>
#include <map>

class TLevel
{
	public:
		// level-loading functions
		bool loadLevel(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels);

	private:
		bool loadGraal(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels);
		bool loadZelda(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels);
		bool loadNW(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels);
};

#endif // TLEVEL_H
