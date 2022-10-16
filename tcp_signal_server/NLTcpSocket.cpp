#include "NLTcpSocket.h"

NLTcpSocket::NLTcpSocket(QTcpSocket* socket)
{
    tcpSocket = socket;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
}

NLTcpSocket::~NLTcpSocket()
{
    // delete tcpSocket;
}

void NLTcpSocket::slotReadyRead()
{
    emit dataReady(this);
}

void NLTcpSocket::slotConnected()
{
    emit socketConnected(this);
}

void NLTcpSocket::slotDisconnected()
{
    emit socketDisconnected(this);
}

QString NLTcpSocket::getData()
{
    qDebug() << tcpSocket->canReadLine();
    QByteArray ba = tcpSocket->readLine();
    qDebug() << ba;
    return QString(ba);
}

void NLTcpSocket::write(QString data)
{
    tcpSocket->write(data.toStdString().c_str());
}
