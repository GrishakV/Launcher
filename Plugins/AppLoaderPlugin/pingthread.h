#ifndef PINGTHREAD_H
#define PINGTHREAD_H

#include <QObject>
#include <QThread>
#include "ping.h"

class PingThread : public QThread
{
    Q_OBJECT

public:
    explicit PingThread(QObject *parent = nullptr);
    ~PingThread();

    void run() override;

public slots:
    void sendPing(const QString &host);

signals:
    void pingResult(const QString &result);

private:
    Ping ping;

};

#endif // PINGTHREAD_H
