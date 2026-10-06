/**
 ******************************************************************************
 * @file    picbutton.cpp
 * @brief   图片按钮：正常/悬停/按下三态图标切换
 ******************************************************************************
 */

#include "picbutton.h"
#include <QPixmap>
#include <QEvent>
PicButton::PicButton(QWidget *parent)
: QPushButton(parent)
{

}

void PicButton::SetIcons(const QString &normal, const QString &hover, const QString &pressed)
{
    this->_hover=hover;
    this->_normal=normal;
    this->_pressed=pressed;

    QPixmap tempMap;
    tempMap.load(normal);
    tempMap = tempMap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
}

bool PicButton::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Enter:
    {
        setHoverIcon();
        break;
    }
    case QEvent::Leave:
    {
        setNormalIcon();
        break;
    }
    case QEvent::MouseButtonPress:
    {
        setPressIcon();
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        setHoverIcon();
        break;
    }
    default:
    {
        break;
    }
    }
    return QPushButton::event(e);
}

void PicButton::setNormalIcon()
{
    QPixmap tempMap;
    tempMap.load(this->_normal);
    tempMap = tempMap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
}

void PicButton::setHoverIcon()
{
    QPixmap tempMap;
    tempMap.load(this->_hover);
    tempMap = tempMap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
}

void PicButton::setPressIcon()
{
    QPixmap tempMap;
    tempMap.load(this->_pressed);
    tempMap = tempMap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
    this->setIcon(tempMap);
    this->setIconSize(tempMap.size());
}
