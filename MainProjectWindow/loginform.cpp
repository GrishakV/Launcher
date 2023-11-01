#include "loginform.h"

#include <QtWidgets>
#include <QGridLayout>

LoginForm::LoginForm(QDialog *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    setWindowTitle(tr("Авторизация"));
    logLabel = new QLabel(tr("Логин:"));
    logEdit = new QLineEdit();
    pasLabel = new QLabel(tr("Пароль:"));
    pasEdit = new QLineEdit();
    okButton = new QPushButton(tr("Войти"));
    cancelButton = new QPushButton(tr("Войти как Гость"));

    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    logLabel->setBuddy(logEdit);
    pasLabel->setBuddy(pasEdit);

    pasEdit->setEchoMode(QLineEdit::Password);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(logLabel, 0, 0);
    layout->addWidget(logEdit, 0, 1);
    layout->addWidget(pasLabel, 1, 0);
    layout->addWidget(pasEdit, 1, 1);
    layout->addWidget(okButton, 2, 0);
    layout->addWidget(cancelButton, 2, 1);
    setLayout(layout);
}

LoginForm::~LoginForm()
{

}
