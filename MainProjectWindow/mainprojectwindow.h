#ifndef MAINPROJECTWINDOW_H
#define MAINPROJECTWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTreeView>
#include <QMessageBox>
#include <QStandardItemModel>

#include "Interfaces.h"

QT_BEGIN_NAMESPACE
class QMessageBox;
class QMenu;
class QAction;
class QTableView;
QT_END_NAMESPACE

class MainProjectWindow : public QMainWindow
{
    Q_OBJECT

    friend class LoginForm;

public:
    QSqlDatabase db;
    MainProjectWindow(QWidget *parent = nullptr);
    ~MainProjectWindow();

private slots:
    void userChange();
    void handleTabCloseRequested(int index);
    void settingsSaveButtonClicked();

private:
    void dynamicActions();
    void createStaticActions();
    void createStaticMenus();
    void createDynamicMenus(int currentParrent, QMenu *menuParent, QStandardItem *parentItem);
    void checkLogin();
    void logErrMsg();
    void admMenuSettings();
    void paginationWindow();
    void pingWindow();
    void traverseTreeModel(const QModelIndex &parentIndex, QStandardItemModel *model, QSqlQuery &settingsSaveQuery, int &count);


protected:
    bool onDropEventFilter(QObject* obj, QEvent* event, QTreeView &treeView);


private:
    QStandardItemModel *myModel = new QStandardItemModel;
    QVector<int> parentsList;
    QMap<int, int> menuIdPar;
    QList<QString> userRoles;
    QString userLogin;
    QString userName = "Гость";
    QString userPass;
    QString userRolesString;
    QMenu *lMenu;
    QMenu *userMenu;
    QAction *exitMenuAct;
    QAction *userChangeAct;
    QTabWidget *tabWidget;

private:
    struct mItem
    {
      int mId;
      int mParent;
      QString mTitle;
      QString mAct;
    };

    QList<mItem> mItemList;
};



#endif // MAINPROJECTWINDOW_H
