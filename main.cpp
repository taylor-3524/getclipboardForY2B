#include "getclipboard.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	getclipboard w;
	w.show();
	return a.exec();
}
