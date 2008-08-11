#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include <map>
#include "CString.h"
//#include "TServer.h"

class TServer;
class CFileSystem
{
	public:
		CFileSystem() : server(0) { }
		CFileSystem(TServer* pServer);

		void setServer(TServer* pServer) { server = pServer; }
		void load();

		CString find(const CString& file) const;

	private:
		TServer* server;
		std::map<CString, CString> fileList;
};

#endif
