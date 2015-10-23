#pragma once
#include <QAbstractTableModel>
#include "IMGClass.h"
#include <vector>

class IMGTableModel :public QAbstractTableModel
{
public:
	IMGTableModel(QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void SetTableData(const std::vector<IMGClass::IMGTableItem> &);

private:
	std::vector<IMGClass::IMGTableItem> m_ArchiveTable;
};
