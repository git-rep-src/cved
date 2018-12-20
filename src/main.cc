#include "cved.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    
    QFile file(":/style");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(QLatin1String(file.readAll()));
    file.close();

    Cved cved;

    return application.exec();
}
