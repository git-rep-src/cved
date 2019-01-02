#include "manager.h"

Manager::Manager(QObject *parent) :
    QObject(parent),
    ui(new Ui::Manager),
    finder(NULL),
    process(NULL),
    is_pulling(false),
    is_pull_aborted(false)
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
    if (finder->open_db()) {
        finder->get_data("", true);
    } else {
        ui->combo_cve->setProperty("type", "combo-failed");
        ui->combo_cve->style()->unpolish(ui->combo_cve);
        ui->combo_cve->style()->polish(ui->combo_cve);
        ui->combo_cve->setCurrentText("DATABASE ERROR");
        ui->combo_cve->setDisabled(true);
    }

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
    int sh = QApplication::desktop()->screenGeometry().height();

    name = data.at(0);

    for (int i = 0; i < ui->combo_cve->count(); i++)
        ui->combo_cve->removeItem(i);
    for (int i = 0; i < data.size(); i++) {
        ui->combo_cve->addItem(data.at(i).toUpper());
        ui->combo_cve->model()->setData(ui->combo_cve->model()->index(i, 0), QSize(0, int(sh / 38.5)), Qt::SizeHintRole);
        ui->combo_cve->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    ui->combo_cve->setCurrentIndex(0);

    finder->get_data(name, false);
}

void Manager::set_data(const QStringList &data)
{
    ui->label_description_data->setText(data.at(0));
    ui->label_container_data->setText(data.at(1));
    ui->label_size_data->setText(data.at(2));
    ui->label_target_data->setText(data.at(3));
    ui->label_misc_data->setText(data.at(4));

    network = data.at(5);
    options = data.at(6);

    set_status();
}

void Manager::set_status()
{
    process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->set_property(ui->label_status_data, "label-data-stopped");
            ui->label_status_data->setText("STOPPED");
            ui->set_property(ui->label_network_data, "label-data");
            ui->label_network_data->setText(network);
            ui->button_pull->setDisabled(true);
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        } else {
            ui->set_property(ui->label_status_data, "label-data");
            ui->label_status_data->setText("NULL");
            ui->set_property(ui->label_network_data, "label-data");
            ui->label_network_data->setText("NULL");
            ui->button_pull->setEnabled(true);
            ui->button_start->setDisabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setDisabled(true);
        }
        ui->edit_output->clear();
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker images | grep " + name + " &>/dev/null");
}


void Manager::docker_pull()
{
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        ui->edit_output->appendPlainText(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->edit_output->appendHtml("<span style=color:#851111>" + process->readAllStandardError() + "</span>");
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->set_property(ui->label_status_data, "label-data-stopped");
            ui->label_status_data->setText("STOPPED");
            ui->label_network_data->setText(network);
            ui->button_start->setEnabled(true);
        } else {
            if (is_pull_aborted) {
                ui->set_property(ui->label_status_data, "label-data-failed");
                ui->label_status_data->setText("PULL ABORTED");
                is_pull_aborted = false;
            } else {
                ui->set_property(ui->label_status_data, "label-data-failed");
                ui->label_status_data->setText("PULL FAILED");
            }
            ui->button_pull->setEnabled(true);
            ui->button_delete->setDisabled(true);
        }
        ui->combo_cve->setEnabled(true);
        ui->button_delete->setText("DELETE");
        is_pulling = false;
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker pull cved/" + name);

    is_pulling = true;

    ui->combo_cve->setDisabled(true);
    ui->set_property(ui->label_status_data, "label-data-pulling");
    ui->label_status_data->setText("PULLING");
    ui->button_pull->setDisabled(true);
    ui->button_delete->setText("ABORT");
    ui->button_delete->setEnabled(true);
    ui->edit_output->clear();
}

void Manager::docker_start()
{
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        ui->edit_output->appendPlainText(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->edit_output->appendHtml("<span style=color:#851111>" + process->readAllStandardError() + "</span>");
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->set_property(ui->label_status_data, "label-data-running");
            ui->label_status_data->setText("RUNNING");
            ui->set_property(ui->label_network_data, "label-data-active");
            ui->label_network_data->setText(network);
        } else {
            ui->combo_cve->setEnabled(true);
            ui->set_property(ui->label_status_data, "label-data-failed");
            ui->label_status_data->setText("START FAILED");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        }
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker run --rm --detach --name " + name + " " + options + " cved/" + name);

    ui->combo_cve->setDisabled(true);
    ui->button_start->setDisabled(true);
    ui->button_stop->setEnabled(true);
    ui->button_delete->setDisabled(true);
    ui->edit_output->clear();
}

void Manager::docker_stop()
{
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        ui->edit_output->appendPlainText(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->edit_output->appendHtml("<span style=color:#851111>" + process->readAllStandardError() + "</span>");
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->combo_cve->setEnabled(true);
            ui->set_property(ui->label_status_data, "label-data-stopped");
            ui->label_status_data->setText("STOPPED");
            ui->set_property(ui->label_network_data, "label-data");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        } else {
            ui->set_property(ui->label_status_data, "label-data-failed");
            ui->label_status_data->setText("STOP FAILED");
        }
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker stop " + name);

    ui->edit_output->clear();
}

void Manager::docker_delete()
{
    if (is_pulling) {
        is_pull_aborted = true;
        process->close();
    } else {
        process = new QProcess(this);
        connect(process, &QProcess::readyReadStandardOutput, [&] {
            ui->edit_output->appendPlainText(process->readAllStandardOutput());
        });
        connect(process, &QProcess::readyReadStandardError, [&] {
            ui->edit_output->appendHtml("<span style=color:#851111>" + process->readAllStandardError() + "</span>");
        });
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [=] (int exit_code) {
            if (exit_code == 0) {
                ui->set_property(ui->label_status_data, "label-data");
                ui->label_status_data->setText("NULL");
                ui->set_property(ui->label_network_data, "label-data");
                ui->label_network_data->setText("NULL");
                ui->button_pull->setEnabled(true);
            } else {
                ui->set_property(ui->label_status_data, "label-data-failed");
                ui->label_status_data->setText("DELETE FAILED");
            }
            process->deleteLater();
        });
        process->start("/bin/bash", QStringList() << "-c" << "docker rmi -f cved/" + name);
        ui->button_start->setDisabled(true);
        ui->button_delete->setDisabled(true);
        ui->edit_output->clear();
    }
}
