#ifndef MANAGER_H
#define MANAGER_H

#include "ui.h"
#include "finder.h"
#include "process.h"

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
    Process *process;

    bool is_container;
    bool is_running;

    QString name;
    QString network;
    const QString command = "/bin/bash";
    QString options;
    QStringList arguments;

private slots:
    void set_combo(const QStringList &data);
    void set_data(const QStringList &data);
    void set_status();
    void docker_pull();
    void docker_start();
    void docker_stop();
    void docker_delete();
    void has_container();
};

#endif
