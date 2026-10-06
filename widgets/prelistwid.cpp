/**
 ******************************************************************************
 * @file    prelistwid.cpp
 * @brief   预览列表控件：待播放图片队列显示
 ******************************************************************************
 */

#include "prelistwid.h"
#include "protreeitem.h"
#include "const.h"
#include "prelistitem.h"
#include <QPainter>
#include <QGuiApplication>
PreListWid::PreListWid(QWidget *parent)
    : QListWidget(parent)
    , _global_num(0)
    , _last_index(17)
{
    //设置成只展示图标
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE,PREICON_SIZE));
    this->setSpacing(5);

    connect(this,&PreListWid::itemPressed,this,&PreListWid::onItemPressed);
}

void PreListWid::AddListItem(QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dst_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));
    dst_pixmap.fill(QColor(220,220,220,50));
    QPainter painter(&dst_pixmap);
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();

    auto dist_width = dst_pixmap.width();
    auto dist_height = dst_pixmap.height();
    auto x = (dist_width-src_width)/2;
    auto y = (dist_height-src_height)/2;
    painter.drawPixmap(x,y,src_pixmap);

    this->_global_num++;
    auto *pItem=new PreListItem(QIcon(dst_pixmap),path,this->_global_num,this);
    this->addItem(pItem);
    this->_set_item[path]=pItem;
    if(this->_global_num==1)
    {
        this->_pos_oringal=this->pos();
    }
}

void PreListWid::onUpdatePreList(QTreeWidgetItem *item)
{
    if(!item){
        qDebug() << "tree_item is empty" <<"\r\n";
        return;
    }

    auto * pro_item = dynamic_cast<ProTreeItem*>(item);
    auto path = pro_item->Getpath();
    auto iter = _set_item.find(path);
    if(iter != _set_item.end()){
        qDebug() << "path "<<path <<" exists" << "\r\n";
        return;
    }

    AddListItem(path);
}

void PreListWid::onUpdateChose(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }
    auto *tree_item=dynamic_cast<ProTreeItem*>(item);
    auto path=tree_item->Getpath();
    auto iter=this->_set_item.find(path);
    if(this->_set_item.end()==iter)
    {
        return;
    }

    auto *list_item=dynamic_cast<PreListItem*>(iter.value());
    auto index=list_item->GetIndex();
    if(index>17)
    {
        //这个函数的含义是将自己的左上角的位置放到父控件的(x,y)坐标
        this->move(this->_pos_oringal.x()-(index-this->_last_index)*100
                   ,this->_pos_oringal.y());
        //this->_last_index=index;
    }
    else
    {
        this->move(this->_pos_oringal);
        this->_last_index=17;
    }

    this->setCurrentItem(list_item);
}

void PreListWid::onItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    auto * list_item = dynamic_cast<PreListItem*>(item);
    auto path = list_item->GetPath();
    this->setCurrentItem(item);
    emit sigUpdateSelectShow(path);
}
