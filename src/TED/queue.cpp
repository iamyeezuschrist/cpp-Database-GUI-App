#include "queue.h"
#include "models.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <QDebug>

Queue::Queue(QWidget *parent) :
    QWidget(parent)
{
    que_model = new QueModel();
    que_view = new QTableView(this);
    total = new QLabel();

    QGridLayout *l = new QGridLayout(this);

    que_view->setModel(que_model);
    total->setText("Total number of videos: 0");
    total->setStyleSheet(QString("font-size: %1px").arg(30));

    connect(que_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(info(QModelIndex)));

    l->addWidget(total);
    l->addWidget(que_view);
}

void Queue::info(const QModelIndex & ind)
{
    QueItem res;
    int row = ind.row();
    res = que_model->que_data[row];
    emit send_data(res);
}

Queue::~Queue()
{
}
