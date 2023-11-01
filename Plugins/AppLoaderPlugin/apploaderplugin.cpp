#include "apploaderplugin.h"

AppLoaderPlugin::AppLoaderPlugin(QObject *parent)
    : QObject(parent)
{
}

QObject *AppLoaderPlugin::getPaginationWidget()
{
    PaginationWidget *paginationWidget = new PaginationWidget;
    return qobject_cast<QObject*>(paginationWidget);
}

QObject *AppLoaderPlugin::getPingWidget()
{
    PingWidget *pingWidget = new PingWidget;
    return qobject_cast<QObject*>(pingWidget);
}

AppLoaderPlugin::~AppLoaderPlugin()
{

}
