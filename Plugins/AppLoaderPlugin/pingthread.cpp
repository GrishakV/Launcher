#include <QCoreApplication>
#include "pingthread.h"

PingThread::PingThread(QObject *parent) : QThread(parent)
{
    connect(&ping, &Ping::pingResult, this, &PingThread::pingResult);
}

void PingThread::run()
{
    qDebug() << "Thread started";
    exec();
    qDebug() << "Thread finished";

}

void PingThread::sendPing(const QString &host)
{
    qDebug() << "sendingping";
    ping.sendPing(host);
}

PingThread::~PingThread()
{
    if(ping.thread())
    {
        ping.moveToThread(QCoreApplication::instance()->thread());
    }
}
