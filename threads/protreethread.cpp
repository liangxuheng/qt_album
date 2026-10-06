/**
 ******************************************************************************
 * @file    protreethread.cpp
 * @brief   项目树加载线程：后台遍历目录，避免 UI 卡顿
 ******************************************************************************
 */

#include "protreethread.h"
#include <Qdir>
#include "protreeitem.h"
#include "const.h"
ProTreeThread::ProTreeThread(const QString &src_path, const QString &dist_path,
                             QTreeWidgetItem *parent_item, int file_count,
                             QTreeWidget *self, QTreeWidgetItem *root,
                             QObject *parent)
    :QThread(parent)
    ,_src_path(src_path)
    ,_dist_path(dist_path)
    ,_file_count(file_count)
    ,_parent_item(parent_item)
    ,_self(self)
    ,_root(root)
{

}

ProTreeThread::~ProTreeThread()
{
}

void ProTreeThread::run()
{
    QTreeWidgetItem *pre_item = nullptr;
    CreateProTree(this->_src_path,this->_dist_path,this->_parent_item,this->_file_count
                  ,this->_self,this->_root,pre_item);
    if(this->_bstop)
    {
        auto path = dynamic_cast<ProTreeItem*>(_root)->Getpath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit sigFinishProcess(this->_file_count);

}

void ProTreeThread::CreateProTree(const QString &src_path,
                                  const QString &dist_path,
                                  QTreeWidgetItem *parent_item,
                                  int &file_count, QTreeWidget *self,
                                  QTreeWidgetItem *root, QTreeWidgetItem *&preItem)
{
    if(this->_bstop)
    {
        return;
    }
    if(src_path==dist_path)
    {
        return;
    }
    QDir import_dir(src_path);
    //设置文件过滤器
    QStringList nameFilters;
    import_dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    //按照名字排序
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList();
    for(uint32_t i=0;i<list.size();++i)
    {
        if(this->_bstop)
        {
            return;
        }
        QFileInfo file_info=list.at(i);
        bool isDir=file_info.isDir();
        if(isDir)
        {
            if(this->_bstop)
            {
                return;
            }
            file_count++;
            emit sigUpdateProcess(file_count);
            QDir dist_dir(dist_path);
            //构建子目录的文件路径a
            QString sub_path=dist_dir.absoluteFilePath(file_info.fileName());
            QDir sub_dir(sub_path);
            if(!sub_dir.exists())
            {
                bool is_ok=sub_dir.mkpath(sub_path);
                if(!is_ok)
                {
                    continue;
                }
            }
            auto * item = new ProTreeItem(parent_item, file_info.fileName(),
                                         sub_path, root,TreeItemDir);
            item->setData(0,Qt::DisplayRole, file_info.fileName());
            item->setData(0,Qt::DecorationRole, QIcon(":/res/dir.png"));
            item->setData(0,Qt::ToolTipRole, sub_path);
            ;
            CreateProTree(file_info.absoluteFilePath(), sub_path, item, file_count, self,root,preItem);
        }
        else
        {
            if(this->_bstop)
            {
                return;
            }
            //取出文件的后缀
            const QString &suffix=file_info.suffix();
            if(suffix!="png"&&suffix!="jpeg"&&suffix!="jpg")
            {
                continue;
            }
            file_count++;
            emit sigUpdateProcess(file_count);
            QDir dist_file_path(dist_path);
            QString complete_dist_path=dist_file_path.absoluteFilePath(
                file_info.fileName());
            if(!QFile::copy(file_info.absoluteFilePath(),complete_dist_path))
            {
                continue;
            }
            auto * item = new ProTreeItem(parent_item, file_info.fileName(),
                                         complete_dist_path, root, TreeItemPic);
            item->setData(0, Qt::DisplayRole,file_info.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/res/pic.png"));
            item->setData(0, Qt::ToolTipRole, complete_dist_path);

            //连接成一个双向链表
            if(preItem)
            {
                //前向节点非空
                dynamic_cast<ProTreeItem*>(preItem)->SetNextItem(item);
            }
            item->SetPreItem(preItem);
            preItem=item;
        }
    }
}

void ProTreeThread::onCancelProgress()
{
    this->_bstop=true;
}
