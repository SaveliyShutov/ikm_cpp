#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>

#include "DatabaseManager.h"
#include "HabitsModel.h"
#include "LogsModel.h"
#include "UsersModel.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    const QString host = QStringLiteral("127.0.0.1");
    const int port = 5432;
    const QString dbname = QStringLiteral("habitdb");
    const QString user = QStringLiteral("habit_user");
    const QString password = QStringLiteral("habit_pass");

    if (!DatabaseManager::instance().openPostgres(host, port, dbname, user, password)) {
        qCritical() << "Cannot open Postgres DB. Check credentials and drivers.";
        qDebug() << "Available drivers:" << QSqlDatabase::drivers();
        return -1;
    }

    QString sqlFilePath = QCoreApplication::applicationDirPath() + "/../sql/init_postgres.sql";
    QFileInfo checkFile(sqlFilePath);
    if (!checkFile.exists()) {
        qCritical() << "SQL file not found:" << sqlFilePath;
        return -1;
    }

    if (!DatabaseManager::instance().initSchema(sqlFilePath)) {
        qCritical() << "Failed to initialize database schema.";
        return -1;
    }

    QQmlApplicationEngine engine;

    UsersModel usersModel;
    HabitsModel habitsModel;
    LogsModel logsModel;

    engine.rootContext()->setContextProperty("UsersModel", &usersModel);
    engine.rootContext()->setContextProperty("HabitsModel", &habitsModel);
    engine.rootContext()->setContextProperty("LogsModel", &logsModel);

QString qmlPath = QDir::current().absoluteFilePath("../qml/main.qml");
engine.load(QUrl::fromLocalFile(qmlPath));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
