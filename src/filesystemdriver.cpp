#include "filesystemdriver.h"

FileSystemDriver::FileSystemDriver(QObject* parent): m_watcher(new QFileSystemWatcher)
{
    Q_UNUSED(parent);
}

FileSystemDriver::~FileSystemDriver()
{
    db::DatabaseQuery::generate_drop_file_table();
    db::DatabaseQuery::generate_drop_file_options_table();
    delete m_watcher;
}

void FileSystemDriver::newFile(QString path)
{

}

void FileSystemDriver::createDataBase()
{
    setPath(SettingsController::get_instanse().dir_list().toStdString(),
            SettingsController::get_instanse().sub_list_dirs().toStdString());
}

void FileSystemDriver::updateDataBase()
{

}

Driver::Driver(QObject* parent)
{
    Q_UNUSED(parent);
}

Driver::~Driver()
{
}

void FileSystemDriver::setPath(const std::string &dir_list, const std::string& sub_list_dirs)
{
    db::DatabaseQuery::generate_drop_file_table();
    db::DatabaseQuery::generate_drop_file_options_table();
    db::DatabaseQuery::generate_create_files_table();
    db::DatabaseQuery::generate_crete_file_options_table();
    ///Перезагружаем таблицы файлов

    for(auto const &it : split(dir_list, PATH_SEPARATOR)){
        auto lst = allPath(QString::fromStdString(it));
        for(const auto &i : qAsConst(lst)){
            QFileInfo info(i);
            if(info.isDir()){
                QDir dir(i);
                if(dir.exists()){
                    db::DatabaseQuery::generate_add_dir(dir);
                }
                else continue;
            }
            else{
                QFile file(i);
                if(file.exists()){
                   db::DatabaseQuery::generate_add_file(file);
                }
                else continue;
            }
        }
        m_watcher->addPaths(lst); ///Добавляем в filewatcher пути
    }
    ///Добавляем пути к доступным деректориям
    for(const auto &it : split(sub_list_dirs, PATH_SEPARATOR)){
        auto lst = allPath(QString::fromStdString(it));
        for(const auto &i : qAsConst(lst)){
            QFileInfo info(i);
            if(info.isDir()){
                QDir dir(i);
                if(dir.exists()){
                    db::DatabaseQuery::generate_add_dir(dir, true);
                }
                else continue;
            }
            else{
                QFile file(i);
                if(file.exists()){
                   db::DatabaseQuery::generate_add_file(file, true);
                }
                else continue;
            }
        }
        m_watcher->addPaths(lst);
    }
    ///Добавляем пути к расшариным дерикториям
}

bool FileSystemDriver::addFile(QString &path)
{
    return false;
}

bool FileSystemDriver::addFolder(QString &path)
{
    return false;
}

bool FileSystemDriver::deleteFile(QString &path)
{
    return false;
}

bool FileSystemDriver::deleteFolder(QString &path)
{
    return false;
}

bool FileSystemDriver::copyFile(QString &path, QString &dist_path)
{
    return false;
}

bool FileSystemDriver::moveFile(QString &path, QString &dist_path)
{
    return false;
}

QStringList FileSystemDriver::allPath(const QString &dir_name)
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
