#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_myimgtool.h"

class IMGClass;
class IMGTableModel;
class QDir;
class QString;
class QStringList;
class QSortFilterProxyModel;
class QProgressDialog;
class QLable;

class MYIMGTOOL : public QMainWindow
{
	Q_OBJECT

public:
	MYIMGTOOL(QWidget *parent = nullptr);

	void CreateVersion1IMG();
	void CreateVersion2IMG();
	void OpenIMG(const QString &);
	void OpenIMGDialog();
	void CloseIMG();
	void ImportFiles(const QStringList &);
	void ImportFilesDialog();
	void ExportFiles(const QString &);
	void ExportFilesDialog();
	void ImportFolderDialog();
	void RemoveFiles();
	void RebuildIMG();

	void ShowAboutQt();

	void RefreshTableView();
	void FilterTable(const QString &);
	void SetWindowIconByIMGVersion();
	void IncProgressBar();

	void UpdateStatusBarTips();

protected:
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

private:
	Ui::MYIMGTOOLClass ui;

	IMGTableModel *m_pTableModel;

	QSortFilterProxyModel *m_pProxyModel;

	QProgressDialog *m_pProgressDialog;

	IMGClass *m_pIMGClass;

	QLabel *m_pStatusBarTips;

	void SetIMGActionsEnable(bool);
	void MapIndexesToSource(QModelIndexList &);
	static QStringList GetFilePathListOfFolder(const QDir &);
	static void RaiseErrorMessage(std::initializer_list<QString>);
};
