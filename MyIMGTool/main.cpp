#pragma execution_character_set("utf-8")

#include "myimgtool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MYIMGTOOL window;
	window.show();

	if (argc > 1)
		window.OpenArchive(QString::fromLocal8Bit(argv[1]));
	else
		window.OpenArchive(R"(D:\游戏\Grand Theft Auto San Andreas\models\gta3.img)");

	return a.exec();
}
