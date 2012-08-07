#include "main.h"
#include "TServerList.h"

/*
	Global Variables
*/

const char *serverTypes[] = {"P ", "U ", "H "};
#define serverTypesCount 3

/*
	Constructor
*/

TServer::TServer()
{
	name = language = description = url = version = ip = "";;
	playercount = port = type = 0;

	item = new QTreeWidgetItem();
}

/*
	Destructor
*/

TServer::~TServer()
{
	if (item != NULL)
		delete item;

	ServerWindow->getServerList().remove(this);
}

/*
	Functions
*/

// get
CBuffer& TServer::getDescription()
{
	return description;
}

CBuffer& TServer::getIp()
{
	return ip;
}

CBuffer& TServer::getLanguage()
{
	return language;
}

CBuffer& TServer::getName()
{
	return name;
}

CBuffer& TServer::getUrl()
{
	return url;
}

CBuffer& TServer::getVersion()
{
	return version;
}

int TServer::getPCount()
{
	return playercount;
}

int TServer::getPort()
{
	return port;
}

int TServer::getType()
{
	return type;
}

QTreeWidgetItem* TServer::getItem(int pId)
{
	// Declare Variables
	QString imageName;

	// Set imageName by type
	switch (type)
	{
		case TYPE_HIDDEN: imageName = ":/RC/icon_hidden.png"; break;
		case TYPE_HOSTED: imageName = ":/RC/icon_hosted.png"; break;
		case TYPE_GOLD: imageName = ":/RC/icon_gold.png"; break;
		default: imageName = ":/RC/icon_classic.png"; break;
	}

	// Set Item
	item->setIcon(0, QIcon(imageName));
	item->setText(0, name.text());
	item->setText(1, toString(playercount).text());
	item->setStatusTip(0, toString(pId).text());
	return item;
}

// set
void TServer::setDescription(CBuffer pBuffer)
{
	description = pBuffer;
}

void TServer::setIp(CBuffer pBuffer)
{
	ip = pBuffer;
}

void TServer::setLanguage(CBuffer pBuffer)
{
	language = pBuffer;
}

void TServer::setName(CBuffer pBuffer)
{
	// Grab Type
	for (int i = 0; i < serverTypesCount; i++)
	{
		if (pBuffer.copy(0, 2) == serverTypes[i])
		{
			type = i+1;
			break;
		}
	}

	// Set Name
	name = (type == 0 ? pBuffer : pBuffer.copy(2));
}

void TServer::setPCount(int pInt)
{
	playercount = pInt;
}

void TServer::setPort(int pInt)
{
	port = pInt;
}

void TServer::setType(int pInt)
{
	type = pInt;
}

void TServer::setUrl(CBuffer pBuffer)
{
	url = pBuffer;
}

void TServer::setVersion(CBuffer pBuffer)
{
	version = pBuffer;
}
