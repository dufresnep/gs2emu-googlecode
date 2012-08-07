#ifndef TSERVER_H
#define TSERVER_H

#include <QTreeWidgetItem>
#include "CBuffer.h"

class TServer
{
	public:
		TServer();
		~TServer();

		// get function
		CBuffer& getDescription();
		CBuffer& getIp();
		CBuffer& getLanguage();
		CBuffer& getName();
		CBuffer& getUrl();
		CBuffer& getVersion();
		int getPCount();
		int getPort();
		int getType();
		QTreeWidgetItem* getItem(int pId);

		// set function
		void setDescription(CBuffer pBuffer);
		void setIp(CBuffer pBuffer);
		void setLanguage(CBuffer pBuffer);
		void setName(CBuffer pBuffer);
		void setPCount(int pInt);
		void setPort(int pInt);
		void setType(int pInt);
		void setUrl(CBuffer pBuffer);
		void setVersion(CBuffer pBuffer);

	private:
		CBuffer name, language, description, url, version, ip;
		int playercount, port, type;
		QTreeWidgetItem *item;
};

#endif // TSERVER_H

