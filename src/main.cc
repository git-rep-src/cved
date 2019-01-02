#include "cved.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    if (argc == 2) {
        if ((strcmp("-h", argv[1]) == 0) || (strcmp("--help", argv[1]) == 0)) {
            qDebug() << "Vulnerable docker containers manager (https://gitlab.com/git-rep/cved)." << endl;
            qDebug() << "Usage: cved [OPTION]" << endl;
            qDebug() << "Options:";
            qDebug() << "  -n, --no-update no update database";
            qDebug() << "  -h, --help      display this help and exit";
            qDebug() << "  -v, --version   output version information and exit";
            return 0;
        } else if ((strcmp("-v", argv[1]) == 0) || (strcmp("--version", argv[1]) == 0)) {
            qDebug() << "0.1.1";
            return 0;
        }
    }

    QApplication application(argc, argv);
    
    QFile file(":/style");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(QLatin1String(file.readAll()));
    file.close();

    Cved cved;

    return application.exec();
}
