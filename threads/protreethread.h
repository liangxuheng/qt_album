#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidget>
class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString &src_path, const QString& dist_path, QTreeWidgetItem* parent_item,
                  int file_count, QTreeWidget* self, QTreeWidgetItem* root, QObject* parent =nullptr);
    ~ProTreeThread();
protected:
    virtual void run();
private:
    void CreateProTree(const QString& src_path, const QString& dist_path,
                       QTreeWidgetItem* parent_item,
                       int &file_count, QTreeWidget* self, QTreeWidgetItem* root, QTreeWidgetItem *&preItem);

    QString _src_path;
    QString _dist_path;
    int _file_count;
    //当前节点父节点
    QTreeWidgetItem* _parent_item;
    //当前节点所在的widget
    QTreeWidget* _self;
    //当前节点的
    QTreeWidgetItem * _root;
    bool _bstop;

public slots:
    void onCancelProgress(void);
signals:
    void sigUpdateProcess(int);
    void sigFinishProcess(int);
};

#endif // PROTREETHREAD_H
