#ifndef APPLOADERPLUGIN_H
#define APPLOADERPLUGIN_H

#include <QObject>
#include "Interfaces.h"
#include "pingwidget.h"
#include "paginationwidget.h"

class AppLoaderPlugin : public QObject,
                        public PingInterface,
                        public PaginationInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.PaginationInterface/1.0" FILE "AppLoaderPlugin.json")
    Q_INTERFACES(PingInterface PaginationInterface)

public:
    explicit AppLoaderPlugin(QObject *parent = nullptr);
    ~AppLoaderPlugin();

public:
    QObject *getPaginationWidget() override;
    QObject *getPingWidget() override;

};

#endif // APPLOADERPLUGIN_H
