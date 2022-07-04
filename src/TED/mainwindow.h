#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "queue.h"
#include "models.h"
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QSortFilterProxyModel>

class VidModel;
class Vid;
class FilterModel;

class MainWindow : public QMainWindow
{
public:
    Q_OBJECT
    QTableView *vid_view = nullptr;
    VidModel *vid_model = nullptr;
    FilterModel *filter_model = nullptr;
    QPushButton *open_que;
    QLabel *total;
    QLabel *search_lb;
    QLineEdit *search_line;
    QComboBox *sorting_value;


public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<Vid> load_data (const QString &dir);
    void save_data (const QString &dir);
    void save_queue (const QString &dir);
    void view_info(Vid &res);
    Queue *que_window;
    ~MainWindow();

private slots:
    void load_data_function();
    void save_data_function();
    void save_queue_function();
    void filter_data();
    void get_info(const QModelIndex &ind);
    void receive_add(Vid& air);
    void receive_delete(Vid& air);
    void receive_like_add(Vid &air, bool &check);
    void receive_like_del(Vid &air, bool &check);
    void show_queue();
    void get_info_que(QueItem& air);
    void update_total();
    void sort_function();
    void about_window();

signals:
    void sendData(Vid& air, bool& flag, QString& next);

friend class FilterModel;
};

#endif // MAINWINDOW_H
