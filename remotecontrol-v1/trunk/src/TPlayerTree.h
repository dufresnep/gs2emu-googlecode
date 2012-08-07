#ifndef TTREEWIDGET_H
#define TTREEWIDGET_H

#include <QTreeWidget>
#include <QtGui>

class TPlayerTree : public QTreeWidget
{
	Q_OBJECT

	public:
		TPlayerTree(QWidget *parent = 0);
		~TPlayerTree();

	protected:
		void contextMenuEvent(QContextMenuEvent * event);

	private:
		QList<QAction *> menuList;

	private slots:
		void testSlot();
};

#endif // TTREEWIDGET_H
