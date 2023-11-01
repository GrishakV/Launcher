#include "mainprojectwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //qputenv("QT_DEBUG_PLUGINS", QByteArray("1")); //Отслеживание загрузки плагинов

    QApplication a(argc, argv);
    MainProjectWindow w;
    w.show();
    return a.exec();
}
