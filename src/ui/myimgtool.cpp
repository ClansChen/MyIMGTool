#pragma execution_character_set("utf-8")

#include "myimgtool.h"
#include "../img/IMGClass.h"
#include "IMGTableModel.h"
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include "MyProgressDialog.h"
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

MYIMGTOOL::MYIMGTOOL(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    setWindowIcon(QIcon(":/icons/icon.png"));
    ui.action_new_ver1->setIcon(QIcon(":/icons/vc.png"));
    ui.action_new_ver2->setIcon(QIcon(":/icons/sa.png"));

	m_pIMGClass = new IMGClass(this);
	m_pTableModel = new IMGTableModel(this);

	m_pTableModel->setDataSource(&m_pIMGClass->GetDirectory());

	m_pProxyModel = new QSortFilterProxyModel(this);
	m_pProxyModel->setFilterKeyColumn(0);
	m_pProxyModel->setSourceModel(m_pTableModel);

	ui.tableView->setModel(m_pProxyModel);
	ui.tableView->addActions({ ui.action_export, ui.action_delete });

	m_pProgressDialog = new MyProgressDialog("", QString(), 0, 1, this, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	setWindowModality(Qt::WindowModal);
	m_pProgressDialog->close();

	m_pStatusBarTips = new QLabel(this);
	m_pStatusBarTips->setFixedWidth(300);
	m_pStatusBarTips->setAlignment(Qt::AlignRight);
	statusBar()->addPermanentWidget(m_pStatusBarTips);

	connect(ui.action_new_ver1, &QAction::triggered, this,&MYIMGTOOL::CreateVersion1IMG);
	connect(ui.action_new_ver2, &QAction::triggered, this, &MYIMGTOOL::CreateVersion2IMG);
	connect(ui.action_open, &QAction::triggered, this, &MYIMGTOOL::OpenIMGDialog);
	connect(ui.action_close, &QAction::triggered, this, &MYIMGTOOL::CloseIMG);
	connect(ui.action_exit, &QAction::triggered, this, &QApplication::quit);
	connect(ui.action_import, &QAction::triggered, this, &MYIMGTOOL::ImportFilesDialog);
	connect(ui.action_import_folder, &QAction::triggered, this, &MYIMGTOOL::ImportFolderDialog);
	connect(ui.action_export, &QAction::triggered, this, &MYIMGTOOL::ExportFilesDialog);
	connect(ui.action_delete, &QAction::triggered, this, &MYIMGTOOL::RemoveFiles);
	connect(ui.action_rebuild, &QAction::triggered, this, &MYIMGTOOL::RebuildIMG);
	connect(ui.action_showqtinfo, &QAction::triggered, this, &MYIMGTOOL::ShowAboutQt);

	connect(m_pIMGClass, &IMGClass::ImportingFileFullPath, m_pProgressDialog, &MyProgressDialog::setLabelText);
	connect(m_pIMGClass, &IMGClass::ExportingFileName, m_pProgressDialog,  &MyProgressDialog::setLabelText);
	connect(m_pIMGClass, &IMGClass::RebuildingFileName, m_pProgressDialog,  &MyProgressDialog::setLabelText);

    connect(m_pIMGClass, &IMGClass::IMGDirectoryChanged, this, &MYIMGTOOL::RefreshTableView);
	connect(m_pIMGClass, &IMGClass::ErrorOccoured, this, &MYIMGTOOL::RaiseErrorMessage);
	connect(m_pIMGClass, &IMGClass::IncreaseProgressBar, this, &MYIMGTOOL::IncProgressBar);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &MYIMGTOOL::FilterTable);

	connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MYIMGTOOL::UpdateStatusBarTips);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &MYIMGTOOL::UpdateStatusBarTips);
}

void MYIMGTOOL::SetWindowIconByIMGVersion()
{
	switch (m_pIMGClass->GetIMGVersion())
	{
	case IMGClass::IMGVersion::VERSION1:
        setWindowIcon(QIcon(":/icons/vc.png"));
		break;

	case IMGClass::IMGVersion::VERSION2:
        setWindowIcon(QIcon(":/icons/sa.png"));
		break;

	default:
        setWindowIcon(QIcon(":/icons/myimgtool.png"));
		break;
	}
}

void MYIMGTOOL::CreateVersion1IMG()
{
	QString imgpath = QFileDialog::getSaveFileName(this, "选择保存位置", "/", "IMG文件 (*.img)");

	if (imgpath.isEmpty())
		return;

	if (m_pIMGClass->CreateIMG(imgpath, IMGClass::IMGVersion::VERSION1))
		OpenIMG(imgpath);
}

void MYIMGTOOL::CreateVersion2IMG()
{
	QString imgpath = QFileDialog::getSaveFileName(this, "选择保存位置", "/", "IMG文件 (*.img)");

	if (imgpath.isEmpty())
		return;

	if (m_pIMGClass->CreateIMG(imgpath, IMGClass::IMGVersion::VERSION2))
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
		UpdateStatusBarTips();
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
	UpdateStatusBarTips();
}

void MYIMGTOOL::ImportFiles(const QStringList &paths)
{
	if (paths.isEmpty())
		return;

	m_pProgressDialog->setWindowTitle("正在导入");

	m_pProgressDialog->setRange(0, paths.size());

	m_pProgressDialog->setValue(0);

	m_pProgressDialog->open();

	m_pIMGClass->ImportFiles(paths);

	m_pProgressDialog->close();
}

void MYIMGTOOL::ImportFilesDialog()
{
	ImportFiles(QFileDialog::getOpenFileNames(this, "选择要导入的文件", "/", "所有文件 (*.*)"));
}

void MYIMGTOOL::ImportFolderDialog()
{
	QString folder = QFileDialog::getExistingDirectory(this, "选择要导入的文件夹", "/");

	if (folder.isEmpty())
		return;

	QStringList paths = GetFilePathListOfFolder(folder);

	if (paths.isEmpty())
	{
		RaiseErrorMessage({ "没有文件可以导入" });
		return;
	}

	ImportFiles(paths);
}

void MYIMGTOOL::ExportFiles(const QString &dest)
{
	QModelIndexList indexes = ui.tableView->selectionModel()->selectedRows();

	if (dest.isEmpty() || indexes.isEmpty())
		return;

	MapIndexesToSource(indexes);

	m_pProgressDialog->setWindowTitle("正在导出");

	m_pProgressDialog->setRange(0, indexes.size());

	m_pProgressDialog->setValue(0);

	m_pProgressDialog->open();

	m_pIMGClass->ExportFiles(dest, indexes);

	m_pProgressDialog->close();
}

void MYIMGTOOL::ExportFilesDialog()
{
	ExportFiles(QFileDialog::getExistingDirectory(this, "选择导出文件存放的文件夹", "/"));
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

	m_pProgressDialog->open();

	m_pIMGClass->RebuildIMG();

	m_pProgressDialog->close();

	OpenIMG(m_pIMGClass->GetIMGFullPath());
}

void MYIMGTOOL::ShowAboutQt()
{
	QMessageBox::aboutQt(this, "关于Qt");
}

void MYIMGTOOL::RefreshTableView()
{

    dynamic_cast<IMGTableModel *>(m_pProxyModel->sourceModel())->RefreshView();
    ui.tableView->repaint();
}

void MYIMGTOOL::FilterTable(const QString &keyword)
{
	ui.tableView->clearSelection();

	m_pProxyModel->setFilterRegExp(QRegExp(keyword, Qt::CaseInsensitive));
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

QStringList MYIMGTOOL::GetFilePathListOfFolder(const QDir &dir)
{
	QStringList result;

	for (auto &subfile : dir.entryInfoList(QDir::Files))
		result += subfile.absoluteFilePath();

	return result;
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
	{
		RaiseErrorMessage({ "没有文件可以导入" });
		return;
	}

	ImportFiles(paths);
}

void MYIMGTOOL::IncProgressBar()
{
	m_pProgressDialog->setValue(m_pProgressDialog->value() + 1);
}

void MYIMGTOOL::UpdateStatusBarTips()
{
	if (m_pIMGClass->GetIMGVersion() == IMGClass::IMGVersion::UNDEFINED || m_pProxyModel->rowCount() == 0)
	{
		m_pStatusBarTips->clear();
		return;
	}

	int selected_count = ui.tableView->selectionModel()->selectedRows().size();

	if (selected_count == 0)
		m_pStatusBarTips->setText(QString("共%1个项目").arg(QString::number(m_pProxyModel->rowCount())));
	else
		m_pStatusBarTips->setText(QString("已选择%1个项目，共%2个").arg(QString::number(selected_count), QString::number(m_pProxyModel->rowCount())));
}
