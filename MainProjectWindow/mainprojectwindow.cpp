#include "mainprojectwindow.h"
#include "loginform.h"

#include <QtWidgets>
#include <QSqlError>
#include <QFile>
#include <QTreeView>
#include <QHeaderView>
#include <QPluginLoader>


MainProjectWindow::MainProjectWindow(QWidget *parent)
    : QMainWindow(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("newfilter.sqlite");

    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);

    connect(tabWidget->tabBar(), &QTabBar::tabCloseRequested, this, &MainProjectWindow::handleTabCloseRequested);

    setCentralWidget(tabWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    tabWidget->setLayout(mainLayout);

    createStaticActions();
    createStaticMenus();
    menuBar()->clear();

    setWindowTitle(tr("Лаунчер"));
    setMinimumSize(160, 160);
    resize(800, 600);
    userChange();
}

void MainProjectWindow::checkLogin()
{
    db.open();
    QSqlQuery loginQerry;
    QSqlQuery takeRolesQerry;
    QSqlQuery menuQuerry;

    //Take user info from DB and check if login and password are correct
    QString sqlLogin = "select prs_name, prs_lastname from admPersones where prs_login='"+userLogin+"' and prs_password='"+userPass+"'";
    if (!loginQerry.exec(sqlLogin))
    {
        QMessageBox messageBox;
        QString errText = loginQerry.lastError().text();
        messageBox.critical(0,"Error", errText);
    }
    else
    {
        if (loginQerry.next())
        {
            userName = loginQerry.value("prs_lastname").toString() + " " + loginQerry.value("prs_name").toString();
            userMenu->setTitle(userName);
        }
        else
        {
            logErrMsg();
            return;
        }
    }

    //Take roles from DB
    userRoles.clear();
    QString sqlTakeRoles = "select role from admRolePersones join admPersones on admPersones.prs_id = admRolePersones.persone \
            where admPersones.prs_login='"+userLogin+"'";
    if (!takeRolesQerry.exec(sqlTakeRoles))
    {
        //MainProjectWindow::close();
        QMessageBox messageBox;
        QString errText = takeRolesQerry.lastError().text();
        messageBox.critical(0,"Error", errText);
        if (messageBox.exec())
        {
            //закрыть приложение
        }
    }
    while (takeRolesQerry.next())
    {
        userRoles.append(takeRolesQerry.value("role").toString());
    }

    QList<QString>::iterator i;
    userRolesString.clear();
    for (i = userRoles.begin(); i != userRoles.end(); ++i)
    {
        if (std::next(i) != userRoles.end())
        {
            userRolesString += *i + ", ";
        }
        else
        {
            userRolesString += *i;
        }
    }

    //Take menu info from DB
    QString sqlMenuInfo = "select menu_id, menu_title, menu_action, menu_parent from admMenus \
            where menu_id in (select menu from admMenuRoles where role in (#roles#)) \
            order by menu_sorting asc";
    sqlMenuInfo.replace("#roles#", userRolesString);
    parentsList.clear();
    mItemList.clear();
    if (!menuQuerry.exec(sqlMenuInfo))
    {
        QMessageBox messageBox;
        QString errText = menuQuerry.lastError().text();
        messageBox.critical(0,"Error", errText);

    }
    while (menuQuerry.next())
    {
        mItemList.append(mItem {menuQuerry.value("menu_id").toInt(), \
                                menuQuerry.value("menu_parent").toInt(), \
                                menuQuerry.value("menu_title").toString(), \
                                menuQuerry.value("menu_action").toString()});
        parentsList.append(menuQuerry.value("menu_parent").toInt());
    }
    db.close();
}

void MainProjectWindow::createStaticActions()
{
    userChangeAct = new QAction(tr("Сменить пользователя"), this);
    userChangeAct->setStatusTip(tr("Открывает окно логина"));
    exitMenuAct = new QAction(tr("Выход"), this);
    exitMenuAct->setStatusTip(tr("Закрыть приложение"));

    connect(userChangeAct, &QAction::triggered, this, &MainProjectWindow::userChange);
    connect(exitMenuAct, &QAction::triggered, this, &MainProjectWindow::close);
}

void MainProjectWindow::createDynamicMenus(int currentParrent, QMenu *menuParent, QStandardItem *parentItem)
{
    QList<mItem>::iterator menuIter;
    for (menuIter = mItemList.begin(); menuIter != mItemList.end(); ++menuIter)
    {
        int idx = menuIter - mItemList.begin();
        if (mItemList.at(idx).mParent == currentParrent)
        {
            if(parentsList.contains(mItemList.at(idx).mId) and currentParrent == 0)
            {
                QMenu *menu = new QMenu(mItemList.at(idx).mAct);
                menuBar()->addMenu(menu);
                menu->setTitle(mItemList.at(idx).mTitle);

                QStandardItem *modelItem = new QStandardItem(mItemList.at(idx).mTitle);

                parentItem->appendRow(modelItem);
                parentItem = modelItem;

                createDynamicMenus(mItemList.at(idx).mId, menu, parentItem);
                parentItem = myModel->invisibleRootItem();
            }
            else if (parentsList.contains(mItemList.at(idx).mId) and currentParrent != 0)
            {
                QMenu *menu = new QMenu(mItemList.at(idx).mAct);
                menuParent->addMenu(menu);
                menu->setTitle(mItemList.at(idx).mTitle);

                QStandardItem *modelItem = new QStandardItem(mItemList.at(idx).mTitle);
                parentItem->appendRow(modelItem);
                parentItem = modelItem;
                createDynamicMenus(mItemList.at(idx).mId, menu, parentItem);
                parentItem = modelItem->parent();
            }
            else
            {
                QAction *action = new QAction(mItemList.at(idx).mAct);
                menuParent->addAction(action);
                action->setObjectName(mItemList.at(idx).mTitle);
                action->setText(mItemList.at(idx).mTitle);
                connect(action, &QAction::triggered, this, &MainProjectWindow::dynamicActions);

                QStandardItem *modelItem = new QStandardItem(mItemList.at(idx).mTitle);
                parentItem->appendRow(modelItem);

            }
        }
    }
}

void MainProjectWindow::createStaticMenus()
{
    QMenuBar *rMenu = new QMenuBar();
    menuBar()->setCornerWidget(rMenu);
    userMenu = rMenu->addMenu(userName);
    userMenu->addAction(userChangeAct);
    rMenu->addAction(exitMenuAct);
}

void MainProjectWindow::userChange()
{
    LoginForm loginForm;
    if (loginForm.exec() == QDialog::Accepted)
    {
        userLogin = loginForm.logEdit->text();
        userPass = loginForm.pasEdit->text();
        checkLogin();
        menuBar()->clear();
        QStandardItem *parentItem = myModel->invisibleRootItem();
        createDynamicMenus(0, lMenu, parentItem);
    }
    else
    {
        loginForm.close();
    }
    loginForm.deleteLater();
}

void MainProjectWindow::handleTabCloseRequested(int index)
{
    QWidget *widgetToRemove = tabWidget->widget(index);
        if (widgetToRemove) {
            widgetToRemove->deleteLater();
            tabWidget->removeTab(index);
        }
}

void MainProjectWindow::logErrMsg()
{
    QMessageBox loginError;
    loginError.setText("Неверное имя пользовалеля и/или пароль");
    loginError.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    loginError.setInformativeText("Ok - Войти как гость\nCancel - Вернуться на форму логина");
    loginError.setIcon(QMessageBox::Critical);
    if (loginError.exec() == QMessageBox::Cancel)
    {
       userChange();
    }
}

void MainProjectWindow::dynamicActions()
{
    QObject *obj = sender();
    QString appName = obj->objectName();
    QWidget *appWidget = new QWidget;

    if (appName == "Настройка меню")
    {
        admMenuSettings();
    }
    else if (appName == "Пагинация")
    {
        paginationWindow();
    }
    else if (appName == "Пинг")
    {
        pingWindow();
    }
    else
    {
        QLabel *voidText = new QLabel("Когда-нибудь здесь будет приложение '"+appName+"'");

        QHBoxLayout *appLayout = new QHBoxLayout();

        appLayout->addWidget(voidText);
        appWidget->setLayout(appLayout);

        tabWidget->addTab(appWidget, appName);
    }

}

void MainProjectWindow::admMenuSettings()
{
    QWidget *settingsWidget = new QWidget;

    QTreeView *treeView = new QTreeView();
    treeView->header()->hide();
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setColumnHidden(1, true);
    treeView->setColumnHidden(2, true);
    treeView->setColumnHidden(3, true);
    treeView->hideColumn(1);
    treeView->setModel(myModel);

    treeView->setDragDropMode(QAbstractItemView::InternalMove);

    treeView->viewport()->installEventFilter(this);

    QVBoxLayout *appLayout = new QVBoxLayout();
    QPushButton *settingsSaveButton = new QPushButton("Сохранить");

    connect(settingsSaveButton, SIGNAL(clicked()), SLOT(settingsSaveButtonClicked()));

    appLayout->addWidget(treeView);
    appLayout->addWidget(settingsSaveButton);

    settingsWidget->setLayout(appLayout);

    tabWidget->addTab(settingsWidget, "Настройка меню");
}

void MainProjectWindow::paginationWindow()
{
    QPluginLoader loader;
    loader.setFileName("plugins/AppLoaderPlugin");
    PaginationInterface *paginationInterface = qobject_cast<PaginationInterface *>(loader.instance());
    if (paginationInterface)
    {
        QObject *paginationObject = paginationInterface->getPaginationWidget();
        QWidget *paginationWidget = qobject_cast<QWidget*>(paginationObject);

        tabWidget->addTab(paginationWidget, "Пагинация");
    } else
    {
        qDebug() << loader.errorString();
    }

}

void MainProjectWindow::pingWindow()
{
    QPluginLoader loader;
    loader.setFileName("plugins/AppLoaderPlugin");
    PingInterface *pingInterface = qobject_cast<PingInterface *>(loader.instance());
    if (pingInterface)
    {
        QObject *pingObject = pingInterface->getPingWidget();
        QWidget *pingWidget = qobject_cast<QWidget*>(pingObject);

        tabWidget->addTab(pingWidget, "Пинг");
    } else
    {
        qDebug() << loader.errorString();
    }

}

void MainProjectWindow::traverseTreeModel(const QModelIndex &parentIndex, QStandardItemModel *model, QSqlQuery &settingsSaveQuery, int &count)
{
    for (int row = 0; row < model->rowCount(parentIndex); ++row)
    {
        QModelIndex index = model->index(row, 0, parentIndex);
        QString dataValue = index.data().toString();

        settingsSaveQuery.prepare("UPDATE admMenus SET menu_sorting = :count WHERE menu_title = :dataValue");
        settingsSaveQuery.bindValue(":count", count);
        settingsSaveQuery.bindValue(":dataValue", dataValue);

        if (!settingsSaveQuery.exec())
        {
            qDebug() << "Error executing query:" << settingsSaveQuery.lastError().text();
        }

        count++;
        traverseTreeModel(index, model, settingsSaveQuery, count);
    }
}

void MainProjectWindow::settingsSaveButtonClicked()
{
    QMessageBox::information(nullptr, "Информация", "Настройки будут применены после перезапуска приложения.");

    db.open();
    QSqlQuery settingsSaveQuery;
    QModelIndex rootIndex = myModel->invisibleRootItem()->index();
    int count = 1;
    traverseTreeModel(rootIndex, myModel, settingsSaveQuery, count);
    db.close();
}

bool MainProjectWindow::onDropEventFilter(QObject *obj, QEvent *event, QTreeView &treeView)
{
    if (obj == treeView.viewport() && event->type() == QEvent::Drop)
    {
        // Преобразуем событие в QDropEvent
        QDropEvent *dropEvent = static_cast<QDropEvent*>(event);

        // Получаем индекс перетащенного элемента
        QModelIndex parentIndex = treeView.indexAt(dropEvent->pos());
        int row = parentIndex.row();

        // Удаляем перетащенный элемент из treeView и модели
        if (row != -1)
        {
            treeView.model()->removeRow(row, parentIndex.parent());
        }

        return true; // Перехватили событие, обработали и вернули true
    }

    return QMainWindow::eventFilter(obj, event);
}

MainProjectWindow::~MainProjectWindow()
{

}


