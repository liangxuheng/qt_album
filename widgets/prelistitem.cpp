/**
 ******************************************************************************
 * @file    prelistitem.cpp
 * @brief   预览列表项：单个图片的缩略图和索引显示
 ******************************************************************************
 */

#include "prelistitem.h"

PreListItem::PreListItem(const QIcon &icon, const QString &text,
                         const int &index, QListWidget *listview, int type)
    :QListWidgetItem(icon,"",listview,type)
    ,_path(text)
    ,_index(index)
{

}

const QString PreListItem::GetPath()
{
    return this->_path;
}

int PreListItem::GetIndex()
{
    return this->_index;
}
