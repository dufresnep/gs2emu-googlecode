#ifndef TPLAYERWINDOW_H
#define TPLAYERWINDOW_H

#include "TTabWidget.h"
#include "ui_playerlist.h"

class TPlayerWindow : public QWidget, public Ui::FPlayerWindow
{
	Q_OBJECT

	public:
		TPlayerWindow(QWidget *parent = 0);

		void addItem(QTreeWidgetItem *pItem);
		void remItem(QTreeWidgetItem *pItem);

		TTabWidget* getTab()
		{
			return (TTabWidget *)parent();
		}

		QTreeWidget* getTreeWidget()
		{
			return (QTreeWidget *)treeWidget;
		}
};

#endif // TPLAYERWINDOW_H

