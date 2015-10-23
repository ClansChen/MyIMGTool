#pragma once
#include <QString>
#include <QFile>
#include <vector>
#include <initializer_list>

class QStringList;

class IMGClass
{
public:
#pragma pack(push, 1)
	struct IMGTableItem
	{
		quint32 Position;
		quint16 SizeSecondPriority;
		quint16 SizeFirstPriority = 0;
		char Name[24];
	};
#pragma pack(pop)

    enum ArchiveVersion
	{
		UNDEFINED,
		VERSION1,
		VERSION2
	};

	static const qint64 IMG_BLOCK_SIZE = 2048;
	static const qint64 MAX_FILE_SIZE = 0xFFFF * IMG_BLOCK_SIZE;
	static const qint64 MAX_ARCHIVE_SIZE = 0xFFFFFFFF * IMG_BLOCK_SIZE;
	static const qint64 TABLE_ITEM_SIZE = sizeof IMGTableItem;

    //新建档案
    bool CreateArchive(const QString &, ArchiveVersion);

    //打开版本1或版本2档案
    bool OpenArchive(const QString &);

	//关闭档案
	void CloseArchive();

	//导入一个文件
	bool ImportFile(const QString &);

	//导出指定的文件
	bool ExportFile(const QString &, quint32);

	//获取版本2档案目录的可用空间数
	quint32 GetTableFreeSlotsCount() const;

	//获取档案中文件数
	quint32 GetFilesCount() const;

	//返回当前档案的路径
	QString GetArchiveFullPath() const;

	//返回错误信息
	QString GetErrorMessage() const; 

	//返回当前档案的版本
    ArchiveVersion GetArchiveVersion() const;

	//按块对齐偏移值
	static quint32 AlignOffsetToBlocks(qint64);

	const std::vector<IMGTableItem> &GetTableData() const;

private:
	IMGClass(const IMGClass &);
	IMGClass &operator=(const IMGClass &);

	QFile m_IMGHandle;
	QFile m_DirHandle;

	//与IMGTableModel共享的档案目录数据
	std::vector<IMGTableItem> m_ArchiveTable;

	//档案版本
	ArchiveVersion m_ArchiveVersion = UNDEFINED;

	//版本2目录的未使用条目数
    quint32 m_FreeTableSlotsCount;

	//最后一次有记录错误的具体消息
	mutable QString m_ErrorMessage;

	//写入目录项到指定位置
    void WriteArchiveTableItem(const IMGTableItem &);

	//生成错误信息
	void WriteErrorMessage(std::initializer_list<QString>) const;

	//将第一个文件数据移到档案末尾以增加空闲目录条目，更新m_FreeTableSlotsCount
	void MakeArchiveTableFreeSlots();

	//更新版本2档案的文件计数
	void WriteVersion2FilesCount();
};
