#include "update.h"

#include <QStandardPaths>

Update::Update(QObject *parent) :
    QObject(parent),
    ui(new Ui::Update),
    process(NULL)
{
    ui->setup();
}

Update::~Update()
{
    if (process)
        delete process;
    delete ui;
}

void Update::start()
{
    const QString command = "/bin/bash";
    QStringList arguments;
    arguments << "-c" << "git -C " + QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                            QString(),
                                                            QStandardPaths::LocateDirectory) + "/.cved " + "pull";

    process = new Process(this);
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        Q_UNUSED(exit_code);
        emit signal_finished();
    });
    process->start(command, arguments);
}

