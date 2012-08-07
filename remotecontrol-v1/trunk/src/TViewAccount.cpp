#include "main.h"
#include "TViewAccount.h"

/*
	Constructor / Destructor
*/

TViewAccount::TViewAccount(QWidget *parent) : QWidget(parent)
{
    /* Setup Ui */
    setupUi(this);

    /* Add Colors */
    ppSkin->addItems(colors);
    ppCoat->addItems(colors);
    ppSleeve->addItems(colors);
    ppShoe->addItems(colors);
    ppBelt->addItems(colors);
}

/*
	Functions
*/

TTabWidget* TViewAccount::getTab()
{
	return (TTabWidget *)parent();
}


CPacket TViewAccount::getData()
{
	return CPacket();
}

void TViewAccount::setData(CPacket& pPacket)
{
	// Declare Variables
	CPacket propPacket;
	int val;

	/*
		Read Packet Data
	*/

	// Player-Account
	player.plyrId = pPacket.readByteU2();
	player.acctName = pPacket.readChars(pPacket.readByteU1());
	pPacket.readChars(pPacket.readByteU1()); // world??

	// Player-Properties
	propPacket = pPacket.readChars(pPacket.readByteU1());
	player.setProps(propPacket);

	// Player-Flags
	val = pPacket.readByteU2();
	for (int i = 0; i < val; i++)
		player.plyrFlags.push_back(pPacket.readChars(pPacket.readByteU1()));

	// Player-Chests
	val = pPacket.readByteU2();
	for (int i = 0; i < val; i++)
	{
		int len = pPacket.readByteU1();
		player.plyrChests.push_back(CString() << toString(pPacket.readByteU1()) << ":" << toString(pPacket.readByteU1()) << ":" << pPacket.readChars(len - 2));
	}

	// Player-Weapons
	val = pPacket.readByteU1();
	for (int i = 0; i < val; i++)
		player.plyrWeapons.push_back(pPacket.readChars(pPacket.readByteU1()));

	/*
		Set Window Data
	*/

	// Tab 1 - Player Properties
	ppLevel->setText(player.plyrLevel.text());
	ppX->setText(toString(player.plyrX).text());
	ppY->setText(toString(player.plyrY).text());

	ppMaxPower->setText(toString(player.plyrMaxPower).text());
	ppPower->setText(toString(player.plyrPower).text());
	ppAP->setText(toString(player.plyrAp).text());
	ppMP->setText(toString(player.plyrMagic).text());
	ppGralats->setText(toString(player.plyrGralats).text());
	ppGlove->setText(toString(player.plyrGlovePower).text());
	ppBombs->setText(toString(player.plyrBombs).text());
	ppArrows->setText(toString(player.plyrDarts).text());
	ppSwordPower->setText(toString(player.plyrSwordPower).text());
	ppSwordImg->setText(player.plyrSword.text());
	ppShieldPower->setText(toString(player.plyrShieldPower).text());
	ppShieldImg->setText(player.plyrShield.text());
	ppGender->setChecked(player.plyrStatus & 4);
	ppWeapons->setChecked(player.plyrStatus & 16);
	ppSpin->setChecked(player.plyrStatus & 64);

	ppHead->setText(player.plyrHead.text());
	ppBody->setText(player.plyrBody.text());
	ppHat->setText(player.plyrAttr[0].text());
	ppAni->setText(player.plyrGani.text());
	ppSkin->setCurrentIndex((int)player.plyrColors[0]);
	ppCoat->setCurrentIndex((int)player.plyrColors[1]);
	ppSleeve->setCurrentIndex((int)player.plyrColors[2]);
	ppShoe->setCurrentIndex((int)player.plyrColors[3]);
	ppBelt->setCurrentIndex((int)player.plyrColors[4]);

	// Tab 2 - Player Attributes
	paChests->clear();
	for (int i = 0; i < (int)player.plyrChests.size(); i++)
		paChests->append(player.plyrChests[i].text());

	paFlags->clear();
	for (int i = 0; i < (int)player.plyrFlags.size(); i++)
		paFlags->append(player.plyrFlags[i].text());

	paWeapons->clear();
	for (int i = 0; i < (int)player.plyrWeapons.size(); i++)
		paWeapons->append(player.plyrWeapons[i].text());

	// Tab 3 - Player Rights
	prAccount->setText(player.acctName.text());

	prFolders->clear();
	for (int i = 0; i < (int)player.acctFolders.size(); i++)
		prFolders->append(player.acctFolders[i].text());

	/*
	// Declare Variables
	CBuffer propPacket;
	int val;

	// Player-Attributes
	playerId = (unsigned short)pPacket.readByte2();
	playerAccount = pPacket.readChars((unsigned char)pPacket.readByte1());
	pPacket.readChars((unsigned char)pPacket.readByte1());

	// Player-Properties
	propPacket = pPacket.readChars((unsigned char)pPacket.readByte1());
	while (propPacket.bytesLeft() > 0)
		propPacket.clear();

	// Player-Flags
	val = (unsigned char)pPacket.readByte2();
	for (int i = 0 ; i < val; i++)
		paFlags->append(pPacket.readChars(pPacket.readByte1()));

	// Player-Chests
	val = (unsigned char)pPacket.readByte2();
	for (int i = 0; i < val; i++)
	{
		int len = pPacket.readByte1();
		paChests->append(CString(CString() << toString(pPacket.readByte1()) << ":" << toString(pPacket.readByte1()) << ":" << pPacket.readChars(len - 2)).text());
	}

	// Player-Weapons
	val = (unsigned char)pPacket.readByte1();
	for (int i = 0; i < val; i++)
		paWeapons->append(pPacket.readChars((unsigned char)pPacket.readByte1()));


	pPacket.readByte5();
	prIPRange->setText(pPacket.readChars(pPacket.readByte1()));

	// Admin Folders
	val = (unsigned char)pPacket.readByte1();
	for (int i = 0; i < val; i++)
		prFolders->append(pPacket.readChars(pPacket.readByte1()));

	printf("SETTED: %s\nLeft: %i\n", pPacket.text(), pPacket.bytesLeft());
	*/
}
