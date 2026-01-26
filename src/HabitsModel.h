#pragma once
#include <QObject>
#include "SqlQueryModel.h"
#include "HabitRepository.h"

class HabitsModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    
public:
    explicit HabitsModel(QObject* parent = nullptr);
    ~HabitsModel();
    
    Q_INVOKABLE QVariant getModel();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addHabit(const QVariantMap& data);
    Q_INVOKABLE bool editHabit(const QString& id, const QVariantMap& data);
    Q_INVOKABLE bool deleteHabit(const QString& id);
    
    QString lastError() const { return m_lastError; }
    
signals:
    void lastErrorChanged(const QString& error);
    
private:
    SqlQueryModel* m_model;
    HabitRepository* m_repository;
    QString m_lastError;
    
    void setLastError(const QString& error);
};