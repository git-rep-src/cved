#include "finder.h"

Finder::Finder(QObject *parent) :
    QObject(parent)
{
    if (!open_db()) {
        // error
    }
}

Finder::~Finder()
{
    if (db.isOpen())
        db.close();
}

bool Finder::open_db()
{
    QFile file;
    if (file.exists(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                           QString(),
                                           QStandardPaths::LocateDirectory) + "/.cved/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                  QString(),
                                                  QStandardPaths::LocateDirectory) + ".cved/db.sqlite");
        file.close();
    } else {
       return false;
    }

    return true;
}

void Finder::get_data(const QString &name, bool is_combo)
{
    QStringList data;

    if (db.open()) {
        QSqlQuery query;
        if (is_combo) {
            query.prepare("SELECT name FROM data ORDER BY name DESC");
        } else {
            query.prepare("SELECT description, image, size, target, status, network,"
                          " options, containerized FROM data WHERE name=:name");
            query.bindValue(":name", name);
        }
        if (query.exec()) {
            while (query.next()) {
                if (is_combo) {
                    data.append(query.value(0).toString());
                } else {
                    data.append(query.value(0).toString());
                    data.append(query.value(1).toString());
                    data.append(query.value(2).toString());
                    data.append(query.value(3).toString());
                    data.append(query.value(4).toString());
                    data.append(query.value(5).toString());
                    data.append(query.value(6).toString());
                    data.append(query.value(7).toString());
                }
            }
        }
        db.close();
    } else {
        // error
    }

    emit signal_send_data(data, is_combo);
}

void Finder::set_data(const QString &name, QString key, QString data)
{
    if (db.open()) {
        QSqlQuery query;
        query.prepare(QString("UPDATE data SET %1=:data WHERE name=:name").arg(key));
        query.bindValue(":data", data);
        query.bindValue(":name", name);
        query.exec();
        db.close();
    } else {
        // error
    }
}
