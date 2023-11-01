#ifndef PINGWIDGET_H
#define PINGWIDGET_H

#include "ping.h"

#include <QWidget>
#include <QtWidgets>

class PingWidget : public QWidget
{
    Q_OBJECT

public:
    PingWidget(QWidget *parent = nullptr);
    ~PingWidget();

private slots:
    void ping();

private:
    QLineEdit *hostName;
    QPushButton *sendPingBtn;
    QLabel *label;
    QPlainTextEdit *console;
};
#endif // PINGWIDGET_H
