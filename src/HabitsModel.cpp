#include "HabitsModel.h"
#include "DatabaseManager.h"
#include <QDebug>

HabitsModel::HabitsModel(QObject* parent) : QObject(parent) {
    m_model = new SqlQueryModel(this);
    m_repository = new HabitRepository(DatabaseManager::instance().database(), this);
    refresh();
}

HabitsModel::~HabitsModel() {}

QVariant HabitsModel::getModel() {
    return QVariant::fromValue(static_cast<QObject*>(m_model));
}

void HabitsModel::refresh() {
    m_model->refresh("SELECT habit_id, name, description, category, frequency, reminder_time, created_at FROM habit_templates ORDER BY name");
}

bool HabitsModel::addHabit(const QVariantMap& data) {
    HabitEntity habit(data);
    bool ok = m_repository->create(habit);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "addHabit error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

bool HabitsModel::editHabit(const QString& id, const QVariantMap& data) {
    HabitEntity habit(data);
    habit.habit_id = id;
    bool ok = m_repository->update(habit);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "editHabit error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

bool HabitsModel::deleteHabit(const QString& id) {
    bool ok = m_repository->remove(id);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "deleteHabit error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

void HabitsModel::setLastError(const QString& error) {
    if (m_lastError != error) {
        m_lastError = error;
        emit lastErrorChanged(error);
    }
}