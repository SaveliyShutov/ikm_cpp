#pragma once
#include <QObject>
#include <QVariantMap>

class LogEntity {
public:
    QString user_id;
    QString habit_id;
    QString log_date;
    bool is_completed;
    QString completion_time;
    QString notes;
    QString created_at;
    
    LogEntity() = default;
    
    LogEntity(const QVariantMap& map) {
        user_id = map["user_id"].toString();
        habit_id = map["habit_id"].toString();
        log_date = map["log_date"].toString();
        is_completed = map["is_completed"].toBool();
        completion_time = map["completion_time"].toString();
        notes = map["notes"].toString();
        created_at = map["created_at"].toString();
    }
    
    QVariantMap toMap() const {
        return {
            {"user_id", user_id},
            {"habit_id", habit_id},
            {"log_date", log_date},
            {"is_completed", is_completed},
            {"completion_time", completion_time},
            {"notes", notes},
            {"created_at", created_at}
        };
    }
};