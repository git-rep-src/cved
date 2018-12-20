#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QComboBox>
#include <QListView>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QApplication>

QT_BEGIN_NAMESPACE

class Ui_Base
{
public:
    QWidget *widget;
    QVBoxLayout *layout;

    void setup()
    {
        widget = new QWidget;
        widget->setMaximumSize(QSize(1600, 900)); // TODO: Percent
        widget->setWindowFlags(Qt::CustomizeWindowHint);
        
        layout = new QVBoxLayout(widget);
        layout->setSpacing(0);
        layout->setMargin(10); // TODO: Percent
        layout->setAlignment(Qt::AlignTop);

        widget->setLayout(layout);
    }
};

class Ui_Manager
{
public:
    QWidget *widget;
    QComboBox *combo_cve;
    QLabel *label_description;
    QLabel *label_description_data;
    QLabel *label_author;
    QLabel *label_author_data;
    QLabel *label_image;
    QLabel *label_image_data;
    QLabel *label_size;
    QLabel *label_size_data;
    QLabel *label_target;
    QLabel *label_target_data;
    QLabel *label_status;
    QLabel *label_status_data;
    QLabel *label_network;
    QLabel *label_network_data;
    QPushButton *button_pull;
    QPushButton *button_start;
    QPushButton *button_stop;
    QPushButton *button_delete;
    QPlainTextEdit *edit_output;
    QVBoxLayout *layout_labels;
    QHBoxLayout *layout_buttons;
    QVBoxLayout *layout;

    void setup()
    {
        QFont font(":/font");
        font.setPointSize(8); // TODO: Percent
        font.setBold(true);

        widget = new QWidget;

        combo_cve = new QComboBox(widget);
        combo_cve->setFont(font);
        combo_cve->setMinimumHeight(33); // TODO: Percent
        combo_cve->setEditable(true);
        combo_cve->setView(new QListView());
        combo_cve->view()->setFont(font);
        combo_cve->lineEdit()->setFont(font);
        combo_cve->lineEdit()->setReadOnly(true);
        combo_cve->lineEdit()->setAlignment(Qt::AlignCenter);
        combo_cve->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        label_description = new QLabel(widget);
        label_description->setFont(font);
        label_description->setProperty("type", "title");
        label_description->setMinimumHeight(33); // TODO: Percent
        label_description->setText("DESCRIPTION<hr>");

        label_description_data = new QLabel(widget);
        label_description_data->setProperty("type", "data");
        label_description_data->setMinimumHeight(33); // TODO: Percent
        label_description_data->setWordWrap(true);
        label_description_data->setDisabled(true);

        label_author = new QLabel(widget);
        label_author->setFont(font);
        label_author->setProperty("type", "title");
        label_author->setMinimumHeight(33); // TODO: Percent
        label_author->setText("AUTHOR<hr>");

        label_author_data = new QLabel(widget);
        label_author_data->setProperty("type", "data");
        label_author_data->setMinimumHeight(33); // TODO: Percent
        label_author_data->setDisabled(true);

        label_image = new QLabel(widget);
        label_image->setFont(font);
        label_image->setProperty("type", "title");
        label_image->setMinimumHeight(33); // TODO: Percent
        label_image->setText("IMAGE<hr>");

        label_image_data = new QLabel(widget);
        label_image_data->setProperty("type", "data");
        label_image_data->setMinimumHeight(33); // TODO: Percent
        label_image_data->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByMouse);
        label_image_data->setOpenExternalLinks(true);

        label_size = new QLabel(widget);
        label_size->setFont(font);
        label_size->setProperty("type", "title");
        label_size->setMinimumHeight(33); // TODO: Percent
        label_size->setText("SIZE<hr>");

        label_size_data = new QLabel(widget);
        label_size_data->setProperty("type", "data");
        label_size_data->setMinimumHeight(33); // TODO: Percent
        label_size_data->setDisabled(true);

        label_target = new QLabel(widget);
        label_target->setFont(font);
        label_target->setProperty("type", "title");
        label_target->setMinimumHeight(33); // TODO: Percent
        label_target->setText("TARGET<hr>");

        label_target_data = new QLabel(widget);
        label_target_data->setProperty("type", "data");
        label_target_data->setMinimumHeight(33); // TODO: Percent
        label_target_data->setDisabled(true);

        label_status = new QLabel(widget);
        label_status->setFont(font);
        label_status->setProperty("type", "title");
        label_status->setMinimumHeight(33); // TODO: Percent
        label_status->setText("STATUS<hr>");

        label_status_data = new QLabel(widget);
        label_status_data->setFont(font);
        label_status_data->setProperty("type", "data");
        label_status_data->setMinimumHeight(33); // TODO: Percent

        label_network = new QLabel(widget);
        label_network->setFont(font);
        label_network->setProperty("type", "title");
        label_network->setMinimumHeight(33); // TODO: Percent
        label_network->setText("NETWORK<hr>");

        label_network_data = new QLabel(widget);
        label_network_data->setFont(font);
        label_network_data->setProperty("type", "data");

        layout_labels = new QVBoxLayout;
        layout_labels->setSpacing(0);
        layout_labels->setMargin(0);
        layout_labels->addWidget(label_description);
        layout_labels->addWidget(label_description_data);
        layout_labels->addWidget(label_author);
        layout_labels->addWidget(label_author_data);
        layout_labels->addWidget(label_image);
        layout_labels->addWidget(label_image_data);
        layout_labels->addWidget(label_size);
        layout_labels->addWidget(label_size_data);
        layout_labels->addWidget(label_target);
        layout_labels->addWidget(label_target_data);
        layout_labels->addWidget(label_status);
        layout_labels->addWidget(label_status_data);
        layout_labels->addWidget(label_network);
        layout_labels->addWidget(label_network_data);

        button_pull = new QPushButton(widget);
        button_pull->setFont(font);
        button_pull->setMinimumHeight(30); // TODO: Percent
        button_pull->setFlat(true);
        button_pull->setText("PULL");
        button_pull->setDisabled(true);

        button_start = new QPushButton(widget);
        button_start->setFont(font);
        button_start->setMinimumHeight(30); // TODO: Percent
        button_start->setFlat(true);
        button_start->setText("START");
        button_start->setDisabled(true);

        button_stop = new QPushButton(widget);
        button_stop->setFont(font);
        button_stop->setMinimumHeight(30); // TODO: Percent
        button_stop->setFlat(true);
        button_stop->setText("STOP");
        button_stop->setDisabled(true);

        button_delete = new QPushButton(widget);
        button_delete->setFont(font);
        button_delete->setMinimumHeight(30); // TODO: Percent
        button_delete->setFlat(true);
        button_delete->setText("DELETE");
        button_delete->setDisabled(true);

        layout_buttons = new QHBoxLayout;
        layout_buttons->setSpacing(1);
        layout_buttons->setMargin(0);
        layout_buttons->addWidget(button_pull);
        layout_buttons->addWidget(button_start);
        layout_buttons->addWidget(button_stop);
        layout_buttons->addWidget(button_delete);

        edit_output = new QPlainTextEdit(widget);
        edit_output->setProperty("type", "data");
        edit_output->setReadOnly(true);
        edit_output->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        edit_output->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        layout = new QVBoxLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->addWidget(combo_cve);
        layout->addSpacing(30);// TODO: Percent
        layout->addLayout(layout_labels);
        layout->addSpacing(30);// TODO: Percent
        layout->addLayout(layout_buttons);
        layout->addWidget(edit_output);

        widget->setLayout(layout);
    }
};

namespace Ui {
    class Cved: public Ui_Base {};
    class Manager: public Ui_Manager {};
}

QT_END_NAMESPACE

#endif
