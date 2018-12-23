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
    void get_data(const QString &name, bool is_combo);

signals:
    void signal_send_combo(const QStringList &data);
    void signal_send_data(const QStringList &data);

private:
    QSqlDatabase db;

private slots:
    bool open_db();
};

#endif
