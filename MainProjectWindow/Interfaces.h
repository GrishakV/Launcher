#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>

class PaginationInterface
{
public:
    virtual ~PaginationInterface() {}
    virtual QObject *getPaginationWidget() = 0;
};

class PingInterface
{
public:
    virtual ~PingInterface() {}
    virtual QObject *getPingWidget() = 0;
};

#define PaginationInterface_iid "com.example.PaginationInterface/1.0"
Q_DECLARE_INTERFACE(PaginationInterface, PaginationInterface_iid)

#define PingInterface_iid "com.example.PingInterface/1.0"
Q_DECLARE_INTERFACE(PingInterface, PingInterface_iid)


#endif // INTERFACES_H
