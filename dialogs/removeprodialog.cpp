/**
 ******************************************************************************
 * @file    removeprodialog.cpp
 * @brief   删除项目确认对话框
 ******************************************************************************
 */

#include "removeprodialog.h"
#include "ui_removeprodialog.h"

RemoveProDialog::RemoveProDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemoveProDialog)
{
    ui->setupUi(this);
}

RemoveProDialog::~RemoveProDialog()
{
    delete ui;
}

bool RemoveProDialog::isRemove()
{
    return this->ui->checkBox->isChecked();
}
