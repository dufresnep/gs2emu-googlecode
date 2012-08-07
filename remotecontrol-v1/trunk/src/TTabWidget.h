#ifndef TTABWIDGET_H
#define TTABWIDGET_H

#include "CBuffer.h"
#include "ui_rctab.h"

class TTabWidget : public QWidget, public Ui::FRCTab
{
	Q_OBJECT

	public:
		TTabWidget(QWidget *parent = 0);

        int getType();
        void setType(int pTabType);
		virtual void sendText(CBuffer pText);

    private:
        int tabType;
};

#endif // TTABWIDGET_H
