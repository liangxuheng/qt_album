#ifndef ProTreeWidget_H
#define ProTreeWidget_H

#include <QTreeWidget>
#include <QProgressDialog>
#include "protreethread.h"
#include "opentreethread.h"
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
class SlideShowDialog;
class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget*parent=nullptr);
public slots:
    void addProToTree(const QString&path,const QString&name);
private:
    QSet<QString>_set_file_path;
    QTreeWidgetItem*_right_btn_item;
    QTreeWidgetItem*_active_item;
    //记录被选中的条目
    QTreeWidgetItem*_select_item;
    QAction * _action_import;
    QAction * _action_setstart;
    QAction * _action_closepro;
    QAction * _action_slideshow;
    QProgressDialog*_dialog_progress;
    QProgressDialog*_open_progress_dialog;
    std::shared_ptr<ProTreeThread>_tree_thread_ptr;
    std::shared_ptr<OpenTreeThread>_open_tree_thread_ptr;
    std::shared_ptr<SlideShowDialog>_slide_show_ptr;
    QMediaPlayer*_player;
    QAudioOutput*_audio_output;
    QList<QUrl>_player_list;
    int _cur_play_index;
private slots:
    void onItemPress(QTreeWidgetItem*pressitem,int column);
    void onImport(void);
    void onUpdateProgress(int file_count);
    void onFinishProgress(void);
    void onCancelProgress(void);
    void onSetActive(void);
    void onClosePro(void);

    void onUpdateOpenProgress(int file_count);
    void onFinishOpenProgress(void);
    void onCancelOpenProgress(void);

    void onDoubleClickItem(QTreeWidgetItem*item,int column);
public slots:
    void onOpenPro(const QString&path);
    void onNextBtn(void);
    void onPreBtn(void);
    void onSlideShow(void);
    void onSetMusic(void);
    void onStartMusic(void);
    void onStopMusic(void);
signals:
    void sigCancelProgress(void);
    void sigCancelOpenProgress(void);
    void sigUpdateSelected(const QString&);
    void sigUpdatePic(const QString&);
    void sigClearSelected(void);
};

#endif // ProTreeWidget_H
