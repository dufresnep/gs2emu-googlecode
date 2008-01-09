#include <QtGui>
#include "CRCAccount.h"
#include "main.h"

// Sorry for this ugly file.. but I don't know of any way loading all this automatically.
// Sorry =(.

// RemoteControl Account GUI
CRCAccount::CRCAccount(CRCChat *tab, CPacket& pPacket, QWidget *parent) : QWidget(parent)
{
	this->setupUi(this);
	this->tab = tab;
	
	// Connect Signal-Slots
	QObject::connect(pApply, SIGNAL(clicked()), this, SLOT(ApplyChanges()));
	QObject::connect(pCancel, SIGNAL(clicked()), this, SLOT(CancelChanges()));
	
	// Player Properties / Attributes
	this->player = new CPlayer(tab);
	this->player->id          = pPacket.readByte2();
	this->player->account     = pPacket.readChars(pPacket.readByte1());
	this->player->playerWorld = pPacket.readChars(pPacket.readByte1());
	this->packet << pPacket.readChars((unsigned char)pPacket.readByte1());
	this->player->setProps(packet);
	
	for (int i = pPacket.readByte2(); i > 0; i--)
		this->player->myFlags.add(pPacket.readChars(pPacket.readByte1()));
	
	for (int i = pPacket.readByte2(); i > 0; i--)
	{
		int len = pPacket.readByte1();
		this->player->myChests.add(CString() << toString(pPacket.readByte1()) << ":" << toString(pPacket.readByte1()) << ":" << pPacket.readChars(len - 2));
	}
	
	for (int i = pPacket.readByte1(); i > 0; i--)
		this->player->myWeapons.add(pPacket.readChars(pPacket.readByte1()));
	
	// Player Rights
	this->player->adminRights = (int)pPacket.readByte5();
	this->player->adminIp = pPacket.readChars(pPacket.readByte1());
	
	for (int i = pPacket.readByte1(); i > 0; i--)
		this->player->adminFolders.add(pPacket.readChars(pPacket.readByte1()));
	
	// Player Comments
	this->player->comments = pPacket.readChars(pPacket.readByte1());
	this->player->banned = (pPacket.readByte1() > 0);
	this->player->banTime = pPacket.readByte3();
	this->player->banhistory = pPacket.readChars(pPacket.readByte1());
	this->player->banreason1 = pPacket.readChars(pPacket.readByte1());
	this->player->banreason2 = pPacket.readChars(pPacket.readByte1());
	for (int i = pPacket.readByte1(); i > 0; i--)
	{
		int len = pPacket.readByte1();
		if (len > 0)
			this->player->banList << pPacket.readChars(len);
	}
	
	// Tab 1
	this->ppLevel->setText(player->level.text());
	this->ppX->setText(toString(player->x).text());
	this->ppY->setText(toString(player->y).text());
	
	this->ppMaxPower->setText(toString(player->maxPower).text());
	this->ppPower->setText(toString(player->power).text());
	this->ppAP->setText(toString(player->ap).text());
	this->ppMP->setText(toString(player->magicPoints).text());
	this->ppGralats->setText(toString(player->rubins).text());
	this->ppGlove->setText(toString(player->glovePower).text());
	this->ppBombs->setText(toString(player->bombCount).text());
	this->ppArrows->setText(toString(player->darts).text());
	this->ppSwordPower->setText(toString(player->swordPower).text());
	this->ppSwordImg->setText(player->swordImage.text());
	this->ppShieldPower->setText(toString(player->shieldPower).text());
	this->ppShieldImg->setText(player->shieldImage.text());
	this->ppGender->setChecked(player->status & 4);
	this->ppWeapons->setChecked(player->status & 16);
	this->ppSpin->setChecked(player->status & 64);
	
	this->ppHead->setText(player->headimg.text());
	this->ppBody->setText(player->bodyimg.text());
	this->ppHat->setText(player->ganiAttribs[0].text());
	this->ppAni->setText(player->gAni.text());
	
	this->ppSkin->addItems(colors);
	this->ppSkin->setCurrentIndex((int)player->colors[0]);
	
	this->ppCoat->addItems(colors);
	this->ppCoat->setCurrentIndex((int)player->colors[1]);
	
	this->ppSleeve->addItems(colors);
	this->ppSleeve->setCurrentIndex((int)player->colors[2]);
	
	this->ppShoe->addItems(colors);
	this->ppShoe->setCurrentIndex((int)player->colors[3]);
	
	this->ppBelt->addItems(colors);
	this->ppBelt->setCurrentIndex((int)player->colors[4]);
	
	// Tab 2
	this->paChests->setText(player->myChests.join("\n").text());
	this->paFlags->setText(player->myFlags.join("\n").text());
	this->paWeapons->setText(player->myWeapons.join("\n").text());
	
	// Tab 3
	this->prAccount->setText(player->account.text());
	this->prIPRange->setText(player->adminIp.text());	
	this->prFolders->setText(player->adminFolders.join("\n").text());
	this->pr1->setChecked(player->adminRights & 1);
	this->pr2->setChecked(player->adminRights & 2);
	this->pr3->setChecked(player->adminRights & 4);
	this->pr4->setChecked(player->adminRights & 8);
	this->pr5->setChecked(player->adminRights & 16);
	this->pr6->setChecked(player->adminRights & 32);
	this->pr7->setChecked(player->adminRights & 64);
	this->pr8->setChecked(player->adminRights & 128);
	this->pr9->setChecked(player->adminRights & 256);
	this->pr10->setChecked(player->adminRights & 512);
	this->pr11->setChecked(player->adminRights & 1024);
	this->pr12->setChecked(player->adminRights & 2048);
	this->pr13->setChecked(player->adminRights & 4096);
	this->pr14->setChecked(player->adminRights & 8192);
	this->pr15->setChecked(player->adminRights & 16384);
	this->pr16->setChecked(player->adminRights & 32768);
	this->pr17->setChecked(player->adminRights & 65536);
	this->pr18->setChecked(player->adminRights & 131072);
	this->pr19->setChecked(player->adminRights & 262144);
	this->pr20->setChecked(player->adminRights & 524288);
	
	// Tab 4
	this->pcBans->addItems(player->banList);
	for (int i = 0; i < this->pcBans->count(); i++)
	{
		if (pcBans->itemText(i) == player->banreason2.text())
		{
			pcBans->setCurrentIndex(i);
			break;
		}
	}
	
	this->pcBanned->setChecked(player->banned);
	this->pcBanReason->setText(player->banreason1.text());
	this->pcComments->setText(player->comments.text());
	
	// Show GUI
	this->setWindowTitle(QString() + "Viewing Account: " + player->account.text() + " (" + toString(player->lastIp).text() + ")");
	this->show();
}

CRCAccount::~CRCAccount()
{
	this->tab->WVIEWACCOUNT.remove(this);
	
	delete player;
}

void CRCAccount::closeEvent(QCloseEvent * event)
{
	delete this;
}

void CRCAccount::ApplyChanges()
{
	CPacket packet, props;
	CString str;
	CStringList list;
	int adminRights = (pr1->isChecked() ? RIGHT1 : 0) + (pr2->isChecked() ? RIGHT2 : 0) + (pr3->isChecked() ? RIGHT3 : 0) + (pr4->isChecked() ? RIGHT4 : 0) + (pr5->isChecked() ? RIGHT5 : 0) + (pr6->isChecked() ? RIGHT6 : 0) + (pr7->isChecked() ? RIGHT7 : 0) + (pr8->isChecked() ? RIGHT8 : 0) + (pr9->isChecked() ? RIGHT9 : 0) + (pr10->isChecked() ? RIGHT10 : 0) + (pr11->isChecked() ? RIGHT11 : 0) + (pr12->isChecked() ? RIGHT12 : 0) + (pr13->isChecked() ? RIGHT13 : 0) + (pr14->isChecked() ? RIGHT14 : 0) + (pr15->isChecked() ? RIGHT15 : 0) + (pr16->isChecked() ? RIGHT16 : 0) + (pr17->isChecked() ? RIGHT17 : 0) + (pr18->isChecked() ? RIGHT18 : 0) + (pr19->isChecked() ? RIGHT19 : 0) + (pr20->isChecked() ? RIGHT20 : 0);
	int curStatus   = (ppGender->isChecked() ? RIGHT3 : 0) + (ppWeapons->isChecked() ? RIGHT5 : 0) + (ppSpin->isChecked() ? RIGHT7 : 0);
	
	packet << (char)DSETACCPLPROPS << (char)player->account.length() << player->account << (char)player->playerWorld.length() << player->playerWorld;
	
	// this is in the order of the gui, so 'sorry' if its out of order ;)
	props << (char)CURLEVEL     << getPacket(ppLevel)
		  << (char)PLAYERX      << (char)(ppX->text().toFloat() * 2)
		  << (char)PLAYERY      << (char)(ppY->text().toFloat() * 2)
		  << (char)CURPOWER     << (char)(ppPower->text().toFloat() * 2)
		  << (char)MAXPOWER     << (char)ppMaxPower->text().toFloat()
		  << (char)PALIGNMENT   << (char)ppAP->text().toInt()
		  << (char)MAGICPOINTS  << (char)ppMP->text().toInt()
		  << (char)RUPEESCOUNT  << (int)ppGralats->text().toInt()
		  << (char)GLOVEPOWER   << (char)ppGlove->text().toInt()
		  << (char)BOMBSCOUNT   << (char)ppBombs->text().toInt()
		  << (char)ARROWSCOUNT  << (char)ppArrows->text().toInt()
		  << (char)SWORDPOWER   << (char)(ppSwordPower->text().toInt() + 30) << getPacket(ppSwordImg)
		  << (char)SHIELDPOWER  << (char)(ppShieldPower->text().toInt() + 10) << getPacket(ppShieldImg)
		  << (char)STATUS       << (char)curStatus
		  << (char)HEADGIF      << (char)(ppHead->text().length() + 100) << (const char *)ppHead->text().toAscii()
		  << (char)BODYIMG      << getPacket(ppBody)
//		  << (char)GATTRIB1     << getPacket(ppHat)
		  << (char)PLAYERANI    << getPacket(ppAni)
		  << (char)PLAYERCOLORS << (char)ppSkin->currentIndex() << (char)ppCoat->currentIndex() << (char)ppSleeve->currentIndex() << (char)ppShoe->currentIndex() << (char)ppBelt->currentIndex();
	
	packet.writeByte(props.length() + 32);
	packet << props << getPacket(paFlags, 1) << getPacket(paChests, 2) << getPacket(paWeapons, 0) << (long long)adminRights << getPacket(prIPRange) << getPacket(prFolders, 0) << getPacket(pcComments) << (char)pcBanned->isChecked() << getPacket(pcBanReason) << getPacket(pcBans);
	
	this->tab->textEdit->append(packet.text());
	this->tab->sock->sendPacket(packet);
	
	delete this;
}

void CRCAccount::CancelChanges()
{
	delete this;
}

CPacket CRCAccount::getPacket(QComboBox *p)
{
	return CPacket() << (char)p->itemText(p->currentIndex()).length() << (const char *)p->itemText(p->currentIndex()).toAscii();
}

CPacket CRCAccount::getPacket(QLineEdit *p)
{
	return CPacket() << (char)p->text().length() << (const char *)p->text().toAscii();
}

CPacket CRCAccount::getPacket(QTextEdit *p)
{
	return CPacket() << (char)p->toPlainText().length() << (const char *)p->toPlainText().toAscii();
}

CPacket CRCAccount::getPacket(QTextEdit *text, int type)
{
	CPacket packet;
	CStringList list;
	list.load(text->toPlainText().toAscii(), "\n");
	
	if (type < 1)
		packet << (char)list.count();
	else
		packet << (short)list.count();
	
	for (int i = 0; i < list.count(); i++)
	{
		switch (type)
		{
			case 1:
				if (list[i].find("=") >= 0)
				{
					packet << (char)list[i].length() << list[i];
				}
			break;
			
			case 2:
			{
				CStringList chest;
				chest.load(list[i].text(), ":");
				
				if (chest.count() == 3)
				{
					CPacket chestData;
					chestData << (char)atoi(chest[0].text()) << (char)atoi(chest[1].text()) << chest[2];
					packet << (char)chestData.length() << chestData;
				}
				break;
			}
			
			default:
				packet << (char)list[i].length() << list[i];
			break;
		}
	}

	return packet;
}