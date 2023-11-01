#ifndef PAGINATIONWIDGET_H
#define PAGINATIONWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "paginationproxymodel.h"

class PaginationWidget : public QWidget
{
    Q_OBJECT

public:
    PaginationWidget(QWidget *parent = nullptr);
    ~PaginationWidget();

private slots:
    void goToPage(int page);
    void updateTableView();
    void goToPageByLineEdit();
    void applyFilters();

private:
    void setupUI();
    void updatePageCount();

    QSqlDatabase paginationDB;
    QStandardItemModel *paginationModel;
    PaginationProxyModel *proxyModel;
    QTableView *paginationTableView;
    int currentPage;
    int itemsPerPage;
    int totalItems;
    int totalPages;
    QLabel *pageCountLabel;
    QLineEdit *pageLineEdit;
    QLineEdit *sernameFilterEdit;
    QLineEdit *nameFilterEdit;
    QLineEdit *secondNameFilterEdit;
    QLineEdit *genderFilterEdit;
    QLineEdit *ageFilterEdit;
    QLineEdit *jobTitleFilterEdit;
    QPushButton *goToButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
};
#endif // PAGINATIONWIDGET_H
