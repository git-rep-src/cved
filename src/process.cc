#include "process.h"

Process::Process(QObject *parent) :
    QObject(parent),
    proc(NULL)
{
    proc = new QProcess(this);
    connect(proc, &QProcess::readyReadStandardOutput, [&] {
        emit signal_send_stdout(proc->readAllStandardOutput());
    });
    connect(proc, &QProcess::readyReadStandardError, [&] {
        emit signal_send_stderr(proc->readAllStandardError());
    });
    connect(proc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [&] (int exit_code, QProcess::ExitStatus exit_status) {
        Q_UNUSED(exit_status);
        emit signal_finished(exit_code);
    });
}

Process::~Process()
{
    if (proc)
        delete proc;
}

void Process::start(const QString &command, const QStringList &arguments)
{
    proc->start(command, arguments);
}

void Process::stop()
{
    proc->close();
}
