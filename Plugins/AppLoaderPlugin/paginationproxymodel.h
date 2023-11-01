#ifndef PAGINATIONPROXYMODEL_H
#define PAGINATIONPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class PaginationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PaginationProxyModel(QObject *parent = nullptr);
    void invalidateFilter();
    void setFilterStrings(const QStringList &filterStrings);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QStringList m_filterStrings;
};

#endif // PAGINATIONPROXYMODEL_H
