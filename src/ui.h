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

#include <QApplication>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE

class Ui_Cved
{
public:
    QWidget *widget;
    QPushButton *button_close;
    QVBoxLayout *layout;

    void setup()
    {
        int sw = QApplication::desktop()->screenGeometry().width();
        int sh = QApplication::desktop()->screenGeometry().height();

        widget = new QWidget;
        widget->setMaximumSize(QSize(int(sw / 1.2), int(sh / 1.2)));
        widget->setWindowFlags(Qt::CustomizeWindowHint);
        
        button_close = new QPushButton(widget);
        button_close->setProperty("type", "button-close");
        button_close->setGeometry((int(sw / 1.2) - (sw / 192)), 0, (sw / 192), (sh / 108));
        button_close->setFlat(true);
        button_close->setText("x");
        button_close->setHidden(true);

        layout = new QVBoxLayout(widget);
        layout->setSpacing(0);
        layout->setMargin(sw / 192);

        widget->setLayout(layout);
    }
};

class Ui_Update
{
public:
    QWidget *widget;
    QLabel *label_message;
    QVBoxLayout *layout;

    void setup()
    {
        int sw = QApplication::desktop()->screenGeometry().width();

        QFont font(":/font");
        font.setPointSize(sw / 240);
        font.setBold(true);

        widget = new QWidget;

        label_message = new QLabel(widget);
        label_message->setFont(font);
        label_message->setProperty("type", "label-update");
        label_message->setText("UPDATING DATABASE");

        layout = new QVBoxLayout(widget);
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->setAlignment(Qt::AlignCenter);
        layout->addWidget(label_message);

        widget->setLayout(layout);
    }
};

class Ui_Manager
{
public:
    QWidget *widget;
    QComboBox *combo_name;
    QLabel *label_description_data;
    QLabel *label_author;
    QLabel *label_author_data;
    QLabel *label_source;
    QLabel *label_source_data;
    QLabel *label_size;
    QLabel *label_size_data;
    QLabel *label_target;
    QLabel *label_target_data;
    QLabel *label_misc;
    QLabel *label_misc_data;
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
        int sw = QApplication::desktop()->screenGeometry().width();
        int sh = QApplication::desktop()->screenGeometry().height();

        QFont font(":/font");
        font.setPointSize(sw / 240);
        font.setBold(true);

        widget = new QWidget;

        combo_name = new QComboBox(widget);
        combo_name->setFont(font);
        combo_name->setMinimumHeight(int(sh / 32.7));
        combo_name->setEditable(true);
        combo_name->setView(new QListView());
        combo_name->view()->setFont(font);
        combo_name->view()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        combo_name->lineEdit()->setFont(font);
        combo_name->lineEdit()->setReadOnly(true);
        combo_name->lineEdit()->setAlignment(Qt::AlignCenter);
        combo_name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        label_description_data = new QLabel(widget);
        label_description_data->setProperty("type", "label-data-description");
        label_description_data->setMargin(sw / 147);
        label_description_data->setWordWrap(true);

        label_author = new QLabel(widget);
        label_author->setFont(font);
        label_author->setProperty("type", "label-title");
        label_author->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_author->setText("AUTHOR");

        label_author_data = new QLabel(widget);
        label_author_data->setProperty("type", "label-data");
        label_author_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));
        label_author_data->setTextInteractionFlags(Qt::TextSelectableByMouse);

        label_source = new QLabel(widget);
        label_source->setFont(font);
        label_source->setProperty("type", "label-title");
        label_source->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_source->setText("SOURCE");

        label_source_data = new QLabel(widget);
        label_source_data->setProperty("type", "label-data");
        label_source_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));
        label_source_data->setTextInteractionFlags(Qt::TextSelectableByMouse);

        label_size = new QLabel(widget);
        label_size->setFont(font);
        label_size->setProperty("type", "label-title");
        label_size->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_size->setText("SIZE");

        label_size_data = new QLabel(widget);
        label_size_data->setProperty("type", "label-data");
        label_size_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));

        label_target = new QLabel(widget);
        label_target->setFont(font);
        label_target->setProperty("type", "label-title");
        label_target->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_target->setText("TARGET");

        label_target_data = new QLabel(widget);
        label_target_data->setProperty("type", "label-data");
        label_target_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));

        label_misc = new QLabel(widget);
        label_misc->setFont(font);
        label_misc->setProperty("type", "label-title");
        label_misc->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_misc->setText("MISC");

        label_misc_data = new QLabel(widget);
        label_misc_data->setProperty("type", "label-data");
        label_misc_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));
        label_misc_data->setTextInteractionFlags(Qt::TextSelectableByMouse);

        label_status = new QLabel(widget);
        label_status->setFont(font);
        label_status->setProperty("type", "label-title");
        label_status->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_status->setText("STATUS");

        label_status_data = new QLabel(widget);
        label_status_data->setFont(font);
        label_status_data->setProperty("type", "label-data");
        label_status_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));

        label_network = new QLabel(widget);
        label_network->setFont(font);
        label_network->setProperty("type", "label-title");
        label_network->setContentsMargins((sw / 147), 0, 0, (sh / 154));
        label_network->setText("NETWORK");

        label_network_data = new QLabel(widget);
        label_network_data->setFont(font);
        label_network_data->setProperty("type", "label-data");
        label_network_data->setContentsMargins((sw / 147), (sh / 216), (sw / 147), (sh / 51));
        label_network_data->setTextInteractionFlags(Qt::TextSelectableByMouse);

        layout_labels = new QVBoxLayout;
        layout_labels->setSpacing(0);
        layout_labels->setMargin(0);
        layout_labels->addWidget(label_description_data);
        layout_labels->addSpacing(sh / 36);
        layout_labels->addWidget(label_author);
        layout_labels->addWidget(label_author_data);
        layout_labels->addWidget(label_source);
        layout_labels->addWidget(label_source_data);
        layout_labels->addWidget(label_size);
        layout_labels->addWidget(label_size_data);
        layout_labels->addWidget(label_target);
        layout_labels->addWidget(label_target_data);
        layout_labels->addWidget(label_misc);
        layout_labels->addWidget(label_misc_data);
        layout_labels->addWidget(label_status);
        layout_labels->addWidget(label_status_data);
        layout_labels->addWidget(label_network);
        layout_labels->addWidget(label_network_data);

        button_pull = new QPushButton(widget);
        button_pull->setFont(font);
        button_pull->setProperty("type", "button-pull");
        button_pull->setMinimumSize(QSize(int(sw / 19.2), (sh / 45)));
        button_pull->setFlat(true);
        button_pull->setText("PULL");
        button_pull->setDisabled(true);

        button_start = new QPushButton(widget);
        button_start->setFont(font);
        button_start->setProperty("type", "button-start");
        button_start->setMinimumSize(QSize(int(sw / 19.2), (sh / 45)));
        button_start->setFlat(true);
        button_start->setText("START");
        button_start->setDisabled(true);

        button_stop = new QPushButton(widget);
        button_stop->setFont(font);
        button_stop->setProperty("type", "button-stop");
        button_stop->setMinimumSize(QSize(int(sw / 19.2), (sh / 45)));
        button_stop->setFlat(true);
        button_stop->setText("STOP");
        button_stop->setDisabled(true);

        button_delete = new QPushButton(widget);
        button_delete->setFont(font);
        button_delete->setProperty("type", "button-delete");
        button_delete->setMinimumSize(QSize(int(sw / 19.2), (sh / 45)));
        button_delete->setFlat(true);
        button_delete->setText("DELETE");
        button_delete->setDisabled(true);

        layout_buttons = new QHBoxLayout;
        layout_buttons->setSpacing(2);
        layout_buttons->setMargin(0);
        layout_buttons->setAlignment(Qt::AlignLeft);
        layout_buttons->addWidget(button_pull);
        layout_buttons->addWidget(button_start);
        layout_buttons->addWidget(button_stop);
        layout_buttons->addWidget(button_delete);

        edit_output = new QPlainTextEdit(widget);
        edit_output->setReadOnly(true);
        edit_output->setWordWrapMode(QTextOption::WordWrap);
        edit_output->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        layout = new QVBoxLayout(widget);
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->addWidget(combo_name);
        layout->addSpacing(sh / 216);
        layout->addLayout(layout_labels);
        layout->addSpacing(sh / 108);
        layout->addLayout(layout_buttons);
        layout->addSpacing(sh / 216);
        layout->addWidget(edit_output);

        widget->setLayout(layout);
    }

    void set_property(QLabel *label, QString property)
    {
        label->setProperty("type", property);
        label->style()->unpolish(label);
        label->style()->polish(label);
    }
};

namespace Ui {
    class Cved: public Ui_Cved {};
    class Update: public Ui_Update {};
    class Manager: public Ui_Manager {};
}

QT_END_NAMESPACE

#endif
