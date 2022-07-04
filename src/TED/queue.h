#ifndef QUEUE_H
#define QUEUE_H
#include <QMainWindow>
#include <QDialog>
#include <QTableView>
#include <unordered_set>
#include <QLineEdit>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QLabel>
#include "models.h"


class QueModel;
class QueItem;

class Queue : public QWidget
{
    Q_OBJECT
public:
    QTableView *que_view = nullptr;
    QueModel *que_model = nullptr;
    QLabel *total;

public:
    explicit Queue(QWidget *parent = nullptr);
    ~Queue();

private slots:
    void info(const QModelIndex & ind);

signals:
    void send_data(QueItem &res);

};
#endif // QUEUE_H
