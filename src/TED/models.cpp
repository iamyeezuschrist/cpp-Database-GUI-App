#include "models.h"

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

QVariant VidModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        vid_fields field = (vid_fields)section;
        return enum_to_string (field);
    }
    return {};
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

FilterModel::FilterModel (QObject *parent) : QSortFilterProxyModel (parent)
{}

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
    auto search_model = sear;
    const Vid& video = vid->vid_data[source_row];
    return (!search_model.size() || compare(search_model, QString(video.data[(int)vid_fields::title].toString().toStdString().c_str())));
}

void FilterModel::receive(const QString &str)
{
    sear = str;
}
