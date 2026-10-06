/**
 ******************************************************************************
 * @file    prosetpage.cpp
 * @brief   向导项目设置页：选择项目名和图片目录
 ******************************************************************************
 */

#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>
ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    //将这两个按键注册为向导页的字段，在向导页的其他页面里可以通过字段名来获取这两个lineedit的信息
    //设置成带*的就是这两个字段是必填的，如果没有填写会自动禁用next按键
    registerField("proPath*", this->ui->lineEdit_2);
    registerField("proName*", this->ui->lineEdit);
    //输入之后会发送ProSetPage::completeChanged信号,然后会触发iscomplete函数
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    //设置有清除按钮
    ui->lineEdit_2->setClearButtonEnabled(true);
    ui->lineEdit->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::getProSettings(QString &path, QString &name) const
{
    path=this->ui->lineEdit_2->text();
    name=this->ui->lineEdit->text();
}

bool ProSetPage::isComplete() const
{
    if(this->ui->lineEdit->text()==""||
        this->ui->lineEdit_2->text()=="")
    {
        return false;
    }
    QDir dir(this->ui->lineEdit_2->text());
    if(!dir.exists())
    {
        //项目路径不存在
        this->ui->tips->setText("this project path is no exist");
        return false;
    }
    //拼接成完整的项目的路径
    QString ab_path=dir.absoluteFilePath(this->ui->lineEdit->text());
    QDir dir_dist(ab_path);
    if(dir_dist.exists())
    {
        //如果项目已经存在
        this->ui->tips->setText("this project has already exist");
        return false;
    }
    this->ui->tips->setText("");
    return QWizardPage::isComplete();
}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("请选择要打开的文件夹");
    auto path=QDir::currentPath();
    //默认显示当前路径
    file_dialog.setDirectory(path);
    //设置视图模式为详细模式
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList file_names;
    if(file_dialog.exec())
    {
        file_names=file_dialog.selectedFiles();
    }else
    {
        return;
    }
    if(file_names.length()<=0)
    {
        return;
    }

    QString import_path= file_names.at(0);
    this->ui->lineEdit_2->setText(import_path);
}

