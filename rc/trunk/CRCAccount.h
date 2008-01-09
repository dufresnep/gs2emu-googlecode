#ifndef CRCACCOUNT_H
#define CRCACCOUNT_H

#include <QtGui>
#include "CBuffer.h"
#include "CClasses.h"
#include "CList.h"
#include "CPacket.h"
#include "CRCLive.h"
#include "CRCSock.h"
#include "CSocket.h"
#include "zlib.h"

#include "ui_viewaccount.h"

class CRCChat;

// RemoteControl RC GUI
class CRCAccount : public QWidget, private Ui::CRCAccount
{
	Q_OBJECT
	
	public:
		CRCAccount(CRCChat *tab, CPacket& pPacket, QWidget *parent = 0);
		~CRCAccount();
	
	public slots:
		void ApplyChanges();
		void CancelChanges();
		
	private:
		CPacket packet;
		CPlayer *player;
		CRCChat *tab;
		
		CPacket getPacket(QComboBox *p);
		CPacket getPacket(QLineEdit *p);
		CPacket getPacket(QTextEdit *p);
		CPacket getPacket(QTextEdit *text, int type);
	protected:
		void closeEvent(QCloseEvent * event);
};

#endif