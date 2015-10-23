#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_myimgtool.h"
#include "IMGClass.h"
#include "IMGTableModel.h"
#include <QSortFilterProxyModel>
#include <memory>

class QLabel;
class QString;
class QStringList;

class MYIMGTOOL : public QMainWindow
{
	Q_OBJECT

public:
	MYIMGTOOL(QWidget * = nullptr);

	//创建版本1档案
    void CreateArchiveVersion1();

	//创建版本2档案
    void CreateArchiveVersion2();

	//打开档案
	void OpenArchive(const QString &);

	//从对话框选择档案
	void OpenArchiveDialog();

	//导入文件
	void ImportFiles(const QStringList &);
	
	//从对话框选择导入文件
	void ImportFilesDialog();
	
	//导入文件夹
	void ImportFolder(const QString &);
    
	//从对话框选择导入文件夹
	void ImportFolderDialog();

	//导出文件
	void ExportFiles(const QString &);
    
	//从对话框选择导出文件
	void ExportFilesDialog();
	
	//重建档案
	void RebuildArchive();

	//关闭档案
	void CloseArchive();

	//显示“关于”消息框
	void ShowAbout();

	//更改关键字时更新过滤显示
	void FilterTable(const QString &);
	
	//更改选区时更新在右下角提示
	void IndicateSelection();
	
	//显示内部错误信息
	void RaiseErrorMessage();

private:
	Ui::MYIMGTOOLClass ui;
	std::unique_ptr<IMGClass> m_pIMGClassInstance;
	std::unique_ptr<IMGTableModel> m_pTableModel;
	std::unique_ptr<QSortFilterProxyModel> m_pProxyModel;
	QLabel *m_StatusBarTips;
};
