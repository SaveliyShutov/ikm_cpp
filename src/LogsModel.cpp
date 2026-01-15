#include "LogsModel.h"
#include "DatabaseManager.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

LogsModel::LogsModel(QObject *parent) : QObject(parent) {
  m_model = new SqlQueryModel(this);
  refresh();
}

QVariant LogsModel::getModel() {
  return QVariant::fromValue(static_cast<QObject *>(m_model));
}

void LogsModel::refresh() {
  m_model->refresh(
      "SELECT user_id, habit_id, to_char(log_date, 'YYYY-MM-DD') as log_date, "
      "is_completed, completion_time, notes, created_at "
      "FROM habit_log ORDER BY log_date DESC");
}

bool LogsModel::addLog(const QVariantMap &data) {
  QSqlQuery q(DatabaseManager::instance().database());
  q.prepare(
      "INSERT INTO habit_log(user_id, habit_id, log_date, is_completed, "
      "completion_time, notes) VALUES(:uid,:hid,:date,:done,:time,:notes)");
  q.bindValue(":uid", data.value("user_id").toString());
  q.bindValue(":hid", data.value("habit_id").toString());
  q.bindValue(":date", data.value("log_date").toString());
  q.bindValue(":done", data.value("is_completed").toBool());
  q.bindValue(":time", data.value("completion_time").toString());
  q.bindValue(":notes", data.value("notes").toString());
  bool ok = q.exec();
  if (!ok)
    qDebug() << "addLog error:" << q.lastError().text();
  refresh();
  return ok;
}

bool LogsModel::editLog(const QString &userId, const QString &habitId,
                        const QString &logDate, const QVariantMap &data) {
  QSqlQuery q(DatabaseManager::instance().database());
  q.prepare(
      "UPDATE habit_log SET is_completed=:done, completion_time=:time, "
      "notes=:notes WHERE user_id=:uid AND habit_id=:hid AND log_date=:date");
  q.bindValue(":done", data.value("is_completed").toBool());
  q.bindValue(":time", data.value("completion_time").toString());
  q.bindValue(":notes", data.value("notes").toString());
  q.bindValue(":uid", userId);
  q.bindValue(":hid", habitId);
  q.bindValue(":date", logDate);
  bool ok = q.exec();
  if (!ok)
    qDebug() << "editLog error:" << q.lastError().text();
  refresh();
  return ok;
}

bool LogsModel::deleteLog(const QString &userId, const QString &habitId,
                          const QString &logDate) {
  QSqlQuery q(DatabaseManager::instance().database());
  q.prepare("DELETE FROM habit_log WHERE user_id=:uid AND habit_id=:hid AND "
            "log_date=:date");
  q.bindValue(":uid", userId);
  q.bindValue(":hid", habitId);
  q.bindValue(":date", logDate);
  bool ok = q.exec();
  if (!ok)
    qDebug() << "deleteLog error:" << q.lastError().text();
  refresh();
  return ok;
}
