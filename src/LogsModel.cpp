#include "LogsModel.h"
#include "DatabaseManager.h"
#include <QDebug>

LogsModel::LogsModel(QObject* parent) : QObject(parent) {
    m_model = new SqlQueryModel(this);
    m_repository = new LogRepository(DatabaseManager::instance().database(), this);
    refresh();
}

LogsModel::~LogsModel() {
    // Автоматически удалится благодаря родительству
}

QVariant LogsModel::getModel() {
    return QVariant::fromValue(static_cast<QObject*>(m_model));
}

void LogsModel::refresh() {
    m_model->refresh("SELECT user_id, habit_id, to_char(log_date, 'YYYY-MM-DD') as log_date, "
                     "is_completed, completion_time, notes, created_at "
                     "FROM habit_log ORDER BY log_date DESC");
}

bool LogsModel::addLog(const QVariantMap& data) {
    LogEntity log(data);
    bool ok = m_repository->create(log);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "addLog error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

bool LogsModel::editLog(const QString& userId, const QString& habitId,
                        const QString& logDate, const QVariantMap& data) {
    LogEntity log(data);
    log.user_id = userId;
    log.habit_id = habitId;
    log.log_date = logDate;
    bool ok = m_repository->update(log);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "editLog error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

bool LogsModel::deleteLog(const QString& userId, const QString& habitId,
                          const QString& logDate) {
    bool ok = m_repository->remove(userId, habitId, logDate);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "deleteLog error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

void LogsModel::setLastError(const QString& error) {
    if (m_lastError != error) {
        m_lastError = error;
        emit lastErrorChanged(error);
    }
}