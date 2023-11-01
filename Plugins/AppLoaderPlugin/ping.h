#ifndef PING_H
#define PING_H

#include <QObject>
#include <QTcpSocket>

class Ping : public QObject
{
    Q_OBJECT

public:
    explicit Ping(QObject *parent = nullptr);

public slots:
    static QString sendPing(const QString &host);
    void onReadyRead();

private:
    QTcpSocket tcpSocket;
};

#endif // PING_H
