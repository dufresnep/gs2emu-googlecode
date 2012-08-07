#include "TPlayerTree.h"

/*
	Constructor / Destructor
*/

TPlayerTree::TPlayerTree(QWidget *parent) : QTreeWidget(parent)
{
	QAction *action = NULL;

	action = new QAction("Test", this);
	connect(action, SIGNAL(triggered()), this, SLOT(testSlot()));
	menuList.append(action);
}

TPlayerTree::~TPlayerTree()
{
}

/*
    Virtual Functions
*/
void TPlayerTree::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);
    menu.addActions(menuList);
    menu.exec(event->globalPos());
}

void TPlayerTree::testSlot()
{
	printf("ey\n");
}
