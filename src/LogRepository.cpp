#include "LogRepository.h"
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

LogRepository::LogRepository(QSqlDatabase& database, QObject* parent) 
    : QObject(parent), db(database) {}

QList<LogEntity> LogRepository::getAll() {
    QList<LogEntity> logs;
    QSqlQuery query(db);
    
    if (!query.exec("SELECT user_id, habit_id, to_char(log_date, 'YYYY-MM-DD') as log_date, "
                    "is_completed, completion_time, notes, created_at "
                    "FROM habit_log ORDER BY log_date DESC")) {
        setLastError(query);
        return logs;
    }
    
    while (query.next()) {
        QVariantMap map;
        for (int i = 0; i < query.record().count(); ++i) {
            map[query.record().fieldName(i)] = query.value(i);
        }
        logs.append(LogEntity(map));
    }
    
    return logs;
}

LogEntity LogRepository::getById(const QString& userId, const QString& habitId, const QString& logDate) {
    QSqlQuery query(db);
    query.prepare("SELECT user_id, habit_id, to_char(log_date, 'YYYY-MM-DD') as log_date, "
                  "is_completed, completion_time, notes, created_at "
                  "FROM habit_log WHERE user_id = :uid AND habit_id = :hid AND log_date = :date");
    query.bindValue(":uid", userId);
    query.bindValue(":hid", habitId);
    query.bindValue(":date", logDate);
    
    if (!query.exec() || !query.next()) {
        setLastError(query);
        return LogEntity();
    }
    
    QVariantMap map;
    for (int i = 0; i < query.record().count(); ++i) {
        map[query.record().fieldName(i)] = query.value(i);
    }
    
    return LogEntity(map);
}

bool LogRepository::create(const LogEntity& log) {
    QSqlQuery query(db);
    
    query.prepare("INSERT INTO habit_log(user_id, habit_id, log_date, is_completed, completion_time, notes) "
                  "VALUES(:uid, :hid, :date, :done, :time, :notes)");
    
    query.bindValue(":uid", log.user_id);
    query.bindValue(":hid", log.habit_id);
    query.bindValue(":date", log.log_date);
    query.bindValue(":done", log.is_completed);
    query.bindValue(":time", log.completion_time);
    query.bindValue(":notes", log.notes);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return true;
}

bool LogRepository::update(const LogEntity& log) {
    QSqlQuery query(db);
    
    query.prepare("UPDATE habit_log SET is_completed = :done, completion_time = :time, notes = :notes "
                  "WHERE user_id = :uid AND habit_id = :hid AND log_date = :date");
    
    query.bindValue(":done", log.is_completed);
    query.bindValue(":time", log.completion_time);
    query.bindValue(":notes", log.notes);
    query.bindValue(":uid", log.user_id);
    query.bindValue(":hid", log.habit_id);
    query.bindValue(":date", log.log_date);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

bool LogRepository::remove(const QString& userId, const QString& habitId, const QString& logDate) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM habit_log WHERE user_id = :uid AND habit_id = :hid AND log_date = :date");
    query.bindValue(":uid", userId);
    query.bindValue(":hid", habitId);
    query.bindValue(":date", logDate);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

void LogRepository::setLastError(const QSqlQuery& query) {
    lastError = query.lastError().text();
}