#include "folderexpl.h"

FolderExpl::FolderExpl(QObject *parent) :
    QObject(parent) , m_sub_model(nullptr), m_dir_model(nullptr)
{}

void FolderExpl::initFromSettings()
{
    clear_members();
    m_dir_model = new DirsModel(SettingsController::get_instanse().parse_dir_list());
    m_sub_model = new DirsModel(SettingsController::get_instanse().parse_sub_list());

    if(m_sub_model!=nullptr && m_dir_model!=nullptr){
        QObject::connect(m_sub_model,SIGNAL(copyFile(QString)),m_dir_model,SLOT(copyFrom(QString)));
        QObject::connect(m_dir_model,SIGNAL(copyFile(QString)),m_sub_model,SLOT(copyFrom(QString)));
    }
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
        QFileInfo info(folder->absolutePath() + QDir::separator() + m_filenames.at(index));
        if(info.isFile()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
            ///open file
            return;
        }
        else if(info.isDir()){
            watcher->removePath(folder->absolutePath());
            folder->cd(m_filenames.at(index));
        }
        else return;
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
        folder->refresh();
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

void DirsModel::deleteFiles(int start, int end)
{
    if(folder!=nullptr){
        for(int it = start; it <= end; ++it){
            QString path = folder->absolutePath() + QDir::separator() + m_filenames.at(it);
            QFileInfo info(path);
            if(folder->exists(path)){
                if(info.isFile())
                    folder->remove(path);
                else if(info.isDir()){
                    QDir subDir(path);
                    if(subDir.exists())
                        subDir.removeRecursively();
                    else folder->rmdir(path);
                }
            }
        }
    }
    else{
        ///ОШИБКА, нельзя удалять стандартные пути
    }
    refreshModel();
}

void DirsModel::addFolder()
{
    folder->mkdir("Новая папка");
}

void DirsModel::deleteFolder(int index)
{
    folder->rmdir(m_filenames.at(index));
}

void DirsModel::copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

void DirsModel::copySelections(int start, int end)
{
    if(folder!=nullptr)
    for(int i = start; i<=end; ++i){
        if(i>0 && i<m_filenames.size())
            copyTo(folder->absoluteFilePath(m_filenames[i]));
    }

}

void DirsModel::copyFrom(QString path)
{
    if(folder!=nullptr){
        if(QFile::exists(path)){
            QFileInfo info(path);
            if(info.isFile()){
                QString copy_file_name = folder->absolutePath()+ QDir::separator() +info.fileName();
                QFile::remove(copy_file_name);
                QFile::copy(path, copy_file_name);
            }
            else if(info.isDir()){
                QString copy_file_name = folder->absolutePath()+ QDir::separator() +info.fileName();
                QDir dir(copy_file_name);
                if(dir.exists()) dir.removeRecursively();
                folder->mkdir(info.fileName());
                this->copyPath(path,copy_file_name);
            }
        }
    }
}

void DirsModel::copyTo(QString path)
{
    emit copyFile(path);
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
        filepath = folder->absolutePath() + QDir::separator() +  m_filenames.at(index.row());
    }
    QFileInfo info(filepath);
    QFileIconProvider provider;
    switch (role) {
    case FilePathRole:
        return info.absoluteFilePath();
        break;
    case NameRole:
        return info.fileName();
        break;
    case DateRole:
        return info.lastModified().toLocalTime().toString();
        break;
    case SuffixRole:
        return info.suffix();
        break;
    case SizeRole:
        if(info.size()==0) return "<Папка>";
        else {
            qint64 nSize = info.size();
            qint64 i = 0;
            for (; nSize > 1023; nSize /= 1024, ++i) { }
            return QString().setNum(nSize) + " " + "BKMGT"[i];
        }
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
    roles.insert(FilePathRole, QByteArray("filepath"));
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(DateRole, QByteArray("date"));
    roles.insert(SizeRole, QByteArray("size"));
    roles.insert(SuffixRole, QByteArray("suffix"));
    roles.insert(isFolderRole, QByteArray("isFolder"));
    return roles;
}

IconProvider :: IconProvider (): QQuickImageProvider (QQuickImageProvider :: Pixmap)
{}

QPixmap IconProvider :: requestPixmap(const QString & id, QSize * size, const QSize & requiredSize)
{
    int width = requiredSize.width ()> 0? requiredSize.width (): 8;
    int height = width;
    QFileInfo info(id);
    if (size) *size = QSize(width, height);
    if (QFileInfo (id) .isDir ()){
//        return QPixmap();
        QIcon icon =  m_provider.icon(info);
        QPixmap pixmap = icon.pixmap(width, height);
        return pixmap;
    }
    else {
        if(id=="") return QPixmap();
        QMimeType mime = m_mimeDB.mimeTypeForFile (id);
        if (QIcon :: hasThemeIcon (mime.iconName ())) return QIcon :: fromTheme (mime.iconName ()). pixmap (width, height);
            return m_provider.icon (info) .pixmap (width, height);
    }
    return QPixmap();
}
