/**
 ******************************************************************************
 * @file    confirmpage.cpp
 * @brief   向导确认页：汇总项目配置信息
 ******************************************************************************
 */

#include "confirmpage.h"
#include "ui_confirmpage.h"

ConfirmPage::ConfirmPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ConfirmPage)
{
    ui->setupUi(this);
}

ConfirmPage::~ConfirmPage()
{
    delete ui;
}
