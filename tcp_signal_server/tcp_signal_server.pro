# mostly copied from https://github.com/parveen1112/Handling-Multiple-TCP-Connections-in-Qt
# other links:
# russian lessons: https://evileg.com/ru/knowledge/qt/?page=5
# qt creator manual (and docs here) https://doc.qt.io/qtcreator/index.html
# english lessons: https://www.bogotobogo.com/Qt/Qt5_QTcpServer_QThreadPool_Multithreaded_Client_Server.php
# https://www.qcustomplot.com/
# big chunk of info (not only qt): https://webhamster.ru/mytetrashare/index/mtb0#15391037486wf1a12v57

QT       += core
QT       += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        NLTcpSocket.cpp \
        TcpServer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    NLTcpSocket.h \
    TcpServer.h
