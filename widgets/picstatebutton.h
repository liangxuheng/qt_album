#ifndef PICSTATEBUTTON_H
#define PICSTATEBUTTON_H

#include <QPushButton>

class PicStateButton : public QPushButton
{
public:
    PicStateButton(QWidget*parent=nullptr);
    void SetIcons(const QString& normal, const QString& hover, const QString&pressed,
                  const QString& normal_2, const QString& hover_2, const QString& pressed_2);
protected:
    virtual bool event(QEvent*event)override;
private:
    QString _normal;
    QString _hover;
    QString _pressed;
    QString _normal_2;
    QString _pressed_2;
    QString _hover_2;
    int _cur_state;
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressIcon();

    void setNormal2Icon();
    void setHover2Icon();
    void setPress2Icon();
public slots:
    void SlotStart(void);
    void SlotStop(void);
};

#endif // PICSTATEBUTTON_H
