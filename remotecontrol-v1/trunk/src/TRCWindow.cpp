#include "main.h"
#include "TRCWindow.h"

TRCWindow::TRCWindow(QMainWindow *parent) : QMainWindow(parent)
{
    /* Setup Ui */
    setupUi(this);

    /* Tab Altering */
    removeTab(0);
	addTab(new TTabWidget(), "Status Tab", 0);

    /* Ui Signal/Slot Connections */
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(sendText()));
    connect(actionFolder_Configuration, SIGNAL(triggered()), this, SLOT(openFolderConfig()));
    connect(actionPlayerlist, SIGNAL(triggered()), this, SLOT(openPlayerList()));
    connect(actionServer_Options, SIGNAL(triggered()), this, SLOT(openServerOptions()));
}

/*
    Tab-Functions
*/

void TRCWindow::addTab(void *pTab, QString pName, int pType)
{
	switch (pType)
	{
		case STYPE_NONE:
			tabWidget->addTab((TTabWidget *)pTab, pName);
		break;

		case STYPE_IRC:
		break;

		case STYPE_SERVER:
			tabWidget->addTab((TRCSock *)pTab, pName);
		break;
	}
}

void TRCWindow::removeTab(int pTabId)
{
    QWidget *widget = tabWidget->widget(pTabId);
    if (widget == NULL)
        return;

    tabWidget->removeTab(pTabId);
    delete widget;
}

void TRCWindow::removeTab(void *pTab)
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QWidget *widget = tabWidget->widget(i);
		if (widget != pTab)
			continue;

		removeTab(i);
	}
}

void TRCWindow::setTab(int pTabId)
{
	tabWidget->setCurrentIndex(pTabId);
}

void TRCWindow::setTab(void *pTab)
{
	tabWidget->setCurrentWidget((QWidget *)pTab);
}

/*
	Slots
*/
void TRCWindow::openFolderConfig()
{
	// make sure the tab exists
	TRCSock *curWidget = (TRCSock *)tabWidget->currentWidget();
	if (curWidget == NULL)
		return;

	// make sure its a server-tab
	if (curWidget->getType() != STYPE_SERVER)
		return;

    // open folder config
    curWidget->openFolderConfig();
}

void TRCWindow::openPlayerList()
{
	// make sure the tab exists
	TRCSock *curWidget = (TRCSock *)tabWidget->currentWidget();
	if (curWidget == NULL)
		return;

	// make sure its a server-tab
	if (curWidget->getType() != STYPE_SERVER)
		return;

    // open folder config
    curWidget->openPlayerList();
}

void TRCWindow::openServerOptions()
{
	// make sure the tab exists
	TRCSock *curWidget = (TRCSock *)tabWidget->currentWidget();
	if (curWidget == NULL)
		return;

	// make sure its a server-tab
	if (curWidget->getType() != STYPE_SERVER)
		return;

    // open server options
    curWidget->openServerOptions();
}

void TRCWindow::sendText()
{
	// make sure the tab exists
	TTabWidget *curWidget = (TTabWidget *)tabWidget->currentWidget();
	if (curWidget == NULL)
		return;

	// make sure theres text
	CBuffer textStr = (const char *)lineEdit->text().toAscii();
	if (textStr.length() <= 0)
		return;

	// send text to the widget event
	curWidget->sendText(textStr);
	lineEdit->clear();
}

/*
	Events
*/
void TRCWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_F8:
			ServerWindow->setVisible(true);
			ServerWindow->raise();
		break;

		default:
			event->ignore();
		break;
	}
}
