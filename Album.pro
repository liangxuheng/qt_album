QT += core gui widgets multimedia

CONFIG += c++17

INCLUDEPATH += \
    $$PWD/core \
    $$PWD/widgets \
    $$PWD/dialogs \
    $$PWD/threads

SOURCES += \
    core/main.cpp \
    core/mainwindow.cpp \
    dialogs/confirmpage.cpp \
    dialogs/prosetpage.cpp \
    dialogs/removeprodialog.cpp \
    dialogs/slideshowdialog.cpp \
    dialogs/wizard.cpp \
    threads/opentreethread.cpp \
    threads/protreethread.cpp \
    widgets/ProTreeWidget.cpp \
    widgets/picanimation.cpp \
    widgets/picbutton.cpp \
    widgets/picshow.cpp \
    widgets/picstatebutton.cpp \
    widgets/prelistitem.cpp \
    widgets/prelistwid.cpp \
    widgets/protree.cpp \
    widgets/protreeitem.cpp

HEADERS += \
    core/mainwindow.h \
    dialogs/confirmpage.h \
    dialogs/prosetpage.h \
    dialogs/removeprodialog.h \
    dialogs/slideshowdialog.h \
    dialogs/wizard.h \
    threads/opentreethread.h \
    threads/protreethread.h \
    widgets/ProTreeWidget.h \
    widgets/const.h \
    widgets/picanimation.h \
    widgets/picbutton.h \
    widgets/picshow.h \
    widgets/picstatebutton.h \
    widgets/prelistitem.h \
    widgets/prelistwid.h \
    widgets/protree.h \
    widgets/protreeitem.h

FORMS += \
    core/mainwindow.ui \
    dialogs/confirmpage.ui \
    dialogs/prosetpage.ui \
    dialogs/removeprodialog.ui \
    dialogs/slideshowdialog.ui \
    dialogs/wizard.ui \
    widgets/picshow.ui \
    widgets/protree.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
