#pragma once
#include <QAbstractTableModel>
#include "myimgtool.h"
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
	//bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	//Qt::ItemFlags flags(const QModelIndex &index);

	void SetSourceData(const std::vector<IMGClass::IMGDirectoryEntryWrap> &source);

private:
	std::vector<IMGClass::IMGDirectoryEntryWrap> m_IMGDirectory;
};
