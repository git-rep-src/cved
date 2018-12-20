#ifndef CVED_H
#define CVED_H

#include "ui.h"
#include "manager.h"

#include <QApplication>

namespace Ui {
class Cved;
}

class Cved : public QObject
{
    Q_OBJECT

public:
    Cved();
    ~Cved();

private:
    Ui::Cved *ui;

    Manager *manager;
};

#endif
