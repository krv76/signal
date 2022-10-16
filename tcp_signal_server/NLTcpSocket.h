#ifndef NLTCPSOCKET_H
#define NLTCPSOCKET_H

#include <QTcpSocket>

class NLTcpSocket : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* tcpSocket;

public:
    NLTcpSocket(QTcpSocket* socket);
    ~NLTcpSocket();
    QString getData();
    void write(QString data);

private slots:
    void slotReadyRead();
    void slotConnected();
    void slotDisconnected();

signals:
    void dataReady(NLTcpSocket* socket);
    void socketConnected(NLTcpSocket* socket);
    void socketDisconnected(NLTcpSocket* socket);
};
#endif
