#include "manager.h"

Manager::Manager(QObject *parent) :
    QObject(parent),
    ui(new Ui::Manager),
    finder(NULL),
    process(NULL),
    is_running(false),
    is_containerized(false)
{
    ui->setup();

    connect(ui->button_pull, SIGNAL(clicked()), this, SLOT(docker_pull()));
    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(docker_start()));
    connect(ui->button_stop, SIGNAL(clicked()), this, SLOT(docker_stop()));
    connect(ui->button_delete, SIGNAL(clicked()), this, SLOT(docker_delete()));

    finder = new Finder(this);
    connect(finder, &Finder::signal_send_data, [&] (const QStringList &data, bool is_combo) {
        this->set_data(data, is_combo);
    });
    finder->get_data("", true);

    connect(ui->combo_cve, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), [&] {
        name = ui->combo_cve->currentText().toLower();
        finder->get_data(name, false);
    });
    connect(this, &Manager::signal_send_data, [&] (const QString &name, QString key, QString data) {
        finder->set_data(name, key, data);
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

void Manager::set_data(const QStringList &data, bool is_combo)
{
    if (is_combo) {
        for (int i = 0; i < data.size(); i++) {
            ui->combo_cve->addItem(data.at(i).toUpper());
            ui->combo_cve->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
        }
        ui->combo_cve->setCurrentIndex(0);
        name = data.at(0);
        finder->get_data(name, false);
    } else {
        ui->label_description_data->setText(data.at(0) + "<br>");
        ui->label_image_data->setText(data.at(1) + "<br>");
        ui->label_size_data->setText(data.at(2) + "<br>");
        ui->label_target_data->setText(data.at(3) + "<br>");
        ui->label_status_data->setText(data.at(4) + "<br>");
        network = data.at(5);
        options = data.at(6);
        if (data.at(7) == "true")
            is_containerized = true;
        ui->button_pull->setDisabled(true);
        ui->button_start->setDisabled(true);
        ui->button_stop->setDisabled(true);
        ui->button_delete->setDisabled(true);
        if (ui->label_status_data->text().contains("NULL")) {
            ui->label_network_data->setText("NULL");
            ui->button_pull->setEnabled(true);
        } else {
            ui->label_network_data->setText(network);
            ui->button_start->setEnabled(true);
            ui->button_delete->setEnabled(true);
        }
        ui->edit_output->clear();
    }
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
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("STOPPED<br>");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
            emit signal_send_data(name, "status", "STOPPED");
        } else {
            ui->label_status_data->setText("ERROR<br>");
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
    if (is_containerized)
        arguments << "-c" << "docker start " + name;
    else
        arguments << "-c" << "docker run --detach --name " + name + " " + options + " cved/" + name;

    process = new Process(this);
    connect(process, &Process::signal_send_stderr, [&] (QString str) {
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("RUNNING<br>");
            ui->label_network_data->setText(network);
            is_containerized = true;
            is_running = true;
            emit signal_send_data(name, "containerized", "true");
        } else {
            ui->combo_cve->setEnabled(true);
            ui->label_status_data->setText("ERROR<br>");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
            is_containerized = false;
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
        connect(process, &Process::signal_send_stderr, [&] (QString str) {
            ui->edit_output->appendPlainText(str);
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
                ui->label_status_data->setText("ERROR<br>");
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
    if (is_containerized)
        arguments << "-c" << "docker container rm " + name + " && docker rmi -f cved/" + name;
    else
        arguments << "-c" << "docker rmi -f cved/" + name;

    process = new Process(this);
    connect(process, &Process::signal_send_stdout, [&] (QString str) {
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_send_stderr, [&] (QString str) {
        ui->edit_output->appendPlainText(str);
    });
    connect(process, &Process::signal_finished, [&] (int exit_code) {
        if (exit_code == 0) {
            ui->label_status_data->setText("NULL<br>");
            ui->button_pull->setEnabled(true);
            is_containerized = false;
            emit signal_send_data(name, "status", "NULL");
            emit signal_send_data(name, "containerized", "false");
        } else {
            ui->label_status_data->setText("ERROR<br>");
        }
        process->deleteLater();
    });
    process->start(command, arguments);

    ui->button_start->setDisabled(true);
    ui->button_delete->setDisabled(true);
    ui->edit_output->clear();
}
