#pragma execution_character_set("utf-8")

#include <QLabel>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMessageBox>
#include "myimgtool.h"
#include "IMGTableModel.h"

MYIMGTOOL::MYIMGTOOL(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    auto IMGTable = std::make_shared<std::vector<IMGClass::IMGTableItem> >();
    
	m_pIMGClassInstance.reset(new IMGClass);

	m_pTableModel.reset(new IMGTableModel);

	m_pProxyModel.reset(new QSortFilterProxyModel);
	m_pProxyModel->setFilterKeyColumn(0);
	m_pProxyModel->setSourceModel(m_pTableModel.get());
	ui.tableView->setModel(m_pProxyModel.get());

	m_StatusBarTips = new QLabel;
	m_StatusBarTips->setAlignment(Qt::AlignRight);
	m_StatusBarTips->setFixedWidth(300);
	ui.statusBar->addPermanentWidget(m_StatusBarTips);

	connect(ui.action_open, &QAction::triggered, this, &MYIMGTOOL::OpenArchiveDialog);
	connect(ui.action_close, &QAction::triggered, this, &MYIMGTOOL::CloseArchive);
	connect(ui.action_exit, &QAction::triggered, &QApplication::quit);
	//connect(ui.action_import, &QAction::triggered,this, &MYIMGTOOL::ImportFilesDialog);
	//connect(ui.action_import_folder, &QAction::triggered, this, &MYIMGTOOL::ImportFolderDialog);
	//connect(ui.action_export, &QAction::triggered, this, &MYIMGTOOL::ExportFilesDialog);
	connect(ui.action_new_ver1, &QAction::triggered, this, &MYIMGTOOL::CreateArchiveVersion1);
	connect(ui.action_new_ver2, &QAction::triggered, this, &MYIMGTOOL::CreateArchiveVersion2);
	//connect(ui.action_showinfo, &QAction::triggered, this, &MYIMGTOOL::ShowAbout);
	connect(ui.lineEdit, &QLineEdit::textEdited, this, &MYIMGTOOL::FilterTable);
	connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MYIMGTOOL::IndicateSelection);
}

void MYIMGTOOL::OpenArchive(const QString &imgpath)
{
	if (imgpath.isEmpty())
		return;

	CloseArchive();

	if (m_pIMGClassInstance->OpenArchive(imgpath))
	{
		ui.tableView->setEnabled(true);
		m_pTableModel->SetTableData(m_pIMGClassInstance->GetTableData());
		ui.action_close->setEnabled(true);
		ui.action_import->setEnabled(true);
		ui.action_import_folder->setEnabled(true);
		ui.action_rebuild->setEnabled(true);
		ui.lineEdit->setEnabled(true);
		setWindowTitle(QString("MYIMGTOOL (%1)").arg(m_pIMGClassInstance->GetArchiveFullPath()));
		m_StatusBarTips->setText(QString("共%1个项目").arg(m_pIMGClassInstance->GetFilesCount()));
	}
	else
		RaiseErrorMessage();
}

void MYIMGTOOL::OpenArchiveDialog()
{
	OpenArchive(QFileDialog::getOpenFileName(this, "选择一个IMG文件", "/", "IMG文件 (*.img)"));
}

void MYIMGTOOL::ImportFiles(const QStringList &filepaths)
{
	if (filepaths.isEmpty())
		return;

	for (auto path : filepaths)
	{
		if (!m_pIMGClassInstance->ImportFile(path))
		{
			RaiseErrorMessage();
			return;
		}
	}
}

void MYIMGTOOL::ImportFilesDialog()
{
	ImportFiles(QFileDialog::getOpenFileNames(this, "选择要导入的文件", "/", "所有文件 (*.*)"));
}

void MYIMGTOOL::CloseArchive()
{
	m_pIMGClassInstance->CloseArchive();
	ui.tableView->horizontalHeader()->setSortIndicator(4, Qt::AscendingOrder);
	m_pTableModel->SetTableData(m_pIMGClassInstance->GetTableData());
	ui.tableView->setEnabled(false);
	ui.action_close->setEnabled(false);
	ui.action_import->setEnabled(false);
	ui.action_import_folder->setEnabled(false);
	ui.action_rebuild->setEnabled(false);
	ui.lineEdit->clear();
	ui.lineEdit->setEnabled(false);
	setWindowTitle("MYIMGTOOL");
}

void MYIMGTOOL::FilterTable(const QString &keyword)
{
	ui.tableView->clearSelection();

	m_pProxyModel->setFilterFixedString(keyword);

	if (keyword.isEmpty())
		m_StatusBarTips->setText(QString("共%1个项目").arg(m_pIMGClassInstance->GetFilesCount()));
	else
		m_StatusBarTips->setText(QString("已显示%1/%2个项目").arg(m_pProxyModel->rowCount()).arg(m_pIMGClassInstance->GetFilesCount()));
}

void MYIMGTOOL::IndicateSelection()
{
	int selectedItemCount = ui.tableView->selectionModel()->selectedRows().size();

	if (selectedItemCount == 0)
		m_StatusBarTips->setText(QString("共%1个项目").arg(m_pIMGClassInstance->GetFilesCount()));
	else
		m_StatusBarTips->setText(QString("已选择%1/%2个项目").arg(selectedItemCount).arg(m_pIMGClassInstance->GetFilesCount()));
}

void MYIMGTOOL::RaiseErrorMessage()
{
	QMessageBox::critical(this, "错误", m_pIMGClassInstance->GetErrorMessage());
}

void MYIMGTOOL::ExportFiles(const QString &dest)
{
    QModelIndexList selectionIndexes = m_pProxyModel->mapSelectionToSource(ui.tableView->selectionModel()->selection()).indexes();

    if (selectionIndexes.isEmpty())
		return;

    for(auto index : selectionIndexes)
    {
        if(!m_pIMGClassInstance->ExportFile(dest, index.row()))
        {
            RaiseErrorMessage();
            return;
        }
    }
}

void MYIMGTOOL::ExportFilesDialog()
{
    ExportFiles(QFileDialog::getExistingDirectory(this, "选择导出文件存放的文件夹", "/"));
}

void MYIMGTOOL::CreateArchiveVersion1()
{
	QString path = QFileDialog::getSaveFileName(this, "选择档案存放的文件夹", "/", "IMG文件 (*.img)");

    if(path.isEmpty())
        return;

	if (!m_pIMGClassInstance->CreateArchive(path, IMGClass::ArchiveVersion::VERSION1))
		RaiseErrorMessage();

	OpenArchive(path);
}

void MYIMGTOOL::CreateArchiveVersion2()
{
	QString path = QFileDialog::getSaveFileName(this, "选择档案存放的文件夹", "/", "IMG文件 (*.img)");

	if (path.isEmpty())
		return;

	if (!m_pIMGClassInstance->CreateArchive(path, IMGClass::ArchiveVersion::VERSION2))
		RaiseErrorMessage();

	OpenArchive(path);
}

void MYIMGTOOL::ImportFolder(const QString &)
{

}
