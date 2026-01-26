#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QList>
#include <QVariantMap>
#include "UserEntity.h"

class UserRepository : public QObject {
    Q_OBJECT
public:
    explicit UserRepository(QSqlDatabase& database, QObject* parent = nullptr);
    
    QList<UserEntity> getAll();
    UserEntity getById(const QString& id);
    bool create(const UserEntity& user);
    bool update(const UserEntity& user);
    bool remove(const QString& id);
    
    QString getLastError() const { return lastError; }
    
private:
    QSqlDatabase& db;
    QString lastError;
    
    void setLastError(const QSqlQuery& query);
};