#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include <map>
#include "CString.h"

class CFileSystem
{
	public:
		CFileSystem();

		void init();

		CString find(const CString& file) const;
		CString load(const CString& file) const;
		time_t getModTime(const CString& file) const;
		int getFileSize(const CString& file) const;

	private:
		std::map<CString, CString> fileList;
};

#endif
