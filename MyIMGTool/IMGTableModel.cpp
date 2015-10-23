#pragma execution_character_set("utf-8")

#include "IMGTableModel.h"

IMGTableModel::IMGTableModel(QObject *parent)
	:QAbstractTableModel(parent)
{
	m_ArchiveTable.reserve(20000);
}

int IMGTableModel::rowCount(const QModelIndex &parent) const
{
	return m_ArchiveTable.size();
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
			return "偏移量";
		case 3:
			return "序号";
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
			return m_ArchiveTable.at(index.row()).Name;
		case 1:
			return m_ArchiveTable.at(index.row()).SizeSecondPriority * IMGClass::IMG_BLOCK_SIZE / 1024;
		case 2:
			return m_ArchiveTable.at(index.row()).Position * IMGClass::IMG_BLOCK_SIZE;
		case 3:
			return index.row();
		default:
			break;
		}
	}
	else if (role == Qt::TextAlignmentRole)
	{
		switch (index.column())
		{
		case 0:
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		case 1:
		case 2:
		case 3:
			return int(Qt::AlignHCenter | Qt::AlignVCenter);
		default:
			break;
		}
	}

	return QVariant();
}

void IMGTableModel::SetTableData(const std::vector<IMGClass::IMGTableItem> &source)
{
	beginResetModel();
	m_ArchiveTable = source;
	endResetModel();
}
