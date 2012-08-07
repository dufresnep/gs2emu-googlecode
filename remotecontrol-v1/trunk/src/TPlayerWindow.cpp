#include "main.h"
#include "TPlayerWindow.h"

/*
	Constructor / Destructor
*/

TPlayerWindow::TPlayerWindow(QWidget *parent) : QWidget(parent)
{
    /* Setup Ui */
    setupUi(this);

    /* Ui Signal/Slot Connections */
    connect(pClose, SIGNAL(released()), this, SLOT(close()));
}

/*
	Functions
*/

void TPlayerWindow::addItem(QTreeWidgetItem *pItem)
{
	treeWidget->addTopLevelItem(pItem);
}

void TPlayerWindow::remItem(QTreeWidgetItem *pItem)
{
	// not even needed
	// treeWidget doesn't show items that are properly deleted
	// yay
}
