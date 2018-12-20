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
    void set_data(const QString &name, QString key, QString data);

signals:
    void signal_send_data(const QStringList &data, bool is_combo);

private:
    QSqlDatabase db;

private slots:
    bool open_db();
};

#endif
