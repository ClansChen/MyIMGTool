#pragma once
#include <QProgressDialog>

class MyProgressDialog :public QProgressDialog
{
public:
	MyProgressDialog(const QString &labelText, const QString &cancelButtonText, int minimum, int maximum, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

protected:
	void keyPressEvent(QKeyEvent *);
};
