#ifndef TRCWINDOW_H
#define TRCWINDOW_H

#include "ui_rcmain.h"

class TRCWindow : public QMainWindow, private Ui::FRCWindow
{
    Q_OBJECT

    public:
        /*
            Constructor / Destructor Functions
        */
        TRCWindow(QMainWindow *parent = 0);

        /*
            Tab Functions
        */
        void addTab(void *pTab, QString pName, int pType);
        void removeTab(int pTabId);
		void removeTab(void *pTab);
		void setTab(int pTabId);
		void setTab(void *pTab);

	private slots:
        void openFolderConfig();
		void openPlayerList();
		void openServerOptions();
		void sendText();

	protected:
		void keyPressEvent(QKeyEvent *event);
};

#endif // TRCWINDOW_H

