#include <QtGui>
#include "CRCLive.h"
#include "main.h"

// RemoteControl RC GUI
CRCLive::CRCLive(QMainWindow *parent) : QMainWindow(parent)
{
	this->setupUi(this);
	this->tabWidget->removeTab(0);
	
	// Connect Buttons
	QObject::connect(bFileBrowser, SIGNAL(clicked()), this, SLOT(sbFileBrowser()));
	
	QObject::connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(SendChat()));
	QObject::connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(TabChanged(int)));
}

void CRCLive::AddChat(CRCSock *sock, QString chat)
{
	CRCChat *temp = ReturnTab(sock);
	
	if (temp == NULL)
		return;
	
	temp->textEdit->append(chat);
}

void CRCLive::PlayerAdd(CRCChat *temp, CPlayer *player)
{
	temp->Players.add(player);
	
	if (player->type == CLIENTRC)
	{
		char buffer[4];
		QListWidgetItem *item = new QListWidgetItem();
		
		item->setText(player->nickname.text());
		item->setStatusTip(itoa(player->id, buffer, 10));
		temp->listWidget->addItem(item);
	}
	
	UpdatePlayers();
}

void CRCLive::PlayerRem(CRCChat *temp, CString name)
{
	if (temp == NULL)
		return;
	
	for (int i = 0; i < temp->Players.count(); i++)
	{
		CPlayer *player = (CPlayer*)temp->Players[i];
		
		if (player->nickname == name)
			delete player;
	}
	
	UpdatePlayers();
}

void CRCLive::PlayerRem(CRCChat *temp, int pId)
{
	if (temp == NULL)
		return;
	
	for (int i = 0; i < temp->Players.count(); i++)
	{
		CPlayer *player = (CPlayer*)temp->Players[i];
		
		if (player->id == pId)
			delete player;
	}
	
	UpdatePlayers();
}

void CRCLive::UpdatePlayers()
{
	CRCChat *temp = (CRCChat*)tabWidget->currentWidget();
	
	if (temp == NULL)
		return;
	
	char buffer[40];
	sprintf(buffer, "Players: %i", temp->Players.count());
	lPlayers->setText(buffer);
}

void CRCLive::UpdateServer()
{
	CRCChat *temp = (CRCChat*)tabWidget->currentWidget();
	
	if (temp == NULL)
		return;
	
	char buffer[40];
	sprintf(buffer, "Server: %s", temp->server.text());
	lServer->setText(buffer);
}

CRCChat* CRCLive::TabAdd(CRCSock *sock, QString tab)
{
	const char *temp = tab.toAscii();
	
	if (ReturnID(sock, temp) >= 0)
		return NULL;
	
	CRCChat *newtab = new CRCChat(sock, temp);
	tabWidget->addTab(newtab, tab);
	tabWidget->setCurrentIndex(tabWidget->count());
	
	return newtab;
}

void CRCLive::TabChanged(int tab)
{
	CRCChat *temp = (CRCChat*)tabWidget->widget(tab);
	
	if (temp == NULL)
		return;
	
	UpdatePlayers();
	UpdateServer();
}

void CRCLive::TabRem(CRCSock *sock)
{
	int tab = -1;
	
	do
	{
		tab = ReturnID(sock);
		
		if (tab >= 0)
			delete tabWidget->widget(tab);
	} while (tab >= 0);
}

void CRCLive::TabRem(CRCSock *sock, CString tabname)
{
	int tab = -1;
	
	do
	{
		tab = ReturnID(sock, tabname);
		
		if (tab >= 0)
			delete tabWidget->widget(tab);
	} while (tab >= 0);
}

void CRCLive::SendChat()
{
	const char *temp = this->lineEdit->text().toAscii();
	CString data = temp;
	
	this->lineEdit->clear();
	
	CStringList text;
	text.load(data.text(), " ");
	
	if (text.count() < 1)
		return;
	
	CRCChat *schat = (CRCChat*)tabWidget->currentWidget();
	CRCSock *sock  = (CRCSock*)schat->sock;
	
	// /connect 'server' 'port' 'channel'
	// /connect irc.graalreborn.com 6667
	if (text[0] == "/connect" && text.count() == 3)
	{
		CRCSock *irc = new CRCSock(IRC, text[1]);
		if (irc->connect(text[1].text(), atoi(text[2].text())))
		{
			CRCChat *newtab = TabAdd(irc, text[1].text());
			newtab->ismain = true;
		}
	}
		else if (text[0] == "/join" && text.count() == 2 && sock->type == IRC)
	{
		sock->sendPacket(CPacket() << "JOIN " << text[1] << "\r\n\0");
	}
		else if ((text[0] == "/leave" || text[0] == "/part" || text[0] == "/quit") && text.count() < 3 && sock->type == IRC)
	{
		CString chan = (text.count() == 2 ? text[1] : schat->server);
		sock->sendPacket(CPacket() << "PART " << chan << "\r\n\0");
		
		if (schat->ismain)
		{
			sock->sendPacket(CPacket() << "QUIT :Graal Reborn IRC/RC by Joey\r\n\0");
		}
			else
		{
			sock->sendPacket(CPacket() << "PART " << chan << "\r\n\0");
			TabRem(sock, chan);
		}
	}
		else if (text[0] == "/open" && text.count() > 1)
	{
		//CString account = text.join(" ").copy(6);
		//sock->sendPacket(CPacket() << (char)DWANTACCPLPROPS << (char)account.length() << account);
	}
		else
	{
		if (sock != NULL)
		{
			switch (sock->type)
			{
				case SERVER:
					sock->sendPacket(CPacket() << (char)DRCCHAT << data);
				break;
				
				case IRC:
					CPacket packet = CPacket() << "PRIVMSG " << (const char *)tabWidget->tabText(tabWidget->currentIndex()).toLatin1() << " :" << data << "\r\n\0";
					sock->sendPacket(packet);
					schat->textEdit->append("<" + (QString)cNickname.text() + "> " + (QString)data.text());
				break;
			}
		}
	}
}

void CRCLive::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_F8:
			RC->showList(true);
		break;
		
		default:
			event->ignore();
		break;
	}
}

// RemoteControl Buttons
void CRCLive::sbFileBrowser()
{
	MessageBoxA(NULL, "FileBrowser", "yes", MB_OK);
}

// RemoteControl RC Chat Widget
CRCChat::CRCChat(CRCSock *sock, CString server, QWidget *parent) : QWidget(parent)
{
	this->setupUi(this);
	this->server = server;
	this->sock   = sock;
	this->ismain = false;
}

CRCChat::~CRCChat()
{
	for (int i = Players.count(); i > 0; i--)
		delete (CPlayer*)Players[i];
	
	for (int i = WVIEWACCOUNT.count(); i > 0; i--)
		delete (CRCAccount*)WVIEWACCOUNT[i];
	
	delete browser;

	if (ismain)
		delete sock;
}

void CRCChat::createWindow(int type, CPacket& data)
{
	switch (type)
	{
		case VIEWACCOUNT:
			WVIEWACCOUNT.add(new CRCAccount(this, data));
		break;

		case FILEBROWSER:
			if (browser == NULL)
				browser = new CRCBrowser(this);
			else
				browser->raise();
		break;
		
		default:
		break;
	}
}