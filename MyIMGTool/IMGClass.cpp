#pragma execution_character_set("utf-8")

#include "IMGClass.h"
#include "CKeyGen.h"
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <cstring>
#include <algorithm>

IMGClass::IMGDirectoryEntryWrap::IMGDirectoryEntryWrap(const IMGClass::IMGDirectoryEntry &raw)
{
	m_RawData = raw;
	m_NameHash = CKeyGen::GetKeyOfUpCasedString(raw.m_Name);
}

IMGClass::IMGClass(QObject *parent) :QObject(parent){}

bool IMGClass::CreateIMG(const QString &imgpath, IMGVersion version)
{
	QFile newIMG, newDir;
	QString dirpath = imgpath;

	newIMG.setFileName(imgpath);
	newDir.setFileName(dirpath.replace(dirpath.length() - 3, 3, "dir"));

	if (!newIMG.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		emit ErrorOccoured({ "创建 ", imgpath, " 失败" });
		return false;
	}

	if (version == VERSION1)
	{
		if (!newDir.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			newIMG.remove();
			emit ErrorOccoured({ "创建 ", dirpath, " 失败" });
			return false;
		}
	}
	else if (version == VERSION2)
	{
		newIMG.write("VER2", 4);
		newIMG.write("\x00\x00\x00\x00", 4);
	}

	return true;
}

bool IMGClass::OpenIMG(const QString &imgpath)
{
	quint32 n_files;
	IMGDirectoryEntry tempItem;
	QByteArray imgHeader;

	m_IMGHandle.setFileName(imgpath);

	if (!m_IMGHandle.open(QIODevice::ReadWrite))
	{
		emit ErrorOccoured({ "打开 ", imgpath, " 失败" });
		return false;
	}

	imgHeader = m_IMGHandle.read(4);

	if (imgHeader == "VER2")
	{
		m_IMGHandle.read(reinterpret_cast<char *>(&n_files), 4);

		m_IMGDirectory.reserve(n_files * 2);

		while (n_files != 0)
		{
			m_IMGHandle.read(reinterpret_cast<char *>(&tempItem), DIRECTORY_ENTRY_SIZE);

			m_IMGDirectory.push_back(IMGDirectoryEntryWrap(tempItem));
			--n_files;
		}

		GetVersion2IMGDirectoryFreeSlotsCount();

		m_IMGVersion = VERSION2;
	}
	else
	{
		QString dirpath(imgpath);

		m_DIRHandle.setFileName(dirpath.replace(dirpath.length() - 3, 3, "dir"));

		if (!m_DIRHandle.open(QIODevice::ReadWrite))
		{
			emit ErrorOccoured({ "打开 ", dirpath, " 失败" });
			m_IMGHandle.close();
			return false;
		}

		if ((m_DIRHandle.size() % 32) != 0)
		{
			emit ErrorOccoured({ "IMG目录 ", dirpath, " 大小不合法" });
			m_IMGHandle.close();
			m_IMGHandle.close();
			return false;
		}

		n_files = m_DIRHandle.size() / DIRECTORY_ENTRY_SIZE;

		m_IMGDirectory.reserve(n_files * 2);

		while (n_files != 0)
		{
			m_DIRHandle.read(reinterpret_cast<char *>(&tempItem), DIRECTORY_ENTRY_SIZE);

			m_IMGDirectory.push_back(IMGDirectoryEntryWrap(tempItem));
			--n_files;
		}

		m_IMGVersion = VERSION1;
	}

	emit IMGDirectoryChanged(m_IMGDirectory);

	return true;
}

void IMGClass::CloseIMG()
{
	m_IMGHandle.close();
	m_DIRHandle.close();

	m_IMGVersion = UNDEFINED;

	m_IMGDirectory.clear();
}

void IMGClass::ImportFiles(const QStringList &paths)
{
	QFile importFile;
	QFileInfo fileInfo;
	QString wideFileName;
	std::string narrowFileName;
	IMGDirectoryEntry tempEntry;

	if (paths.isEmpty())
		return;

	for (auto &path : paths)
	{
		importFile.setFileName(path);
		if (!importFile.open(QIODevice::ReadOnly))
		{
			emit ErrorOccoured({ "打开", path, "失败" });
			break;
		}

		if (importFile.size() > MAX_FILE_SIZE)
		{
			emit ErrorOccoured({ "文件", path, "太大" });
			break;
		}

		fileInfo.setFile(importFile);
		wideFileName = fileInfo.fileName();
		narrowFileName = wideFileName.toStdString();
		if (narrowFileName.length() > 23)
		{
			emit ErrorOccoured({ "文件名", wideFileName, "太长" });
			break;
		}

		emit ImportingFileFullPath(path);
		emit IncreaseProgressBar();
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		std::strcpy(tempEntry.m_Name, narrowFileName.c_str());
		tempEntry.m_SizeLow16 = AlignOffsetToBlocks(importFile.size());
		tempEntry.m_Offset = AlignOffsetToBlocks(m_IMGHandle.size());
		m_IMGHandle.seek(tempEntry.m_Offset * IMG_BLOCK_SIZE);
		m_IMGHandle.write(importFile.readAll());

		quint32 index = GetIMGDirectoryEntryIndexByName(tempEntry.m_Name);

		if (index == m_IMGDirectory.size())
		{
			m_IMGDirectory.push_back(IMGDirectoryEntryWrap(tempEntry));

			if (m_IMGVersion == VERSION2 && m_Version2IMGDirectoryFreeSlotsCount == 0)
				MakeIMGDirectoryFreeSlots();

			--m_Version2IMGDirectoryFreeSlotsCount;
		}
		else
			m_IMGDirectory[index] = IMGDirectoryEntryWrap(tempEntry);
	}

	WriteIMGDirectory();
	emit IMGDirectoryChanged(m_IMGDirectory);
}

void IMGClass::ExportFiles(const QString &dest, const QModelIndexList &indexes)
{
	QFile exportFile;
	QDir destFolder;
	QString fileName;

	if (!destFolder.cd(dest))
	{
		emit ErrorOccoured({ "访问输出文件夹失败" });
		return;
	}

	for (auto &index : indexes)
	{
		auto iterToExportingEntry = m_IMGDirectory.begin() + index.row();
		fileName = iterToExportingEntry->m_RawData.m_Name;

		emit ExportingFileName(fileName);
		emit IncreaseProgressBar();
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		exportFile.setFileName(destFolder.absoluteFilePath(fileName));
		if (!exportFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			emit ErrorOccoured({ "创建", exportFile.fileName(), "失败" });
			return;
		}

		m_IMGHandle.seek(iterToExportingEntry->m_RawData.m_Offset * IMG_BLOCK_SIZE);
		exportFile.write(m_IMGHandle.read(iterToExportingEntry->m_RawData.m_SizeLow16 * IMG_BLOCK_SIZE));
	}
}

void IMGClass::RemoveFiles(const QModelIndexList &indexes)
{
	for (auto &index : indexes)
		m_IMGDirectory.at(index.row()).m_NameHash = 0;

	m_IMGDirectory.erase(std::remove_if(m_IMGDirectory.begin(), m_IMGDirectory.end(),
		[](IMGDirectoryEntryWrap &entry){return entry.m_NameHash == 0; }), m_IMGDirectory.end());

	if (m_IMGVersion == VERSION2)
		m_Version2IMGDirectoryFreeSlotsCount += indexes.size();

	WriteIMGDirectory();
	emit IMGDirectoryChanged(m_IMGDirectory);
}

void IMGClass::RebuildIMG()
{
	QFile rebuildingIMG, rebuildingDIR;

	quint32 writingBlock = GetFirstWritingBlockForRebuilding() + 1;

	rebuildingIMG.setFileName(m_IMGHandle.fileName() + ".rebuilding");
	rebuildingDIR.setFileName(m_DIRHandle.fileName() + ".rebuilding");

	if (!rebuildingIMG.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		emit ErrorOccoured({ "创建临时文件失败" });
		return;
	}

	if (m_IMGVersion == VERSION1)
		if (!rebuildingDIR.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			emit ErrorOccoured({ "创建临时文件失败" });
			rebuildingIMG.remove();
			return;
		}

	for (auto &entry : m_IMGDirectory)
	{
		emit IncreaseProgressBar();
		emit RebuildingFileName(entry.m_RawData.m_Name);
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		m_IMGHandle.seek(entry.m_RawData.m_Offset * IMG_BLOCK_SIZE);

		rebuildingIMG.seek(writingBlock * IMG_BLOCK_SIZE);
		rebuildingIMG.write(m_IMGHandle.read(entry.m_RawData.m_SizeLow16 * IMG_BLOCK_SIZE));

		entry.m_RawData.m_Offset = writingBlock;
		writingBlock += entry.m_RawData.m_SizeLow16;
	}

	if (m_IMGVersion == VERSION1)
		WriteVersion1IMGDirectory(rebuildingDIR);
	else if (m_IMGVersion == VERSION2)
		WriteVersion2IMGDirectory(rebuildingIMG);

	m_IMGHandle.remove();
	rebuildingIMG.rename(m_IMGHandle.fileName());

	if (m_IMGVersion == VERSION1)
	{
		m_DIRHandle.remove();
		rebuildingDIR.rename(m_DIRHandle.fileName());
	}
}

quint32 IMGClass::AlignOffsetToBlocks(qint64 offset)
{
	if (offset < MAX_IMG_SIZE)
		if ((offset % IMG_BLOCK_SIZE) == 0)
			return static_cast<quint32>(offset / IMG_BLOCK_SIZE);
		else
			return static_cast<quint32>(offset / IMG_BLOCK_SIZE + 1);
	else
		return INVALID_OFFSET;
}

void IMGClass::WriteVersion1IMGDirectory(QFile &file)
{
	file.seek(0);

	for (auto &entry : m_IMGDirectory)
		file.write(reinterpret_cast<const char *>(&entry.m_RawData), DIRECTORY_ENTRY_SIZE);

	file.resize(m_IMGDirectory.size() * DIRECTORY_ENTRY_SIZE);
}

void IMGClass::WriteVersion2IMGDirectory(QFile &file)
{
	quint32 count = m_IMGDirectory.size();
	file.seek(0);
	file.write("VER2", 4);
	file.write(reinterpret_cast<const char *>(&count), 4);

	for (auto &entry : m_IMGDirectory)
		file.write(reinterpret_cast<const char *>(&entry.m_RawData), DIRECTORY_ENTRY_SIZE);
}

void IMGClass::WriteIMGDirectory()
{
	if (m_IMGVersion == VERSION1)
		WriteVersion1IMGDirectory(m_DIRHandle);
	else if (m_IMGVersion == VERSION2)
		WriteVersion2IMGDirectory(m_IMGHandle);
}

quint32 IMGClass::GetFirstWritingBlockForRebuilding()
{
	if (m_IMGVersion == VERSION1)
		return 0;
	else if (m_IMGVersion == VERSION2)
		return AlignOffsetToBlocks(8 + m_IMGDirectory.size() * DIRECTORY_ENTRY_SIZE) + 1;
	else
		return INVALID_OFFSET;
}

void IMGClass::GetVersion2IMGDirectoryFreeSlotsCount()
{
	auto iterToMinFileOffset = std::min_element(m_IMGDirectory.begin(), m_IMGDirectory.end(),
		[](const IMGDirectoryEntryWrap &lhs, const IMGDirectoryEntryWrap &rhs){return lhs.m_RawData.m_Offset < rhs.m_RawData.m_Offset; });

	if (iterToMinFileOffset == m_IMGDirectory.end())
	{
		m_IMGHandle.resize(IMG_BLOCK_SIZE);
		m_Version2IMGDirectoryFreeSlotsCount = (IMG_BLOCK_SIZE - 8) / DIRECTORY_ENTRY_SIZE;
		return;
	}

	m_Version2IMGDirectoryFreeSlotsCount = (iterToMinFileOffset->m_RawData.m_Offset * IMG_BLOCK_SIZE - 8 - m_IMGDirectory.size() * DIRECTORY_ENTRY_SIZE) / DIRECTORY_ENTRY_SIZE;
}

void IMGClass::MakeIMGDirectoryFreeSlots()
{
	QByteArray buffer;

	auto iterToMinFileOffset = std::min_element(m_IMGDirectory.begin(), m_IMGDirectory.end(),
		[](const IMGDirectoryEntryWrap &lhs, const IMGDirectoryEntryWrap &rhs){return lhs.m_RawData.m_Offset < rhs.m_RawData.m_Offset; });

	if (iterToMinFileOffset == m_IMGDirectory.end())
		return;

	m_IMGHandle.seek(iterToMinFileOffset->m_RawData.m_Offset * IMG_BLOCK_SIZE);
	buffer = m_IMGHandle.read(iterToMinFileOffset->m_RawData.m_SizeLow16 * IMG_BLOCK_SIZE);

	iterToMinFileOffset->m_RawData.m_Offset = AlignOffsetToBlocks(m_IMGHandle.size());
	m_IMGHandle.seek(iterToMinFileOffset->m_RawData.m_Offset*IMG_BLOCK_SIZE);
	m_IMGHandle.write(buffer);

	WriteIMGDirectoryEntry(iterToMinFileOffset->m_RawData, iterToMinFileOffset - m_IMGDirectory.begin());

	m_Version2IMGDirectoryFreeSlotsCount += iterToMinFileOffset->m_RawData.m_SizeLow16 * IMG_BLOCK_SIZE / DIRECTORY_ENTRY_SIZE;

	emit IMGDirectoryChanged(m_IMGDirectory);
}

quint32 IMGClass::GetIMGDirectoryEntryIndexByName(const char *name)
{
	quint32 hash = CKeyGen::GetKeyOfUpCasedString(name);

	return std::find_if(m_IMGDirectory.begin(), m_IMGDirectory.end(),
		[hash](const IMGDirectoryEntryWrap &entrywrap){return entrywrap.m_NameHash == hash; }) - m_IMGDirectory.begin();
}

void IMGClass::WriteIMGDirectoryEntry(const IMGDirectoryEntry &item, quint32 index)
{
	if (m_IMGVersion == VERSION1)
	{
		m_DIRHandle.seek(index * DIRECTORY_ENTRY_SIZE);
		m_DIRHandle.write(reinterpret_cast<const char *>(&item), DIRECTORY_ENTRY_SIZE);
	}
	else if (m_IMGVersion == VERSION2)
	{
		m_IMGHandle.seek(8 + index * DIRECTORY_ENTRY_SIZE);
		m_IMGHandle.write(reinterpret_cast<const char *>(&item), DIRECTORY_ENTRY_SIZE);
	}
}
