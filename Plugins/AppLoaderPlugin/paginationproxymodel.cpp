#include "paginationproxymodel.h"

PaginationProxyModel::PaginationProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void PaginationProxyModel::invalidateFilter()
{
    QSortFilterProxyModel::invalidateFilter();
}

void PaginationProxyModel::setFilterStrings(const QStringList &filterStrings)
{
    m_filterStrings = filterStrings;
    invalidateFilter();
}

bool PaginationProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_filterStrings.isEmpty())
        return true;
    int countTrue = 0;
    QString sourceData;
    for (int i = 0; i < m_filterStrings.size(); ++i)
    {
        QString sourceData = sourceModel()->data(sourceModel()->index(sourceRow, i, sourceParent)).toString();
        if (sourceData.contains(m_filterStrings.at(i), Qt::CaseInsensitive))
        {
            countTrue++;
        }
    }
    return countTrue == m_filterStrings.size();
}
