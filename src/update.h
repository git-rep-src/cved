#ifndef UPDATE_H
#define UPDATE_H

#include "ui.h"
#include "process.h"

namespace Ui {
class Update;
}

class Update : public QObject
{
    Q_OBJECT

public:
    Update(QObject *parent);
    ~Update();

public slots:
    Ui::Update *get_ui() { return ui; }

    void check();

signals:
    void signal_finished();

private:
    Ui::Update *ui;

private slots:
    void start();
};

#endif
