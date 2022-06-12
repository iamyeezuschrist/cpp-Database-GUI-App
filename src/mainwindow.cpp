#include "mainwindow.h"
#include "view.h"
#include "about.h"
#include <QGridLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central_widget = new QWidget (this);
    vid_model = new VidModel();
    filter_model = new FilterModel(this, this);
    que_window = new queue;
    setCentralWidget(central_widget);
    vid_view = new QTableView(central_widget);

    QGridLayout *layout = new QGridLayout(central_widget);

    open_que = new QPushButton();
    total = new QLabel();
    search_line = new QLineEdit();
    search_lb = new QLabel();
    sorting_value = new QComboBox();

    vid_view->setModel(filter_model);
    filter_model->setSourceModel(vid_model);
    filter_model->setSortCaseSensitivity(Qt::CaseInsensitive);
    open_que->setText("Open Queue");
    open_que->setStyleSheet(QString("font-size: %1px").arg(30));
    vid_view->hideColumn(5);
    total->setText("Total number of videos: 0");
    total->setStyleSheet(QString("font-size: %1px").arg(30));
    search_lb->setText("Search");
    search_lb->setStyleSheet(QString("font-size: %1px").arg(20));
    search_line->setStyleSheet(QString("font-size: %1px").arg(20));
    search_line->setMaximumWidth(200);
    sorting_value->setStyleSheet(QString("font-size: %1px").arg(20));
    sorting_value->addItem("Default");
    sorting_value->addItem("Views Ascending");
    sorting_value->addItem("Views Descending");
    sorting_value->addItem("Likes Ascending");
    sorting_value->addItem("Likes Descending");
    sorting_value->addItem("Name Ascending");
    sorting_value->addItem("Name Descending");

    connect(vid_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(get_info(QModelIndex)));
    connect(open_que, SIGNAL(clicked()), this, SLOT(show_queue()));
    connect(que_window, SIGNAL(send_data(QueItem&)), this, SLOT(get_info_que(QueItem&)));
    connect(search_line, SIGNAL(textChanged(QString)), this, SLOT(filter_data()));
    connect(sorting_value, SIGNAL(currentIndexChanged(QString)), SLOT(sort_function()));

    QHBoxLayout *box = new QHBoxLayout();
    layout->addWidget(open_que);
    box->addWidget(search_lb);
    box->addWidget(search_line);
    box->addWidget(sorting_value);
    box->addStretch();
    layout->addLayout(box, 1, 0);
    layout->addWidget(total);
    layout->addWidget(vid_view);

    auto file_menu = menuBar()->addMenu("File");
    QAction *open_action = new QAction("Open");
    connect(open_action, &QAction::triggered, this, &MainWindow::load_data_function);
    file_menu->addAction(open_action);

    QAction *saveas_action = new QAction("Save As");
    connect(saveas_action, &QAction::triggered, this, &MainWindow::save_data_function);
    file_menu->addAction(saveas_action);

    QAction *saveas_queue = new QAction("Save Queue As");
    connect(saveas_queue, &QAction::triggered, this, &MainWindow::save_queue_function);
    file_menu->addAction(saveas_queue);

    QAction *about = new QAction("About");
    connect(about, &QAction::triggered, this, &MainWindow::about_window);
    menuBar()->addAction(about);

}

void MainWindow::about_window()
{
    about about_window;
    about_window.exec();
}

void MainWindow::update_total()
{
    int length = que_window->que_model->que_data.size();
    total->setText("Total number of videos: " + QString::number(length));
    que_window->total->setText("Total number of videos: " + QString::number(length));
}

void MainWindow::show_queue()
{
    que_window->show();
}

void MainWindow::get_info_que(QueItem& air)
{
    Vid data;
    for(auto &item : vid_model->vid_data) {
        if(air.data[0] == item.data[0]) {
            item.data[3] = item.data[3].toDouble() + 1;
            data = item;
            break;
        }
    }
    view_info(data);
}

void MainWindow::get_info(const QModelIndex &ind)
{
    Vid res;
    int row = ind.row();
    vid_model->vid_data[row].data[3] = vid_model->vid_data[row].data[3].toDouble() + 1;
    res = vid_model->vid_data[row];
    view_info(res);
}

void MainWindow::view_info(Vid &res)
{
    bool flag = false;
    QString next = "None";
    for(unsigned int i = 0; i < que_window->que_model->que_data.size();i++)
    {
        if(que_window->que_model->que_data[i].data[0] == res.data[0])
        {
            flag = true;
            if(i != que_window->que_model->que_data.size() - 1)
                next = que_window->que_model->que_data[i + 1].data[0].toString();
            que_window->que_model->que_data[i].data[1] = que_window->que_model->que_data[i].data[1].toDouble() + 1;
            break;
        }
    }
    view v;
    connect(this, SIGNAL(sendData(Vid&,bool&,QString&)), &v, SLOT(recieveData(Vid&,bool&,QString&)));
    connect(&v, SIGNAL(send_delete(Vid&)), this, SLOT(receive_delete(Vid&)));
    connect(&v, SIGNAL(send_add(Vid&)), this, SLOT(receive_add(Vid&)));
    connect(&v, SIGNAL(like_delete(Vid&,bool&)), this, SLOT(receive_like_del(Vid&,bool&)));
    connect(&v, SIGNAL(like_add(Vid&,bool&)), this, SLOT(receive_like_add(Vid&,bool&)));
    emit sendData(res, flag, next);
    v.exec();
}

void MainWindow::sort_function()
{
    QString text = sorting_value->currentText();
    if(text == "Default")
        filter_model->setSortRole(Qt::InitialSortOrderRole);
    else if (text == "Views Ascending")
    {
        filter_model->setSortRole(Qt::SortOrder());
        filter_model->sort(3, Qt::AscendingOrder);
    }
    else if (text == "Views Descending")
    {
        filter_model->setSortRole(Qt::SortOrder());
        filter_model->sort(3, Qt::DescendingOrder);
    }
    else if (text == "Likes Ascending")
    {
        filter_model->setSortRole(Qt::SortOrder());
        filter_model->sort(4, Qt::AscendingOrder);
    }
    else if (text == "Likes Descending")
    {
        filter_model->setSortRole(Qt::SortOrder());
        filter_model->sort(4, Qt::DescendingOrder);
    }
    else if (text == "Name Ascending")
    {
        filter_model->setSortRole(Qt::SortOrder());
        filter_model->sort(0, Qt::AscendingOrder);
    }
    else if (text == "Name Descending")
    {
        filter_model->setSortRole(Qt::SortOrder());
        filter_model->sort(0, Qt::DescendingOrder);
    }
    filter_model->invalidate();
}

void MainWindow::receive_add(Vid& air)
{
    QueItem res;
    res.data[0] = air.data[0];
    res.data[1] = air.data[3];
    res.data[2] = air.data[4];
    que_window->que_model->add_data(res);
    update_total();
}

void MainWindow::receive_delete(Vid& air)
{
    int row = -1;
    for(unsigned int i = 0; i < que_window->que_model->que_data.size();i++)
    {
        if(que_window->que_model->que_data[i].data[0] == air.data[0])
        {
            row = i;
            break;
        }
    }
    if(row != -1)
    {
        std::unordered_set<size_t> rows_to_delete = {size_t(row)};
        que_window->que_model->delete_rows(rows_to_delete);
    }
    update_total();
}

void MainWindow::receive_like_add(Vid &air, bool &check)
{
    for(auto &item : vid_model->vid_data)
        if(item.data[0] == air.data[0])
        {
            item.data[4] = item.data[4].toDouble() + 1;
            break;
        }
    if(check)
        for(auto &item : que_window->que_model->que_data)
            if(item.data[0] == air.data[0])
            {
                item.data[2] = item.data[2].toDouble() + 1;
                break;
            }
}

void MainWindow::receive_like_del(Vid &air, bool &check)
{
    for(auto &item : vid_model->vid_data)
        if(item.data[0] == air.data[0])
        {
            item.data[4] = item.data[4].toDouble() - 1;
            break;
        }
    if(check)
        for(auto &item : que_window->que_model->que_data)
            if(item.data[0] == air.data[0])
            {
                item.data[2] = item.data[2].toDouble() - 1;
                break;

            }
}

void MainWindow::filter_data()
{
  filter_model->invalidate();
}

void MainWindow::load_data_function ()
{
    auto dir = QFileDialog::getOpenFileName (this, "Open File", QDir::currentPath ());
    if (!dir.size())
        return;
    auto data = load_data(dir);
    vid_model->set_data (data);
}


void MainWindow::save_data_function ()
{
  auto dir = QFileDialog::getSaveFileName (this, "Save File As", QDir::currentPath ());
  if (!dir.size())
    return;
  save_data(dir);
}

void MainWindow::save_queue_function()
{
  auto dir = QFileDialog::getSaveFileName (this, "Save File As", QDir::currentPath ());
  if (!dir.size())
    return;
  save_queue(dir);
}

std::vector<Vid> MainWindow::load_data (const QString &dir)
{
    std::fstream file;
    std::vector<Vid> result;
    file.open(dir.toStdString ());
    std::string temp1, temp2;
    int j = 0;
    while(std::getline(file, temp1))
    {
        Vid res;
        int i = 0;
        std::stringstream str(temp1);
        std::string temp3 = "";
        bool flag = false;
        while(std::getline(str, temp2, ','))
        {
            if(temp2[0] == '"' || flag == true)
            {
                temp3 += temp2;
                temp3 += ',';
                flag = true;
                if(temp2[temp2.size()-1] == '"')
                {
                    flag = false;
                    temp3.erase(temp3.begin());
                    temp3.erase(temp3.end()-1);
                    temp3.erase(temp3.end()-1);
                    res.data[i] = QString(temp3.c_str());
                    temp3 = "";
                    ++i;
                }
            }
            else
            {
                res.data[i] = QString(temp2.c_str());
                ++i;
            }
        }
        if(j)
        {
            res.data[3] = std::strtod(res.data[3].toString().toStdString().c_str(), NULL);
            res.data[4] = std::strtod(res.data[4].toString().toStdString().c_str(), NULL);
            result.push_back(res);
        }
        j++;
    }
    file.close();
    return result;
}

void MainWindow::save_data (const QString &dir)
{
    std::ofstream file (dir.toStdString ());
    int total_columns = static_cast<int> (vid_fields::COUNT);
    for (auto &item : vid_model->vid_data)
    {
        for (int i_column = 0; i_column < total_columns; ++i_column)
        {
            bool flag = false;
            std::string s = item.data[i_column].toString().toStdString();
            if(s.find(',') != std::string::npos)
            {
                file << "\"";
                flag = true;
            }
            file << item.data[i_column].toString().toStdString();
            if(flag)
                file << "\"";
            if (i_column < total_columns - 1)
                file << ",";
            else
                file << "\n";
        }
    }
}

void MainWindow::save_queue(const QString &dir)
{
    std::ofstream file (dir.toStdString ());
    int total_columns = static_cast<int> (que_fields::COUNT);
    for (auto &item : que_window->que_model->que_data)
    {
        for (int i_column = 0; i_column < total_columns; ++i_column)
        {
            bool flag = false;
            std::string s = item.data[i_column].toString().toStdString();
            if(s.find(',') != std::string::npos)
            {
                file << "\"";
                flag = true;
            }
            file << item.data[i_column].toString().toStdString();
            if(flag)
                file << "\"";
            if (i_column < total_columns - 1)
                file << ",";
            else
                file << "\n";
        }
    }
}

MainWindow::~MainWindow()
{
}

FilterModel::FilterModel (QObject *parent, const MainWindow *main_window) : QSortFilterProxyModel (parent), main_window (main_window)
{}

QVariant VidModel::data (const QModelIndex &index, int role) const
{
   if (role == Qt::DisplayRole || role == Qt::EditRole)
   {
       const Vid &current_vid = vid_data[index.row ()];
        return current_vid.data[index.column()];
   }
   return {};
}

const char *enum_to_string (vid_fields field)
{
    switch (field)
    {
        case vid_fields::title  : return "Title";
        case vid_fields::author : return "Author";
        case vid_fields::date   : return "Date";
        case vid_fields::views  : return "Number of views";
        case vid_fields::likes  : return "Likes";
        case vid_fields::link   : return "link";
        case vid_fields::COUNT  : return "";
    }
    return {};
}

bool FilterModel::compare(const QString &s, const QString &g) const
{
    std::string a = s.toStdString(), b = g.toStdString();
    std::transform(a.begin(),a.end(), a.begin(), tolower);
    std::transform(b.begin(),b.end(), b.begin(), tolower);
    if(a.size() > b.size())
        return false;
    for(unsigned int i = 0; i < a.size();i++) {
        if(a[i] != b[i])
            return false;
    }
    return true;
}

bool FilterModel::filterAcceptsRow (int source_row, const QModelIndex &/*source_parent*/) const
{
    auto search_model = main_window->search_line->text();
    const Vid& video = main_window->vid_model->vid_data[source_row];
    return (!search_model.size() || compare(search_model, QString(video.data[(int)vid_fields::title].toString().toStdString().c_str())));
}

QVariant VidModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        vid_fields field = (vid_fields)section;
        return enum_to_string (field);
    }
    return {};
}
