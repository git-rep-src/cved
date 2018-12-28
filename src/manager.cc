#include "manager.h"

Manager::Manager(QObject *parent) :
    QObject(parent),
    ui(new Ui::Manager),
    finder(NULL),
    process(NULL),
    is_running(false)
{
    ui->setup();

    connect(ui->button_pull, SIGNAL(clicked()), this, SLOT(docker_pull()));
    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(docker_start()));
    connect(ui->button_stop, SIGNAL(clicked()), this, SLOT(docker_stop()));
    connect(ui->button_delete, SIGNAL(clicked()), this, SLOT(docker_delete()));

    finder = new Finder(this);
    connect(finder, &Finder::signal_send_combo, [&] (const QStringList &data) {
        this->set_combo(data);
    });
    connect(finder, &Finder::signal_send_data, [&] (const QStringList &data) {
        this->set_data(data);
    });

    finder->get_data("", true);

    connect(ui->combo_cve, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), [&] {
        name = ui->combo_cve->currentText().toLower();
        finder->get_data(name, false);
    });
}

Manager::~Manager()
{
    if (finder)
        delete finder;
    if (process)
        delete process;
    delete ui;
}

void Manager::set_combo(const QStringList &data)
{
    name = data.at(0);

    for (int i = 0; i < ui->combo_cve->count(); i++)
        ui->combo_cve->removeItem(i);
    for (int i = 0; i < data.size(); i++) {
        ui->combo_cve->addItem(data.at(i).toUpper());
        ui->combo_cve->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    ui->combo_cve->setCurrentIndex(0);

    finder->get_data(name, false);
}

void Manager::set_data(const QStringList &data)
{
    ui->label_description_data->setText(data.at(0) + "<br>");
    ui->label_container_data->setText(data.at(1) + "<br>");
    ui->label_size_data->setText(data.at(2) + "<br>");
    ui->label_target_data->setText(data.at(3) + "<br>");
    ui->label_misc_data->setText(data.at(4) + "<br>");

    network = data.at(5);
    options = data.at(6);

    set_status();
}

void Manager::set_status()
{
    arguments.clear();
    arguments << "-c" << "docker images | grep " + name + " &>/dev/null";

    process = new Process(this);
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("STOPPED<br>");
            ui->label_network_data->setText(network);
            ui->button_pull->setDisabled(true);
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        } else {
            ui->label_status_data->setText("NULL<br>");
            ui->label_network_data->setText("NULL");
            ui->button_pull->setEnabled(true);
            ui->button_start->setDisabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setDisabled(true);
        }
        ui->edit_output->clear();
        process->deleteLater();
    });
    process->start(command, arguments);
}

void Manager::docker_pull()
{
    arguments.clear();
    arguments << "-c" << "docker pull cved/" + name;

    process = new Process(this);
    connect(process, &Process::signal_send_stdout, [&] (QString str) {
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_send_stderr, [&] (QString str) {
        ui->edit_output->appendHtml("<span style=color:#851111>" + str + "</span>");
    });
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("STOPPED<br>");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        } else {
            ui->label_status_data->setText("PULL FAILED<br>");
            ui->button_pull->setEnabled(true);
            ui->button_stop->setDisabled(true);
        }
        ui->combo_cve->setEnabled(true);
        process->deleteLater();
    });
    process->start(command, arguments);

    ui->combo_cve->setDisabled(true);
    ui->label_status_data->setText("PULLING<br>");
    ui->button_pull->setDisabled(true);
    ui->button_stop->setEnabled(true);
    ui->edit_output->clear();
}

void Manager::docker_start()
{
    arguments.clear();
    arguments << "-c" << "docker run --rm --detach --name " + name + " " + options + " cved/" + name;

    process = new Process(this);
    connect(process, &Process::signal_send_stdout, [&] (QString str) {
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_send_stderr, [&] (QString str) {
        ui->edit_output->appendHtml("<span style=color:#851111>" + str + "</span>");
    });
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("RUNNING<br>");
            ui->label_network_data->setText(network);
            is_running = true;
        } else {
            ui->combo_cve->setEnabled(true);
            ui->label_status_data->setText("START FAILED<br>");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
            is_running = false;
        }
        process->deleteLater();
    });
    process->start(command, arguments);

    ui->combo_cve->setDisabled(true);
    ui->button_start->setDisabled(true);
    ui->button_stop->setEnabled(true);
    ui->button_delete->setDisabled(true);
    ui->edit_output->clear();
}

void Manager::docker_stop()
{
    if (is_running) {
        arguments.clear();
        arguments << "-c" << "docker stop " + name;
        process = new Process(this);
        connect(process, &Process::signal_send_stdout, [&] (QString str) {
            ui->edit_output->appendPlainText(str);
        });
        connect(process, &Process::signal_send_stderr, [&] (QString str) {
            ui->edit_output->appendHtml("<span style=color:#851111>" + str + "</span>");
        });
        connect(process, &Process::signal_finished, [&] (int exit_code) {
            if (exit_code == 0) {
                ui->combo_cve->setEnabled(true);
                ui->label_status_data->setText("STOPPED<br>");
                ui->label_network_data->setText("NULL");
                ui->button_start->setEnabled(true);
                ui->button_stop->setDisabled(true);
                ui->button_delete->setEnabled(true);
                is_running = false;
            } else {
                ui->label_status_data->setText("STOP FAILED<br>");
            }
            process->deleteLater();
        });
        process->start(command, arguments);
    } else {
        process->stop();
    }

    ui->edit_output->clear();
}

void Manager::docker_delete()
{
    arguments.clear();
    arguments << "-c" << "docker rmi -f cved/" + name;

    process = new Process(this);
    connect(process, &Process::signal_send_stdout, [&] (QString str) {
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_send_stderr, [&] (QString str) {
        ui->edit_output->appendHtml("<span style=color:#851111>" + str + "</span>");
    });
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("NULL<br>");
            ui->button_pull->setEnabled(true);
        } else {
            ui->label_status_data->setText("DELETE FAILED<br>");
        }
        process->deleteLater();
    });
    process->start(command, arguments);

    ui->button_start->setDisabled(true);
    ui->button_delete->setDisabled(true);
    ui->edit_output->clear();
}
