#include "main.h"
#include "TLoginWindow.h"

TLoginWindow::TLoginWindow(QMainWindow *parent) : QMainWindow(parent)
{
    /* Setup Ui */
    setupUi(this);

	/* Create QTimer */
	sTimer = new QTimer(this);

    /* Ui Signal/Slot Connections */
    connect(BCancel, SIGNAL(released()), this, SLOT(slotCancel()));
    connect(BConnect, SIGNAL(released()), this, SLOT(slotConnect()));
    connect(sTimer, SIGNAL(timeout()), this, SLOT(runSockets()));

	lNickname->setText("Joey");
	lAccount->setText("KuJi");
	lPassword->setText("fuckgraal");

    /* Start Timer */
    sTimer->start(50);
}

TLoginWindow::~TLoginWindow()
{
	delete sTimer;
}

/*
	Slots
*/
void TLoginWindow::runSockets()
{
	sockList.run();
}

void TLoginWindow::slotCancel()
{
	close();
}

void TLoginWindow::slotConnect()
{
	if (ServerWindow->connectList(toString(lAccount->text()), toString(lNickname->text()), toString(lPassword->text())))
	{
		LoginWindow->setVisible(false);
		ServerWindow->setVisible(true);
	}
}
