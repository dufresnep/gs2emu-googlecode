#include <sys/stat.h>
#include <dirent.h>
#include <map>
#include "CString.h"
#include "CFileSystem.h"

#if defined(_WIN32) || defined(_WIN64)
	#define fSep "\\"
#else
	#define fSep "/"
#endif

extern CString homepath;

static void loadAllDirectories(std::map<CString, CString>& fileList, const CString& directory);

CFileSystem::CFileSystem()
{
}

void CFileSystem::init()
{
	fileList.clear();
	loadAllDirectories(fileList, CString() << homepath << "levels" << fSep);
}

CString CFileSystem::find(const CString& file) const
{
	std::map<CString, CString>::const_iterator i = fileList.find(file);
	if (i == fileList.end()) return CString();
	return CString(i->second);
}

#if defined(_WIN32) || defined(_WIN64)
void loadAllDirectories(std::map<CString, CString>& fileList, const CString& directory)
{
	CString searchdir = CString() << directory << "*";
	WIN32_FIND_DATAA filedata;
	HANDLE hFind = FindFirstFileA(searchdir.text(), &filedata);

	if (hFind != 0)
	{
		do
		{
			if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (filedata.cFileName[0] != '.')
					loadAllDirectories(fileList, CString() << directory << CString(filedata.cFileName) << fSep);
			}
			else
			{
				// Grab the file name.
				CString file(filedata.cFileName);
				fileList[file] = CString() << directory << filedata.cFileName;
			}
		} while (FindNextFileA(hFind, &filedata));
	}
	FindClose(hFind);
}
#else
void loadAllDirectories(std::map<CString, CString>& fileList, const CString& directory)
{
	DIR *dir;
	struct stat statx;
	struct dirent *ent;

	// Try to open the directory.
	if ((dir = opendir(directory.text())) == 0)
		return;

	// Read everything in it now.
	while ((ent = readdir(dir)) != 0)
	{
		if (ent->d_name[0] != '.')
		{
			CString dir = CString() << directory << ent->d_name;
			stat(dir.text(), &statx);
			if (statx.st_mode & S_IFDIR)
			{
				loadAllDirectories(fileList, dir);
				continue;
			}
		}
		else continue;

		// Grab the file name.
		CString file(ent->d_name);
		fileList[file] = CString() << directory << ent->d_name;
	}
	closedir(dir);
}
#endif

CString CFileSystem::load(const CString& file) const
{
	// Get the full path to the file.
	CString fileName = find(file);
	if (fileName.length() == 0) return CString();

	// Load the file.
	CString fileData;
	fileData.load(fileName);

	return fileData;
}

time_t CFileSystem::getModTime(const CString& file) const
{
	// Get the full path to the file.
	CString fileName = find(file);
	if (fileName.length() == 0) return 0;

	struct stat fileStat;
	if (stat(fileName.text(), &fileStat) != -1)
		return (time_t)fileStat.st_mtime;
	return 0;
}

int CFileSystem::getFileSize(const CString& file) const
{
	// Get the full path to the file.
	CString fileName = find(file);
	if (fileName.length() == 0) return 0;

	struct stat fileStat;
	if (stat(fileName.text(), &fileStat) != -1)
		return fileStat.st_size;
	return 0;
}