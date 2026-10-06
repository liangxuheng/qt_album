#ifndef PICANIMATION_H
#define PICANIMATION_H
#include <QWidget>
#include <QTreeWidget>
class PicAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimation(QWidget *parent = nullptr);
    void SetPixmap(QTreeWidgetItem* item);
    void Start(void);
    void Stop(void);
protected:
    virtual void paintEvent(QPaintEvent *event)override;
private:
    float _factor;
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem * _cur_item;
    QMap<QString, QTreeWidgetItem*> _map_items;
    bool _b_start;

private:
    void UpSelectPixMap(QTreeWidgetItem*item);
private slots:
    void TimerOut(void);
public slots:
    void SlotUpdateSelectShow(const QString&path);
    void SlotStartOrStop(void);
public:
    void SlideNext(void);
    void SlidePre(void);
signals:
    void SignUpdatePreList(QTreeWidgetItem*);
    void SignUpdateChose(QTreeWidgetItem*);
    void SignStart(void);
    void SignStop(void);
    void SignStartMusic(void);
    void SignStopMusic(void);
};

#endif // PICANIMATION_H
