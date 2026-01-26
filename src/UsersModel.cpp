#include "UsersModel.h"
#include "DatabaseManager.h"
#include <QDebug>

UsersModel::UsersModel(QObject* parent) : QObject(parent) {
    m_model = new SqlQueryModel(this);
    m_repository = new UserRepository(DatabaseManager::instance().database(), this);
    refresh();
}

UsersModel::~UsersModel() {
    // Автоматически удалится благодаря родительству
}

QVariant UsersModel::getModel() {
    return QVariant::fromValue(static_cast<QObject*>(m_model));
}

void UsersModel::refresh() {
    m_model->refresh("SELECT user_id, name, email, to_char(registration_date, 'YYYY-MM-DD') as registration_date, "
                     "role, to_char(created_at, 'YYYY-MM-DD HH24:MI:SS') as created_at "
                     "FROM users ORDER BY name");
}

bool UsersModel::addUser(const QVariantMap& data) {
    UserEntity user(data);
    bool ok = m_repository->create(user);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "addUser error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

bool UsersModel::editUser(const QString& id, const QVariantMap& data) {
    UserEntity user(data);
    user.user_id = id;
    bool ok = m_repository->update(user);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "editUser error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

bool UsersModel::deleteUser(const QString& id) {
    bool ok = m_repository->remove(id);
    
    if (!ok) {
        setLastError(m_repository->getLastError());
        qDebug() << "deleteUser error:" << m_lastError;
    } else {
        refresh();
    }
    
    return ok;
}

void UsersModel::setLastError(const QString& error) {
    if (m_lastError != error) {
        m_lastError = error;
        emit lastErrorChanged(error);
    }
}