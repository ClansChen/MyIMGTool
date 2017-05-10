/********************************************************************************
** Form generated from reading UI file 'MyIMGTool.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYIMGTOOL_H
#define UI_MYIMGTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MYIMGTOOLClass
{
public:
    QAction *action_open;
    QAction *action_close;
    QAction *action_exit;
    QAction *action_import;
    QAction *action_rebuild;
    QAction *action_import_folder;
    QAction *action_showqtinfo;
    QAction *action_export;
    QAction *action_new_ver1;
    QAction *action_new_ver2;
    QAction *action_delete;
    QAction *action_showinfo;
    QAction *action_recentlist;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QMenu *menu_file;
    QMenu *menuNew;
    QMenu *menu_operations;
    QMenu *menu_misc;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MYIMGTOOLClass)
    {
        if (MYIMGTOOLClass->objectName().isEmpty())
            MYIMGTOOLClass->setObjectName(QStringLiteral("MYIMGTOOLClass"));
        MYIMGTOOLClass->setWindowModality(Qt::NonModal);
        MYIMGTOOLClass->setEnabled(true);
        MYIMGTOOLClass->resize(747, 566);
        MYIMGTOOLClass->setAcceptDrops(false);
        action_open = new QAction(MYIMGTOOLClass);
        action_open->setObjectName(QStringLiteral("action_open"));
        action_open->setEnabled(true);
        action_close = new QAction(MYIMGTOOLClass);
        action_close->setObjectName(QStringLiteral("action_close"));
        action_close->setEnabled(false);
        action_exit = new QAction(MYIMGTOOLClass);
        action_exit->setObjectName(QStringLiteral("action_exit"));
        action_import = new QAction(MYIMGTOOLClass);
        action_import->setObjectName(QStringLiteral("action_import"));
        action_import->setEnabled(false);
        action_rebuild = new QAction(MYIMGTOOLClass);
        action_rebuild->setObjectName(QStringLiteral("action_rebuild"));
        action_rebuild->setEnabled(false);
        action_import_folder = new QAction(MYIMGTOOLClass);
        action_import_folder->setObjectName(QStringLiteral("action_import_folder"));
        action_import_folder->setEnabled(false);
        action_showqtinfo = new QAction(MYIMGTOOLClass);
        action_showqtinfo->setObjectName(QStringLiteral("action_showqtinfo"));
        action_export = new QAction(MYIMGTOOLClass);
        action_export->setObjectName(QStringLiteral("action_export"));
        action_export->setEnabled(false);
        action_new_ver1 = new QAction(MYIMGTOOLClass);
        action_new_ver1->setObjectName(QStringLiteral("action_new_ver1"));
        action_new_ver2 = new QAction(MYIMGTOOLClass);
        action_new_ver2->setObjectName(QStringLiteral("action_new_ver2"));
        action_delete = new QAction(MYIMGTOOLClass);
        action_delete->setObjectName(QStringLiteral("action_delete"));
        action_delete->setEnabled(false);
        action_showinfo = new QAction(MYIMGTOOLClass);
        action_showinfo->setObjectName(QStringLiteral("action_showinfo"));
        action_recentlist = new QAction(MYIMGTOOLClass);
        action_recentlist->setObjectName(QStringLiteral("action_recentlist"));
        centralWidget = new QWidget(MYIMGTOOLClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setEnabled(false);
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(10);
        tableView->setFont(font);
        tableView->setFocusPolicy(Qt::NoFocus);
        tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
        tableView->setAcceptDrops(false);
        tableView->setStyleSheet(QLatin1String("selection-color: rgb(255, 255, 255);\n"
"selection-background-color: rgb(0, 170, 255);"));
        tableView->setMidLineWidth(0);
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
        tableView->setDragEnabled(true);
        tableView->setDragDropOverwriteMode(false);
        tableView->setDragDropMode(QAbstractItemView::DragDrop);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setTextElideMode(Qt::ElideMiddle);
        tableView->setShowGrid(true);
        tableView->setGridStyle(Qt::SolidLine);
        tableView->setSortingEnabled(true);
        tableView->setWordWrap(false);
        tableView->setCornerButtonEnabled(false);
        tableView->horizontalHeader()->setDefaultSectionSize(180);
        tableView->horizontalHeader()->setHighlightSections(false);
        tableView->horizontalHeader()->setMinimumSectionSize(60);
        tableView->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(20);
        tableView->verticalHeader()->setHighlightSections(false);
        tableView->verticalHeader()->setMinimumSectionSize(20);
        tableView->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableView->verticalHeader()->setStretchLastSection(false);

        verticalLayout_2->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(false);
        lineEdit->setAcceptDrops(false);

        horizontalLayout->addWidget(lineEdit);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(verticalLayout_2);

        MYIMGTOOLClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MYIMGTOOLClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 747, 23));
        menu_file = new QMenu(menuBar);
        menu_file->setObjectName(QStringLiteral("menu_file"));
        menuNew = new QMenu(menu_file);
        menuNew->setObjectName(QStringLiteral("menuNew"));
        menu_operations = new QMenu(menuBar);
        menu_operations->setObjectName(QStringLiteral("menu_operations"));
        menu_misc = new QMenu(menuBar);
        menu_misc->setObjectName(QStringLiteral("menu_misc"));
        MYIMGTOOLClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(MYIMGTOOLClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MYIMGTOOLClass->setStatusBar(statusBar);

        menuBar->addAction(menu_file->menuAction());
        menuBar->addAction(menu_operations->menuAction());
        menuBar->addAction(menu_misc->menuAction());
        menu_file->addAction(menuNew->menuAction());
        menu_file->addAction(action_open);
        menu_file->addAction(action_close);
        menu_file->addAction(action_exit);
        menuNew->addAction(action_new_ver1);
        menuNew->addAction(action_new_ver2);
        menu_operations->addAction(action_import);
        menu_operations->addAction(action_import_folder);
        menu_operations->addAction(action_export);
        menu_operations->addAction(action_delete);
        menu_operations->addAction(action_rebuild);
        menu_misc->addAction(action_showqtinfo);

        retranslateUi(MYIMGTOOLClass);

        QMetaObject::connectSlotsByName(MYIMGTOOLClass);
    } // setupUi

    void retranslateUi(QMainWindow *MYIMGTOOLClass)
    {
        MYIMGTOOLClass->setWindowTitle(QApplication::translate("MYIMGTOOLClass", "MYIMGTOOL", Q_NULLPTR));
        action_open->setText(QApplication::translate("MYIMGTOOLClass", "\346\211\223\345\274\200IMG...(&O)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_open->setToolTip(QApplication::translate("MYIMGTOOLClass", "\346\211\223\345\274\200IMG\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_open->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\346\211\223\345\274\200\344\270\200\344\270\252IMG\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_open->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+O", Q_NULLPTR));
        action_close->setText(QApplication::translate("MYIMGTOOLClass", "\345\205\263\351\227\255IMG(&C)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_close->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\205\263\351\227\255\345\275\223\345\211\215\346\211\223\345\274\200\347\232\204IMG", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_close->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\345\205\263\351\227\255\345\275\223\345\211\215\346\211\223\345\274\200\347\232\204\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_exit->setText(QApplication::translate("MYIMGTOOLClass", "\351\200\200\345\207\272\347\250\213\345\272\217(&Q)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_exit->setToolTip(QApplication::translate("MYIMGTOOLClass", "\351\200\200\345\207\272\347\250\213\345\272\217", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_exit->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\351\200\200\345\207\272\347\250\213\345\272\217", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_exit->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+Q", Q_NULLPTR));
        action_import->setText(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\205\245\346\226\207\344\273\266...(&I)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_import->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\205\245\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_import->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\205\245\344\270\200\344\270\252\346\210\226\345\244\232\344\270\252\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_import->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+I", Q_NULLPTR));
        action_rebuild->setText(QApplication::translate("MYIMGTOOLClass", "\351\207\215\345\273\272IMG(&R)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_rebuild->setToolTip(QApplication::translate("MYIMGTOOLClass", "\351\207\215\345\273\272IMG", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_rebuild->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\351\207\215\345\273\272IMG\344\273\245\345\216\273\351\231\244\346\227\240\347\224\250\347\251\272\351\227\264(\351\234\200\350\246\201\344\270\200\346\256\265\346\227\266\351\227\264)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_rebuild->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+R", Q_NULLPTR));
        action_import_folder->setText(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\205\245\346\226\207\344\273\266\345\244\271...(&F)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_import_folder->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\205\245\346\226\207\344\273\266\345\244\271", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_import_folder->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\205\245\346\226\207\344\273\266\345\244\271\345\206\205\347\232\204\346\211\200\346\234\211\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_import_folder->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+F", Q_NULLPTR));
        action_showqtinfo->setText(QApplication::translate("MYIMGTOOLClass", "\345\205\263\344\272\216Qt...(&Q)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_showqtinfo->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\205\263\344\272\216Qt", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_showqtinfo->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\346\230\276\347\244\272\346\234\254\345\267\245\345\205\267\347\232\204\344\277\241\346\201\257", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        action_export->setText(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\207\272\346\211\200\351\200\211\346\226\207\344\273\266...(&E)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_export->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\257\274\345\207\272\346\211\200\351\200\211\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_export->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+E", Q_NULLPTR));
        action_new_ver1->setText(QApplication::translate("MYIMGTOOLClass", "\347\275\252\346\201\266\351\203\275\345\270\202IMG...(&1)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_new_ver1->setToolTip(QApplication::translate("MYIMGTOOLClass", "\346\226\260\345\273\272\347\211\210\346\234\2541\346\241\243\346\241\210", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_new_ver2->setText(QApplication::translate("MYIMGTOOLClass", "\345\234\243\345\256\211\345\234\260\345\210\227\346\226\257IMG...(&2)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_new_ver2->setToolTip(QApplication::translate("MYIMGTOOLClass", "\346\226\260\345\273\272\347\211\210\346\234\2542\346\241\243\346\241\210", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_delete->setText(QApplication::translate("MYIMGTOOLClass", "\345\210\240\351\231\244\351\200\211\344\270\255\347\232\204\346\226\207\344\273\266(&D)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_delete->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\210\240\351\231\244\351\200\211\344\270\255\347\232\204\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_delete->setShortcut(QApplication::translate("MYIMGTOOLClass", "Ctrl+D", Q_NULLPTR));
        action_showinfo->setText(QApplication::translate("MYIMGTOOLClass", "\345\205\263\344\272\216\346\234\254\347\250\213\345\272\217...(&A)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_showinfo->setToolTip(QApplication::translate("MYIMGTOOLClass", "\345\205\263\344\272\216\346\234\254\347\250\213\345\272\217", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_recentlist->setText(QApplication::translate("MYIMGTOOLClass", "\346\234\200\350\277\221\347\232\204\346\226\207\344\273\266(&R)", Q_NULLPTR));
        action_recentlist->setIconText(QApplication::translate("MYIMGTOOLClass", "\346\234\200\350\277\221\346\211\223\345\274\200\347\232\204\346\226\207\344\273\266\345\210\227\350\241\250", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_recentlist->setToolTip(QApplication::translate("MYIMGTOOLClass", "\346\234\200\350\277\221\347\232\204\346\226\207\344\273\266(R)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_recentlist->setStatusTip(QApplication::translate("MYIMGTOOLClass", "\346\234\200\350\277\221\346\211\223\345\274\200\347\232\204\346\226\207\344\273\266\345\210\227\350\241\250", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("MYIMGTOOLClass", "\346\220\234\347\264\242\345\205\263\351\224\256\345\255\227", Q_NULLPTR));
        menu_file->setTitle(QApplication::translate("MYIMGTOOLClass", "\346\226\207\344\273\266(&F)", Q_NULLPTR));
        menuNew->setTitle(QApplication::translate("MYIMGTOOLClass", "\346\226\260\345\273\272(&N)", Q_NULLPTR));
        menu_operations->setTitle(QApplication::translate("MYIMGTOOLClass", "\346\223\215\344\275\234(&A)", Q_NULLPTR));
        menu_misc->setTitle(QApplication::translate("MYIMGTOOLClass", "\346\235\202\351\241\271(&M)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MYIMGTOOLClass: public Ui_MYIMGTOOLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYIMGTOOL_H
