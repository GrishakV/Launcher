#include "paginationwidget.h"
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>

PaginationWidget::PaginationWidget(QWidget *parent)
    : QWidget(parent), currentPage(0), itemsPerPage(50)
{
    paginationDB = QSqlDatabase::addDatabase("QSQLITE");
    paginationDB.setDatabaseName("pagination.sqlite");

    if (!paginationDB.open())
    {
        qWarning() << "Cannot open DB" << paginationDB.lastError().text();
    }

    paginationModel = new QStandardItemModel(this);
    QSqlQuery paginationQuery("SELECT * FROM peoples;");
    int rowCount = 0;

    while (paginationQuery.next())
    {
        QString sername = paginationQuery.value(0).toString();
        QString name = paginationQuery.value(1).toString();
        QString secondName = paginationQuery.value(2).toString();
        QString gender = paginationQuery.value(3).toString();
        int age = paginationQuery.value(4).toInt();
        QString jobTitle = paginationQuery.value(5).toString();

        QStandardItem *sernameItem = new QStandardItem(sername);
        QStandardItem *nameItem = new QStandardItem(name);
        QStandardItem *secondNameItem = new QStandardItem(secondName);
        QStandardItem *genderItem = new QStandardItem(gender);
        QStandardItem *ageItem = new QStandardItem();
        ageItem->setData(age, Qt::DisplayRole);
        QStandardItem *jobTitleItem = new QStandardItem(jobTitle);

        paginationModel->setItem(rowCount, 0, sernameItem);
        paginationModel->setItem(rowCount, 1, nameItem);
        paginationModel->setItem(rowCount, 2, secondNameItem);
        paginationModel->setItem(rowCount, 3, genderItem);
        paginationModel->setItem(rowCount, 4, ageItem);
        paginationModel->setItem(rowCount, 5, jobTitleItem);

        rowCount++;
    }

    paginationDB.close();

    paginationTableView = new QTableView(this);
    proxyModel = new PaginationProxyModel(this);
    proxyModel->setSourceModel(paginationModel);
    proxyModel->setHeaderData(0, Qt::Horizontal, "Фамилия");
    proxyModel->setHeaderData(1, Qt::Horizontal, "Имя");
    proxyModel->setHeaderData(2, Qt::Horizontal, "Отчество");
    proxyModel->setHeaderData(3, Qt::Horizontal, "Пол");
    proxyModel->setHeaderData(4, Qt::Horizontal, "Возраст");
    proxyModel->setHeaderData(5, Qt::Horizontal, "Профессия");

    paginationTableView->setModel(proxyModel);
    paginationTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setupUI();
    updateTableView();
}

void PaginationWidget::updateTableView()
{
    QElapsedTimer timer;
    timer.start();

    int firstRow = currentPage * itemsPerPage;
    int lastRow = qMin(firstRow + itemsPerPage - 1, proxyModel->rowCount() - 1);

    QtConcurrent::run([this, firstRow, lastRow]()
    {
        for (int row = 0; row < proxyModel->rowCount(); ++row)
        {
            bool isVisible = (row >= firstRow && row <= lastRow);
            paginationTableView->setRowHidden(row, !isVisible);
        }

        // Update UI elements in the main thread
        QMetaObject::invokeMethod(this, &PaginationWidget::updatePageCount, Qt::QueuedConnection);
    });
    qDebug() << "Completed, time spent: "<< timer.elapsed() << "ms";
}

void PaginationWidget::goToPageByLineEdit()
{
    bool ok;
    int page = pageLineEdit->text().toInt(&ok);

    if (ok && page > 0 && page <= totalPages)
        goToPage(page - 1);
}

void PaginationWidget::applyFilters()
{
    proxyModel->invalidateFilter();

    QStringList filters;

    if (!sernameFilterEdit->text().isEmpty())
        filters.append(QString(sernameFilterEdit->text()));
    else
        filters.append(QString(""));

    if (!nameFilterEdit->text().isEmpty())
        filters.append(QString(nameFilterEdit->text()));
    else
        filters.append(QString(""));

    if (!secondNameFilterEdit->text().isEmpty())
        filters.append(QString(secondNameFilterEdit->text()));
    else
        filters.append(QString(""));

    if (!genderFilterEdit->text().isEmpty())
        filters.append(QString(genderFilterEdit->text()));
    else
        filters.append(QString(""));

    if (!ageFilterEdit->text().isEmpty())
        filters.append(QString(ageFilterEdit->text()));
    else
        filters.append(QString(""));

    if (!jobTitleFilterEdit->text().isEmpty())
        filters.append(QString(jobTitleFilterEdit->text()));
    else
        filters.append(QString(""));

    proxyModel->setFilterStrings(filters);
    goToPage(0);
}

void PaginationWidget::setupUI()
{
    sernameFilterEdit = new QLineEdit(this);
    nameFilterEdit = new QLineEdit(this);
    secondNameFilterEdit = new QLineEdit(this);
    genderFilterEdit = new QLineEdit(this);
    ageFilterEdit = new QLineEdit(this);
    jobTitleFilterEdit = new QLineEdit(this);
    prevButton = new QPushButton("Назад", this);
    nextButton = new QPushButton("Вперед", this);
    goToButton = new QPushButton("Перейти", this);
    pageCountLabel = new QLabel(this);
    pageLineEdit = new QLineEdit(this);
    pageLineEdit->setMaximumWidth(60);

    connect(sernameFilterEdit, &QLineEdit::returnPressed, this, &PaginationWidget::applyFilters);
    connect(nameFilterEdit, &QLineEdit::returnPressed, this, &PaginationWidget::applyFilters);
    connect(secondNameFilterEdit, &QLineEdit::returnPressed, this, &PaginationWidget::applyFilters);
    connect(genderFilterEdit, &QLineEdit::returnPressed, this, &PaginationWidget::applyFilters);
    connect(ageFilterEdit, &QLineEdit::returnPressed, this, &PaginationWidget::applyFilters);
    connect(jobTitleFilterEdit, &QLineEdit::returnPressed, this, &PaginationWidget::applyFilters);
    connect(prevButton, &QPushButton::clicked, this, [this]() { goToPage(currentPage - 1); });
    connect(nextButton, &QPushButton::clicked, this, [this]() { goToPage(currentPage + 1); });
    connect(pageLineEdit, &QLineEdit::returnPressed, this, &PaginationWidget::goToPageByLineEdit);
    connect(goToButton, &QPushButton::clicked, this, &PaginationWidget::goToPageByLineEdit);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QHBoxLayout *filterLayout = new QHBoxLayout();

    filterLayout->addWidget(sernameFilterEdit);
    filterLayout->addWidget(nameFilterEdit);
    filterLayout->addWidget(secondNameFilterEdit);
    filterLayout->addWidget(genderFilterEdit);
    filterLayout->addWidget(ageFilterEdit);
    filterLayout->addWidget(jobTitleFilterEdit);

    layout->addLayout(filterLayout);
    layout->addWidget(paginationTableView);

    horizontalLayout->addWidget(prevButton);
    horizontalLayout->addWidget(nextButton);
    horizontalLayout->addWidget(pageCountLabel);
    horizontalLayout->addWidget(pageLineEdit);
    horizontalLayout->addWidget(goToButton);

    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout->addItem(spacer);

    layout->addLayout(horizontalLayout);

    setLayout(layout);
}

void PaginationWidget::goToPage(int page)
{
    if (page < 0 || page > totalPages)
        return;

    currentPage = page;
    updateTableView();
}

void PaginationWidget::updatePageCount()
{
    totalPages = proxyModel->rowCount() / itemsPerPage + ((proxyModel->rowCount() % itemsPerPage > 0) ? 1 : 0);
    QString pageCountText = QString("Страница %1 из %2").arg(currentPage + 1).arg(totalPages);
    pageCountLabel->setText(pageCountText);

    prevButton->setEnabled(currentPage > 0);
    nextButton->setEnabled(currentPage < totalPages - 1);

    pageLineEdit->setValidator(new QIntValidator(1, totalPages, this));
}

PaginationWidget::~PaginationWidget()
{
    if (paginationTableView)
        delete paginationTableView;
    if (paginationModel)
        delete paginationModel;
    if (proxyModel)
        delete proxyModel;
}
