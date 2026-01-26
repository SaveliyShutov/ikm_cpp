#pragma once
#include <QObject>
#include <QVariantMap>

class UserEntity {
public:
    QString user_id;
    QString name;
    QString email;
    QString registration_date;
    QString role;
    QString created_at;
    
    UserEntity() = default;
    
    UserEntity(const QVariantMap& map) {
        user_id = map["user_id"].toString();
        name = map["name"].toString();
        email = map["email"].toString();
        registration_date = map["registration_date"].toString();
        role = map["role"].toString();
        created_at = map["created_at"].toString();
    }
    
    QVariantMap toMap() const {
        return {
            {"user_id", user_id},
            {"name", name},
            {"email", email},
            {"registration_date", registration_date},
            {"role", role},
            {"created_at", created_at}
        };
    }
};