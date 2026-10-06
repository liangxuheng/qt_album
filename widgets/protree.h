#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include <QTreeWidget>
namespace Ui {
class ProTree;
}

class ProTreeWidget;

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();
    ProTreeWidget*GetTreeWidget(void);

private:
    Ui::ProTree *ui;

public slots:
    void addProToTree(const QString&path,const QString&name);
};

#endif // PROTREE_H
