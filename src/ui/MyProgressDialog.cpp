#include "MyProgressDialog.h"
#include <QKeyEvent>

MyProgressDialog::MyProgressDialog(const QString &labelText, const QString &cancelButtonText, int minimum, int maximum, QWidget *parent, Qt::WindowFlags flags)
	:QProgressDialog(labelText, cancelButtonText, minimum, maximum, parent, flags)
{
}

void MyProgressDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key() != Qt::Key_Escape)
		QProgressDialog::keyPressEvent(event);
}
