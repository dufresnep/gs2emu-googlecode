#include <QApplication>
#include "CList.h"
#include "CRCLive.h"
#include "CRCSock.h"
#include "CRemoteControl.h"
#include "main.h"

CList Connections, Serverlist;
CRemoteControl *RC;
CString cAccount, cNickname, cPassword;
QStringList colors;

int main(int argc, char *argv[])
{
	// Register MetaTypes
	qRegisterMetaType<CPacket>("CPacket&");
	qRegisterMetaType<CPacket>("CPacket");
	qRegisterMetaType<CPacket>("CString");
	
	QApplication app(argc, argv);
	
	// Create Color List
	loadClothes();
	
	// Create RemoteControl Window
	RC = new CRemoteControl();
	RC->showLogin(true);
	
	// Start new Thread
	CRCThread *thread = new CRCThread();
	thread->start();
	
	return app.exec();
}

void CRCThread::run()
{
	while (true)
	{
		for (int i = 0; i < Connections.count(); i++)
		{
			CRCSock *RCSock = (CRCSock*)Connections[i];
			
			if (!RCSock->main())
			{
				delete RCSock;
				i--;
			}
		}
		
		Sleep(100);
	}
}

CPlayer* FindPlayerID(CRCSock *sock, int pId)
{
	CRCChat *temp = ReturnTab(sock);
	
	if (temp == NULL)
		return NULL;
	
	for (int i = 0; i < temp->Players.count(); i++)
	{
		CPlayer *player = (CPlayer*)temp->Players[i];
		
		if (player->id == pId)
		{
			return player;
		}
	}
	
	return NULL;
}

QListWidgetItem* ReturnItem(CRCSock *sock, int pId)
{
	CRCChat *temp = ReturnTab(sock);
	
	if (temp == NULL)
		return NULL;
	
	for (int i = 0; i < temp->listWidget->count(); i++)
	{
		QListWidgetItem *item = (QListWidgetItem*)temp->listWidget->item(i);
		
		if (item->statusTip().toInt() == pId)
		{
			return item;
		}
	}
	
	return NULL;
}

int ReturnID(CRCSock *sock)
{
	for (int i = 0; i < RC->RCLive->tabWidget->count(); i++)
	{
		CRCChat *temp = (CRCChat*)RC->RCLive->tabWidget->widget(i);
		
		if (temp->sock == sock)
		{
			return i;
		}
	}
	
	return -1;
}

int ReturnID(CRCSock *sock, CString tab)
{
	for (int i = 0; i < RC->RCLive->tabWidget->count(); i++)
	{
		CRCChat *temp = (CRCChat*)RC->RCLive->tabWidget->widget(i);
		
		if (temp->sock == sock && temp->server == tab)
		{
			return i;
		}
	}
	
	return -1;
}

CRCChat* ReturnTab(CRCSock *sock)
{
	for (int i = 0; i < RC->RCLive->tabWidget->count(); i++)
	{
		CRCChat *temp = (CRCChat*)RC->RCLive->tabWidget->widget(i);
		
		if (temp->sock == sock)
		{
			return temp;
		}
	}
	
	return NULL;
}

CRCChat* ReturnTab(CRCSock *sock, CString tab)
{
	for (int i = 0; i < RC->RCLive->tabWidget->count(); i++)
	{
		CRCChat *temp = (CRCChat*)RC->RCLive->tabWidget->widget(i);
		
		if (temp->sock == sock && temp->server == tab)
		{
			return temp;
		}
	}
	
	return NULL;
}

void loadClothes()
{
	const char *colours[] = {"white", "yellow", "orange", "pink", "red", "darkred", "lightgreen", "green", "darkgreen", "lightblue", "blue", "darkblue", "brown", "cynober", "purple", "darkpurple", "lightgray", "gray", "black", "transparent"};
	for (int i = 0; i < sizeof(colours) / 4; i++)
		colors << colours[i];
}

/*
c:\Qt\4.3\static\lib\qtmain.lib
c:\Qt\4.3\static\lib\QtGui.lib
c:\Qt\4.3\static\lib\QtCore.lib
imm32.lib
winmm.lib
ws2_32.lib
zdll.lib
*/