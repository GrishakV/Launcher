#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class LoginForm : public QDialog
{
    friend class MainProjectWindow;
    Q_OBJECT
public:
    explicit LoginForm(QDialog *parent = nullptr);
    ~LoginForm();

private:
    QLabel *logLabel;
    QLabel *pasLabel;
    QLineEdit *logEdit;
    QLineEdit *pasEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // LOGINFORM_H
