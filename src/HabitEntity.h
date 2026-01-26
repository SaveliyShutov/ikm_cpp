#pragma once
#include <QObject>
#include <QVariantMap>

class HabitEntity {
public:
  QString habit_id;
  QString name;
  QString description;
  QString category;
  QString frequency;
  QString reminder_time;
  QString created_at;

  HabitEntity() = default;

  HabitEntity(const QVariantMap &map) {
    habit_id = map["habit_id"].toString();
    name = map["name"].toString();
    description = map["description"].toString();
    category = map["category"].toString();
    frequency = map["frequency"].toString();
    reminder_time = map["reminder_time"].toString();
    created_at = map["created_at"].toString();
  }

  QVariantMap toMap() const {
    return {{"habit_id", habit_id},       {"name", name},
            {"description", description}, {"category", category},
            {"frequency", frequency},     {"reminder_time", reminder_time},
            {"created_at", created_at}};
  }
};