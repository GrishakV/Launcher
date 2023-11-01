#include "pingwidget.h"
#include <QtConcurrent/QtConcurrent>

PingWidget::PingWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    label = new QLabel(this);
    label->setText("Введите адрес:");
    hostName = new QLineEdit(this);
    sendPingBtn = new QPushButton(this);
    sendPingBtn->setText("Пинг");
    sendPingBtn->setMaximumWidth(50);
    console = new QPlainTextEdit(this);
    console->setReadOnly(true);

    connect(hostName, &QLineEdit::returnPressed, this, &PingWidget::ping);
    connect(sendPingBtn, &QPushButton::clicked, this, &PingWidget::ping);

    horizontalLayout->addWidget(label);
    horizontalLayout->addWidget(hostName);
    horizontalLayout->addWidget(sendPingBtn);
    layout->addLayout(horizontalLayout);
    layout->addWidget(console);

    setLayout(layout);
}

void PingWidget::ping()
{    
    QString host = hostName->text();

    // Запускаем функцию sendPing в отдельном потоке
    QFuture<QString> future = QtConcurrent::run(Ping::sendPing, host);
    // Ожидаем завершения выполнения и получаем результат
    future.waitForFinished();
    QString result = future.result();
    console->appendPlainText(result);
}

PingWidget::~PingWidget()
{
}




