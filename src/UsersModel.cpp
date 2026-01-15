#include "UsersModel.h"
#include "DatabaseManager.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

UsersModel::UsersModel(QObject *parent) : QObject(parent) {
  m_model = new SqlQueryModel(this);
  refresh();
}

QVariant UsersModel::getModel() {
  return QVariant::fromValue(static_cast<QObject *>(m_model));
}

void UsersModel::refresh() {
  m_model->refresh("SELECT user_id, name, email, to_char(registration_date, 'YYYY-MM-DD') as registration_date, role, "
                   "to_char(created_at, 'YYYY-MM-DD HH24:MI:SS') as created_at FROM users ORDER BY name");
}

bool UsersModel::addUser(const QVariantMap &data) {
  QSqlQuery q(DatabaseManager::instance().database());
  bool hasId = data.contains("user_id") &&
               !data.value("user_id").toString().trimmed().isEmpty();
  if (hasId) {
    q.prepare("INSERT INTO users(user_id, name, email, registration_date, "
              "role) VALUES(:id,:name,:email,:reg,:role)");
    q.bindValue(":id", data.value("user_id").toString());
  } else {
    q.prepare("INSERT INTO users(name, email, registration_date, role) "
              "VALUES(:name,:email,:reg,:role)");
  }
  q.bindValue(":name", data.value("name").toString());
  q.bindValue(":email", data.value("email").toString());
  q.bindValue(":reg", data.value("registration_date").toString());
  q.bindValue(":role", data.value("role").toString());
  bool ok = q.exec();
  if (!ok)
    qDebug() << "addUser error:" << q.lastError().text();
  refresh();
  return ok;
}

bool UsersModel::editUser(const QString &id, const QVariantMap &data) {
  QSqlQuery q(DatabaseManager::instance().database());
  q.prepare("UPDATE users SET name = :name, email = :email, registration_date "
            "= :reg, role = :role WHERE user_id = :id");
  q.bindValue(":name", data.value("name").toString());
  q.bindValue(":email", data.value("email").toString());
  q.bindValue(":reg", data.value("registration_date").toString());
  q.bindValue(":role", data.value("role").toString());
  q.bindValue(":id", id);
  bool ok = q.exec();
  if (!ok)
    qDebug() << "editUser error:" << q.lastError().text();
  refresh();
  return ok;
}

bool UsersModel::deleteUser(const QString &id) {
  QSqlQuery q(DatabaseManager::instance().database());
  q.prepare("DELETE FROM users WHERE user_id = :id");
  q.bindValue(":id", id);
  bool ok = q.exec();
  if (!ok)
    qDebug() << "deleteUser error:" << q.lastError().text();
  refresh();
  return ok;
}
