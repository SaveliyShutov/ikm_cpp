#include "DatabaseManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

bool DatabaseManager::openPostgres(const QString& host, int port, const QString& dbname,
                                   const QString& user, const QString& password)
{
    if (QSqlDatabase::contains("pg_default")) {
        db_ = QSqlDatabase::database("pg_default");
    } else {
        db_ = QSqlDatabase::addDatabase("QPSQL", "pg_default");
    }
    db_.setHostName(host);
    db_.setPort(port);
    db_.setDatabaseName(dbname);
    db_.setUserName(user);
    db_.setPassword(password);

    if (!db_.open()) {
        qWarning() << "Cannot open Postgres:" << db_.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::initSchema(const QString& sqlFilePath) {
    QFile f(sqlFilePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open SQL file:" << sqlFilePath;
        return false;
    }
    QTextStream in(&f);
    QString all = in.readAll();
    f.close();

    QStringList parts = all.split(';', Qt::SkipEmptyParts);
    QSqlQuery q(db_);
    for (QString part : parts) {
        QString stmt = part.trimmed();
        if (stmt.isEmpty()) continue;
        if (!q.exec(stmt)) {
            qWarning() << "SQL exec error:" << q.lastError().text() << "\nStmt:" << stmt;
        }
    }
    return true;
}

QSqlDatabase& DatabaseManager::database() {
    return db_;
}
