#include "about.h"
#include "ui_about.h"
#include <QPainter>
#include <QLayout>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());

}

about::~about()
{
    delete ui;
}

void about::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen({QColorConstants::Svg::red, 20});
    painter.setBrush(QBrush(QColorConstants::Svg::white));
    painter.translate(width()/2,height()/2);
    painter.drawEllipse(-175,-250,350,350);
    painter.setPen({QColorConstants::Svg::red, 10});
    painter.drawLine(-110, -50, -140, -100);
    painter.drawLine(-110, -50, -80, -100);
    painter.drawLine(-50, -50, -80, -100);
    painter.drawLine(-50, -50, -20, -100);
    painter.drawLine(60, -100, 0, -100);
    painter.drawLine(80, -100, 140, -100);
    painter.drawLine(30, -100, 30, -50);
    painter.drawLine(110, -100, 110, -50);
}
