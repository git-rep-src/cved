#ifndef MANAGER_H
#define MANAGER_H

#include "ui.h"
#include "finder.h"

#include <QProcess>

namespace Ui {
class Manager;
}

class Manager : public QObject
{
    Q_OBJECT

public:
    Manager(QObject *parent);
    ~Manager();

public slots:
    Ui::Manager *get_ui() { return ui; }

private:
    Ui::Manager *ui;

    Finder *finder;

    QProcess *process;

    bool is_pulling;
    bool is_pull_aborted;

    QString name;
    QString network;
    QString options;

private slots:
    void set_combo(const QStringList &data);
    void set_data(const QStringList &data);
    void set_status();
    void docker_pull();
    void docker_start();
    void docker_stop();
    void docker_delete();
};

#endif
