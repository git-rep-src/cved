#include "finder.h"

Finder::Finder(QObject *parent) :
    QObject(parent)
{
}

Finder::~Finder()
{
    if (db.isOpen())
        db.close();
}

bool Finder::open_db()
{
    const QString home = QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                QString(),
                                                QStandardPaths::LocateDirectory);

    QFile file;
    if (file.exists(home + "/.cved/db.sqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(home + ".cved/db.sqlite");
        file.close();
    } else {
        return false;
    }

    return true;
}

void Finder::get_data(const QString &str, bool is_combo)
{
    QStringList data;

    if (db.open()) {
        QSqlQuery query;
        if (is_combo) {
            if (!str.isEmpty())
                query.prepare("SELECT name FROM data WHERE target LIKE '%" + str + "%' ORDER BY name DESC");
            else
                query.prepare("SELECT name FROM data ORDER BY name DESC");
        } else {
            query.prepare("SELECT description, author, source, size, target, misc, network, options "
                          "FROM data WHERE name=:name");
            query.bindValue(":name", str);
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
            if (data.size() != 0) {
                if (is_combo)
                    emit signal_send_combo(data);
                else
                    emit signal_send_data(data);
            } else {
                if ((is_combo) && (!str.isEmpty())) {
                    emit signal_empty_data();
                }
            }
        }
        db.close();
    }
}
