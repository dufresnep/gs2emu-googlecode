#include "main.h"
#include "TTabWidget.h"

/*
	Constructor / Destructor
*/

TTabWidget::TTabWidget(QWidget *parent) : QWidget(parent)
{
	// Reset Tab Type
	setType(STYPE_NONE);

	// Setup UI
    setupUi(this);
}

/*
    Tab-Type Functions
*/

int TTabWidget::getType()
{
    return tabType;
}

void TTabWidget::setType(int pTabType)
{
    tabType = pTabType;
}

/*
    Virtual Functions
*/
void TTabWidget::sendText(CBuffer pText)
{
	pText.clear();
}
