/**
 ******************************************************************************
 * @file    wizard.cpp
 * @brief   新建项目向导：分步引导创建相册项目
 ******************************************************************************
 */

#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::done(int result)
{
    if(result==QWizard::Rejected)
    {
        return QWizard::done(result);
    }
    QString path,name;
    this->ui->wizardPage1->getProSettings(path,name);
    emit sigProSetting(path,name);
    return QWizard::done(result);
}
