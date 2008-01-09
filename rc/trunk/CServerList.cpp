#include "CServerList.h"
#include "main.h"

CServer::~CServer()
{
	Serverlist.remove(this);
}

CServer::CServer(CString name, CString language, CString description, CString url, CString version, int playercount, CString ip, int port)
{
	this->type        = (name.copy(0, 2) == "P " ? GOLD : (name.copy(0, 2) == "H " ? HOSTED : CLASSIC));
	this->name        = (type != CLASSIC ? name.copy(2, name.length() - 2) : name);
	
	this->id          = Serverlist.count();
	this->language    = language;
	this->description = description;
	this->url         = url;
	this->version     = version;
	this->playercount = playercount;
	this->ip          = ip;
	this->port        = port;
}