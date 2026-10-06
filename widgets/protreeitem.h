#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget* view, const QString& name, const QString& path, int type = Type);
    ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type = Type);
    QTreeWidgetItem* GetRoot();
    const QString&Getpath(void);
    void SetPreItem(QTreeWidgetItem * item);
    void SetNextItem(QTreeWidgetItem* item);
    ProTreeItem* GetPreItem(void);
    ProTreeItem* GetNextItem(void);
    ProTreeItem* GetLastPicChild(void);
    ProTreeItem* GetFirstPicChild(void);
private:
    QString _path;
    QString _name;
    QTreeWidgetItem * _root;
    QTreeWidgetItem * _pre_item;
    QTreeWidgetItem * _next_item;
};

#endif // PROTREEITEM_H
