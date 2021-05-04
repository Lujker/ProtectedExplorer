#include "folderexpl.h"

FolderExpl::FolderExpl(QObject *parent) :
    QObject(parent) , m_sub_model(nullptr), m_dir_model(nullptr)
{}

void FolderExpl::initFromSettings()
{
    clear_members();
    m_dir_model = new DirsModel(SettingsController::get_instanse().parse_dir_list());
    m_sub_model = new SubsModel(SettingsController::get_instanse().parse_sub_list());
}

void FolderExpl::init(SubsModel *subs, DirsModel *dirs)
{
    clear_members();
    m_sub_model = subs;
    m_dir_model = dirs;
}

void FolderExpl::clear_members()
{
    if(m_sub_model!=nullptr){
        delete m_sub_model;
        m_dir_model = nullptr;
    }
    if(m_dir_model!=nullptr){
        delete m_dir_model;
        m_dir_model = nullptr;
    }
}

SubsModel::SubsModel(std::list<std::string> subs, QObject *parent)
{

}

QModelIndex SubsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column)
    if (!parent.isValid())
                return createIndex(row, 0, static_cast<quintptr>(0));

    return createIndex(row, 0, parent.row() + 1);
}

QModelIndex SubsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalId() == 0)
                   return QModelIndex();

       return createIndex(child.internalId() - 1, 0, static_cast<quintptr>(0));
}

int SubsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
        return m_subs.size();
}

int SubsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant SubsModel::data(const QModelIndex &index, int role) const
{
//    if (!index.isValid() || index.row() > (item_list.size()-1) || !this->hasIndex(index.row(),0)) {
//            return QVariant();
//        }

//        TreeModelItem *item = nullptr;
//        try{
//        item = item_list.at(index.row());
//        }  catch (std::exception& exep) {
//            qDebug()<<exep.what();
//            return QVariant();
//        }
//        switch (role) {
//        case Qt::DisplayRole:
//            return QString::fromStdString(item->get_attr_name());
//        case NameRole:
//            return QString::fromStdString(item->get_attr_name());
//        case SizeTreeRole:
//            return QString::number(DataKeeperTree::count_tree(item));
//        default:
            return QVariant();
//        }
}

QHash<int, QByteArray> SubsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(DateRole, QByteArray("date"));
    return roles;
}

DirsModel::DirsModel(std::list<std::string> dirs, QObject *parent)
{

}

void DirsModel::openFolder(int index)
{

}

void DirsModel::comeBack()
{

}

void DirsModel::comeToBeginning()
{

}

QModelIndex DirsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column)
    if (!parent.isValid())
                return createIndex(row, 0, static_cast<quintptr>(0));

    return createIndex(row, 0, parent.row() + 1);
}

QModelIndex DirsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalId() == 0)
                   return QModelIndex();

       return createIndex(child.internalId() - 1, 0, static_cast<quintptr>(0));
}

int DirsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
        return m_dirs.size();
}

int DirsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant DirsModel::data(const QModelIndex &index, int role) const
{
     return QVariant();
}

QHash<int, QByteArray> DirsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(DateRole, QByteArray("date"));
    roles.insert(SizeRole, QByteArray("size"));
    roles.insert(isFolderRole, QByteArray("isFolder"));
    return roles;
}
