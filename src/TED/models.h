#ifndef MODELS_H
#define MODELS_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <unordered_set>

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

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterModel (QObject *parent);
    bool compare(const QString &s, const QString &g) const;
    const VidModel *vid = nullptr;
protected:
    virtual bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const override;
private:
    QString sear;
public slots:
    void receive(const QString &str);
};

#endif // MODELS_H
