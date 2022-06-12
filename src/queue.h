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

class QueModel;
class QueItem;

class queue : public QWidget
{
    Q_OBJECT
public:
    QTableView *que_view = nullptr;
    QueModel *que_model = nullptr;
    QLabel *total;

public:
    explicit queue(QWidget *parent = nullptr);
    ~queue();

private slots:
    void info(const QModelIndex & ind);

signals:
    void send_data(QueItem &res);

};

enum class que_fields
{
    title,
    views,
    likes,
    COUNT
};

const char *enum_to_string_cart (que_fields field);

class QueItem
{
public:
    std::array<QVariant, (static_cast<int> (que_fields::COUNT))> data;
    friend bool operator==(const QueItem &a, const QueItem &b) {
        return(a.data[0] == b.data[0] && a.data[1] == b.data[1]);
    }
};

class QueModel : public QAbstractTableModel
{
Q_OBJECT

public:
    std::vector<QueItem> que_data;

int rowCount(const QModelIndex &/*parent*/) const override
{
    return que_data.size ();
}

int columnCount(const QModelIndex &/*parent*/) const override
{
    return static_cast<int> (que_fields::COUNT);
}

void add_data(const QueItem &data)
{
    beginResetModel ();
    que_data.push_back(data);
    endResetModel ();
    emit layoutChanged ();
}

void delete_rows (const std::unordered_set<size_t> &selected_rows)
{

    beginResetModel();
    size_t j = 0;
    for(size_t i = 0; i < que_data.size(); ++i)
        if(selected_rows.find(i) == selected_rows.end()) // not found
            que_data[j++] = que_data[i];
    que_data.resize(j);
    endResetModel();
    emit layoutChanged();
}


QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};
#endif // QUEUE_H
