#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ProTree;
class PicShow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    ProTree *_proTree;
    PicShow *_picshow;
private slots:
    void onCreatePro(bool);
    void onOpenPro(void);
signals:
    void sigOpenPro(const QString &path);
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
