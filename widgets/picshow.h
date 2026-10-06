#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QPropertyAnimation>
namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void Reload(void);

protected:
    virtual bool event(QEvent*e)override;

private:
    Ui::PicShow *ui;
    QPropertyAnimation*_animation_show_pre;
    QPropertyAnimation*_animation_show_next;
    bool is_visible;
    QString _select_path;
    QPixmap _pix_map;
private:
    void ShowPreNextBtns(bool is_on);
public slots:
    void onSelectItem(const QString&path);
    void onUpdatePic(const QString&path);
    void onDeleteItem(void);
signals:
    void sigNextBtn(void);
    void sigPreBtn(void);
};

#endif // PICSHOW_H
