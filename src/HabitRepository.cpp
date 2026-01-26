#include "HabitRepository.h"
#include <QSqlRecord> 
#include <QSqlError>
#include <QDebug>

HabitRepository::HabitRepository(QSqlDatabase& database, QObject* parent) 
    : QObject(parent), db(database) {}

QList<HabitEntity> HabitRepository::getAll() {
    QList<HabitEntity> habits;
    QSqlQuery query(db);
    
    if (!query.exec("SELECT habit_id, name, description, category, frequency, reminder_time, created_at "
                    "FROM habit_templates ORDER BY name")) {
        setLastError(query);
        return habits;
    }
    
    while (query.next()) {
        QVariantMap map;
        for (int i = 0; i < query.record().count(); ++i) {
            map[query.record().fieldName(i)] = query.value(i);
        }
        habits.append(HabitEntity(map));
    }
    
    return habits;
}

HabitEntity HabitRepository::getById(const QString& id) {
    QSqlQuery query(db);
    query.prepare("SELECT habit_id, name, description, category, frequency, reminder_time, created_at "
                  "FROM habit_templates WHERE habit_id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec() || !query.next()) {
        setLastError(query);
        return HabitEntity();
    }
    
    QVariantMap map;
    for (int i = 0; i < query.record().count(); ++i) {
        map[query.record().fieldName(i)] = query.value(i);
    }
    
    return HabitEntity(map);
}

bool HabitRepository::create(const HabitEntity& habit) {
    QSqlQuery query(db);
    
    if (!habit.habit_id.isEmpty()) {
        query.prepare("INSERT INTO habit_templates(habit_id, name, description, category, frequency, reminder_time) "
                      "VALUES(:id, :name, :desc, :cat, :freq, :rem)");
        query.bindValue(":id", habit.habit_id);
    } else {
        query.prepare("INSERT INTO habit_templates(name, description, category, frequency, reminder_time) "
                      "VALUES(:name, :desc, :cat, :freq, :rem)");
    }
    
    query.bindValue(":name", habit.name);
    query.bindValue(":desc", habit.description);
    query.bindValue(":cat", habit.category);
    query.bindValue(":freq", habit.frequency);
    query.bindValue(":rem", habit.reminder_time);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return true;
}

bool HabitRepository::update(const HabitEntity& habit) {
    QSqlQuery query(db);
    
    query.prepare("UPDATE habit_templates SET name = :name, description = :desc, "
                  "category = :cat, frequency = :freq, reminder_time = :rem "
                  "WHERE habit_id = :id");
    
    query.bindValue(":name", habit.name);
    query.bindValue(":desc", habit.description);
    query.bindValue(":cat", habit.category);
    query.bindValue(":freq", habit.frequency);
    query.bindValue(":rem", habit.reminder_time);
    query.bindValue(":id", habit.habit_id);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

bool HabitRepository::remove(const QString& id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM habit_templates WHERE habit_id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

void HabitRepository::setLastError(const QSqlQuery& query) {
    lastError = query.lastError().text();
}