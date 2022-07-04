#ifndef VIEW_H
#define VIEW_H
#include "models.h"
#include "mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QLabel>


class View : public QDialog
{
    Q_OBJECT
    QLabel *info0;
    QLabel *info1;
    QLabel *info2;
    QLabel *info3;
    QLabel *info4;
    QLabel *info5;
    QLabel *info6;
    QLabel *info7;
    QPushButton *add;
    QPushButton *like;

public:
    explicit View(QWidget *parent = nullptr);
    ~View();
    bool check = false;
    Vid a;
    int likes = 0;
    bool like_press = false;

signals:
    void update_cart_sum();
    void send_delete(Vid& air);
    void send_add(Vid& air);
    void like_delete(Vid& air, bool& check);
    void like_add(Vid& air, bool& check);

private slots:
    void recieveData(Vid& air, bool& flag, QString& next);
    void change();
    void change_like();

};

#endif // VIEW_H
