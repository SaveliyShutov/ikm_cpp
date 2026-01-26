#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QList>
#include <QVariantMap>
#include "LogEntity.h"

class LogRepository : public QObject {
    Q_OBJECT
public:
    explicit LogRepository(QSqlDatabase& database, QObject* parent = nullptr);
    
    QList<LogEntity> getAll();
    LogEntity getById(const QString& userId, const QString& habitId, const QString& logDate);
    bool create(const LogEntity& log);
    bool update(const LogEntity& log);
    bool remove(const QString& userId, const QString& habitId, const QString& logDate);
    
    QString getLastError() const { return lastError; }
    
private:
    QSqlDatabase& db;
    QString lastError;
    
    void setLastError(const QSqlQuery& query);
};