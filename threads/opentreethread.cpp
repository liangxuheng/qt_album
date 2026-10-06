/**
 ******************************************************************************
 * @file    opentreethread.cpp
 * @brief   打开目录线程：递归扫描图片文件，在子线程中执行
 ******************************************************************************
 */

#include "opentreethread.h"
#include <QTreeWidgetItem>
#include "protreeitem.h"
#include <QDir>
#include "const.h"
#include <QString>
OpenTreeThread::OpenTreeThread(const QString &src_path,
                               int file_count, QTreeWidget *self,
                               QObject *parent)
    :QThread(parent)
    ,_src_path(src_path)
    ,_file_count(file_count)
    ,_self(self)
    ,_bstop(false)
    ,_root(nullptr)
{

}

void OpenTreeThread::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    QDir src_dir(src_path);
    auto name = src_dir.dirName();
    auto * item = new ProTreeItem(self, name, src_path, TreeItemPro);
    item->setData(0,Qt::DisplayRole, name);
    item->setData(0,Qt::DecorationRole, QIcon(":/res/dir.png"));
    item->setData(0,Qt::ToolTipRole, src_path);
    this->_root = item;
    QTreeWidgetItem *pre_item = nullptr;
    this->RecursiveProTree(src_path,file_count,self,this->_root,item,pre_item);
}

void OpenTreeThread::run()
{
    this->OpenProTree(_src_path,_file_count,_self);
    if(this->_bstop)
    {
        auto path = dynamic_cast<ProTreeItem*>(_root)->Getpath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit sigFinishProgress(this->_file_count);

}

void OpenTreeThread::RecursiveProTree(const QString &src_path, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *&preitem)
{
    if(this->_bstop)
    {
        return;
    }
    QDir src_dir(src_path);
    //设置文件过滤器
    QStringList nameFilters;
    src_dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    //按照名字排序
    src_dir.setSorting(QDir::Name);
    QFileInfoList list = src_dir.entryInfoList();
    for(uint32_t i=0;i<list.size();++i)
    {
        QFileInfo file_info=list.at(i);
        bool is_dir=file_info.isDir();
        if(is_dir)
        {
            file_count ++;
            emit sigUpdateProgress(file_count);
            auto * item = new ProTreeItem(parent, file_info.fileName(), file_info.absoluteFilePath(),
                                         _root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, file_info.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/res/dir.png"));
            item->setData(0, Qt::ToolTipRole, file_info.absoluteFilePath());
            RecursiveProTree(file_info.absoluteFilePath(), file_count,
                             self, root, item, preitem);
        }
        else
        {
            if(_bstop){
                return;
            }
            const QString& suffix = file_info.completeSuffix();
            if(suffix!="png"&&suffix!="jpg"&&suffix!="jpeg")
            {
                continue;
            }
            file_count ++;
            emit sigUpdateProgress(file_count);
            auto * item = new ProTreeItem(_root, file_info.fileName(), file_info.absoluteFilePath(),
                                         _root, TreeItemPic);
            item->setData(0,Qt::DisplayRole,file_info.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/res/pic.png"));
            item->setData(0,Qt::ToolTipRole,file_info.absoluteFilePath());
            if(preitem){
             dynamic_cast<ProTreeItem*>(preitem)->SetNextItem(item);
            }
            item->SetPreItem(preitem);
            preitem=item;
        }
    }

    emit sigFinishProgress(file_count);
}

void OpenTreeThread::onCancelProgress()
{
    this->_bstop=true;
}
