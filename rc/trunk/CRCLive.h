#ifndef CRCLIVE_H
#define CRCLIVE_H

#include <QtGui>
#include "CBuffer.h"
#include "CClasses.h"
#include "CList.h"
#include "CPacket.h"
#include "CRCAccount.h"
#include "CRCBrowser.h"
#include "CRCSock.h"
#include "CSocket.h"
#include "zlib.h"

#include "ui_chat.h"
#include "ui_main.h"

class CRCChat;
class CRCLive;

// RemoteControl RC GUI
class CRCLive : public QMainWindow, private Ui::CRCLive
{
	Q_OBJECT
	
	public:
		CRCLive(QMainWindow *parent = 0);
		
		void AddChat(CRCSock *sock, QString chat);
		void PlayerAdd(CRCChat *temp, CPlayer *player);
		void PlayerRem(CRCChat *temp, CString name);
		void PlayerRem(CRCChat *temp, int id);
		void UpdatePlayers();
		void UpdateServer();
		CRCChat* TabAdd(CRCSock *sock, QString tab);
		void TabRem(CRCSock *sock);
		void TabRem(CRCSock *sock, CString tab);
		
		friend int ReturnID(CRCSock *sock);
		friend int ReturnID(CRCSock *sock, CString tab);
		friend CPlayer* FindPlayerID(CRCSock *sock, int pId);
		friend CRCChat* ReturnTab(CRCSock *sock);
		friend CRCChat* ReturnTab(CRCSock *sock, CString tabname);
		friend QListWidgetItem* ReturnItem(CRCSock *sock, int pId);
		
	public slots:
		void SendChat();
		void TabChanged(int tab);
		
		void sbFileBrowser();
		
	protected:
		void keyPressEvent(QKeyEvent *event);
};

// RemoteControl RC Chat Widget
class CRCChat : public QWidget, public Ui::CRCChat
{
	Q_OBJECT
	
	public:
		CRCChat(CRCSock *sock, CString server, QWidget *parent = 0);
		~CRCChat();
		
		bool ismain;
		CList Players;
		CList WVIEWACCOUNT;
		CRCBrowser *browser;
		CRCSock *sock;
		CString server;
		
		void createWindow(int type, CPacket& data);
};

#endif