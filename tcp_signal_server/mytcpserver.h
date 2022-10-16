#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H


class MyTcpServer : public QObject
{
Q_OBJECT

private:

    QLocalServer *server;
    QLocalSocket *socket;

    QList<QLocalSocket*> *list;

    QSignalMapper *mapper;

public:
    explicit MyTcpServer(QObject *parent = nullptr);

//private slots:

//    void slotNewConnection();
//    void slotReadyRead(int index);
};

#endif // MYTCPSERVER_H
