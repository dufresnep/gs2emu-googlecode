#ifndef TSERVERWINDOW_H
#define TSERVERWINDOW_H

 #include <QDesktopServices>
#include "ui_serverlist.h"
#include "TRCListSock.h"
#include "TServerList.h"

// note:
// TRCListSock is causing an error on destruct

class TServerWindow : public QMainWindow, private Ui::FServerWindow
{
    Q_OBJECT

    public:
        TServerWindow(QMainWindow *parent = 0);

		TServerList& getServerList();

		bool connectList(CBuffer pAccount, CBuffer pNickname, CBuffer pPassword);
		bool connectServer(TServer *pServer);
		void setServers(CPacket& pPacket);
		void showServers();

    private:
		TRCListSock listSock;
		TServerList serverList;

	private slots:
		void connectButton();
		void connectClick(QTreeWidgetItem * item, int column);
		void refreshButton();
		void viewClick(QTreeWidgetItem * item, int column);
		void urlButton();

	protected:
		void keyPressEvent(QKeyEvent *event);
};

#endif // TSERVERWINDOW_H
