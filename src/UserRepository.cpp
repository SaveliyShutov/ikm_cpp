#include "UserRepository.h"
#include <QSqlRecord> 
#include <QSqlError>
#include <QDebug>

UserRepository::UserRepository(QSqlDatabase& database, QObject* parent) 
    : QObject(parent), db(database) {}

QList<UserEntity> UserRepository::getAll() {
    QList<UserEntity> users;
    QSqlQuery query(db);
    
    if (!query.exec("SELECT user_id, name, email, to_char(registration_date, 'YYYY-MM-DD') as registration_date, "
                    "role, to_char(created_at, 'YYYY-MM-DD HH24:MI:SS') as created_at "
                    "FROM users ORDER BY name")) {
        setLastError(query);
        return users;
    }
    
    while (query.next()) {
        QVariantMap map;
        for (int i = 0; i < query.record().count(); ++i) {
            map[query.record().fieldName(i)] = query.value(i);
        }
        users.append(UserEntity(map));
    }
    
    return users;
}

UserEntity UserRepository::getById(const QString& id) {
    QSqlQuery query(db);
    query.prepare("SELECT user_id, name, email, to_char(registration_date, 'YYYY-MM-DD') as registration_date, "
                  "role, to_char(created_at, 'YYYY-MM-DD HH24:MI:SS') as created_at "
                  "FROM users WHERE user_id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec() || !query.next()) {
        setLastError(query);
        return UserEntity();
    }
    
    QVariantMap map;
    for (int i = 0; i < query.record().count(); ++i) {
        map[query.record().fieldName(i)] = query.value(i);
    }
    
    return UserEntity(map);
}

bool UserRepository::create(const UserEntity& user) {
    QSqlQuery query(db);
    
    if (!user.user_id.isEmpty()) {
        query.prepare("INSERT INTO users(user_id, name, email, registration_date, role) "
                      "VALUES(:id, :name, :email, :reg, :role)");
        query.bindValue(":id", user.user_id);
    } else {
        query.prepare("INSERT INTO users(name, email, registration_date, role) "
                      "VALUES(:name, :email, :reg, :role)");
    }
    
    query.bindValue(":name", user.name);
    query.bindValue(":email", user.email);
    query.bindValue(":reg", user.registration_date);
    query.bindValue(":role", user.role);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return true;
}

bool UserRepository::update(const UserEntity& user) {
    QSqlQuery query(db);
    
    query.prepare("UPDATE users SET name = :name, email = :email, registration_date = :reg, role = :role "
                  "WHERE user_id = :id");
    
    query.bindValue(":name", user.name);
    query.bindValue(":email", user.email);
    query.bindValue(":reg", user.registration_date);
    query.bindValue(":role", user.role);
    query.bindValue(":id", user.user_id);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

bool UserRepository::remove(const QString& id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE user_id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        setLastError(query);
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

void UserRepository::setLastError(const QSqlQuery& query) {
    lastError = query.lastError().text();
}