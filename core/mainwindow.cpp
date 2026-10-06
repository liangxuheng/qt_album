/**
 ******************************************************************************
 * @file    mainwindow.cpp
 * @brief   主窗口：左侧项目树 + 右侧图片显示，信号槽连接
 ******************************************************************************
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include "wizard.h"
#include "protree.h"
#include "ProTreeWidget.h"
#include <QFileDialog>
#include "picshow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629, 869);
    ui->setupUi(this);
    QMenu *menu_file = menuBar()->addMenu(tr("文件(&F)"));

    auto *action_create = new QAction(QIcon(":/res/create_pro.png")
                                      , tr("创建项目"), this);
    action_create->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(action_create);

    auto *action_open = new QAction(QIcon(":/res/open_pro.png")
                                    , tr("打开项目"), this);
    action_open->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(action_open);

    QMenu *menu_set = menuBar()->addMenu(tr("设置(&S)"));
    QAction *act_music = new QAction(QIcon(":/res/back_music.png"),
                                     tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(act_music);

    connect(action_create, &QAction::triggered, this, &MainWindow::onCreatePro);
    connect(action_open, &QAction::triggered, this, &MainWindow::onOpenPro);

    this->_proTree = new ProTree();
    this->ui->prolayout->addWidget(this->_proTree);

    auto *pro_tree_widget = this->_proTree->GetTreeWidget();
    connect(this, &MainWindow::sigOpenPro, pro_tree_widget, &ProTreeWidget::onOpenPro);

    this->_picshow = new PicShow();
    this->ui->piclayout->addWidget(this->_picshow);

    connect(pro_tree_widget, &ProTreeWidget::sigUpdateSelected,
            this->_picshow, &PicShow::onSelectItem);

    connect(this->_picshow, &PicShow::sigNextBtn, pro_tree_widget,
            &ProTreeWidget::onNextBtn);
    connect(this->_picshow, &PicShow::sigPreBtn, pro_tree_widget,
            &ProTreeWidget::onPreBtn);

    connect(pro_tree_widget, &ProTreeWidget::sigUpdatePic,
            this->_picshow, &PicShow::onUpdatePic);

    connect(pro_tree_widget, &ProTreeWidget::sigClearSelected, this->_picshow,
            &PicShow::onDeleteItem);

    connect(act_music, &QAction::triggered,
            pro_tree_widget, &ProTreeWidget::onSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (this->_proTree)
    {
        delete this->_proTree;
        this->_proTree = nullptr;
    }
}

/**
 * @brief  创建项目：弹出Wizard对话框
 * @param  checked  按钮状态
 * @retval None
 */
void MainWindow::onCreatePro(bool)
{
    qDebug() << "create pro \r\n";
    Wizard wizard(this);
    wizard.setWindowTitle("创建项目");
    auto *page = wizard.page(0);
    page->setTitle("设置项目配置");
    connect(&wizard, &Wizard::sigProSetting,
            this->_proTree, &ProTree::addProToTree);
    wizard.show();
    wizard.exec();
}

/**
 * @brief  打开已有项目：弹出目录选择对话框
 * @retval None
 */
void MainWindow::onOpenPro()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec())
    {
        fileNames = file_dialog.selectedFiles();
    }

    if (fileNames.length() <= 0)
    {
        return;
    }

    QString import_path = fileNames.at(0);
    qDebug() << "import_path is " << import_path << "\r\n";
    emit sigOpenPro(import_path);
}

/**
 * @brief  窗口大小改变：自适应调整左右分栏
 * @param  event  大小事件
 * @retval None
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->_picshow->Reload();
    return QMainWindow::resizeEvent(event);
}
