#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>

namespace Ui {
class Wizard;
}

class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = nullptr);
    ~Wizard();
protected:
    virtual void done(int result)override;

private:
    Ui::Wizard *ui;
signals:
    void sigProSetting(const QString&path,const QString&name);
};

#endif // WIZARD_H
