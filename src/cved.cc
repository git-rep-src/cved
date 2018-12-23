#include "cved.h"

Cved::Cved() :
    ui(new Ui::Cved),
    update(NULL),
    manager(NULL)
{
    ui->setup();
    ui->widget->showFullScreen();

//    update = new Update(this);
//    connect(update, &Update::signal_finished, [&] () {
//        ui->layout->removeWidget(update->get_ui()->widget);
//        delete update->get_ui()->widget;
//        delete update;
//        manager = new Manager(this);
//        ui->layout->addWidget(manager->get_ui()->widget);
//    });

//    ui->layout->addWidget(update->get_ui()->widget);

//    update->check();

    manager = new Manager(this);//
    ui->layout->addWidget(manager->get_ui()->widget);//
}

Cved::~Cved()
{
    if (update)
        delete update;
    if (manager)
        delete manager;
    delete ui;
}
