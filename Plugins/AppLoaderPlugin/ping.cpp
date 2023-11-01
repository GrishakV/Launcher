#include "ping.h"
#include <QHostInfo>
#include <QDebug>

Ping::Ping(QObject *parent) : QObject(parent)
{
    connect(&tcpSocket, &QTcpSocket::readyRead, this, &Ping::onReadyRead);
}

QString Ping::sendPing(const QString &host)
{
    QString msg;
    QTcpSocket tcpSocket;
    tcpSocket.abort(); // Abort any existing connections
    QHostInfo hostInfo = QHostInfo::fromName(host);

    if (!hostInfo.addresses().isEmpty())
    {
        QHostAddress address = hostInfo.addresses().first();
        tcpSocket.connectToHost(address, 80); // Use port 80 for ping

        if (tcpSocket.waitForConnected())
        {
            msg = "Ping successful to " + host + " IP: " + address.toString();
            //qDebug() << "Ping successful to" << host << "IP:" << address.toString();
        }
        else
        {
            msg = "Ping failed to " + host + " IP: " + address.toString();
            //qDebug() << "Ping failed to" << host << "IP:" << address.toString();
        }
    }
    else
    {
        msg =  "Could not resolve host: " + host;
        //qDebug() << "Could not resolve host:" << host;
    }
    return msg;
}

void Ping::onReadyRead()
{
    QByteArray response = tcpSocket.readAll();
    qDebug() << "Response:" << response;
}
