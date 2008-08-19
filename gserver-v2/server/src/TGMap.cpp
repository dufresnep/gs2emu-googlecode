#include <map>
#include <vector>
#include "CString.h"
#include "TGMap.h"
#include "TServer.h"

TGMap::TGMap()
: modTime(0), width(0), height(0)
{
}

TGMap::TGMap(const CString& pFileName, TServer* server)
: modTime(0), width(0), height(0)
{
	load(pFileName, server);
}

bool TGMap::load(const CString& pFileName, TServer* server)
{
	CFileSystem* fileSystem = server->getFileSystem();
	CString fileName = fileSystem->find(pFileName);
	modTime = fileSystem->getModTime(pFileName);
	mapName = pFileName;

	// Load the gmap.
	std::vector<CString> fileData = CString::loadToken(fileName);

	// Parse it.
	for (std::vector<CString>::iterator i = fileData.begin(); i != fileData.end(); ++i)
	{
		// Tokenize
		std::vector<CString> curLine = i->removeAll("\r").tokenize();
		if (curLine.size() < 1)
			continue;

		// Parse Each Type
		if (curLine[0] == "WIDTH")
		{
			if (curLine.size() != 2)
				continue;

			width = strtoint(curLine[1]);
		}
		else if (curLine[0] == "HEIGHT")
		{
			if (curLine.size() != 2)
				continue;

			height = strtoint(curLine[1]);
		}
		else if (curLine[0] == "GENERATED")
		{
			if (curLine.size() != 2)
				continue;

			// Not really needed.
		}
		else if (curLine[0] == "LEVELNAMES")
		{
			levels.clear();

			++i;
			int gmapx = 0;
			int gmapy = 0;
			while (i != fileData.end())
			{
				CString line = i->removeAll("\r");
				if (line == "LEVELNAMESEND") break;

				// Untokenize the level names and put them into a vector for easy loading.
				line.guntokenizeI();
				std::vector<CString> names = line.tokenize("\n");
				for (std::vector<CString>::iterator j = names.begin(); j != names.end(); ++j)
				{
					// Check for blank levels.
					if (*j == "\r")
					{
						++gmapx;
						continue;
					}

					// Save the level into the map.
					SGMapLevel lvl(gmapx++, gmapy);
					levels[*j] = lvl;
				}

				gmapx = 0;
				++gmapy;
				++i;
			}
		}
		else if (curLine[0] == "MAPIMG")
		{
			if (curLine.size() != 2)
				continue;
			
			mapImage = curLine[1];
		}
		else if (curLine[0] == "MINIMAPIMG")
		{
			if (curLine.size() != 2)
				continue;

			miniMapImage = curLine[1];
		}
		else if (curLine[0] == "NOAUTOMAPPING")
		{
			// Clientside only.
		}
		else if (curLine[0] == "LOADFULLMAP")
		{
			// Not supported currently.
		}
		else if (curLine[0] == "LOADATSTART")
		{
			// Not supported currently.
			++i;
			while (i != fileData.end())
			{
				CString line = i->removeAll("\r");
				if (line == "LOADATSTARTEND") break;
			}
		}
		// TODO: 3D settings maybe?
	}

	return true;
}

bool TGMap::isLevelOnGMap(const CString& level) const
{
	for (std::map<CString, SGMapLevel>::const_iterator i = levels.begin(); i != levels.end(); ++i)
	{
		if (i->first == level)
			return true;
	}
	return false;
}

CString TGMap::getLevelAt(int x, int y) const
{
	for (std::map<CString, SGMapLevel>::const_iterator i = levels.begin(); i != levels.end(); ++i)
	{
		if (i->second.gmapx == x && i->second.gmapy == y)
			return i->first;
	}
	return CString();
}

int TGMap::getLevelX(const CString& level) const
{
	SGMapLevel lvl = levels.find(level)->second;
	return lvl.gmapx;
}

int TGMap::getLevelY(const CString& level) const
{
	SGMapLevel lvl = levels.find(level)->second;
	return lvl.gmapy;
}
