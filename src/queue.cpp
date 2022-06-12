#include "queue.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <QDebug>

queue::queue(QWidget *parent) :
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

void queue::info(const QModelIndex & ind)
{
    QueItem res;
    int row = ind.row();
    res = que_model->que_data[row];
    emit send_data(res);
}

queue::~queue()
{
}

QVariant QueModel::data (const QModelIndex &index, int role) const
{
   if (role == Qt::DisplayRole || role == Qt::EditRole)
   {
       const QueItem &current_que = que_data[index.row ()];
       return current_que.data[index.column ()];
   }
   return {};
}


const char *enum_to_string_cart (que_fields field)
{
    switch (field)
    {
        case que_fields::title  : return "Title";
        case que_fields::views  : return "Views";
        case que_fields::likes  : return "Likes";
        case que_fields::COUNT  : return "";
    }
    return {};
}

QVariant QueModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        que_fields field = (que_fields)section;
        return enum_to_string_cart(field);
    }
    return {};
}
