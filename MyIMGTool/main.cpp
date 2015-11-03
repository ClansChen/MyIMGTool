#pragma execution_character_set("utf-8")

#include "myimgtool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MYIMGTOOL window;
	window.show();

	if (argc > 1)
		window.OpenIMG(QString::fromLocal8Bit(argv[1]));

	return a.exec();
}
