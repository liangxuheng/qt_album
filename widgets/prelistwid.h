#ifndef PRELISTWID_H
#define PRELISTWID_H

#include <QListWidget>
#include <QTreeWidgetItem>

class PreListWid : public QListWidget
{
    Q_OBJECT
public:
    PreListWid(QWidget*parent=nullptr);
private:
    QMap<QString,QListWidgetItem*>_set_item;
private:
    void AddListItem(QString&path);
private:
    //统计数量
    int _global_num;
    //存第一张图片的位置
    QPoint _pos_oringal;
    int _last_index;
public slots:
    void onUpdatePreList(QTreeWidgetItem*item);
    void onUpdateChose(QTreeWidgetItem*item);
    void onItemPressed(QListWidgetItem*item);
signals:
    void sigUpdateSelectShow(const QString &path);
};

#endif // PRELISTWID_H
