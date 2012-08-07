#include "main.h"
#include "TServerWindow.h"

/*
	Constructor / Deconstructor
*/
TServerWindow::TServerWindow(QMainWindow *parent) : QMainWindow(parent)
{
    /* Setup Ui */
    setupUi(this);

    /* Ui Signal/Slot Connections */
    connect(pConnect, SIGNAL(released()), this, SLOT(connectButton()));
    connect(pRefresh, SIGNAL(released()), this, SLOT(refreshButton()));
	connect(pWebsite, SIGNAL(clicked()), this, SLOT(urlButton()));
	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(viewClick(QTreeWidgetItem *, int)));
	connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(connectClick(QTreeWidgetItem *, int)));
}

/*
	Functions
*/

TServerList& TServerWindow::getServerList()
{
	return serverList;
}

bool TServerWindow::connectList(CBuffer pAccount, CBuffer pNickname, CBuffer pPassword)
{
	// kill all connections
	sockList.clear();

	// Set Account & Pass
	graalAccount = pAccount;
	graalNickname = pNickname;
	graalPassword = pPassword;

	// Connect
	refreshButton();
	return (listSock.getSocket() != 0);
}

bool TServerWindow::connectServer(TServer *pServer)
{
	// good pointer?
	if (pServer == NULL)
		return false;

	// connect to server
	TRCSock *newSock = TRCSock::createSock(pServer->getName(), pServer->getIp(), pServer->getPort());
	if (newSock == NULL)
		return false;

	// hide serverlist - show rc
	ServerWindow->setVisible(false);
	RCWindow->setVisible(true);

	// send login details
	newSock->sendPacket(CPacket() << (char)1 << "GSRVRC1" << (char)0 << (char)graalAccount.length() << graalAccount << (char)graalPassword.length() << graalPassword);
	newSock->sendPacket(CPacket() << (char)RCO_PLPROPS << (char)NICKNAME << (char)graalNickname.length() << graalNickname);
	return true;
}

void TServerWindow::setServers(CPacket& pPacket)
{
	// Clear Serverlist
	serverList.clear();

	// Declare Variables
	int serverCount = 0;

	// Reinstate List
	serverCount = (unsigned char)pPacket.readByte1();

	for (int i = 0; i < serverCount; i++)
	{
		// create server
		TServer *server = new TServer();

		// empty byte
		pPacket.readByte1();

		// parse server
		server->setName(pPacket.readChars((unsigned char)pPacket.readByte1()));
		server->setLanguage(pPacket.readChars((unsigned char)pPacket.readByte1()));
		server->setDescription(pPacket.readChars((unsigned char)pPacket.readByte1()));
		server->setUrl(pPacket.readChars((unsigned char)pPacket.readByte1()));
		server->setVersion(pPacket.readChars((unsigned char)pPacket.readByte1()));
		server->setPCount(atoi(pPacket.readChars((unsigned char)pPacket.readByte1())));
		server->setIp(pPacket.readChars((unsigned char)pPacket.readByte1()));
		server->setPort(atoi(pPacket.readChars((unsigned char)pPacket.readByte1())));

		// add server
		serverList.add(server);
	}

	showServers();
}

void TServerWindow::showServers()
{
	// Clear TreeWidget
	treeWidget->clear();

	// Declare Variables
	QList<QTreeWidgetItem *> items;
	QStringList Labels = QStringList() << "Name" << "Players";

	// Reinstate List
	for (int i = 0; i < serverList.count(); i++)
	{
		TServer *server = serverList[i];
		if (server == NULL)
			continue;

		items.append(server->getItem(i));
	}

	// Set Widget
	treeWidget->setHeaderLabels(Labels);
	treeWidget->setColumnWidth(0, 125);
	treeWidget->insertTopLevelItems(0, items);
	treeWidget->sortByColumn(1);
}

/*
	Signal-Slot Functions
*/

void TServerWindow::connectButton()
{
	connectServer(serverList[treeWidget->currentItem()->statusTip(0).toInt()]);
}

void TServerWindow::connectClick(QTreeWidgetItem *item, int column)
{
	connectServer(serverList[item->statusTip(0).toInt()]);
}

void TServerWindow::refreshButton()
{
	// If the socket exist, don't continue
	if (listSock.getSocket() != 0)
		return;

	// Create a new socket
	listSock.setSync(false);

	// Connect
	if (!listSock.connectSock(list_ip, list_port))
		return;

	// Send Login Details
	listSock.sendPacket(CPacket() << (char)0 << "rc");
	listSock.sendPacket(CPacket() << (char)1 << (char)graalAccount.length() << graalAccount << (char)graalPassword.length() << graalPassword);
}

void TServerWindow::viewClick(QTreeWidgetItem *item, int column)
{
	TServer *server = serverList[item->statusTip(0).toInt()];
	if (server == NULL)
		return;

	// reset column
	column = 0;

	// set data
	eDescription->setText(server->getDescription().text());
	eHomepage->setText(server->getUrl().text());
	eLanguage->setText(server->getLanguage().text());
	eVersion->setText(server->getVersion().text());
}

void TServerWindow::urlButton()
{
	QUrl url = eHomepage->text();
	if (url.isEmpty())
		return;
	if (url.toString().indexOf("http://") == 0)
		QDesktopServices::openUrl(QUrl(eHomepage->text()));
}

/*
	Events
*/
void TServerWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_F8:
			// kill all connections
			sockList.clear();

			// change visibility
			LoginWindow->setVisible(true);
			ServerWindow->setVisible(false);
			RCWindow->setVisible(false);
		break;

		default:
			event->ignore();
		break;
	}
}
