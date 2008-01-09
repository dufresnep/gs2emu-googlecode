#include <QtGui>
#include "CRCBrowser.h"
#include "CRCLive.h"
#include "main.h"

// RemoteControl Account GUI
CRCBrowser::CRCBrowser(CRCChat *tab, QWidget *parent) : QWidget(parent)
{
	this->setupUi(this);
	this->tab = tab;
	
	this->setWindowTitle(QString() + "[" + tab->server.text() + "] File Browser");
	this->show();
}

CRCBrowser::~CRCBrowser()
{
	this->tab->browser = NULL;
}

void CRCBrowser::closeEvent(QCloseEvent * event)
{
	delete this;
}