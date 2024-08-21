// DatabaseManager.h
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include "Storage.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase(const QString &dbPath);
    void loadData(Storage &storage);

private:
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &queryStr);
    int getDatabaseVersion() const;
};

#endif // DATABASEMANAGER_H
