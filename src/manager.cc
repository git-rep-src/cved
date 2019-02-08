#include "manager.h"

Manager::Manager(QObject *parent) :
    QObject(parent),
    ui(new Ui::Manager),
    finder(NULL),
    process(NULL),
    is_combo_blocked(true),
    is_pulling(false),
    is_pull_aborted(false)
{
    ui->setup();

    finder = new Finder(this);
    connect(finder, &Finder::signal_send_combo, [&] (const QStringList &data) {
        this->set_combo(data);
    });
    connect(finder, &Finder::signal_send_data, [&] (const QStringList &data) {
        this->set_data(data);
    });
    connect(finder, &Finder::signal_empty_data, [&] () {
        ui->set_property(ui->edit_target, NULL, "edit-target-fail");
    });
    if (finder->open_db()) {
        finder->get_data("", true);
    } else {
        ui->combo_name->setProperty("style", "combo-fail");
        ui->combo_name->style()->unpolish(ui->combo_name);
        ui->combo_name->style()->polish(ui->combo_name);
        ui->combo_name->setCurrentText("DATABASE ERROR");
        ui->combo_name->setDisabled(true);
    }

    connect(ui->combo_name, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), [&] {
        if (!is_combo_blocked) {
            name = ui->combo_name->currentText().toLower();
            finder->get_data(name, false);
        }
    });
    connect(ui->edit_target, &QLineEdit::returnPressed, [&] () {
        ui->set_property(ui->edit_target, NULL, "edit-target-active");
        finder->get_data(ui->edit_target->text(), true);
    });
    connect(ui->edit_target, &QLineEdit::textEdited, [&] (const QString &str) {
        Q_UNUSED(str);
        const QString style = ui->edit_target->property("style").toString();
        if ((style == "edit-target-active") || (style == "edit-target-fail"))
            ui->set_property(ui->edit_target, NULL, "edit-target");
    });
    connect(ui->button_pull, SIGNAL(clicked()), this, SLOT(docker_pull()));
    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(docker_start()));
    connect(ui->button_stop, SIGNAL(clicked()), this, SLOT(docker_stop()));
    connect(ui->button_delete, SIGNAL(clicked()), this, SLOT(docker_delete()));

    QTimer::singleShot(0, ui->edit_target, SLOT(setFocus()));
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

    is_combo_blocked = true;

    for (int i = 0; i < ui->combo_name->count(); i++)
        ui->combo_name->removeItem(i);
    ui->combo_name->clear();
    for (int i = 0; i < data.size(); i++) {
        ui->combo_name->addItem(data.at(i).toUpper());
        ui->combo_name->model()->setData(ui->combo_name->model()->index(i, 0), QSize(0, int(sh / 38.5)), Qt::SizeHintRole);
        ui->combo_name->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    ui->combo_name->setCurrentIndex(0);

    is_combo_blocked = false;

    finder->get_data(name, false);
}

void Manager::set_data(const QStringList &data)
{
    ui->label_description_data->setText(data.at(0));
    ui->label_author_data->setText(data.at(1));
    ui->label_source_data->setText(data.at(2));
    ui->label_size_data->setText(data.at(3));
    ui->label_target_data->setText(data.at(4));
    ui->label_misc_data->setText(data.at(5));

    network = data.at(6);
    options = data.at(7);

    set_status();
}

void Manager::set_status()
{
    const QString buf = "$(docker images | grep " + name +
                        " &>/dev/null); if [ $? -eq 0 ]; then $(docker ps | grep " + name +
                        " &>/dev/null); if [ $? -eq 0 ]; then echo RUNNING; else echo STOPPED; fi; else echo NULL; fi";
    const QString cmd = "echo " + buf.toUtf8().toBase64() + " | base64 -d | /bin/bash";

    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        QString ret = QString::fromUtf8(process->readAllStandardOutput()).remove("\n");
        if (ret == "STOPPED") {
            ui->set_property(NULL, ui->label_status_data, "label-data");
            ui->label_status_data->setText(ret);
            ui->set_property(NULL, ui->label_network_data, "label-data");
            if (!network.isEmpty())
                ui->label_network_data->setText(network);
            else
                ui->label_network_data->setText("NULL");
            ui->button_pull->setDisabled(true);
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        } else if (ret == "RUNNING") {
            ui->set_property(NULL, ui->label_status_data, "label-data-running");
            ui->label_status_data->setText(ret);
            if (!network.isEmpty()) {
                ui->set_property(NULL, ui->label_network_data, "label-data-active");
                ui->label_network_data->setText(network);
            } else {
                ui->set_property(NULL, ui->label_network_data, "label-data");
                ui->label_network_data->setText("NULL");
            }
            ui->button_pull->setDisabled(true);
            ui->button_start->setDisabled(true);
            ui->button_stop->setEnabled(true);
            ui->button_delete->setDisabled(true);
        } else {
            ui->set_property(NULL, ui->label_status_data, "label-data");
            ui->label_status_data->setText("NULL");
            ui->set_property(NULL, ui->label_network_data, "label-data");
            ui->label_network_data->setText("NULL");
            ui->button_pull->setEnabled(true);
            ui->button_start->setDisabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setDisabled(true);
        }
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->set_property(NULL, ui->label_status_data, "label-data");
        ui->label_status_data->setText("NULL");
        ui->set_property(NULL, ui->label_network_data, "label-data");
        ui->label_network_data->setText("NULL");
        ui->button_pull->setEnabled(true);
        ui->button_start->setDisabled(true);
        ui->button_stop->setDisabled(true);
        ui->button_delete->setDisabled(true);
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&] {
        ui->plain_output->clear();
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << cmd );
}

void Manager::docker_pull()
{
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        ui->plain_output->appendPlainText(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->plain_output->appendHtml("<span style=color:#821e1e>" + process->readAllStandardError() + "</span>");
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->set_property(NULL, ui->label_status_data, "label-data");
            ui->label_status_data->setText("STOPPED");
            if (!network.isEmpty())
                ui->label_network_data->setText(network);
            ui->button_start->setEnabled(true);
        } else {
            ui->set_property(NULL, ui->label_status_data, "label-data-fail");
            if (is_pull_aborted) {
                ui->label_status_data->setText("ABORTED");
                is_pull_aborted = false;
            } else {
                ui->label_status_data->setText("FAIL");
            }
            ui->button_pull->setEnabled(true);
            ui->button_delete->setDisabled(true);
        }
        ui->combo_name->setEnabled(true);
        ui->edit_target->setEnabled(true);
        ui->button_delete->setText("DELETE");
        is_pulling = false;
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker pull cved/" + name);

    is_pulling = true;

    ui->combo_name->setDisabled(true);
    ui->edit_target->setDisabled(true);
    ui->set_property(NULL, ui->label_status_data, "label-data-pulling");
    ui->label_status_data->setText("PULLING <span style=color:white>cved/" + name + "</span>");
    ui->button_pull->setDisabled(true);
    ui->button_delete->setText("ABORT");
    ui->button_delete->setEnabled(true);
    ui->plain_output->clear();
}

void Manager::docker_start()
{
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        ui->plain_output->appendPlainText(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->plain_output->appendHtml("<span style=color:#821e1e>" + process->readAllStandardError() + "</span>");
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->set_property(NULL, ui->label_status_data, "label-data-running");
            ui->label_status_data->setText("RUNNING");
            if (!network.isEmpty()) {
                ui->set_property(NULL, ui->label_network_data, "label-data-active");
                ui->label_network_data->setText(network);
            }
        } else {
            ui->combo_name->setEnabled(true);
            ui->edit_target->setEnabled(true);
            ui->set_property(NULL, ui->label_status_data, "label-data-fail");
            ui->label_status_data->setText("FAIL");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        }
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker run --rm --detach --name " + name + " " + options + " cved/" + name);

    ui->combo_name->setDisabled(true);
    ui->edit_target->setDisabled(true);
    ui->button_start->setDisabled(true);
    ui->button_stop->setEnabled(true);
    ui->button_delete->setDisabled(true);
    ui->label_status_data->setText("STARTING...");
    ui->plain_output->clear();
}

void Manager::docker_stop()
{
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, [&] {
        ui->plain_output->appendPlainText(process->readAllStandardOutput());
    });
    connect(process, &QProcess::readyReadStandardError, [&] {
        ui->plain_output->appendHtml("<span style=color:#821e1e>" + process->readAllStandardError() + "</span>");
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int exit_code) {
        if (exit_code == 0) {
            ui->combo_name->setEnabled(true);
            ui->edit_target->setEnabled(true);
            ui->label_status_data->setText("STOPPED");
            ui->set_property(NULL, ui->label_network_data, "label-data");
            ui->button_start->setEnabled(true);
            ui->button_stop->setDisabled(true);
            ui->button_delete->setEnabled(true);
        } else {
            ui->set_property(NULL, ui->label_status_data, "label-data-fail");
            ui->label_status_data->setText("FAIL");
        }
        process->deleteLater();
    });
    process->start("/bin/bash", QStringList() << "-c" << "docker stop " + name);

    ui->set_property(NULL, ui->label_status_data, "label-data");
    ui->label_status_data->setText("STOPPING...");
    ui->plain_output->clear();
}

void Manager::docker_delete()
{
    if (is_pulling) {
        is_pull_aborted = true;
        process->close();
    } else {
        process = new QProcess(this);
        connect(process, &QProcess::readyReadStandardOutput, [&] {
            ui->plain_output->appendPlainText(process->readAllStandardOutput());
        });
        connect(process, &QProcess::readyReadStandardError, [&] {
            ui->plain_output->appendHtml("<span style=color:#821e1e>" + process->readAllStandardError() + "</span>");
        });
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [=] (int exit_code) {
            if (exit_code == 0) {
                ui->set_property(NULL, ui->label_status_data, "label-data");
                ui->label_status_data->setText("NULL");
                ui->set_property(NULL, ui->label_network_data, "label-data");
                ui->label_network_data->setText("NULL");
                ui->button_pull->setEnabled(true);
            } else {
                ui->set_property(NULL, ui->label_status_data, "label-data-fail");
                ui->label_status_data->setText("FAIL");
            }
            process->deleteLater();
        });
        process->start("/bin/bash", QStringList() << "-c" << "docker rmi -f cved/" + name);
        ui->button_start->setDisabled(true);
        ui->button_delete->setDisabled(true);
        ui->plain_output->clear();
    }
}
