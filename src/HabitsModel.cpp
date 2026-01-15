#include "HabitsModel.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>  
#include <QDebug>

HabitsModel::HabitsModel(QObject* parent) : QObject(parent) {
    m_model = new SqlQueryModel(this);
    refresh();
}

QVariant HabitsModel::getModel() {
    return QVariant::fromValue(static_cast<QObject*>(m_model));
}

void HabitsModel::refresh() {
    m_model->refresh("SELECT habit_id, name, description, category, frequency, reminder_time, created_at FROM habit_templates ORDER BY name");
}

bool HabitsModel::addHabit(const QVariantMap& data) {
    QSqlQuery q(DatabaseManager::instance().database());
    bool hasId = data.contains("habit_id") && !data.value("habit_id").toString().trimmed().isEmpty();
    if (hasId) {
        q.prepare("INSERT INTO habit_templates(habit_id, name, description, category, frequency, reminder_time) VALUES(:id,:name,:desc,:cat,:freq,:rem)");
        q.bindValue(":id", data.value("habit_id").toString());
    } else {
        q.prepare("INSERT INTO habit_templates(name, description, category, frequency, reminder_time) VALUES(:name,:desc,:cat,:freq,:rem)");
    }
    q.bindValue(":name", data.value("name").toString());
    q.bindValue(":desc", data.value("description").toString());
    q.bindValue(":cat", data.value("category").toString());
    q.bindValue(":freq", data.value("frequency").toString());
    q.bindValue(":rem", data.value("reminder_time").toString());
    bool ok = q.exec();
    if (!ok) qDebug() << "addHabit error:" << q.lastError().text();
    refresh();
    return ok;
}

bool HabitsModel::editHabit(const QString& id, const QVariantMap& data) {
    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("UPDATE habit_templates SET name=:name, description=:desc, category=:cat, frequency=:freq, reminder_time=:rem WHERE habit_id=:id");
    q.bindValue(":name", data.value("name").toString());
    q.bindValue(":desc", data.value("description").toString());
    q.bindValue(":cat", data.value("category").toString());
    q.bindValue(":freq", data.value("frequency").toString());
    q.bindValue(":rem", data.value("reminder_time").toString());
    q.bindValue(":id", id);
    bool ok = q.exec();
    if (!ok) qDebug() << "editHabit error:" << q.lastError().text();
    refresh();
    return ok;
}

bool HabitsModel::deleteHabit(const QString& id) {
    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("DELETE FROM habit_templates WHERE habit_id=:id");
    q.bindValue(":id", id);
    bool ok = q.exec();
    if (!ok) qDebug() << "deleteHabit error:" << q.lastError().text();
    refresh();
    return ok;
}
