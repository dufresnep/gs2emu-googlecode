#ifndef CRCBROWSER_H
#define CRCBROWSER_H

#include <QtGui>
#include "CBuffer.h"
#include "CClasses.h"
#include "CList.h"
#include "CPacket.h"
#include "CRCSock.h"
#include "CSocket.h"
#include "zlib.h"

#include "ui_filebrowser.h"

class CRCChat;

// RemoteControl RC GUI
class CRCBrowser : public QWidget, private Ui::CRCBrowser
{
	Q_OBJECT
	
	public:
		CRCBrowser(CRCChat *tab, QWidget *parent = 0);
		~CRCBrowser();
	
	public slots:
		//void ApplyChanges();
		//void CancelChanges();
		
	private:
		CRCChat *tab;
		
	protected:
		void closeEvent(QCloseEvent * event);
};

#endif