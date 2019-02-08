#ifndef FINDER_H
#define FINDER_H

#include <QtSql>

class Finder : public QObject
{
    Q_OBJECT

public:
    Finder(QObject *parent);
    ~Finder();

public slots:
    bool open_db();
    void get_data(const QString &str, bool is_combo);

signals:
    void signal_send_combo(const QStringList &data);
    void signal_send_data(const QStringList &data);
    void signal_empty_data();

private:
    QSqlDatabase db;
};

#endif
