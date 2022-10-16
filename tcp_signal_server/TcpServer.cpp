#include "TcpServer.h"
#include <QProcess>
#include <QString>
#include <QtGui>
#include "NLTcpSocket.h"

#include <cmath>
#include <iostream>

#define PORT 6000

double signalFunction(const double x)
{
    return sin(x) * sin(10 * x) * sin(0.1 * x);
}

TcpServer::TcpServer(QObject* parent) : QObject(parent)
{
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    if (server->listen(QHostAddress::Any, PORT))
    {
        qDebug() << "Server Started";
    }
    else
    {
        qDebug() << "Server Could Not be Started";
    }
}

TcpServer::~TcpServer() {}

void TcpServer::slotNewConnection()
{
    qDebug() << "Connected";
    QTcpSocket* socket = server->nextPendingConnection();
    NLTcpSocket* customSocket = new NLTcpSocket(socket);
    socket->write("Signal delivery server");
    connect(customSocket, SIGNAL(dataReady(NLTcpSocket*)), this,
            SLOT(slotReceive(NLTcpSocket*)));
    connect(customSocket, SIGNAL(socketDisconnected(NLTcpSocket*)), this,
            SLOT(slotDisconnectSocket(NLTcpSocket*)));
}

void TcpServer::slotReceive(NLTcpSocket* socket)
{
    const QString captured_string = socket->getData();
    qDebug() << captured_string;
    QRegExp command("^from (.*) to (.*) count (\\d+.*)");
    const int pos = command.indexIn(captured_string);

    const QStringList list = command.capturedTexts();
    if (pos < 0)
    {
        qDebug() << "parsing failed\n";
        return;
    }
    const double from = list[1].toDouble();
    const double to = list[2].toDouble();
    const int count = list[3].toInt();
    qDebug() << from << " " << to << " " << count << "\n";

    socket->write("now ");
    socket->write(
        QDateTime::currentDateTime().toString("dd MMMM yyyy@HH:mm:ss.zzz"));
    socket->write(" data ");
    const auto now_msec = QDateTime::currentMSecsSinceEpoch();
    for (int i = 0; i < count; ++i)
    {
        const double delta = from + i * (to - from) / (count - 1);
        const double arg = 0.001 * now_msec + delta;
        socket->write(QString::number(delta));
        socket->write(" ");
        socket->write(QString::number(signalFunction(arg)));
        socket->write(" ");
    }
}

void TcpServer::slotDisconnectSocket(NLTcpSocket* socket)
{
    delete socket;
    qDebug() << "Disconnected";
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QString executablePath = QCoreApplication::applicationDirPath();
    QDir::setCurrent(executablePath);
    TcpServer tcpServer;
    return app.exec();
}
