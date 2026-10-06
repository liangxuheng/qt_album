/**
 ******************************************************************************
 * @file    picshow.cpp
 * @brief   图片显示控件：左右切换、缩放、幻灯片播放
 ******************************************************************************
 */

#include "picshow.h"
#include "ui_picshow.h"
#include <QGraphicsOpacityEffect>

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
    ,is_visible(false)
{
    ui->setupUi(this);
    this->ui->previousBtn->SetIcons(":/res/previous.png"
                                    ,":/res/previous_hover.png"
                                    ,":/res/previous_press.png");
    this->ui->nextBtn->SetIcons(":/res/next.png"
                                ,":/res/next_hover.png"
                                ,":/res/next_press.png");

    QGraphicsOpacityEffect * opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    this->ui->previousBtn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect * opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    this->ui->nextBtn->setGraphicsEffect(opacity_next);

    this->_animation_show_pre = new QPropertyAnimation(opacity_pre,"opacity",                                           this);
    this->_animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    this->_animation_show_pre->setDuration(500);

    this->_animation_show_next = new QPropertyAnimation(opacity_next,"opacity",                                             this);
    this->_animation_show_next->setEasingCurve(QEasingCurve::Linear);
    this->_animation_show_next->setDuration(500);

    connect(this->ui->nextBtn,&QPushButton::clicked,this,&PicShow::sigNextBtn);
    connect(this->ui->previousBtn,&QPushButton::clicked,this,&PicShow::sigPreBtn);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::Reload()
{
    if(this->_select_path != ""){
        const auto &width = this->ui->gridLayout->geometry().width();
        const auto &height = this->ui->gridLayout->geometry().height();
        this->_pix_map.load(this->_select_path);

        this->_pix_map = this->_pix_map.scaled(width,height,Qt::KeepAspectRatio);
        this->ui->label->setPixmap(this->_pix_map);
    }
}

bool PicShow::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Enter:{
        ShowPreNextBtns(true);
        break;
    }
    case QEvent::Leave:
    {
        ShowPreNextBtns(false);
        break;
    }
    default:
    {
        break;
    }
    }

    return QDialog::event(e);
}

void PicShow::ShowPreNextBtns(bool is_on)
{
    if(this->is_visible&&is_on==false)
    {
        //之前可见现在变成不可见
        this->_animation_show_pre->stop();
        this->_animation_show_pre->setStartValue(1);
        this->_animation_show_pre->setEndValue(0);
        this->_animation_show_pre->start();

        this->_animation_show_next->stop();
        this->_animation_show_next->setStartValue(1);
        this->_animation_show_next->setEndValue(0);
        this->_animation_show_next->start();

        this->is_visible=!this->is_visible;
        return;
    }

    if(this->is_visible==false&&is_on)
    {
        //之前不可见现在变成可见：显示上一张/下一张按钮，透明度从 0 渐变到 1
        this->_animation_show_pre->stop();
        this->_animation_show_pre->setStartValue(0);
        this->_animation_show_pre->setEndValue(1);
        this->_animation_show_pre->start();

        this->_animation_show_next->stop();
        this->_animation_show_next->setStartValue(0);
        this->_animation_show_next->setEndValue(1);
        this->_animation_show_next->start();

        this->is_visible=!this->is_visible;
        return;
    }
}

void PicShow::onSelectItem(const QString &path)
{
    //记录用户在列表选中的图片路径，并重新加载预览
    this->_select_path=path;
    this->Reload();
}

void PicShow::onUpdatePic(const QString &path)
{
    //更新当前展示的图片：按网格区域尺寸等比缩放后显示
    this->_select_path=path;
    if(this->_select_path != ""){
        const auto &width = this->ui->gridLayout->geometry().width();
        const auto &height = this->ui->gridLayout->geometry().height();
        this->_pix_map.load(this->_select_path);

        this->_pix_map = this->_pix_map.scaled(width,height,Qt::KeepAspectRatio);
        this->ui->label->setPixmap(this->_pix_map);
    }
}

void PicShow::onDeleteItem()
{
    //删除当前选中图片：清空选中路径，触发上一级刷新
    this->_select_path="";
}
