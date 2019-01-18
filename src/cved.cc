#include "cved.h"

Cved::Cved() :
    ui(new Ui::Cved),
    update(NULL),
    manager(NULL)
{
    ui->setup();
    connect(ui->button_close, &QPushButton::clicked, [&] () {
        exit(0);
    });

    if (qApp->arguments().contains("-n") || qApp->arguments().contains("--no-update")) {
        manager = new Manager(this);
        ui->layout->addWidget(manager->get_ui()->widget);
        ui->button_close->show();
    } else {
        update = new Update(this);
        connect(update, &Update::signal_finished, [&] () {
            ui->layout->removeWidget(update->get_ui()->widget);
            delete update->get_ui()->widget;
            delete update;
            manager = new Manager(this);
            ui->layout->addWidget(manager->get_ui()->widget);
            ui->button_close->show();
        });
        ui->layout->addWidget(update->get_ui()->widget);
        update->start();
    }

    ui->widget->showFullScreen();
}

Cved::~Cved()
{
    //if (update)
        delete update;
    if (manager)
        delete manager;
    delete ui;
}
