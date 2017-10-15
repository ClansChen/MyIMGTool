#pragma once
#include <QAbstractTableModel>
#include "../img/IMGClass.h"
#include <QVector>

class IMGTableModel :public QAbstractTableModel
{
public:
	IMGTableModel(QObject *parent = nullptr);
    void setDataSource(const QVector<IMGClass::IMGDirectoryEntry> *source);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	void RefreshView();

private:
    const QVector<IMGClass::IMGDirectoryEntry> *m_pIMGDirectory = nullptr;
};
