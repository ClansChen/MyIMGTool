#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_myimgtool.h"
#include "IMGClass.h"

class IMGTableModel;
class QDir;
class QString;
class QStringList;
class QSortFilterProxyModel;
class QProgressDialog;

class MYIMGTOOL : public QMainWindow
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

		IMGDirectoryEntryWrap(const IMGDirectoryEntry &);
	};

	enum IMGVersion
	{
		UNDEFINED,
		VERSION1,
		VERSION2
	};

	static const qint64 IMG_BLOCK_SIZE = 2048;
	static const qint64 MAX_FILE_SIZE = 0xFFFF * IMG_BLOCK_SIZE;
	static const qint64 MAX_IMG_SIZE = 0xFFFFFFFF * IMG_BLOCK_SIZE;
	static const qint64 DIRECTORY_ENTRY_SIZE = 32;
	static const quint32 INVALID_OFFSET = 0xFFFFFFFF;

	MYIMGTOOL(QWidget * = nullptr);

    void CreateVersion1IMG();
    void CreateVersion2IMG();
	void OpenIMG(const QString &);
	void OpenIMGDialog();
	void CloseIMG();
	void ImportFiles(const QStringList &);
	void ImportFilesDialog();
	void ExportFiles(const QString &);
	void ExportFilesDialog();
	void ImportFolder(const QString &);
	void ImportFolderDialog();
	void RemoveFiles();
	void RebuildIMG();
	void ShowAbout();
	void ShowAboutQt();
	void FilterTable(const QString &);
	void RefreshTableView(const std::vector<IMGClass::IMGDirectoryEntryWrap> &newData);
	void SetWindowIconByIMGVersion();
	void IncProgress();

protected:
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

private:
	Ui::MYIMGTOOLClass ui;

	IMGTableModel *m_pTableModel;

	QSortFilterProxyModel *m_pProxyModel;

	QProgressDialog *m_pProgressDialog;

	IMGClass *m_pIMGClass;

	void SetIMGActionsEnable(bool);
	void MapIndexesToSource(QModelIndexList &);
	static QStringList GetFilePathListOfFolder(const QDir &);
	static void RaiseErrorMessage(std::initializer_list<QString>);
};
