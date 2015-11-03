#pragma execution_character_set("utf-8")

#include "myimgtool.h"
#include "IMGTableModel.h"
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

MYIMGTOOL::MYIMGTOOL(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowIcon(QIcon(":/MyIMGTool/Resources/myimgtool.png"));

	m_pTableModel = new IMGTableModel(this);

	m_pProxyModel = new QSortFilterProxyModel(this);
	m_pProxyModel->setFilterKeyColumn(0);
	m_pProxyModel->setSourceModel(m_pTableModel);

	ui.tableView->setModel(m_pProxyModel);
	ui.tableView->addActions({ ui.action_export, ui.action_delete });

	ui.action_new_ver1->setIcon(QIcon(":/MyIMGTool/Resources/vc.png"));
	ui.action_new_ver2->setIcon(QIcon(":/MyIMGTool/Resources/sa.png"));

	m_pProgressDialog = new QProgressDialog("", QString(), 0, 1, this, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	m_pProgressDialog->setWindowModality(Qt::WindowModal);

	m_pIMGClass = new IMGClass(this);

	connect(ui.action_new_ver1, &QAction::triggered, this, &MYIMGTOOL::CreateVersion1IMG);
	connect(ui.action_new_ver2, &QAction::triggered, this, &MYIMGTOOL::CreateVersion2IMG);
	connect(ui.action_open, &QAction::triggered, this, &MYIMGTOOL::OpenIMGDialog);
	connect(ui.action_close, &QAction::triggered, this, &MYIMGTOOL::CloseIMG);
	connect(ui.action_exit, &QAction::triggered, &QApplication::quit);
	connect(ui.action_import, &QAction::triggered,this, &MYIMGTOOL::ImportFilesDialog);
	connect(ui.action_export, &QAction::triggered, this, &MYIMGTOOL::ExportFilesDialog);
	connect(ui.action_delete, &QAction::triggered, this, &MYIMGTOOL::RemoveFiles);
	connect(ui.action_rebuild, &QAction::triggered, this, &MYIMGTOOL::RebuildIMG);
	connect(ui.action_showinfo, &QAction::triggered, this, &MYIMGTOOL::ShowAbout);
	connect(ui.action_showqtinfo, &QAction::triggered, this, &MYIMGTOOL::ShowAboutQt);

	connect(m_pIMGClass, &IMGClass::ImportingFileFullPath, m_pProgressDialog, &QProgressDialog::setLabelText);
	connect(m_pIMGClass, &IMGClass::ExportingFileName, m_pProgressDialog, &QProgressDialog::setLabelText);
	connect(m_pIMGClass, &IMGClass::RebuildingFileName, m_pProgressDialog, &QProgressDialog::setLabelText);
	connect(m_pIMGClass, &IMGClass::IncreaseProgressBar, this,&MYIMGTOOL::IncProgress);
	connect(m_pIMGClass, &IMGClass::IMGDirectoryChanged, this, &MYIMGTOOL::RefreshTableView);
	connect(m_pIMGClass, &IMGClass::ErrorOccoured, this, &MYIMGTOOL::RaiseErrorMessage);

	connect(ui.lineEdit, &QLineEdit::textChanged, this, &MYIMGTOOL::FilterTable);
}

void MYIMGTOOL::SetWindowIconByIMGVersion()
{
	switch (m_pIMGClass->GetIMGVersion())
	{
	case IMGClass::VERSION1:
		setWindowIcon(QIcon(":/MyIMGTool/Resources/vc.png"));
		break;

	case IMGClass::VERSION2:
		setWindowIcon(QIcon(":/MyIMGTool/Resources/sa.png"));
		break;

	default:
		setWindowIcon(QIcon(":/MyIMGTool/Resources/myimgtool.png"));
		break;
	}
}

void MYIMGTOOL::CreateVersion1IMG()
{
	QString imgpath = QFileDialog::getSaveFileName(this, "选择保存位置", "/", "IMG文件 (*.img)");

	if (imgpath.isEmpty())
		return;

	if (m_pIMGClass->CreateIMG(imgpath, IMGClass::VERSION1))
		OpenIMG(imgpath);
}

void MYIMGTOOL::CreateVersion2IMG()
{
	QString imgpath = QFileDialog::getSaveFileName(this, "选择保存位置", "/", "IMG文件 (*.img)");

	if (imgpath.isEmpty())
		return;

	if (m_pIMGClass->CreateIMG(imgpath, IMGClass::VERSION2))
		OpenIMG(imgpath);
}

void MYIMGTOOL::OpenIMG(const QString &imgpath)
{
	CloseIMG();

	if (m_pIMGClass->OpenIMG(imgpath))
	{
		SetIMGActionsEnable(true);
		setWindowTitle(QString("MYIMGTOOL (%1)").arg(imgpath));
		SetWindowIconByIMGVersion();
	}
}

void MYIMGTOOL::OpenIMGDialog()
{
	QString imgpath(QFileDialog::getOpenFileName(this, "选择一个IMG文件", "/", "IMG文件 (*.img)"));

	if (!imgpath.isEmpty())
		OpenIMG(imgpath);
}

void MYIMGTOOL::CloseIMG()
{
	m_pIMGClass->CloseIMG();
	ui.tableView->horizontalHeader()->setSortIndicator(4, Qt::AscendingOrder);
	SetIMGActionsEnable(false);
	SetWindowIconByIMGVersion();
	setWindowTitle("MYIMGTOOL");
}

void MYIMGTOOL::RemoveFiles()
{
	QModelIndexList indexes = ui.tableView->selectionModel()->selectedRows();

	if (indexes.isEmpty())
		return;

	MapIndexesToSource(indexes);
	
	m_pIMGClass->RemoveFiles(indexes);
}

void MYIMGTOOL::RebuildIMG()
{
	m_pProgressDialog->setWindowTitle("正在重建");

	m_pProgressDialog->setRange(0, m_pIMGClass->GetFilesCount());

	m_pProgressDialog->show();

	m_pIMGClass->RebuildIMG();

	m_pProgressDialog->hide();

	OpenIMG(m_pIMGClass->GetIMGFullPath());
}

void MYIMGTOOL::ShowAbout()
{
	QMessageBox::information(this, "测试", "测试");
}

void MYIMGTOOL::ShowAboutQt()
{
	QMessageBox::aboutQt(this, "关于Qt");
}

void MYIMGTOOL::FilterTable(const QString &keyword)
{
	ui.tableView->clearSelection();

	m_pProxyModel->setFilterRegExp(QRegExp(keyword));
}

void MYIMGTOOL::SetIMGActionsEnable(bool enable)
{
	this->setAcceptDrops(enable);
	ui.tableView->setEnabled(enable);
	ui.action_close->setEnabled(enable);
	ui.action_import->setEnabled(enable);
	ui.action_import_folder->setEnabled(enable);
	ui.action_export->setEnabled(enable);
	ui.action_delete->setEnabled(enable);
	ui.action_rebuild->setEnabled(enable);
	ui.lineEdit->clear();
	ui.lineEdit->setEnabled(enable);
}

void MYIMGTOOL::RaiseErrorMessage(std::initializer_list<QString> args)
{
	QString message;

	for (auto &substr : args)
		message += substr;

	QMessageBox::critical(nullptr, "错误", message);
}


void MYIMGTOOL::MapIndexesToSource(QModelIndexList &indexes)
{
	for (auto &index:indexes)
		index = m_pProxyModel->mapToSource(index);
}

void MYIMGTOOL::RefreshTableView(const std::vector<IMGClass::IMGDirectoryEntryWrap> &newData)
{
	dynamic_cast<IMGTableModel *>(m_pProxyModel->sourceModel())->SetSourceData(newData);
	ui.tableView->repaint();
}

void MYIMGTOOL::ImportFiles(const QStringList &paths)
{
	if (paths.isEmpty())
		return;

	m_pProgressDialog->setWindowTitle("正在导入");

	m_pProgressDialog->setRange(0, paths.size());

	m_pProgressDialog->show();

	m_pIMGClass->ImportFiles(paths);

	m_pProgressDialog->hide();
}

void MYIMGTOOL::ImportFilesDialog()
{
	ImportFiles(QFileDialog::getOpenFileNames(this, "选择要导入的文件", "/", "所有文件 (*.*)"));
}

QStringList MYIMGTOOL::GetFilePathListOfFolder(const QDir &dir)
{
	QStringList result;

	for (auto &subfile : dir.entryInfoList(QDir::Files))
		result += subfile.absoluteFilePath();

	return result;
}

void MYIMGTOOL::ExportFiles(const QString &dest)
{
	QModelIndexList indexes = ui.tableView->selectionModel()->selectedRows();
	
	if (dest.isEmpty() || indexes.isEmpty())
		return;

	MapIndexesToSource(indexes);

	m_pProgressDialog->setWindowTitle("正在导出");

	m_pProgressDialog->setRange(0, indexes.size());

	m_pProgressDialog->show();

	m_pIMGClass->ExportFiles(dest, indexes);

	m_pProgressDialog->hide();
}

void MYIMGTOOL::ExportFilesDialog()
{
	ExportFiles(QFileDialog::getExistingDirectory(this, "选择导出文件存放的文件夹", "/"));
}

void MYIMGTOOL::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void MYIMGTOOL::dropEvent(QDropEvent *event)
{
	QStringList paths;
	QString path;
	QDir testIfDirectory;
	QList<QUrl> urls = event->mimeData()->urls();

	if (urls.isEmpty())
		return;

	for (auto &url : urls)
	{
		path = url.toLocalFile();

		if (testIfDirectory.cd(path))
			paths += GetFilePathListOfFolder(testIfDirectory);
		else
			paths.push_back(path);
	}

	if (paths.isEmpty())
		RaiseErrorMessage({ "没有文件可以导入" });

	ImportFiles(paths);
}

void MYIMGTOOL::IncProgress()
{
	m_pProgressDialog->setValue(m_pProgressDialog->value() + 1);
}
