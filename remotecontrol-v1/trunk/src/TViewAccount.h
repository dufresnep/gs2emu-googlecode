#ifndef TVIEWACCOUNT_H
#define TVIEWACCOUNT_H

#include "CPacket.h"
#include "TPlayer.h"
#include "TTabWidget.h"
#include "ui_viewaccount.h"

class TViewAccount : public QWidget, public Ui::FViewAccount
{
	Q_OBJECT

	public:
		TViewAccount(QWidget *parent = 0);

		TTabWidget* getTab();

		CPacket getData();
		void setData(CPacket& pPacket);

	private:
		TPlayer player;
};

#endif // TVIEWACCOUNT_H
