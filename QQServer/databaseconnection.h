#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QtSql>

class DatabaseConnection
{
public:
    QSqlDatabase db;
    DatabaseConnection(){
        db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName("127.0.0.1");
            db.setDatabaseName("QTDB01");
            db.setUserName("root");
            db.setPassword("123456");

            if (db.open()) {
                qDebug() << "Connected to database";
            } else {
                qDebug() << "Failed to connect to database" << db.lastError().text();
            }
    }
    ~DatabaseConnection() {
        if (db.isOpen()) {
            db.close();
        }
    }
};

#endif // DATABASECONNECTION_H
