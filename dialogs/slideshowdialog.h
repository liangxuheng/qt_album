#ifndef SLIDESHOWDIALOG_H
#define SLIDESHOWDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
class SlideShowDialog;
}

class SlideShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDialog(QWidget *parent=nullptr,QTreeWidgetItem*first_item=nullptr
                             ,QTreeWidgetItem*end_item=nullptr);
    ~SlideShowDialog();

private:
    Ui::SlideShowDialog *ui;
    QTreeWidgetItem*_first_item;
    QTreeWidgetItem*_end_item;
protected:
    void closeEvent(QCloseEvent *event) override;
    void reject() override;
private slots:
    void SlotSlideNext(void);
    void SlotSlidePre(void);
};

#endif // SLIDESHOWDIALOG_H
