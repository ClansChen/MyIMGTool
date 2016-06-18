#pragma once
#include <QAbstractTableModel>
#include "IMGClass.h"
#include <vector>
#include <memory>

class IMGTableModel :public QAbstractTableModel
{
public:
	IMGTableModel(std::shared_ptr<const std::vector<IMGClass::IMGDirectoryEntryWrap> > source, QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void RefreshView();

private:
	std::shared_ptr<const std::vector<IMGClass::IMGDirectoryEntryWrap> > m_pIMGDirectory;
};
