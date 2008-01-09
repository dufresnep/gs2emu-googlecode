#ifndef CREMOTECONTROL_H
#define CREMOTECONTROL_H

#include <QtGui>
#include "CBuffer.h"
#include "CClasses.h"
#include "CList.h"
#include "CPacket.h"
#include "CRCLive.h"
#include "CRCSock.h"
#include "CServerList.h"
#include "CSocket.h"
#include "zlib.h"

#include "ui_login.h"
#include "ui_serverlist.h"

class CRCChat;
class CRCLive;
class CRCLogin;
class CRCSList;
class CRemoteControl;

// RemoteControl Class
class CRemoteControl : public QObject
{
	Q_OBJECT
	
	public:
		CRemoteControl();
		void addSock(CRCSock *sock);
		
	public slots:
		void createError(QString error);
		void showLogin(bool visible);
		void showList(bool visible);
		void showLive(bool visible);
		void TabRem(CRCSock *sock);
		void ListPacket(CRCSock *sock, CPacket& pPacket);
		void LivePacket(CRCSock *sock, CPacket& pPacket);
		void IrcPacket(CRCSock *sock, CPacket& pPacket);

		friend class CPlayer;
		friend int ReturnID(CRCSock *sock);
		friend int ReturnID(CRCSock *sock, CString tab);
		friend CRCChat* ReturnTab(CRCSock *sock);
		friend CRCChat* ReturnTab(CRCSock *sock, CString tab);
		
	private:
		CRCLive *RCLive;
		CRCLogin *RCLogin;
		CRCSList *RCSList;
		QMenu *TrayMenu;
		QSystemTrayIcon *TrayIcon;
};

// RemoteControl Login GUI
class CRCLogin : public QMainWindow, private Ui::CRCLogin
{
	Q_OBJECT
	
	public:
		CRCLogin(QMainWindow *parent = 0);
		
	public slots:
		void connectServer();
};

// RemoteControl Serverlist GUI
class CRCSList : public QMainWindow, private Ui::CRCSList
{
	Q_OBJECT
	
	public:
		CRCSList(QMainWindow *parent = 0);
		void ServerList();
		
	public slots:
		void ServerConnect(QTreeWidgetItem * item, int column);
		void ServerRefresh();
		void ServerURL();
		void ServerView(QTreeWidgetItem * item, int column);
		
	private:
		QList<QTreeWidgetItem *> items;
	
	protected:
		void keyPressEvent(QKeyEvent *event);
};

#endif