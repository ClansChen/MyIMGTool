#pragma execution_character_set("utf-8")

#include "IMGTableModel.h"

IMGTableModel::IMGTableModel(QObject *parent)
	:QAbstractTableModel(parent)
{
}

int IMGTableModel::rowCount(const QModelIndex &parent) const
{
	return m_pIMGDirectory->size();
}

int IMGTableModel::columnCount(const QModelIndex &parent) const
{
	return 4;
}

QVariant IMGTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			return "文件名";
		case 1:
			return "大小(KB)";
		case 2:
			return "序号";
		case 3:
			return "偏移量";
		default:
			break;
		}
	}

	return QVariant();
}

QVariant IMGTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0:
			return m_pIMGDirectory->at(index.row()).m_Name;
		case 1:
			return m_pIMGDirectory->at(index.row()).m_SizeLow16 * IMGClass::IMG_BLOCK_SIZE / 1024;
		case 2:
			return index.row();
		case 3:
			return m_pIMGDirectory->at(index.row()).m_Offset * IMGClass::IMG_BLOCK_SIZE;

		default:
			break;
		}
	}
	else if (role == Qt::TextAlignmentRole)
	{
		if (index.column() == 0)
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		else
			return int(Qt::AlignHCenter | Qt::AlignVCenter);
	}

	return QVariant();
}

void IMGTableModel::RefreshView()
{
	beginResetModel();
	endResetModel();
}

void IMGTableModel::setDataSource(const QVector<IMGClass::IMGDirectoryEntry> *source)
{
	m_pIMGDirectory = source;
}

