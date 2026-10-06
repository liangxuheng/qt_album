/**
 ******************************************************************************
 * @file    picstatebutton.cpp
 * @brief   状态图片按钮：带选中/未选中状态的图标按钮
 ******************************************************************************
 */

#include "picstatebutton.h"
#include "const.h"
#include <QEvent>
PicStateButton::PicStateButton(QWidget *parent)
    :QPushButton(parent)
{

}

void PicStateButton::SetIcons(const QString &normal, const QString &hover, const QString &pressed, const QString &normal_2, const QString &hover_2, const QString &pressed_2)
{
     this->_normal = normal;
     this->_hover = hover;
     this->_pressed = pressed;

     this->_normal_2 = normal_2;
     this->_hover_2 = hover_2;
     this->_pressed_2 = pressed_2;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    tmpPixmap = tmpPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
    _cur_state = PicBtnStateNormal;
}

bool PicStateButton::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Enter:
        if(this->_cur_state < PicBtnState2Normal){
            setHoverIcon();
        }else{
            setHover2Icon();
        }

        break;
    case QEvent::Leave:
        if(this->_cur_state < PicBtnState2Normal){
            setNormalIcon();
        }else{
            setNormal2Icon();
        }

        break;
    case QEvent::MouseButtonPress:
        if(this->_cur_state < PicBtnState2Normal){
            setPressIcon();
        }else{
            setPress2Icon();
        }

        break;
    case QEvent::MouseButtonRelease:
        if(this->_cur_state < PicBtnState2Normal){
            setHover2Icon();
        }else{
            setHoverIcon();
        }
        break;
    default:
        break;
    }
    return QPushButton::event(event);
}

void PicStateButton::setNormalIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(this->_normal);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateNormal;
}

void PicStateButton::setHoverIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(this->_hover);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateHover;
}

void PicStateButton::setPressIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(this->_pressed);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStatePress;
}

void PicStateButton::setNormal2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(this->_normal_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Normal;
}

void PicStateButton::setHover2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(this->_hover_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Hover;
}

void PicStateButton::setPress2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(this->_pressed_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Press;
}

void PicStateButton::SlotStart()
{
    this->setNormal2Icon();
}

void PicStateButton::SlotStop()
{
    this->setNormalIcon();
}
