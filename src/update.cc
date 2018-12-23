#include "update.h"

Update::Update(QObject *parent) :
    QObject(parent),
    ui(new Ui::Update)
{
    ui->setup();
    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->button_cancel, &QPushButton::clicked, [&] () {
        emit signal_finished();
    });
}

Update::~Update()
{
    delete ui;
}

void Update::check()
{
    bool has_update = true;//

    if (has_update) {
       ui->label_message->setText("DATABASE UPDATE AVAILABLE");
       ui->button_start->show();
       ui->button_cancel->show();
    } else {
        emit signal_finished();
    }
}

void Update::start()
{
    ui->label_message->setText("UPDATING...");
    ui->button_start->hide();
    ui->button_cancel->hide();

    bool update_ok = true;//

    if (update_ok) {
        emit signal_finished();
    } else {
        ui->button_ok->show();
        ui->label_message->setText("UPDATE FAILED");
        connect(ui->button_ok, &QPushButton::clicked, [&] () {
            emit signal_finished();
        });
    }
}

