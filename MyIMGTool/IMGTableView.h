#pragma once
#include <QTableView>

class QString;
class QStringList;

class IMGTableView :public QTableView
{
	Q_OBJECT

public:
	explicit IMGTableView(QWidget *parent = nullptr);

protected:
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

signals:
	void ImportNow(const QStringList &);
	void ExportSelected(const QString &);
};
