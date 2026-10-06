/**
 ******************************************************************************
 * @file    slideshowdialog.cpp
 * @brief   幻灯片播放对话框：全屏轮播图片
 ******************************************************************************
 */

#include "slideshowdialog.h"
#include "ui_slideshowdialog.h"
#include "prelistwid.h"
#include "ProTreeWidget.h"
SlideShowDialog::SlideShowDialog(QWidget *parent, QTreeWidgetItem *first_item
                                 , QTreeWidgetItem *end_item)
    :QDialog(parent)
    ,ui(new Ui::SlideShowDialog)
    ,_first_item(first_item)
    ,_end_item(end_item)
{
    this->ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    this->ui->slidePreBtn->SetIcons(":/res/previous.png"
                                    ,":/res/previous_hover.png",
                                    ":/res/previous_press.png");

    this->ui->sliedNextBtn->SetIcons(":/res/next.png"
                                     ,":/res/next_hover.png",
                                     ":/res/next_press.png");

    this->ui->closeBtn->SetIcons(":/res/close_stop.png",
                                 ":/res/close_stop_hover.png",
                                 ":/res/close_stop_enter.png");
    this->ui->playBtn->SetIcons(":/res/play.png",":/res/play_hover.png",
                                ":/res/play_press.png",":/res/pause.png",
                                ":/res/pause_hover.png",":/res/pause_enter.png");

    //这个close槽函数是从基类里继承过来的
    connect(this->ui->closeBtn,&QPushButton::clicked,
            this,&SlideShowDialog::close);
    connect(this->ui->sliedNextBtn,&QPushButton::clicked,this,
            &SlideShowDialog::SlotSlideNext);
    connect(this->ui->slidePreBtn,&QPushButton::clicked,this,
            &SlideShowDialog::SlotSlidePre);
    connect(this->ui->playBtn,&PicStateButton::clicked
            ,this->ui->picanimation,&PicAnimation::SlotStartOrStop);

    connect(this->ui->picanimation,&PicAnimation::SignUpdatePreList,
            dynamic_cast<PreListWid*>(this->ui->prelistView),&PreListWid::onUpdatePreList);
    connect(this->ui->picanimation,&PicAnimation::SignUpdateChose,
            dynamic_cast<PreListWid*>(this->ui->prelistView),&PreListWid::onUpdateChose);
    connect(dynamic_cast<PreListWid*>(this->ui->prelistView),&PreListWid::sigUpdateSelectShow,
            this->ui->picanimation,&PicAnimation::SlotUpdateSelectShow);
    connect(this->ui->picanimation,&PicAnimation::SignStart,
            this->ui->playBtn,&PicStateButton::SlotStart);
    connect(this->ui->picanimation,&PicAnimation::SignStop,
            this->ui->playBtn,&PicStateButton::SlotStop);

    auto *tree_item=dynamic_cast<ProTreeWidget*>(parent);
    connect(this->ui->picanimation,&PicAnimation::SignStartMusic
            ,tree_item,&ProTreeWidget::onStartMusic);
    connect(this->ui->picanimation,&PicAnimation::SignStopMusic
            ,tree_item,&ProTreeWidget::onStopMusic);
    this->ui->picanimation->SetPixmap(this->_first_item);

    this->ui->picanimation->Start();
}

SlideShowDialog::~SlideShowDialog()
{
    delete ui;
}

void SlideShowDialog::closeEvent(QCloseEvent *event)
{
    this->ui->picanimation->Stop();   // Stop() 内部会发 SignStopMusic 停音乐
    QDialog::closeEvent(event);       // 交给基类继续正常关闭
}

void SlideShowDialog::reject()
{
    this->ui->picanimation->Stop();   // Esc 键走 reject()，同样先停动画+音乐
    QDialog::reject();                // 交给基类继续正常 reject
}

void SlideShowDialog::SlotSlideNext()
{
    this->ui->picanimation->SlideNext();
}

void SlideShowDialog::SlotSlidePre()
{
    this->ui->picanimation->SlidePre();
}
