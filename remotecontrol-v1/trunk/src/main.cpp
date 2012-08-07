#include "main.h"
#include "zlib.h"

TLoginWindow *LoginWindow;
TRCWindow *RCWindow;
TServerWindow *ServerWindow;

CBuffer graalAccount, graalNickname, graalPassword;
TSockList sockList;
QStringList colors;

int main(int argc, char *argv[])
{
	/* Declare Exit-Function */
	atexit(end);

	/* Start Application */
	QApplication app(argc, argv);

	/* Setup  */
	CSocket::sockStart();
	createRCFunc();
	createColors();

	/* Seed Time */
	srand(time(NULL));

	/* Create Window */
	LoginWindow = new TLoginWindow();
	RCWindow = new TRCWindow();
	ServerWindow = new TServerWindow();

	LoginWindow->setVisible(true);

    /* Execute / Return */
	return app.exec();
}

void end()
{
	// LoginWindow
	delete LoginWindow;

	// RCWindow
	delete RCWindow;

	// ServerWindow
	delete ServerWindow;

	// Empty sockList
	sockList.clear();
}

void createColors()
{
	const char *colours[] = {"white", "yellow", "orange", "pink", "red", "darkred", "lightgreen", "green", "darkgreen", "lightblue", "blue", "darkblue", "brown", "cynober", "purple", "darkpurple", "lightgray", "gray", "black", "transparent"};
	for (unsigned int i = 0; i < 20; i++)
		colors << colours[i];
}

/*
	Compress & Uncompress Functions
*/
CBuffer compressBuffer(CBuffer pBuffer)
{
	/* Input Data Check */
	if (pBuffer.length() <= 0)
		return CBuffer();

	/* Declare Variables */
	CBuffer buf;
	char buffer[65536];
	int error;
	unsigned long clen = sizeof(buffer);

	/* Compress Data */
	if ((error = compress((Bytef *)buffer, (uLongf *)&clen, (const Bytef *)pBuffer.text(), pBuffer.length())) != Z_OK)
		return CBuffer();

	/* Write the Data + Return */
	if (clen > 0)
		buf.writeBytes(buffer, clen);
	return buf;
}

CBuffer decompressBuffer(CBuffer pBuffer)
{
	/* Input Data Check */
	if (pBuffer.length() <= 0)
		return CBuffer();

	/* Declare Variables */
	CBuffer buf;
	char buffer[65536];
	int error;
	unsigned long clen = sizeof(buffer);

	/* Uncompress Data */
	if ((error = uncompress((Bytef *)buffer, (uLongf *)&clen, (const Bytef *)pBuffer.text(), pBuffer.length())) != Z_OK)
		return CBuffer();

	/* Write the Data + Return */
	if (clen > 0)
		buf.writeBytes(buffer, clen);
	return buf;
}

CBuffer toString(QString pBuffer)
{
	return CBuffer((const char *)pBuffer.toAscii());
}
