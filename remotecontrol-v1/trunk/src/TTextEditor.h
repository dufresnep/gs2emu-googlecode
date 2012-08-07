#ifndef TTEXTEDITOR_H
#define TTEXTEDITOR_H

#include "TSyntaxHighlighter.h"
#include "TTabWidget.h"
#include "ui_textedit.h"

class TTextEditor : public QWidget, public Ui::FTextEdit
{
	Q_OBJECT

	public:
		TTextEditor(bool pHighlight, QWidget *parent = 0);

		TTabWidget* getTab();

	private:
		Highlighter highlighter;
};

#endif // TTEXTEDITOR_H
