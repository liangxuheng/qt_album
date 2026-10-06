/**
 ******************************************************************************
 * @file    protree.cpp
 * @brief   项目树容器：组合 ProTreeWidget 和预览列表
 ******************************************************************************
 */

#include "protree.h"
#include "ui_protree.h"

ProTree::ProTree(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProTree)
{
    ui->setupUi(this);
    this->setMaximumWidth(378);
    this->setMinimumWidth(378);
}

ProTree::~ProTree()
{
    delete ui;
}

ProTreeWidget *ProTree::GetTreeWidget()
{
    return this->ui->treeView;
}

void ProTree::addProToTree(const QString &path,const QString &name)
{
    this->ui->treeView->addProToTree(path,name);
}
