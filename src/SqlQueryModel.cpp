#include "SqlQueryModel.h"
#include "DatabaseManager.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

SqlQueryModel::SqlQueryModel(QObject *parent) : QSqlQueryModel(parent) {}

void SqlQueryModel::refresh(const QString &query) {
  QSqlQuery q(DatabaseManager::instance().database());
  if (!q.exec(query)) {
    qWarning() << "SqlQueryModel::refresh() - query failed:"
               << q.lastError().text() << "\nSQL:" << query;
    this->clear();
    return;
  }
  this->setQuery(std::move(q));
  generateRoleNames();
}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DisplayRole)
    return QSqlQueryModel::data(index, role);

  if (m_roleNames.contains(role)) {
    int col = -1;
    QByteArray roleName = m_roleNames.value(role);
    QSqlRecord rec = record();
    for (int i = 0; i < rec.count(); ++i) {
      if (rec.fieldName(i).toUtf8() == roleName) {
        col = i;
        break;
      }
    }
    if (col >= 0)
      return QSqlQueryModel::data(this->index(index.row(), col),
                                  Qt::DisplayRole);
  }
  return QVariant();
}

void SqlQueryModel::generateRoleNames() {
  m_roleNames.clear();
  QSqlRecord rec = record();
  for (int i = 0; i < rec.count(); ++i) {
    int role = Qt::UserRole + 1 + i;
    m_roleNames[role] = rec.fieldName(i).toUtf8();
  }
  emit layoutChanged();
}

QHash<int, QByteArray> SqlQueryModel::roleNames() const { return m_roleNames; }