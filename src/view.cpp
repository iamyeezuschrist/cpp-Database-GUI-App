#include "view.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <fstream>
#include <sstream>

view::view(QWidget *parent) :
    QDialog(parent)
{

    QGridLayout *l = new QGridLayout(this);

    info0 = new QLabel;
    info1 = new QLabel;
    info2 = new QLabel;
    info3 = new QLabel;
    info4 = new QLabel;
    info5 = new QLabel;
    info6 = new QLabel;
    info7 = new QLabel;
    add = new QPushButton;
    like = new QPushButton;

    info0->setStyleSheet(QString("font-size: %1px; font-weight: bold").arg(40));
    info0->setAlignment(Qt::AlignCenter);
    info0->setText("Video Player");
    info1->setStyleSheet(QString("font-size: %1px;").arg(30));
    info1->setAlignment(Qt::AlignCenter);
    info2->setStyleSheet(QString("font-size: %1px").arg(20));
    info3->setStyleSheet(QString("font-size: %1px").arg(20));
    info4->setStyleSheet(QString("font-size: %1px").arg(20));
    info5->setStyleSheet(QString("font-size: %1px").arg(20));
    info6->setStyleSheet(QString("font-size: %1px").arg(25));
    info6->setAlignment(Qt::AlignCenter);
    info7->setStyleSheet(QString("font-size: %1px").arg(20));

    add->setStyleSheet(QString("font-size: %1px").arg(25));
    like->setStyleSheet(QString("font-size: %1px").arg(25));
    like->setText("Like");
    like->setMaximumWidth(70);

    connect(add, SIGNAL(clicked()), this, SLOT(change()));
    connect(like, SIGNAL(clicked()), this, SLOT(change_like()));


    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(info4);
    box->addWidget(info3);
    box->addWidget(info5);
    box->addWidget(like);
    box->addWidget(info7);
    box->addStretch();
    l->addWidget(info0);
    l->addWidget(info1);
    l->addWidget(info2);
    l->addWidget(info6);
    l->addLayout(box, 4, 0);
    l->addWidget(add);

}

void view::change_like()
{
    if(like_press)
    {
        like->setStyleSheet("background-color: rgb(255,255,255);");
        like->setText("Like");
        like_press = false;
        emit like_delete(a, check);
        likes--;
    }
    else
    {
        like->setStyleSheet("background-color: rgb(255,0,0);");
        like->setText("Liked");
        like_press = true;
        emit like_add(a, check);
        likes++;

    }
    info5->setText(QString::number(likes) + " likes");
}

void view::change()
{
    if(check)
    {
        add->setStyleSheet("background-color: rgb(255,255,255);");
        add->setText("Add to Queue");
        check = false;
        emit send_delete(a);
    }
    else
    {
        add->setStyleSheet("background-color: rgb(255,255,0);");
        add->setText("Delete from Queue");
        check = true;
        a.data[4] = likes;
        emit send_add(a);
    }
}

void view::recieveData(Vid& air, bool& flag, QString &next)
{
    a = air;
    likes = air.data[4].toInt();
    info1->setText(air.data[0].toString().toStdString().c_str());
    info2->setText(("Author: " + air.data[1].toString().toStdString()).c_str());
    info3->setText(("Date: " + air.data[2].toString().toStdString()).c_str());
    info4->setText((air.data[3].toString().toStdString() + " views").c_str());
    info5->setText((air.data[4].toString().toStdString() + " likes").c_str());
    std::string s = air.data[5].toString().toStdString();
    info6->setText(("<a href=\"" + s + "\">Video Link</a>").c_str());
    info6->setTextFormat(Qt::RichText);
    info6->setTextInteractionFlags(Qt::TextBrowserInteraction);
    info6->setOpenExternalLinks(true);
    info7->setText("Next Video: " + next);
    check = flag;
    if(check)
    {
        add->setStyleSheet("background-color: rgb(255,255,0);");
        add->setText("Delete from Queue");
    }
    else
    {
        add->setStyleSheet("background-color: rgb(255,255,255);");
        add->setText("Add to Queue");
    }
}

view::~view()
{
}
