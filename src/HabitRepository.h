#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QList>
#include <QVariantMap>
#include "HabitEntity.h"

class HabitRepository : public QObject {
    Q_OBJECT
public:
    explicit HabitRepository(QSqlDatabase& database, QObject* parent = nullptr);
    
    QList<HabitEntity> getAll();
    HabitEntity getById(const QString& id);
    bool create(const HabitEntity& habit);
    bool update(const HabitEntity& habit);
    bool remove(const QString& id);
    
    QString getLastError() const { return lastError; }
    
private:
    QSqlDatabase& db;
    QString lastError;
    
    void setLastError(const QSqlQuery& query);
};