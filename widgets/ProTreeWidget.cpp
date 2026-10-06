/**
 ******************************************************************************
 * @file    ProTreeWidget.cpp
 * @brief   项目树控件：文件目录浏览、右键菜单、拖拽导入
 ******************************************************************************
 */

#include "ProTreeWidget.h"
#include <QDir>
#include <QString>
#include <QSet>
#include <QHeaderView>
#include "protreeitem.h"
#include "const.h"
#include <QMenu>
#include <QGuiApplication>
#include <QAction>
#include <QFileDialog>
#include <QThread>
#include "removeprodialog.h"
#include "slideshowdialog.h"
ProTreeWidget::ProTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
    , _right_btn_item(nullptr)
    , _active_item(nullptr)
    , _select_item(nullptr)
    , _dialog_progress(nullptr)
    , _open_progress_dialog(nullptr)
    , _tree_thread_ptr(nullptr)
    , _open_tree_thread_ptr(nullptr)
    , _slide_show_ptr(nullptr)
{
    //将头进行隐藏
    this->header()->hide();
    //在目录树的地方只要按下按键就会触发这个槽函数
    connect(this,&QTreeWidget::itemPressed
            ,this,&ProTreeWidget::onItemPress);
    this->_action_import = new QAction(QIcon(":/res/import.png"), tr("导入文件"), this);
    this->_action_setstart = new QAction(QIcon(":/res/core.png"), tr("设置活动项目"), this);
    this->_action_closepro = new QAction(QIcon(":/res/close.png"), tr("关闭项目"), this);
    this->_action_slideshow = new QAction(QIcon(":/res/slideshow.png"), tr("轮播图播放"), this);
    connect(this->_action_import,&QAction::triggered,this,
            &ProTreeWidget::onImport);
    connect(this->_action_setstart,&QAction::triggered,this,
            &ProTreeWidget::onSetActive);
    connect(this->_action_closepro,&QAction::triggered,this,
            &ProTreeWidget::onClosePro);
    connect(this->_action_slideshow,&QAction::triggered,this,
            &ProTreeWidget::onSlideShow);
    connect(this,&ProTreeWidget::itemDoubleClicked,this,
            &ProTreeWidget::onDoubleClickItem);

    //Qt6 创建播放器和音频输出（Qt6 必须单独挂 QAudioOutput 才有声音）
    this->_player = new QMediaPlayer(this);
    this->_audio_output = new QAudioOutput(this);
    this->_player->setAudioOutput(this->_audio_output);
    this->_cur_play_index = 0;
    //QMediaPlaylist 在 Qt6 被移除，播完自动切下一首用信号自己实现
    connect(this->_player,&QMediaPlayer::mediaStatusChanged,this,
            [this](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia && !this->_player_list.isEmpty()){
            this->_cur_play_index = (this->_cur_play_index + 1) % this->_player_list.size();
            this->_player->setSource(this->_player_list.at(this->_cur_play_index));
            this->_player->play();
        }
    });
}

/**
 * @brief  向项目树添加一个项目节点
 * @param  path  项目路径
 * @param  name  项目名
 * @retval None
 */
void ProTreeWidget::addProToTree(const QString &path, const QString &name)
{
    qDebug() << "ProTreeWidget::AddProToTree name is " << name
             << " path is " << path << "\r\n";
    QDir dir(path);
    QString file_path=dir.absoluteFilePath(name);
    if(this->_set_file_path.find(file_path)!=this->_set_file_path.end())
    {
        qDebug() << "file has loaded" << "\r\n";
        return;
    }

    //构造项目用的文件夹
    QDir pro_dir(file_path);
    //如果文件夹不存在则创建
    if(!pro_dir.exists()){
        //递归地创建文件夹
        bool enable = pro_dir.mkpath(file_path);
        if(!enable){
            qDebug() << "pro_dir make path failed" <<"\r\n";
            return;
        }
    }
    this->_set_file_path.insert(file_path);

    auto *item=new ProTreeItem(this,name,file_path
                                 ,TreeItemPro);
    //设置显示
    item->setData(0,Qt::DisplayRole, name);
    //设置图片
    item->setData(0,Qt::DecorationRole, QIcon(":/res/dir.png"));
    //设置鼠标悬停信息
    item->setData(0,Qt::ToolTipRole, file_path);

    this->addTopLevelItem(item);
}

/**
 * @brief  树节点被按下：发出选中信号
 * @param  pressitem  被按节点
 * @param  column     列
 * @retval None
 */
void ProTreeWidget::onItemPress(QTreeWidgetItem *pressitem, int column)
{
    Q_UNUSED(column);
    if(QGuiApplication::mouseButtons()==Qt::RightButton)
    {
        QMenu menu(this);
        int press_type=pressitem->type();
        if(press_type==TreeItemPro)
        {
            this->_right_btn_item=pressitem;
            menu.addAction(this->_action_import);
            menu.addAction(this->_action_closepro);
            menu.addAction(this->_action_setstart);
            menu.addAction(this->_action_slideshow);
            menu.exec(QCursor::pos());
        }
    }
}

/**
 * @brief  导入项目：弹出目录选择并启动线程扫描
 * @retval None
 */
void ProTreeWidget::onImport()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if(!this->_right_btn_item){
        qDebug() << "_right_btn_item is empty" <<"\r\n";
        return;
    }
    path=dynamic_cast<ProTreeItem*>(this->_right_btn_item)->Getpath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList file_list;
    if(file_dialog.exec())
    {
        file_list=file_dialog.selectedFiles();
    }
    else
    {
        return;
    }

    if(file_list.length()<=0)
    {
        return;
    }

    QString file_path=file_list.at(0);
    int file_count=0;
    this->_dialog_progress=new QProgressDialog(this);
    this->_tree_thread_ptr=std::make_shared<ProTreeThread>(std::ref(file_path)
                                                             ,std::ref(path),this->_right_btn_item
                                                             ,file_count,this,
                                                             this->_right_btn_item,nullptr);
    connect(this->_tree_thread_ptr.get(),&ProTreeThread::sigUpdateProcess
            ,this,&ProTreeWidget::onUpdateProgress);
    connect(this->_tree_thread_ptr.get(),&ProTreeThread::sigFinishProcess
            ,this,&ProTreeWidget::onFinishProgress);
    connect(this->_dialog_progress,&QProgressDialog::canceled,
            this,&ProTreeWidget::onCancelProgress);
    connect(this,&ProTreeWidget::sigCancelProgress,this->_tree_thread_ptr.get()
            ,&ProTreeThread::onCancelProgress);
    this->_tree_thread_ptr->start();
    this->_dialog_progress->setWindowTitle("请耐心等待...");
    //将模态对话框的长度设置为固定长度
    this->_dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    this->_dialog_progress->setRange(0,PROGRESS_WIDTH);
    this->_dialog_progress->exec();
}

void ProTreeWidget::onUpdateProgress(int file_count)
{
    if(!this->_dialog_progress)
    {
        return;
    }

    if(file_count>=PROGRESS_MAX)
    {
        this->_dialog_progress->setValue(file_count%PROGRESS_MAX);
    }
    else
    {
        this->_dialog_progress->setValue(file_count);
    }
}

void ProTreeWidget::onFinishProgress()
{
    this->_dialog_progress->setValue(PROGRESS_MAX);
    this->_dialog_progress->deleteLater();
}

void ProTreeWidget::onCancelProgress()
{
    emit sigCancelProgress();
    delete this->_dialog_progress;
    this->_dialog_progress=nullptr;
}

void ProTreeWidget::onSetActive()
{
    if(this->_right_btn_item==nullptr)
    {
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);
    //将上次的活动项目的字体还原
    if(this->_active_item){
    this->_active_item->setFont(0,nullFont);
    }
    //将这次的设置成加粗
    this->_active_item=this->_right_btn_item;
    nullFont.setBold(true);
    this->_active_item->setFont(0,nullFont);
}

/**
 * @brief  关闭当前项目
 * @retval None
 */
void ProTreeWidget::onClosePro()
{
    RemoveProDialog remove_dialog;
    auto res=remove_dialog.exec();
    if(res==QDialog::Rejected)
    {
        return;
    }
    bool is_remove=remove_dialog.isRemove();
    auto index_right_btn=this->indexOfTopLevelItem(this->_right_btn_item);
    auto *right_btn_item_real=dynamic_cast<ProTreeItem*>(this->_right_btn_item);
    auto *select_item=dynamic_cast<ProTreeItem*>(this->_select_item);
    auto delete_path=right_btn_item_real->Getpath();
    this->_set_file_path.remove(delete_path);

    //如果还要删除文件项目
    if(is_remove)
    {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }

    if(right_btn_item_real==this->_active_item)
    {
        this->_active_item=nullptr;
    }

    if(select_item&&right_btn_item_real==select_item->GetRoot())
    {
        //如果当前选中的要关闭的项目是右侧正在展示的图片的父级项目
        select_item=nullptr;
        this->_select_item=nullptr;
        emit sigClearSelected();
    }

    delete this->takeTopLevelItem(index_right_btn);
    this->_right_btn_item=nullptr;
}

void ProTreeWidget::onUpdateOpenProgress(int file_count)
{
    if(!this->_open_progress_dialog)
    {
        return;
    }

    if(file_count>=PROGRESS_MAX)
    {
        this->_open_progress_dialog->setValue(file_count%PROGRESS_MAX);
    }
    else
    {
        this->_open_progress_dialog->setValue(file_count);
    }
}

void ProTreeWidget::onFinishOpenProgress()
{
    if(!this->_open_progress_dialog)
    {
        return;
    }

    this->_open_progress_dialog->setValue(PROGRESS_MAX);
    delete this->_open_progress_dialog;
    this->_open_progress_dialog=nullptr;
}

void ProTreeWidget::onCancelOpenProgress()
{
    emit sigCancelOpenProgress();
    delete this->_open_progress_dialog;
    this->_open_progress_dialog=nullptr;
}

void ProTreeWidget::onDoubleClickItem(QTreeWidgetItem *item, int column)
{
    (void)column;
    if(QGuiApplication::mouseButtons()==Qt::LeftButton)
    {
        auto * tree_item=dynamic_cast<ProTreeItem*>(item);
        if(!tree_item)
        {
            return;
        }
        int item_type=tree_item->type();
        if(item_type==TreeItemPic)
        {
            emit sigUpdateSelected(tree_item->Getpath());
            this->_select_item=tree_item;
        }
    }
}

void ProTreeWidget::onOpenPro(const QString &path)
{
    if(this->_set_file_path.find(path)!=this->_set_file_path.end())
    {
        return;
    }

    int file_count=0;
    QDir pro_dir(path);
    QString name=pro_dir.dirName();

    this->_open_tree_thread_ptr=std::make_shared<OpenTreeThread>(path,file_count,this,nullptr);


    this->_open_progress_dialog=new QProgressDialog(this);
    connect(this->_open_tree_thread_ptr.get(),&OpenTreeThread::sigUpdateProgress
            ,this,&ProTreeWidget::onUpdateOpenProgress);
    connect(this->_open_tree_thread_ptr.get(),&OpenTreeThread::sigFinishProgress
            ,this,&ProTreeWidget::onFinishOpenProgress);
    connect(this->_open_progress_dialog,&QProgressDialog::canceled,
            this,&ProTreeWidget::onCancelOpenProgress);
    connect(this,&ProTreeWidget::sigCancelOpenProgress,this->_open_tree_thread_ptr.get()
            ,&OpenTreeThread::onCancelProgress);
    this->_open_progress_dialog->setWindowTitle("请耐心等待...");
    this->_open_tree_thread_ptr->start();
    //将模态对话框的长度设置为固定长度
    this->_open_progress_dialog->setFixedWidth(PROGRESS_WIDTH);
    this->_open_progress_dialog->setRange(0,PROGRESS_WIDTH);
    this->_open_progress_dialog->exec();
}

void ProTreeWidget::onNextBtn()
{
    if(!this->_select_item){
        return;
    }

    auto *curItem = dynamic_cast<ProTreeItem*>(this->_select_item)->GetNextItem();
    if(!curItem){
        return;
    }

    emit sigUpdatePic(curItem->Getpath());
    this->_select_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::onPreBtn()
{
    if(!this->_select_item){
        return;
    }

    auto *curItem = dynamic_cast<ProTreeItem*>(this->_select_item)->GetPreItem();
    if(!curItem){
        return;
    }

    emit sigUpdatePic(curItem->Getpath());
    this->_select_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::onSlideShow()
{
    if(this->_right_btn_item==nullptr)
    {
        return;
    }
    auto *right_btn_real=dynamic_cast<ProTreeItem*>(this->_right_btn_item);
    auto last_child_item=right_btn_real->GetLastPicChild();
    qDebug()<<"last_child_item is "<<last_child_item;
    if(last_child_item==nullptr)
    {
        return;
    }
    auto first_child_item=right_btn_real->GetFirstPicChild();
    qDebug()<<"first_child_item is "<<first_child_item;
    if(!first_child_item)
    {
        return;
    }
    this->_slide_show_ptr=std::make_shared<SlideShowDialog>(this,first_child_item,last_child_item);
    this->_slide_show_ptr->setModal(true);
    this->_slide_show_ptr->showMaximized();
}

void ProTreeWidget::onSetMusic()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle("选择导入的音乐");

    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");

    QStringList fileNames;
    if (file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length() <= 0){
        return;
    }

    this->_player_list.clear();

    for(const QString &filename : fileNames){
        qDebug() << "filename is " << filename << "\r\n";
        this->_player_list.append(QUrl::fromLocalFile(filename));
    }

    if(this->_player->playbackState() != QMediaPlayer::PlayingState)
    {
        this->_cur_play_index = 0;
        this->_player->setSource(this->_player_list.at(0));
    }
}

void ProTreeWidget::onStartMusic()
{
    this->_player->play();
}

void ProTreeWidget::onStopMusic()
{
    this->_player->stop();
}

