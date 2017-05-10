#pragma once
#include <QAbstractTableModel>
#include "IMGClass.h"
#include <vector>
#include <memory>

class IMGTableModel :public QAbstractTableModel
{
public:
	IMGTableModel(QObject *parent = nullptr);
	void setDataSource(const std::vector<IMGClass::IMGDirectoryEntry> *source = nullptr);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	void RefreshView();

private:
	const std::vector<IMGClass::IMGDirectoryEntry> *m_pIMGDirectory = nullptr;
};
