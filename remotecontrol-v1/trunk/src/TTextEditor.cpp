#include "main.h"
#include "TTextEditor.h"

/*
	Constructor / Destructor
*/

TTextEditor::TTextEditor(bool pHighlight, QWidget *parent) : QWidget(parent)
{
	/* Setup Ui */
	setupUi(this);

	/* Ui Signal/Slot Connections */
	connect(BClose, SIGNAL(released()), this, SLOT(close()));
	//connect(BFind, SIGNAL(released()), this, SLOT(slotConnect()));
	//connect(BSend, SIGNAL(released()), this, SLOT(runSockets()));

	/* Start Highlighter ? */
	if (pHighlight)
		highlighter.setDocument(textEdit->document());
}

/*
	Functions
*/

TTabWidget* TTextEditor::getTab()
{
	return (TTabWidget *)parent();
}
