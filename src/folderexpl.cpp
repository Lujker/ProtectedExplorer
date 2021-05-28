#include "folderexpl.h"

FolderExpl::FolderExpl(QObject *parent) :
    QObject(parent) , m_sub_model(nullptr), m_dir_model(nullptr), provider(nullptr)
{}

/*!
 * \brief FolderExpl::initFromSettings
 * Метод для инициализации моделей через файл настрек и установки связей между ними
 */
void FolderExpl::initFromSettings()
{
    clear_members();
    m_dir_model = new DirsModel(SettingsController::get_instanse().parse_dir_list());
    m_sub_model = new DirsModel(SettingsController::get_instanse().parse_sub_list());
    provider = new IconProvider;

    if(m_sub_model!=nullptr && m_dir_model!=nullptr){
        QObject::connect(m_sub_model,SIGNAL(copyFile(QString)),m_dir_model,SLOT(copyFrom(QString)));
        QObject::connect(m_dir_model,SIGNAL(copyFile(QString)),m_sub_model,SLOT(copyFrom(QString)));
    }
}

/*!
 * \brief FolderExpl::init инициализация по готовым обьектам
 * \param subs - метод инициализации модели сетевых папок
 * \param dirs - метод иницилизации модели доступных папок
 */
void FolderExpl::init(DirsModel *subs, DirsModel *dirs)
{
    clear_members();
    m_sub_model = subs;
    m_dir_model = dirs;
}

/*!
 * \brief FolderExpl::clear_members
 * Очистка всех обьектов моделей и иконок
 */
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
    if(provider!=nullptr){
        delete  provider;
        provider = nullptr;
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

IconProvider *FolderExpl::getProvider() const
{
    return provider;
}

void FolderExpl::setProvider(IconProvider *value)
{
    provider = value;
}

/*!
 * \brief DirsModel::DirsModel инициализация модели
 * \param dirs списко доступных папок
 */
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

/*!
 * \brief DirsModel::openFolder метод открытия файла под индексом
 * \param index переход или открытие файла под индексом
 */
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

/*!
 * \brief DirsModel::comeBack переход к верхней папке
 */
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

/*!
 * \brief DirsModel::comeToBeginning переход к вернхним папкам
 */
void DirsModel::comeToBeginning()
{
    watcher->removePath(folder->absolutePath());
    delete folder;
    folder = nullptr;
    m_level_count = 0;
    refreshModel();
}

/*!
 * \brief DirsModel::derictoryChange
 * слот принимащий сигнал при изменении в текущей деректории
 * \param path путь дериктории
 */
void DirsModel::derictoryChange(const QString& path)
{
    refreshModel();
}

/*!
 * \brief DirsModel::refreshModel обновление списка файлов и модели
 */
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

/*!
 * \brief DirsModel::setAsSubModel установка модели по списку сетевых папок
 */
void DirsModel::setAsSubModel()
{
    beginResetModel();
    m_filenames.clear();


    m_level_count = 0;
    if(folder!=nullptr){
        watcher->removePath(folder->absolutePath());
        delete folder;
        folder = nullptr;
    }

    m_dirs.clear();
    for(const auto& it : SettingsController::get_instanse().parse_sub_list()){
        m_dirs.push_back(QString::fromStdString(it));
    }
    for(const auto &it : qAsConst(m_dirs)){
        ///заполнение списка именами папок
        QDir dir(it);
        m_filenames.push_back(dir.dirName());
    }
    endResetModel();
}

/*!
 * \brief DirsModel::setAsDirModel установка модели по списку доступным папок
 */
void DirsModel::setAsDirModel()
{
    beginResetModel();
    m_filenames.clear();
    if(folder!=nullptr){
        watcher->removePath(folder->absolutePath());
        delete folder;
        folder = nullptr;
    }
    m_dirs.clear();
    for(const auto& it : SettingsController::get_instanse().parse_dir_list()){
        m_dirs.push_back(QString::fromStdString(it));
    }
    for(const auto &it : qAsConst(m_dirs)){
        ///заполнение списка именами папок
        QDir dir(it);
        m_filenames.push_back(dir.dirName());
    }
    endResetModel();
}

/*!
 * \brief DirsModel::addFile создание нового текстового файла в данной дериктории
 */
void DirsModel::addFile()
{
    if(folder==nullptr) return;
    QString filename = QString("Новый файл.txt");
    int count = 1;
    while(folder->exists(folder->filePath(folder->absolutePath() + QDir::separator() + filename))){
        filename = QString("Новый файл(") + QString::number(count) + QString(").txt");
        ++count;
    }
    QFile _newFile(folder->absolutePath() + QDir::separator() + filename);
    if(_newFile.open(QFile::WriteOnly)){

    }
    else{

    }
}

/*!
 * \brief DirsModel::deleteFile удаление файла в модели под индексом
 * \param index индекс в списке файлов
 */
void DirsModel::deleteFile(int index)
{
    if(index<=0 || index>=m_filenames.size()) return;
    if(folder==nullptr) return;
    QString path = folder->absolutePath() + QDir::separator() + m_filenames.at(index);
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

/*!
 * \brief DirsModel::deleteFiles удаление списка файлов
 * \param start индекс начала списка файлов
 * \param end индекс конца списка
 */
void DirsModel::deleteFiles(int start, int end)
{
    if(folder!=nullptr){
        if(start!=end)
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
        else{
            deleteFile(start);
        }
    }
    else{
        ///ОШИБКА, нельзя удалять стандартные пути
    }
}

/*!
 * \brief DirsModel::addFolder создание новой папки в дериктории
 */
void DirsModel::addFolder()
{
    if(folder==nullptr) return;
    QString filename = QString("Новая папка");
    int count = 1;
    ///если у нас уже есть такой файл или папка то добавляем к ней цифру под счетчиком
    while(folder->exists(folder->filePath(folder->absolutePath() + QDir::separator() + filename))){
        filename = QString("Новый папка(") + QString::number(count) + QString(")");
        ++count;
    }
    QFile _newFile(folder->absolutePath() + QDir::separator() + filename);
    folder->mkdir(filename);
}

/*!
 * \brief DirsModel::deleteFolder удаление папки под индексом в списке файлов
 * \param index индекс папки в списке
 */
void DirsModel::deleteFolder(int index)
{
    folder->rmdir(m_filenames.at(index));
}

/*!
 * \brief DirsModel::renameFile переименование файла
 * \param index индекс файла в списке файлов
 * \param name новое имя
 */
void DirsModel::renameFile(int index, QString name)
{
    qDebug()<<index << name;
    if(folder==nullptr) return;
    QFile file(folder->absolutePath() + QDir::separator() +  m_filenames.at(index));
    if(file.exists()){
        if(file.rename(folder->absolutePath() + QDir::separator() + name)){
            qDebug()<<"good rename";
        }
        else {
            qDebug()<< "bad rename";
        }
    }
}

/*!
 * \brief DirsModel::setSorting устновка сортировки по индексу и правилу
 * \param column индекс колонки
 * \param order правило сортировки
 */
void DirsModel::setSorting(int column, int order)
{
    beginResetModel();
    if(folder==nullptr) return;
    switch (column) {
    case 0:
        sortBySuffix(order);
        break;
    case 1:
        sortByName(order);
        break;
    case 2:
        sortBySuffix(order);
        break;
    case 3:
        sortBySize(order);
        break;
    case 4:
        sortByDate(order);
        break;
    }
    endResetModel();
}

void DirsModel::sortByName(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(), std::greater<QString>());
    else std::sort(++m_filenames.begin(), m_filenames.end());
}

void DirsModel::sortByDate(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(), [&](QString& first, QString& second){
            QFileInfo f_info(folder->absolutePath() + QDir::separator() +  first);
            QFileInfo s_info(folder->absolutePath() + QDir::separator() +  second);
            return f_info.lastModified().toLocalTime() < s_info.lastModified().toLocalTime();
        });
    else std::sort(++m_filenames.begin(), m_filenames.end(), [&](QString& first, QString& second){
        QFileInfo f_info(folder->absolutePath() + QDir::separator() +  first);
        QFileInfo s_info(folder->absolutePath() + QDir::separator() +  second);
        return f_info.lastModified().toLocalTime() > s_info.lastModified().toLocalTime();
    });
}

void DirsModel::sortBySize(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(), [&](QString& first, QString& second){
            QFileInfo f_info(folder->absolutePath() + QDir::separator() +  first);
            QFileInfo s_info(folder->absolutePath() + QDir::separator() +  second);
            return f_info.size() < s_info.size();
        });
    else std::sort(++m_filenames.begin(), m_filenames.end(), [&](QString& first, QString& second){
        QFileInfo f_info(folder->absolutePath() + QDir::separator() +  first);
        QFileInfo s_info(folder->absolutePath() + QDir::separator() +  second);
        return f_info.size() > s_info.size();
    });
}

void DirsModel::sortBySuffix(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(), [&](QString& first, QString& second){
            QFileInfo f_info(folder->absolutePath() + QDir::separator() +  first);
            QFileInfo s_info(folder->absolutePath() + QDir::separator() +  second);
            return f_info.suffix() < s_info.suffix();
        });
    else std::sort(++m_filenames.begin(), m_filenames.end(), [&](QString& first, QString& second){
        QFileInfo f_info(folder->absolutePath() + QDir::separator() +  first);
        QFileInfo s_info(folder->absolutePath() + QDir::separator() +  second);
        return f_info.suffix() > s_info.suffix();
    });
}

/*!
 * \brief DirsModel::copyPath копирование путей
 * \param src исходный
 * \param dst указанный
 */
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

/*!
 * \brief DirsModel::copySelections копирование выбранных файлов
 * \param start индекс перовго файла
 * \param end индекс последнего файла
 */
void DirsModel::copySelections(int start, int end)
{
    if(folder!=nullptr)
    for(int i = start; i<=end; ++i){
        if(i>0 && i<m_filenames.size())
            ///сигнал для второй таблицы
            copyTo(folder->absoluteFilePath(m_filenames[i]));
    }

}

/*!
 * \brief DirsModel::copyFile копирование файла под индексом
 * \param index индекс файла в списке файлов
 */
void DirsModel::copyFile(int index)
{
    if(index>0 && index<m_filenames.size())
        ///сигнал отправляемый второй таблице
        copyTo(folder->absoluteFilePath(m_filenames[index]));
}

/*!
 * \brief DirsModel::copyFrom слот для копирования переданного файла
 * \param path путь для копирования
 */
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

/*!
 * \brief DirsModel::copyTo метод для отправки сигнала из qml
 * \param path путь к файлу
 */
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

/*!
 * \brief DirsModel::data метод который передает в отображение инормацию под конкретным индексом и ролью
 * \param index индекс
 * \param role роль
 * \return информация под конкретной ролью файла
 */
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
        if(info.isDir()) return QString::fromStdString("<Папка>");
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

/*!
 * \brief DirsModel::roleNames формирование списка ролей для qml
 * \return списко ролей и их имен
 */
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

/*!
 * \brief IconProvider::requestPixmap получение иконки для файла из qml
 * \param id индекс элемента
 * \param size размер иконки
 * \param requiredSize предпочтительный размер
 * \return инока из файловой системы для конкретного пути
 */
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
