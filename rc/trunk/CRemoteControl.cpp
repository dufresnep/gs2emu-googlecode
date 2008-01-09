#include <QtGui>
#include <time.h>
#include "CRemoteControl.h"
#include "main.h"

// RemoteControl Class
CRemoteControl::CRemoteControl()
{
	RCLive   = new CRCLive();
	RCLogin  = new CRCLogin();
	RCSList  = new CRCSList();
	
	// Create Tray Icon
	/*
	TrayIcon = new QSystemTrayIcon(this);
	TrayMenu = new QMenu(RCLive);
	
	QList<QAction*> actions;
	TrayMenu->addActions(actions);
	
	TrayIcon->show();
	TrayIcon->showMessage("TITLE", "BODY", QSystemTrayIcon::Information, 25 * 1000);
	*/
}

void CRemoteControl::ListPacket(CRCSock *sock, CPacket& pPacket)
{
	int messageId = pPacket.readByte1();
	
	if (messageId == 29)
		return;
	
	int a;
	
	switch (messageId)
	{
		case RSERVERLIST:
			for (int i = Serverlist.count() + 1; i >= 0; i--)
				delete ((CServer*)Serverlist[i]);
			
			a = pPacket.readByte1();
			for (int i = 0; i < a; i++)
			{
				CString name, language, description, url, version, ip;
				int playercount, port;
				
				pPacket.readByte1();
				name = pPacket.readChars((unsigned char)pPacket.readByte1());
				language = pPacket.readChars((unsigned char)pPacket.readByte1());
				description = pPacket.readChars((unsigned char)pPacket.readByte1());
				url = pPacket.readChars((unsigned char)pPacket.readByte1());
				version = pPacket.readChars((unsigned char)pPacket.readByte1());
				playercount = atoi(pPacket.readChars((unsigned char)pPacket.readByte1()));
				ip = pPacket.readChars((unsigned char)pPacket.readByte1());
				port = atoi(pPacket.readChars((unsigned char)pPacket.readByte1()));
				Serverlist.add(new CServer(name.text(), language.text(), description.text(), url.text(), version.text(), playercount, ip.text(), port));
			}
			
			showLogin(false);
			showList(true);
		break;
		
		case RHOMEURL:
		break;
		
		case RPAYURL:
			delete sock;
		break;
		
		case RERROR:
			createError(pPacket.text() + 1);
		break;
	}
}

void CRemoteControl::LivePacket(CRCSock *sock, CPacket& pPacket)
{
	int messageId = pPacket.readByte1();
	
	if (messageId == 29)
		return;
	
	CRCChat *ntab = ReturnTab(sock);
	
	switch (messageId)
	{
		case OTHERPLPROPS:
		{
			CPlayer *player = FindPlayerID(sock, pPacket.readByte2());
			
			if (player != NULL)
			{
				player->setProps(pPacket);
			}
			break;
		}
		
		case UNLIMITEDSIG:
			RCLive->TabAdd(sock, sock->server.text());
			showList(false);
			showLive(true);
		break;
		
		case SADDPLAYER:
		{
			CPlayer *player  = new CPlayer(ntab);
			player->id       = pPacket.readByte2();
			player->account  = pPacket.readChars(pPacket.readByte1());
			player->setProps(pPacket);
			RCLive->PlayerAdd(ntab, player);
			break;
		}
		
		case SDELPLAYER:
			RCLive->PlayerRem(ntab, pPacket.readByte2());
		break;
		
		case DACCPLPROPS:
			ntab->createWindow(VIEWACCOUNT, pPacket);
		break;
		
		case DRCLOG:
			RCLive->AddChat(sock, pPacket.text() + 1);
		break;
	}
}

void CRemoteControl::IrcPacket(CRCSock *sock, CPacket& pPacket)
{
	CRCChat *tab = ReturnTab(sock);
	tab->textEdit->append(pPacket.text());
	
	if (pPacket.find("ERROR") >= 0)
	{
		RC->RCLive->TabRem(sock);
		return;
	}
	
	if (pPacket.find("PING") >= 0)
	{
		sock->sendPacket(CPacket() << "PONG " << pPacket.copy(5, pPacket.length() - 5));
	}
	
	if (pPacket.copy(0, 1) == ":")
	{
		CString command, from, name, network, text;
		CStringList words;
		words.load(pPacket.text(), " ");
		command = pPacket.copy(pPacket.find(" ") + 1);
		from    = command.copy(command.find(" ") + 1);
		from    = from.copy(0, from.find(":") - 1);
		text    = command.copy(command.find(":") + 1);
		text    = text.copy(0, text.length() - 1);
		command = command.copy(0, command.find(" "));
		
		if (pPacket.find("!") >= 0)
		{
			name    = pPacket.copy(1, pPacket.find("!") - 1);
			network = pPacket.copy(pPacket.find("!") + 1);
			network = network.copy(0, network.find(" "));
		}
		
		if (command == "JOIN")
		{
			if (name == cNickname)
			{
				CRCChat *tab = RCLive->TabAdd(sock, text.text());
				tab->textEdit->append("* Now talking in " + (QString)text.text());
			}
				else
			{
				CPlayer *player  = new CPlayer(ReturnTab(sock, text));
				player->nickname = name;
				player->type     = CLIENTRC;
				RC->RCLive->PlayerAdd(ReturnTab(sock, text), player);
			}
		}
		
		if (command == "KICK")
		{
			// :Joey!XTJoeyTX@graalreborn.com KICK #graal Joey2 :Joey
			if (name == text)
			{
				RC->RCLive->TabRem(sock, words[2]);
			}
		}
		
		if (command == "MODE")
		{
			// :ChanServ!services@localhost.net MODE #graal +v Joey
		}
		
		if (command == "PART")
		{	
			if (name != cNickname)
			{
				words[2].removeAll("\r");
				words[2].removeAll("\n");
				words[2].removeAll("\0");
				RCLive->PlayerRem(ReturnTab(sock, words[2]), name);
			}
		}
		
		if (command == "PRIVMSG")
		{
			CRCChat *ctab = ReturnTab(sock, from);
			if (ctab == NULL)
			{
				ctab = ReturnTab(sock, name);
				if (ctab == NULL)
					ctab = RCLive->TabAdd(sock, name.text());
			}
			
			ctab->textEdit->append("<" + (QString)name.text() + "> " + (QString)text.text());
		}
		
		if (command == "QUIT")
		{
			// :Joey!XTJoeyTX@graalreborn.com QUIT :Quit: 
		}
		
		if (command == "332")
		{
			CRCChat *ctab = ReturnTab(sock, words[3]);
			ctab->textEdit->append("* Topic is '" + (QString)text.text() + "'");
		}
		
		if (command == "333")
		{
			CRCChat *ctab = ReturnTab(sock, from.copy(from.find(" ") + 1));
//			time_t *ts = atoi(words[5].text());
//			ctab->textEdit->append("* Set by " + (QString)text.text() + " on " + asctime(localtime(ts)));
		}
		
		if (command == "353")
		{
			CRCChat *ctab = ReturnTab(sock, from.copy(from.find("#")));
			
			if (ctab == NULL)
				return;
			
			while (text.bytesLeft())
			{
				CString nickname = text.readString(" ");
				nickname.removeAll("\r");
				if (nickname.length() < 1)
					break;
				
				CPlayer *player  = new CPlayer(ctab);
				player->nickname = nickname;
				player->type     = CLIENTRC;
				RC->RCLive->PlayerAdd(ctab, player);
			}
		}
		
		if (command == "376")
		{
			// message of the day
		}
		
		if (command == "474")
		{
			// cannot join channel?
		}
	}
}

void CRemoteControl::addSock(CRCSock *sock)
{
	QObject::connect((QObject*)sock, SIGNAL(createError(QString)), this, SLOT(createError(QString)));
	QObject::connect((QObject*)sock, SIGNAL(TabRem(CRCSock *)), this, SLOT(TabRem(CRCSock *)));
	
	switch (sock->type)
	{
		case IRC:
			QObject::connect((QObject*)sock, SIGNAL(IrcPacket(CRCSock *, CPacket&)), this, SLOT(IrcPacket(CRCSock *, CPacket&)));
		break;
		
		case SERVER:
			QObject::connect((QObject*)sock, SIGNAL(LivePacket(CRCSock *, CPacket&)), this, SLOT(LivePacket(CRCSock *, CPacket&)));
		break;
		
		case SERVERLIST:
			QObject::connect((QObject*)sock, SIGNAL(ListPacket(CRCSock *, CPacket&)), this, SLOT(ListPacket(CRCSock *, CPacket&)));
		break;
	}
}

void CRemoteControl::createError(QString error)
{
	char buffer[512];
	sprintf(buffer, "<center>%s</center>", error.toAscii());
	
	QMessageBox ErrorMsg;
	ErrorMsg.setText(error.toAscii());
	ErrorMsg.setWindowTitle("Error");
	ErrorMsg.exec();
}

void CRemoteControl::showLogin(bool visible)
{
	(visible ? RCLogin->show() : RCLogin->hide());
}

void CRemoteControl::showList(bool visible)
{
	if (visible)
	{
		RCSList->show();
		RCSList->ServerList();
	}
		else
	{
		RCSList->hide();
	}
}

void CRemoteControl::showLive(bool visible) 
{
	(visible ? RCLive->show() : RCLive->hide());
}

void CRemoteControl::TabRem(CRCSock *sock)
{
	RC->RCLive->TabRem(sock);
}

// RemoteControl Login GUI
CRCLogin::CRCLogin(QMainWindow *parent) : QMainWindow(parent)
{
	this->setupUi(this);
	this->lAccount->setText("(npc-server)");
	this->lNickname->setText("Joey2");
	this->lPassword->setText("rw6EQqKV");
	
	// Connect Buttons
	QObject::connect(BConnect, SIGNAL(clicked()), this, SLOT(connectServer()));
	QObject::connect(BCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void CRCLogin::connectServer()
{
	cAccount  = this->lAccount->text().toAscii();
	cNickname = this->lNickname->text().toAscii();
	cPassword = this->lPassword->text().toAscii();
	
	CRCSock *temp = new CRCSock(SERVERLIST, "SERVERLIST");
	if (temp->connect("graalreborn.com", 14922) == false)
		delete temp;
}

// RemoteControl Serverlist GUI
CRCSList::CRCSList(QMainWindow *parent) : QMainWindow(parent)
{
	this->setupUi(this);
	
	// Connect Signals / Slots
	QObject::connect(pOpenURL, SIGNAL(clicked()), this, SLOT(ServerURL()));
	QObject::connect(pRefresh, SIGNAL(clicked()), this, SLOT(ServerRefresh()));
	QObject::connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(ServerView(QTreeWidgetItem *, int)));
	QObject::connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(ServerConnect(QTreeWidgetItem *, int)));
}

void CRCSList::ServerList()
{
	for (int i = 0; i < items.count(); i++)
		delete items[i];
	items.clear();
	
	QStringList Labels;
	Labels << "Name" << "Players";
	
	treeWidget->clear();
	treeWidget->setHeaderLabels(Labels);
	treeWidget->setColumnWidth(0, 125);
	
	for (int i = 0; i < Serverlist.count(); i++)
	{
		char buffer[4];
		CServer *server = (CServer*)Serverlist[i];
		
		QTreeWidgetItem *item = new QTreeWidgetItem();
		item->setIcon(0, QIcon(server->type == GOLD ? ":/RC/icon_gold.png" : (server->type == HOSTED ? ":/RC/icon_hosted.png" : ":/RC/icon_classic.png")));
		item->setText(0, server->name.text());
		item->setText(1, itoa(server->playercount, buffer, 10));
		item->setStatusTip(0, itoa(server->id, buffer, 10));
		items.append(item);
	}
	
	treeWidget->insertTopLevelItems(0, items);
	treeWidget->sortByColumn(1);
}

void CRCSList::ServerConnect(QTreeWidgetItem * item, int column)
{
	CServer *server = (CServer*)Serverlist[item->statusTip(0).toInt()];
	
	CRCSock *temp = new CRCSock(SERVER, server->name.text());
	temp->connect(server->ip.text(), server->port);
}

void CRCSList::ServerRefresh()
{
	CRCSock *temp = new CRCSock(SERVERLIST, "SERVERLIST");
	temp->connect("graalreborn.com", 14922);
}

void CRCSList::ServerURL()
{
	if (eHomepage->text() == "")
		return;
	
	ShellExecuteA(NULL, "open", eHomepage->text().toAscii(), NULL, "C:\\", SW_SHOW);
}

void CRCSList::ServerView(QTreeWidgetItem * item, int column)
{
	CServer *server = (CServer*)Serverlist[item->statusTip(0).toInt()];
	eLanguage->setText(server->language.text());
	eVersion->setText(server->version.text());
	eDescription->setText(server->description.text());
	eHomepage->setText(server->url.text());
}

void CRCSList::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_F8:
			for (int i = 0; i < Connections.count(); i++)
				delete (CRCSock*)Connections[i];
			
			RC->showLogin(true);
			RC->showLive(false);
			RC->showList(false);
		break;
		
		default:
			event->ignore();
		break;
	}
}