#pragma once
#include <QtSql>
#include <QString>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    static DatabaseManager& instance();
    bool openPostgres(const QString& host, int port, const QString& dbname,
                      const QString& user, const QString& password);
    bool initSchema(const QString& sqlFilePath);
    QSqlDatabase& database();
private:
    DatabaseManager() = default;
    QSqlDatabase db_;
};
