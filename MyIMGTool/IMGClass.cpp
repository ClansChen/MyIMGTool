#pragma execution_character_set("utf-8")

#include "IMGClass.h"
#include <QFileInfo>
#include <QDir>
#include <QByteArray>
#include <string>
#include <cstring>
#include <algorithm>

bool IMGClass::CreateArchive(const QString &path, IMGClass::ArchiveVersion ver)
{
	QFile newArchive, newDir;

	newArchive.setFileName(path);

	if (!newArchive.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		WriteErrorMessage({ "创建档案", path, "失败" });
		return false;
	}

	if (ver == VERSION1)
	{
		QString dirPath(path);
		newDir.setFileName(dirPath.replace(dirPath.length() - 3, 3, "dir"));

		if (!newDir.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			newArchive.remove();
			WriteErrorMessage({ "创建版本1档案目录", path, "失败" });
			return false;
		}
	}
	else if (ver == VERSION2)
	{
		newArchive.write("VER2", 4);
		newArchive.write("\x00\x00\x00\x00", 4);
	}
    
    return true;
}

bool IMGClass::OpenArchive(const QString &imgpath)
{
	quint32 n_files = 0;

	CloseArchive();

	m_IMGHandle.setFileName(imgpath);

	if (!m_IMGHandle.open(QIODevice::ReadWrite))
	{
		WriteErrorMessage({ "档案打开失败" });
		return false;
	}

	if (m_IMGHandle.read(4) == "VER2")
	{
		m_IMGHandle.read(reinterpret_cast<char *>(&n_files), 4);

		if (n_files != 0)
		{
			m_ArchiveTable.resize(n_files);

			m_IMGHandle.read(reinterpret_cast<char *>(m_ArchiveTable.data()), n_files * TABLE_ITEM_SIZE);

			m_FreeTableSlotsCount = GetTableFreeSlotsCount();
		}

		m_ArchiveVersion = VERSION2;
	}
	else
	{
		QString dirpath(imgpath);

		m_DirHandle.setFileName(dirpath.replace(dirpath.length() - 3, 3, "dir"));

		if (!m_DirHandle.open(QIODevice::ReadWrite))
		{
			WriteErrorMessage({ "版本1目录打开失败" });
			m_IMGHandle.close();
			return false;
		}

		n_files = m_DirHandle.size() / TABLE_ITEM_SIZE;

		if (n_files != 0)
		{
			m_ArchiveTable.resize(n_files);

			m_DirHandle.read(reinterpret_cast<char *>(m_ArchiveTable.data()), n_files * TABLE_ITEM_SIZE);
		}

		m_ArchiveVersion = VERSION1;
	}

	return true;
}

void IMGClass::CloseArchive()
{
	m_IMGHandle.close();

	if (m_ArchiveVersion == VERSION1)
		m_DirHandle.close();

	m_ArchiveVersion = UNDEFINED;

	m_ArchiveTable.clear();
	m_ErrorMessage.clear();
}

bool IMGClass::ImportFile(const QString &filepath)
{
	QFile importFile;
	QFileInfo fileInfo;
	std::string fileName;
	IMGTableItem temp;

	importFile.setFileName(filepath);

	if (!importFile.open(QIODevice::ReadOnly))
	{
		WriteErrorMessage({ "打开文件", filepath, "失败" });
		return false;
	}

    if (importFile.size() == 0)
    {
        WriteErrorMessage({ "文件", filepath, "太小，不能导入" });
        return false;
    }

	if (importFile.size() > MAX_FILE_SIZE)
	{
		WriteErrorMessage({ "文件", filepath, "太大，不能导入" });
		return false;
	}

	fileInfo.setFile(importFile);
	fileName = fileInfo.fileName().toStdString();

	if (fileName.length() > 23)
	{
		WriteErrorMessage({ "文件", fileInfo.fileName(), "的文件名太长，不能导入" });
		return false;
	}

	std::strcpy(temp.Name, fileName.c_str());
	temp.SizeSecondPriority = static_cast<quint16>(AlignOffsetToBlocks(importFile.size()));
	temp.Position = AlignOffsetToBlocks(m_IMGHandle.size());

	m_IMGHandle.seek(temp.Position * IMG_BLOCK_SIZE);
	m_IMGHandle.write(importFile.readAll());

	WriteArchiveTableItem(temp);
	return true;
}

bool IMGClass::ExportFile(const QString &dest, quint32 index)
{
	QFile exportFile;
	QDir destination;
	std::vector<IMGTableItem>::const_iterator iterToExportingEntry;

	if (index >= m_ArchiveTable.size())
	{
		WriteErrorMessage({ "尝试导出无效的文件序号" });
		return false;
	}

	iterToExportingEntry = m_ArchiveTable.cbegin() + index;

	if (!destination.cd(dest))
	{
		WriteErrorMessage({ "访问目标文件夹失败" });
		return false;
	}

	exportFile.setFileName(destination.absoluteFilePath(iterToExportingEntry->Name));
	if (!exportFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		WriteErrorMessage({ "创建输出文件", exportFile.fileName(), "失败" });
		return false;
	}

	m_IMGHandle.seek(iterToExportingEntry->Position * IMG_BLOCK_SIZE);
	exportFile.write(m_IMGHandle.read(iterToExportingEntry->SizeSecondPriority * IMG_BLOCK_SIZE));

	return true;
}

quint32 IMGClass::GetTableFreeSlotsCount() const
{
	auto minFileOffsetIter = std::min_element(m_ArchiveTable.cbegin(), m_ArchiveTable.cend(),
		[](const IMGTableItem &lhs, const IMGTableItem &rhs) {return lhs.Position < rhs.Position; });

	if (minFileOffsetIter == m_ArchiveTable.cend())
		return (IMG_BLOCK_SIZE * 1 - 8) / TABLE_ITEM_SIZE;
	else
		return (minFileOffsetIter->Position * IMG_BLOCK_SIZE - 8 - m_ArchiveTable.size() * TABLE_ITEM_SIZE) / TABLE_ITEM_SIZE;
}

quint32 IMGClass::GetFilesCount() const
{
	return m_ArchiveTable.size();
}

QString IMGClass::GetArchiveFullPath() const
{
	return m_IMGHandle.fileName();
}

QString IMGClass::GetErrorMessage() const
{
	return this->m_ErrorMessage;
}

IMGClass::ArchiveVersion IMGClass::GetArchiveVersion() const
{
	return m_ArchiveVersion;
}

quint32 IMGClass::AlignOffsetToBlocks(qint64 offset)
{
	if (offset < MAX_ARCHIVE_SIZE)
	{
		if ((offset % IMG_BLOCK_SIZE) == 0)
			return static_cast<quint32>(offset / IMG_BLOCK_SIZE);
		else
			return static_cast<quint32>(offset / IMG_BLOCK_SIZE + 1);
	}
	else
		return 0xFFFFFFFFu;
}

void IMGClass::WriteErrorMessage(std::initializer_list<QString> args)
{
	m_ErrorMessage.clear();

	for (auto iter = args.begin(); iter != args.end(); ++iter)
		m_ErrorMessage += *iter;
}

void IMGClass::WriteArchiveTableItem(const IMGTableItem &item)
{
	std::vector<IMGTableItem>::iterator::difference_type index;

    auto iterToGivenItem = std::find_if(m_ArchiveTable.begin(), m_ArchiveTable.end(),
        [item](const IMGTableItem &another) {return std::strcmp(item.Name, another.Name) == 0; });

    if (iterToGivenItem == m_ArchiveTable.end())
    {
        if (m_ArchiveVersion == VERSION2)
        {
            if(m_FreeTableSlotsCount == 0)
                MakeArchiveTableFreeSlots();

            --m_FreeTableSlotsCount;
        }

		index = iterToGivenItem - m_ArchiveTable.begin();

        m_ArchiveTable.push_back(item);
    }
    else
        *iterToGivenItem = item;

	if (m_ArchiveVersion == VERSION2)
	{
		m_IMGHandle.seek(8 + index * TABLE_ITEM_SIZE);
		m_IMGHandle.write(reinterpret_cast<const char *>(&item), TABLE_ITEM_SIZE);
		WriteVersion2FilesCount();
	}
	else if (m_ArchiveVersion == VERSION1)
	{
		m_DirHandle.seek(index * TABLE_ITEM_SIZE);
		m_DirHandle.write(reinterpret_cast<const char *>(&item), TABLE_ITEM_SIZE);
	}
}

void IMGClass::MakeArchiveTableFreeSlots()
{
	auto firstFileIter = std::min_element(m_ArchiveTable.begin(), m_ArchiveTable.end(),
		[](const IMGTableItem &lhs, const IMGTableItem &rhs) {return lhs.Position < rhs.Position; });

	m_IMGHandle.seek(firstFileIter->Position * IMG_BLOCK_SIZE);
	QByteArray buffer = m_IMGHandle.read(firstFileIter->SizeSecondPriority * IMG_BLOCK_SIZE);

	firstFileIter->Position = AlignOffsetToBlocks(m_IMGHandle.size());
	m_IMGHandle.seek(firstFileIter->Position * IMG_BLOCK_SIZE);
	m_IMGHandle.write(buffer);

	m_FreeTableSlotsCount += firstFileIter->SizeSecondPriority * IMG_BLOCK_SIZE / TABLE_ITEM_SIZE;

	WriteArchiveTableItem(*firstFileIter);
}

void IMGClass::WriteVersion2FilesCount()
{
	quint32 temp = m_ArchiveTable.size();
	m_IMGHandle.seek(4);
	m_IMGHandle.write(reinterpret_cast<const char *>(&temp), 4);
}

const std::vector<IMGClass::IMGTableItem> &IMGClass::GetTableData() const
{
	return m_ArchiveTable;
}
