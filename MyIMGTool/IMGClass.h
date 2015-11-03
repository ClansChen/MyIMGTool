#pragma once
#include <QFile>
#include <vector>
#include <QModelIndexList>
#include <initializer_list>

class IMGClass :public QObject
{
	Q_OBJECT

public:

#pragma pack(push, 1)
	struct IMGDirectoryEntry
	{
		quint32 m_Offset;
		quint16 m_SizeLow16;
		quint16 m_SizeHigh16 = 0;
		char m_Name[24];
	};
#pragma pack(pop)

	struct IMGDirectoryEntryWrap
	{
		IMGDirectoryEntry m_RawData;
		quint32 m_NameHash;
		bool m_ImportedRecently = false;

		IMGDirectoryEntryWrap(const IMGDirectoryEntry &raw);
	};

	enum IMGVersion
	{
		UNDEFINED,
		VERSION1,
		VERSION2
	};

	enum ErrorType
	{
		NO_ERROR,
		OPEN_ERROR,
		CREATE_ERROR,
		INVALID_SIZE,
		REBUILD_CREATE_ERROR
	};

	static const qint64 IMG_BLOCK_SIZE = 2048;
	static const qint64 MAX_FILE_SIZE = 0xFFFF * IMG_BLOCK_SIZE;
	static const qint64 MAX_IMG_SIZE = 0xFFFFFFFF * IMG_BLOCK_SIZE;
	static const qint64 DIRECTORY_ENTRY_SIZE = 32;
	static const quint32 INVALID_OFFSET = 0xFFFFFFFF;

	IMGClass(QObject *parent = nullptr);

	bool CreateIMG(const QString &imgpath, IMGVersion version);
	bool OpenIMG(const QString &imgpath);
	void CloseIMG();
	void ImportFiles(const QStringList &paths);
	void ExportFiles(const QString &dest, const QModelIndexList &indexes);
	void RemoveFiles(const QModelIndexList &indexes);
	void RebuildIMG();

	IMGVersion	GetIMGVersion(){ return m_IMGVersion; }
	quint32		GetFilesCount(){ return m_IMGDirectory.size(); }
	QString		GetIMGFullPath(){ return m_IMGHandle.fileName(); }

private:
	QFile m_IMGHandle;
	QFile m_DIRHandle;

	std::vector<IMGDirectoryEntryWrap> m_IMGDirectory;

	IMGVersion m_IMGVersion = UNDEFINED;

	quint32 m_Version2IMGDirectoryFreeSlotsCount;

	static quint32 AlignOffsetToBlocks(qint64 offset);
	quint32 GetFirstWritingBlockForRebuilding();
	void WriteVersion1IMGDirectory(QFile &file);
	void WriteVersion2IMGDirectory(QFile &file);
	void WriteIMGDirectory();
	void GetVersion2IMGDirectoryFreeSlotsCount();
	void MakeIMGDirectoryFreeSlots();
	quint32 GetIMGDirectoryEntryIndexByName(const char *name);
	void WriteIMGDirectoryEntry(const IMGDirectoryEntry &data, quint32 index);

signals:
	void IncreaseProgressBar();
	void ImportingFileFullPath(const QString &path);
	void ExportingFileName(const QString &name);
	void RebuildingFileName(const QString &name);
	void IMGDirectoryChanged(const std::vector<IMGDirectoryEntryWrap> &newData);
	void ErrorOccoured(std::initializer_list<QString> args);
};
