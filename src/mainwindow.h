#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "queue.h"
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
    queue *que_window;
    ~MainWindow();

public slots:
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

enum class vid_fields
{
    title,
    author,
    date,
    views,
    likes,
    link,
    COUNT
};

const char *enum_to_string (vid_fields field);

class Vid
{
public:
    std::array<QVariant, (static_cast<int> (vid_fields::COUNT))> data;
    friend bool operator==(const Vid &a, const Vid &b) {
        return(a.data[0] == b.data[0]);
    }
};

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterModel (QObject *parent, const MainWindow *main_window);
    bool compare(const QString &s, const QString &g) const;
protected:
    virtual bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const override;
private:
    const MainWindow *main_window = nullptr;
};

class VidModel : public QAbstractTableModel
{
Q_OBJECT

public:
    std::vector<Vid> vid_data;
    int rowCount(const QModelIndex &/*parent*/) const override
    {
        return vid_data.size ();
    }

    int columnCount(const QModelIndex &/*parent*/) const override
    {
        return static_cast<int> (vid_fields::COUNT);
    }

    void set_data (const std::vector<Vid> &data)
    {
      beginResetModel ();
      vid_data.insert(vid_data.begin(), data.begin(), data.end());
      endResetModel ();
      emit layoutChanged ();
    }

    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // MAINWINDOW_H
