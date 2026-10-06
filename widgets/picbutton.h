#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>

class PicButton : public QPushButton
{
public:
    PicButton(QWidget* parent=nullptr);
    void SetIcons(const QString& normal,
                  const QString& hover, const QString& pressed);
protected:
    bool event(QEvent *e) override;
private:
    QString _normal;
    QString _hover;
    QString _pressed;

private:
    void setNormalIcon(void);
    void setHoverIcon(void);
    void setPressIcon(void);
};

#endif // PICBUTTON_H
