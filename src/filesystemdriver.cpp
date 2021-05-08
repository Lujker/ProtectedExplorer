#include "filesystemdriver.h"

FileSystemDriver::FileSystemDriver(QObject* parent): m_watcher(nullptr)
{
    Q_UNUSED(parent);
}

void FileSystemDriver::newFile(QString path)
{

}

Driver::Driver(QObject* parent)
{
    Q_UNUSED(parent);
}

void Driver::setPath(std::string &dir_list, std::string& sub_list_dirs)
{
    db::DatabaseQuery::generate_drop_file_table();
    db::DatabaseQuery::generate_drop_file_options_table();
    db::DatabaseQuery::generate_create_files_table();
    db::DatabaseQuery::generate_crete_file_options_table();
    ///Перезагружаем таблицы файлов

    for(auto it : split(dir_list, PATH_SEPARATOR)){

    }
    ///Добавляем пути к доступным деректориям
    for(auto it : split(sub_list_dirs, PATH_SEPARATOR)){

    }
    ///Добавляем пути к расшариным дерикториям
}

bool Driver::addFile(QString &path)
{
    return false;
}

bool Driver::addFolder(QString &path)
{
    return false;
}

bool Driver::deleteFile(QString &path)
{
    return false;
}

bool Driver::deleteFolder(QString &path)
{
    return false;
}

bool Driver::copyFile(QString &path, QString &dist_path)
{
    return false;
}

bool Driver::moveFile(QString &path, QString &dist_path)
{
    return false;
}

QStringList Driver::allPath(const QString &dir_name)
{
    QStringList lst;
    QDir dir(dir_name);
    QFileInfoList info_list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for(const auto& fileInfo : qAsConst(info_list)){
        auto path = fileInfo.absoluteFilePath();
        if(fileInfo.isDir()){
            lst << allPath(path);
        }
        else lst << path;
    }
    return lst;
}
