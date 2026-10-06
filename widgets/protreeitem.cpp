/**
 ******************************************************************************
 * @file    protreeitem.cpp
 * @brief   项目树节点：文件/文件夹数据模型，存储路径和选中状态
 ******************************************************************************
 */

#include "protreeitem.h"
#include "const.h"

ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(view,type)
    ,_path(path)
    ,_name(name)
    ,_root(this)
    ,_pre_item(nullptr)
    ,_next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent,type)
    ,_path(path)
    ,_name(name)
    ,_root(root)
    ,_pre_item(nullptr)
    ,_next_item(nullptr)
{

}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return this->_root;
}

const QString &ProTreeItem::Getpath()
{
    return this->_path;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    this->_pre_item=item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    this->_next_item=item;
}

ProTreeItem *ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem*>(this->_pre_item);
}

ProTreeItem *ProTreeItem::GetNextItem()
{
    return dynamic_cast<ProTreeItem*>(this->_next_item);
}

ProTreeItem *ProTreeItem::GetLastPicChild()
{
    if(this->type()==TreeItemPic)
    {
        return nullptr;
    }

    auto child_count=this->childCount();
    if(child_count==0)
    {
        return nullptr;
    }
    for(int i=child_count-1;i>=0;--i)
    {
        auto last_child=dynamic_cast<ProTreeItem*>(this->child(i));
        int type=last_child->type();
        if(type==TreeItemPic)
        {
            return last_child;
        }
        else {
            auto * last_dir=last_child->GetLastPicChild();
            if(!last_dir)
            {
                continue;
            }
            else
            {
                return last_dir;
            }
        }
    }
    return nullptr;
}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{
    if(this->type() == TreeItemPic){
        return nullptr;
    }

    auto child_count = this->childCount();
    if(child_count == 0){
        return nullptr;
    }

    for(int i = 0; i < child_count; i++){
        auto * first_child = this->child(i);
        auto * first_tree_child = dynamic_cast<ProTreeItem*>(first_child);
        auto item_type = first_tree_child->type();
        if(item_type == TreeItemPic){
            return first_tree_child;
        }

        first_child = first_tree_child->GetFirstPicChild();
        if(!first_child){
            continue;
        }

        first_tree_child = dynamic_cast<ProTreeItem*>(first_child);
        return first_tree_child;
    }

    return nullptr;
}
