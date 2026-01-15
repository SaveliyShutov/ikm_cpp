#pragma once
#include <QSqlQueryModel>
#include <QHash>

class SqlQueryModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit SqlQueryModel(QObject* parent = nullptr);

    Q_INVOKABLE void refresh(const QString& query);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

protected:
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
};
