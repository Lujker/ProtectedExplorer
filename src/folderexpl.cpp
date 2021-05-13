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

DirsModel *FolderExpl::sub_model() const
{
    return m_sub_model;
}

DirsModel *FolderExpl::dir_model() const
{
    return m_dir_model;
}

DirsModel::DirsModel(std::list<std::string> dirs, QObject *parent):
    folder(nullptr),  m_level_count(0),  watcher(new QFileSystemWatcher)
{
    Q_UNUSED(parent);
    for(const auto& it : dirs){
        m_dirs.push_back(QString::fromStdString(it));
    }
    connect(watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(derictoryChange(const QString &)));
    refreshModel();
}

void DirsModel::openFolder(int index)
{
//    qDebug()<<index;
    if(index<0){
        return;
    }    

    if(folder != nullptr){
        if(m_filenames.at(index)==".."){
            comeBack();
            return;
        }
        watcher->removePath(folder->absolutePath());
        folder->cd(m_filenames.at(index));
    }
    else{
        folder = new QDir(m_dirs.at(index));
    }
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

void DirsModel::derictoryChange(const QString& path)
{
    refreshModel();
}

void DirsModel::refreshModel()
{
    beginResetModel();
    if(folder==nullptr){
        m_filenames.clear();
        for(const auto &it : qAsConst(m_dirs)){
            ///заполнение списка именами папок
            QDir dir(it);
            m_filenames.push_back(dir.dirName());
        }
    }
    else{
        auto lst = folder->entryList();
        m_filenames.clear();
        for(const auto &it : qAsConst(lst)){
            if(it!=".")
            m_filenames.push_back(it);
        }
    }
    endResetModel();
}

void DirsModel::addFile()
{
    //    folde->
}

void DirsModel::deleteFile(int index)
{
    folder->remove(m_filenames.at(index));
}

void DirsModel::addFolder()
{
    folder->mkdir("Новая папка");
}

void DirsModel::deleteFolder(int index)
{
    folder->rmdir(m_filenames.at(index));
}

void DirsModel::copySelections(int start, int end)
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
    if(folder==nullptr)
        return m_dirs.size();
    else return m_filenames.size();
}

int DirsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant DirsModel::data(const QModelIndex &index, int role) const
{
    QString filepath;
    if(folder==nullptr) {
        filepath = m_dirs.at(index.row());
    }
    else{
        filepath = folder->absolutePath() + "/" +  m_filenames.at(index.row());
    }
    QFileInfo info(filepath);
    QFileIconProvider provider;
    switch (role) {
    case IconRole:
        return provider.icon(info).pixmap(34,34).toImage();
        break;
    case NameRole:
        return info.fileName();
        break;
    case DateRole:
        return info.lastModified().toLocalTime().toString();
        break;
    case SizeRole:
        if(info.size()==0) return "<Папка>";
        else    return info.size();
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
    roles.insert(IconRole, QByteArray("icon"));
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(DateRole, QByteArray("date"));
    roles.insert(SizeRole, QByteArray("size"));
    roles.insert(isFolderRole, QByteArray("isFolder"));
    return roles;
}
