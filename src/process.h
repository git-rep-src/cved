#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>

class Process : public QObject
{
    Q_OBJECT

public:
    Process(QObject *parent);
    ~Process();

public slots:
    void start(const QString &command, const QStringList &arguments);
    void stop();

signals:
    void signal_send_stdout(QString str);
    void signal_send_stderr(QString str);
    void signal_finished(int exit_code);

private:
    QProcess *proc;
};

#endif
