/**
 ******************************************************************************
 * @file    picanimation.cpp
 * @brief   动画图片控件：加载/加载中动效绘制
 ******************************************************************************
 */

#include "picanimation.h"
#include "protreeitem.h"
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
PicAnimation::PicAnimation(QWidget *parent)
    : QWidget{parent}
    ,_factor(0.0)
    ,_cur_item(nullptr)
    ,_b_start(false)
{
    this->_timer=new QTimer(this);
    connect(this->_timer,&QTimer::timeout,this,&PicAnimation::TimerOut);
}

void PicAnimation::SetPixmap(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }
    auto *cur_item=dynamic_cast<ProTreeItem*>(item);
    auto path=cur_item->Getpath();
    this->_pixmap1.load(path);
    this->_cur_item=cur_item;

    if(this->_map_items.find(path)==this->_map_items.end())
    {
        this->_map_items[path]=cur_item;
        emit SignUpdatePreList(cur_item);
    }

    emit SignUpdateChose(item);

    auto *next_item=cur_item->GetNextItem();
    if(!next_item)
    {
        return;
    }

    auto next_path=next_item->Getpath();
    if(this->_map_items.find(next_path)==this->_map_items.end())
    {
        this->_map_items[next_path]=next_item;
        emit SignUpdatePreList(next_item);
    }
}

void PicAnimation::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(_pixmap1.isNull()){
        return;
    }

    QPainter painter(this);
    //开启抗锯齿，让缩放后的图片边缘更平滑。
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = geometry();

    int w = rect.width();
    int h = rect.height();
    this->_pixmap1=this->_pixmap1.scaled(w,h,Qt::KeepAspectRatio);
    //图一渐渐变成透明
    int alpha = 255 * (1.0f -this->_factor);
    //qDebug()<<"_pixmap1.size()" << _pixmap1.size() << endl;
    //创建一个透明的画布
    QPixmap alphaPixmap(this->_pixmap1.size());
    alphaPixmap.fill(Qt::transparent);

    QPainter p1(&alphaPixmap);
    //先将图一画上去
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, this->_pixmap1);
    //再将黑色的遮罩盖上去
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
    p1.end();

    int x = (w - this->_pixmap1.width()) / 2;
    int y = (h - this->_pixmap1.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap);

    if(this->_pixmap2.isNull()){
        return;
    }

    this->_pixmap2=this->_pixmap2.scaled(w,h,Qt::KeepAspectRatio);
    alpha = 255 * (this->_factor);
    QPixmap alphaPixmap2(this->_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, this->_pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap2.rect(), QColor(0, 0, 0, alpha));
    p2.end();
    x = (w - this->_pixmap2.width()) / 2;
    y = (h - this->_pixmap2.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap2);
}

void PicAnimation::UpSelectPixMap(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }

    auto *cur_item=dynamic_cast<ProTreeItem*>(item);
    auto path=cur_item->Getpath();
    this->_pixmap1.load(path);
    this->_cur_item=cur_item;
    if(this->_map_items.find(path)==this->_map_items.end())
    {
        this->_map_items[path]=cur_item;
    }

    auto *next_item=cur_item->GetNextItem();
    if(next_item==nullptr)
    {
        return;
    }
    auto next_path=next_item->Getpath();
    this->_pixmap2.load(next_path);
    if(this->_map_items.find(next_path)==this->_map_items.end())
    {
        this->_map_items[next_path]=next_item;
    }
}

void PicAnimation::Start()
{
    emit SignStart();
    emit SignStartMusic();
    this->_factor=0;
    this->_timer->start(25);
    this->_b_start=true;
}

void PicAnimation::Stop()
{
    emit SignStop();
    emit SignStopMusic();
    this->_timer->stop();
    this->_factor=0;
    this->_b_start=false;
}

void PicAnimation::TimerOut()
{
    if(!this->_cur_item)
    {
        this->Stop();
        update();
        return;
    }
    this->_factor+=0.01;
    if(this->_factor>=1)
    {
        this->_factor=0;
        auto *cur_item=dynamic_cast<ProTreeItem*>(this->_cur_item);
        auto * next_item=cur_item->GetNextItem();
        if(!next_item)
        {
            this->Stop();
            update();
            return;
        }

        this->SetPixmap(next_item);
        update();
        return;

    }
    update();
}

void PicAnimation::SlotUpdateSelectShow(const QString &path)
{
    auto iter=this->_map_items.find(path);
    if(iter==this->_map_items.end())
    {
        return;
    }

    UpSelectPixMap(iter.value());
    update();
}

void PicAnimation::SlotStartOrStop()
{
    if(!_b_start){
        _factor = 0;
        _timer->start(25);
        _b_start = true;
        emit SignStartMusic();
    }else{
        _timer->stop();
        _factor = 0;
        update();
        _b_start = false;
        emit SignStopMusic();
    }
}

void PicAnimation::SlideNext()
{
    this->Stop();
    if(this->_cur_item==nullptr)
    {
        return;
    }
    auto *cur_item=dynamic_cast<ProTreeItem*>(this->_cur_item);
    if(!cur_item)
    {
        return;
    }
    auto *next=cur_item->GetNextItem();
    if(!next)
    {
        return;
    }
    this->SetPixmap(next);
    update();
}

void PicAnimation::SlidePre()
{
    this->Stop();
    if(this->_cur_item==nullptr)
    {
        return;
    }
    auto *cur_item=dynamic_cast<ProTreeItem*>(this->_cur_item);
    if(!cur_item)
    {
        return;
    }
    auto *pre=cur_item->GetPreItem();
    if(!pre)
    {
        return;
    }
    this->SetPixmap(pre);
    update();
}
