#include "cved.h"

Cved::Cved() :
    ui(new Ui::Cved),
    manager(NULL)
{
    ui->setup();
    ui->widget->showFullScreen();

    manager = new Manager(this);
    ui->layout->addWidget(manager->get_ui()->widget);
}

Cved::~Cved()
{
    if (manager)
        delete manager;
    delete ui;
}
