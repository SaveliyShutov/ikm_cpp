#pragma once
#include "SqlQueryModel.h"
#include <QObject>

class LogsModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
  
public:
  explicit LogsModel(QObject *parent = nullptr);
  Q_INVOKABLE QVariant getModel();
  Q_INVOKABLE void refresh();
  Q_INVOKABLE bool addLog(const QVariantMap &data);
  Q_INVOKABLE bool editLog(const QString &userId, const QString &habitId,
                           const QString &logDate, const QVariantMap &data);
  Q_INVOKABLE bool deleteLog(const QString &userId, const QString &habitId,
                             const QString &logDate);
  
  QString lastError() const { return m_lastError; }

signals:
  void lastErrorChanged(const QString &error);

private:
  SqlQueryModel *m_model;
  QString m_lastError;
  
  void setLastError(const QString &error);
};