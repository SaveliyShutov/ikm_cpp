#pragma once
#include <QObject>
#include "SqlQueryModel.h"
#include "UserRepository.h"

class UsersModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    
public:
    explicit UsersModel(QObject* parent = nullptr);
    ~UsersModel();
    
    Q_INVOKABLE QVariant getModel();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addUser(const QVariantMap& data);
    Q_INVOKABLE bool editUser(const QString& id, const QVariantMap& data);
    Q_INVOKABLE bool deleteUser(const QString& id);
    
    QString lastError() const { return m_lastError; }
    
signals:
    void lastErrorChanged(const QString& error);
    
private:
    SqlQueryModel* m_model;
    UserRepository* m_repository;
    QString m_lastError;
    
    void setLastError(const QString& error);
};