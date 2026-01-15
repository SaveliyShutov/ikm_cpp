#pragma once
#include <QObject>
#include "SqlQueryModel.h"

class HabitsModel : public QObject {
    Q_OBJECT
public:
    explicit HabitsModel(QObject* parent = nullptr);
    Q_INVOKABLE QVariant getModel();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addHabit(const QVariantMap& data);
    Q_INVOKABLE bool editHabit(const QString& id, const QVariantMap& data);
    Q_INVOKABLE bool deleteHabit(const QString& id);
private:
    SqlQueryModel* m_model;
};
