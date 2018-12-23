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
            query.prepare("SELECT description, image, size, target, network, options FROM data WHERE name=:name");
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
                }
            }
        }
        db.close();
    } else {
        // error
    }

    if (is_combo)
        emit signal_send_combo(data);
    else
        emit signal_send_data(data);
}
