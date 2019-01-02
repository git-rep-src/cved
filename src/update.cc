#include "update.h"

#include <QProcess>
#include <QStandardPaths>

Update::Update(QObject *parent) :
    QObject(parent),
    ui(new Ui::Update)
{
    ui->setup();
}

Update::~Update()
{
    delete ui;
}

void Update::start()
{
    const QString home = QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                QString(),
                                                QStandardPaths::LocateDirectory);

    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=] () {
        process->deleteLater();
        emit signal_finished();
    });
    process->start("/bin/bash", QStringList() << "-c" << "git -C " + home + "/.cved " + "pull");
}
