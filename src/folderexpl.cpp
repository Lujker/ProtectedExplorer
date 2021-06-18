#include "folderexpl.h"

FolderExpl::FolderExpl(QObject *parent) :
    QObject(parent), provider(nullptr)
{}

FolderExpl::~FolderExpl()
{
    try{
        clear_members();
    }
    catch(std::exception& exep){
        qDebug()<<exep.what();
    }
    catch (...) {

    }
}

/*!
 * \brief FolderExpl::initFromSettings
 * Метод для инициализации моделей через файл настрек и установки связей между ними
 */
void FolderExpl::initFromSettings()
{
    clear_members();
    m_dir_models.push_back(new DirsModel(SettingsController::get_instanse().dir_list()));
    m_dir_models.push_back(new DirsModel(SettingsController::get_instanse().sub_list_dirs()));
    m_email_models.push_back(new EmailModel(SettingsController::get_instanse().abonents()));
    m_email_models.push_back(new EmailModel(SettingsController::get_instanse().abonents()));
    provider = new IconProvider;

    if(m_dir_models.size()==2){
        QObject::connect(m_dir_models.at(1),SIGNAL(copyFile(QString)),m_dir_models.at(0),SLOT(copyFrom(QString)));
        QObject::connect(m_dir_models.at(0),SIGNAL(copyFile(QString)),m_dir_models.at(1),SLOT(copyFrom(QString)));
    }
    for(const auto& it : m_email_models){
        it->initAddressBook();    /// синхронизация адресной книги из настроек и БД
        it->initModelData();      /// синхронизация сообщений из БД и моделью
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
    m_dir_models.push_back(subs);
    m_dir_models.push_back(dirs);
}

/*!
 * \brief FolderExpl::clear_members
 * Очистка всех обьектов моделей и иконок
 */
void FolderExpl::clear_members()
{

    for(unsigned long i = 0 ; i < m_dir_models.size();++i){
        delete m_dir_models[i];
    }
    m_dir_models.clear();

    if(provider!=nullptr){
        delete  provider;
        provider = nullptr;
    }
    for(unsigned long i = 0 ; i < m_email_models.size();++i){
        delete m_email_models[i];
    }
    m_email_models.clear();
}


IconProvider *FolderExpl::getProvider() const
{
    return provider;
}

void FolderExpl::setProvider(IconProvider *value)
{
    provider = value;
}

std::vector<DirsModel *> FolderExpl::getDir_models() const
{
    return m_dir_models;
}

void FolderExpl::setDir_models(const std::vector<DirsModel *> &dir_models)
{
    m_dir_models = dir_models;
}

std::vector<EmailModel *> FolderExpl::getEmail_models() const
{
    return m_email_models;
}

void FolderExpl::setEmail_models(const std::vector<EmailModel *> &email_models)
{
    m_email_models = email_models;
}

/*!
 * \brief DirsModel::DirsModel инициализация модели
 * \param dirs списко доступных папок
 */
DirsModel::DirsModel(std::vector<std::pair<std::string, std::string> > dirs, QObject *parent): QAbstractTableModel(parent),
    m_dirs(dirs), m_folder(nullptr),  m_level_count(0),  m_watcher(new QFileSystemWatcher)
{
    connect(m_watcher, SIGNAL(directoryChanged(const QString &)),
            this, SLOT(derictoryChange(const QString &)));
    refreshModel();
}

DirsModel::~DirsModel()
{
    try {
        if(m_folder!=nullptr){
            delete m_folder;
            m_folder = nullptr;
        }
        if(m_watcher!=nullptr){
            delete m_watcher;
            m_watcher = nullptr;
        }
    }
    catch(std::exception& exep){
        qDebug()<<exep.what();
    }
    catch (...) {

    }
}

QString DirsModel::current_dir()
{
    if(m_folder!=nullptr){
        return QString::fromStdString(m_current_dir);
    }
    else return QString();
}

/*!
 * \brief DirsModel::openFolder метод открытия файла под индексом
 * \param index переход или открытие файла под индексом
 */
void DirsModel::openFolder(int index)
{
//    qDebug()<<index;
    if(index<0 || index >= m_filenames.size()){
        return;
    }

    if(m_folder != nullptr){
        if(m_filenames.at(index)==".."){
            comeBack();
            return;
        }
        QFileInfo info(m_folder->absolutePath() + QDir::separator() + m_filenames.at(index));
        if(info.isFile()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
            ///open file
            return;
        }
        else if(info.isDir()){
            m_watcher->removePath(m_folder->absolutePath());
            m_folder->cd(m_filenames.at(index));
            m_current_dir += "/" + (m_folder->dirName().toStdString());
            emit current_dir_change(QString::fromStdString(m_current_dir));
        }
        else return;
    }
    else{
        m_folder = new QDir(QString::fromStdString(m_dirs.at(index).second));
        if(!m_dirs.at(index).first.empty()){
            m_current_dir = m_dirs.at(index).first;
        }
        else {
            m_current_dir = m_folder->dirName().toStdString();
        }
        emit current_dir_change(QString::fromStdString(m_dirs.at(index).first));
    }
    ++m_level_count;
    m_watcher->addPath(m_folder->absolutePath());
    refreshModel();
}

/*!
 * \brief DirsModel::comeBack переход к верхней папке
 */
void DirsModel::comeBack()
{
    if(m_level_count!=1){
        m_watcher->removePath(m_folder->absolutePath());
        m_folder->cdUp();
        auto pos = m_current_dir.find_last_of('/');
            if (pos != std::string::npos)
                m_current_dir.erase(pos, std::numeric_limits<std::string::size_type>::max());
        emit current_dir_change(QString::fromStdString(m_current_dir));
        --m_level_count;
        m_watcher->addPath(m_folder->absolutePath());
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
    m_watcher->removePath(m_folder->absolutePath());
    delete m_folder;
    m_folder = nullptr;
    m_current_dir.clear();
    emit current_dir_change(QString::fromStdString(m_current_dir));
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
    if(m_folder==nullptr){
        m_filenames.clear();
        for(const auto &it : qAsConst(m_dirs)){
            ///заполнение списка именами папок
            if(!it.first.empty())
                m_filenames.push_back(QString::fromStdString(it.first));
            else {
                m_filenames.push_back(QDir(QString::fromStdString(it.second)).dirName());
            }
        }
    }
    else{
        m_folder->refresh();  ///чтение данныз из файловой системы
        auto lst = m_folder->entryList();
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
    if(m_folder!=nullptr){
        m_watcher->removePath(m_folder->absolutePath());
        delete m_folder;
        m_folder = nullptr;
    }

    m_dirs = SettingsController::get_instanse().sub_list_dirs();
    for(const auto &it : qAsConst(m_dirs)){
        ///заполнение списка именами папок
        m_filenames.push_back(QString::fromStdString(it.first));
    }
    emit current_dir_change(QString());
    endResetModel();
}

/*!
 * \brief DirsModel::setAsDirModel установка модели по списку доступным папок
 */
void DirsModel::setAsDirModel()
{
    beginResetModel();
    m_filenames.clear();
    m_level_count = 0;
    if(m_folder!=nullptr){
        m_watcher->removePath(m_folder->absolutePath());
        delete m_folder;
        m_folder = nullptr;
    }
    m_dirs = SettingsController::get_instanse().dir_list();
    for(const auto &it : qAsConst(m_dirs)){
        ///заполнение списка именами папок
        m_filenames.push_back(QString::fromStdString(it.first));
    }
    emit current_dir_change(QString());
    endResetModel();
}

/*!
 * \brief DirsModel::addFile создание нового текстового файла в данной дериктории
 */
void DirsModel::addFile(QString name)
{
    if(m_folder==nullptr) return;
    QString filename;
    if(name.isEmpty())
        filename = QString("Новый файл.txt");
    else filename = name;
    int count = 1;
    while(m_folder->exists(m_folder->filePath(m_folder->absolutePath() + QDir::separator() + filename))){
        filename = QString("Новый файл(") + QString::number(count) + QString(").txt");
        ++count;
    }
    QFile _newFile(m_folder->absolutePath() + QDir::separator() + filename);
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
    if(m_folder==nullptr) return;
    QString path = m_folder->absolutePath() + QDir::separator() + m_filenames.at(index);
    QFileInfo info(path);
    if(m_folder->exists(path)){
        if(info.isFile())
            m_folder->remove(path);
        else if(info.isDir()){
            QDir subDir(path);
            if(subDir.exists())
                subDir.removeRecursively();
            else m_folder->rmdir(path);
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
    if(m_folder!=nullptr){
        if(start!=end)
        for(int it = start; it <= end; ++it){
            QString path = m_folder->absolutePath() + QDir::separator() + m_filenames.at(it);
            QFileInfo info(path);
            if(m_folder->exists(path)){
                if(info.isFile())
                    m_folder->remove(path);
                else if(info.isDir()){
                    QDir subDir(path);
                    if(subDir.exists())
                        subDir.removeRecursively();
                    else m_folder->rmdir(path);
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
void DirsModel::addFolder(QString name)
{
    if(m_folder==nullptr) return;
    QString filename;
    if(name.isEmpty())
        filename = QString("Новая папка");
    else filename = name;
    int count = 1;
    ///если у нас уже есть такой файл или папка то добавляем к ней цифру под счетчиком
    while(m_folder->exists(m_folder->filePath(m_folder->absolutePath() + QDir::separator() + filename))){
        filename = QString("Новый папка(") + QString::number(count) + QString(")");
        ++count;
    }
    QFile _newFile(m_folder->absolutePath() + QDir::separator() + filename);
    m_folder->mkdir(filename);
}

/*!
 * \brief DirsModel::deleteFolder удаление папки под индексом в списке файлов
 * \param index индекс папки в списке
 */
void DirsModel::deleteFolder(int index)
{
    if(m_folder!=nullptr)
        m_folder->rmdir(m_filenames.at(index));
}

/*!
 * \brief DirsModel::renameFile переименование файла
 * \param index индекс файла в списке файлов
 * \param name новое имя
 */
void DirsModel::renameFile(int index, QString name)
{
    if(m_folder==nullptr) return;
    QFile file(m_folder->absolutePath() + QDir::separator() +  m_filenames.at(index));
    if(file.exists()){
        if(file.rename(m_folder->absolutePath() + QDir::separator() + name)){
        }
        else {
            qDebug()<< "rename error";
        }
    }
}

/*!
 * \brief DirsModel::setSorting устновка сортировки по индексу и правилу
 * \details Сортировка проиходит путем выполнения алгоритма std::sort библиоткеи <algorithm> STL
 * с использованием специальных либо определенных в STL предикатов. Специальные предикаты
 * написаны согласно синтаксису лямбда-функций (при этом принимают контекст и значения по ссылке)
 * \param column индекс колонки
 * \param order правило сортировки
 */
void DirsModel::setSorting(int column, int order)
{
    beginResetModel();
    if(m_folder==nullptr) return;
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

/*!
 * \brief DirsModel::sortByName сортировка по имени файла
 * \param lower параметр который устанавливает порядок соритровки
 */
void DirsModel::sortByName(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(), std::greater<QString>());
    else std::sort(++m_filenames.begin(), m_filenames.end());
}

void DirsModel::sortByDate(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(),
                  [&](const QString& first, const QString& second){
            QFileInfo f_info(m_folder->absolutePath() + QDir::separator() +  first);
            QFileInfo s_info(m_folder->absolutePath() + QDir::separator() +  second);
            return f_info.lastModified().toLocalTime() < s_info.lastModified().toLocalTime();
        });
    else std::sort(++m_filenames.begin(), m_filenames.end(),
                   [&](const QString& first, const QString& second){
        QFileInfo f_info(m_folder->absolutePath() + QDir::separator() +  first);
        QFileInfo s_info(m_folder->absolutePath() + QDir::separator() +  second);
        return f_info.lastModified().toLocalTime() > s_info.lastModified().toLocalTime();
    });
}

void DirsModel::sortBySize(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(),
                  [&](const QString& first, const QString& second){
            QFileInfo f_info(m_folder->absolutePath() + QDir::separator() +  first);
            QFileInfo s_info(m_folder->absolutePath() + QDir::separator() +  second);
            return f_info.size() < s_info.size();
        });
    else std::sort(++m_filenames.begin(), m_filenames.end(),
                   [&](const QString& first, const QString& second){
        QFileInfo f_info(m_folder->absolutePath() + QDir::separator() +  first);
        QFileInfo s_info(m_folder->absolutePath() + QDir::separator() +  second);
        return f_info.size() > s_info.size();
    });
}

void DirsModel::sortBySuffix(bool lower)
{
    if(lower)
        std::sort(++m_filenames.begin(), m_filenames.end(),
                  [&](const QString& first, const QString& second){
            QFileInfo f_info(m_folder->absolutePath() + QDir::separator() +  first);
            QFileInfo s_info(m_folder->absolutePath() + QDir::separator() +  second);
            return f_info.suffix() < s_info.suffix();
        });
    else std::sort(++m_filenames.begin(), m_filenames.end(),
                   [&](const QString& first, const QString& second){
        QFileInfo f_info(m_folder->absolutePath() + QDir::separator() +  first);
        QFileInfo s_info(m_folder->absolutePath() + QDir::separator() +  second);
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
    if(m_folder!=nullptr)
    for(int i = start; i<=end; ++i){
        if(i>0 && i<m_filenames.size())
            ///сигнал для второй таблицы
            copyTo(m_folder->absoluteFilePath(m_filenames[i]));
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
        copyTo(m_folder->absoluteFilePath(m_filenames[index]));
}

void DirsModel::moveFile(int index)
{
    if(m_folder!=nullptr)
        if(index>0 && index<m_filenames.size()){
            copyTo(m_folder->absoluteFilePath(m_filenames[index]));
            deleteFile(index);
        }
}

void DirsModel::signedFile(int index)
{
    Q_UNUSED(index)
}

/*!
 * \brief DirsModel::copyFrom слот для копирования переданного файла
 * \param path путь для копирования
 */
void DirsModel::copyFrom(QString path)
{
    if(m_folder!=nullptr){
        if(QFile::exists(path)){
            QFileInfo info(path);
            if(info.isFile()){
                QString copy_file_name = m_folder->absolutePath()+ QDir::separator() +info.fileName();
                QFile::remove(copy_file_name);
                QFile::copy(path, copy_file_name);
            }
            else if(info.isDir()){
                QString copy_file_name = m_folder->absolutePath()+ QDir::separator() +info.fileName();
                QDir dir(copy_file_name);
                if(dir.exists()) dir.removeRecursively();
                m_folder->mkdir(info.fileName());
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
    if(m_folder==nullptr)
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
    if(m_folder==nullptr) {
        filepath = QString::fromStdString(m_dirs.at(index.row()).second);
    }
    else{
        filepath = m_folder->absolutePath() + QDir::separator() +  m_filenames.at(index.row());
    }
    QFileInfo info(filepath);
    QFileIconProvider provider;
    switch (role) {
    case FilePathRole:
        return info.absoluteFilePath();
        break;
    case NameRole:
        if(m_folder==nullptr){
            if(m_dirs.at(index.row()).first.empty()){
                return QDir(QString::fromStdString(m_dirs.at(index.row()).second)).dirName();
            }
            else    return QString::fromStdString(m_dirs.at(index.row()).first);
        }
        else return info.fileName();
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
    if (info.isDir()){
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
