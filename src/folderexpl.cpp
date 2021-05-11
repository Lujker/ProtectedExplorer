#include "folderexpl.h"

FolderExpl::FolderExpl(QObject *parent) :
    QObject(parent) , m_sub_model(nullptr), m_dir_model(nullptr)
{}

void FolderExpl::initFromSettings()
{
    clear_members();
    m_dir_model = new DirsModel(SettingsController::get_instanse().parse_dir_list());
    m_sub_model = new DirsModel(SettingsController::get_instanse().parse_sub_list());
}

void FolderExpl::init(DirsModel *subs, DirsModel *dirs)
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

DirsModel::DirsModel(std::list<std::string> dirs, QObject *parent):
    m_dirs(dirs), folder(nullptr),  m_level_count(0),  watcher(new QFileSystemWatcher)
{
    Q_UNUSED(parent);
    refreshModel();
}

void DirsModel::openFolder(int index)
{
    if(folder == nullptr){

    }
    watcher->removePath(folder->absolutePath());
    folder->cd(m_filenames.at(index));
    ++m_level_count;
    watcher->addPath(folder->absolutePath());
    refreshModel();
}

void DirsModel::comeBack()
{
    if(m_level_count!=1){
        watcher->removePath(folder->absolutePath());
        folder->cdUp();
        --m_level_count;
        watcher->addPath(folder->absolutePath());
    }
    else{
        comeToBeginning();
        return;
    }
    refreshModel();
}

void DirsModel::comeToBeginning()
{
    watcher->removePath(folder->absolutePath());
    delete folder;
    folder = nullptr;
    m_level_count = 0;
    refreshModel();
}

void DirsModel::refreshModel()
{
    beginResetModel();
    if(folder==nullptr){
        m_filenames.clear();
        for(const auto &it : m_dirs){
            ///заполнение списка именами папок
            QDir dir(QString::fromStdString(it));
            m_filenames.push_back(dir.dirName());
        }
    }
    else{
        auto lst = folder->entryList();
        m_filenames.clear();
        for(const auto &it : qAsConst(lst)){
            m_filenames.push_back(it);
        }
    }
    endResetModel();
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
    QString filepath = folder->path() + m_filenames.at(index.row());
    QFileInfo info(filepath);
    switch (role) {
    case NameRole:
        return info.fileName();
        break;
    case DateRole:
        return info.lastModified().toString();
        break;
    case SizeRole:
        return info.size();
        break;
    case isFolderRole:
        return info.isDir();
        break;
    }

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
