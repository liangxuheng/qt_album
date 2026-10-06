#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile qss(":/style/style.qss");
    if (qss.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug() << "open qss failed";
        return -1;
    }

    w.setWindowTitle("Album");
    w.showMaximized();
    return QApplication::exec();
}
