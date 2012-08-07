#ifndef TLOGINWINDOW_H
#define TLOGINWINDOW_H

#include "ui_login.h"

class TLoginWindow : public QMainWindow, private Ui::FLoginWindow
{
    Q_OBJECT

    public:
        TLoginWindow(QMainWindow *parent = 0);
        ~TLoginWindow();

	private slots:
		void runSockets();
		void slotCancel();
		void slotConnect();

    protected:

    private:
		QTimer *sTimer;
};


#endif // TLOGINWINDOW_H

