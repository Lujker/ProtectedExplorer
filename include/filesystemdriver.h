#ifndef FILESYSTEMDRIVER_H
#define FILESYSTEMDRIVER_H

#include <QFileSystemWatcher>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "databasequery.h"
#include "settingscontroller.h"


class Driver: public QObject{
    Q_OBJECT
public:
    Driver(QObject* parent = nullptr);
    virtual ~Driver();
    virtual void updateDataBase() = 0;
    virtual void createDataBase() = 0;

public slots:
    virtual bool addFile(QString &path) = 0;
    virtual bool addFolder(QString &path) = 0;
    virtual bool deleteFile(QString &path) = 0;
    virtual bool deleteFolder(QString &path) = 0;
    virtual bool copyFile(QString &path, QString &dist_path) = 0;
    virtual bool moveFile(QString &path, QString &dist_path) = 0;
    virtual void newFile(QString path) = 0;

signals:
    void updateBase();
};

class FileSystemDriver: public Driver
{
    Q_OBJECT
public:
    FileSystemDriver(QObject* parent = nullptr);
    virtual ~FileSystemDriver();
    virtual void createDataBase();
    virtual void updateDataBase();

public slots:
    virtual bool addFile(QString &path);
    virtual bool addFolder(QString &path);
    virtual bool deleteFile(QString &path);
    virtual bool deleteFolder(QString &path);
    virtual bool copyFile(QString &path, QString &dist_path);
    virtual bool moveFile(QString &path, QString &dist_path);
    virtual void newFile(QString path);

protected:
    QStringList allPath(const QString& dir_name);
    virtual void setPath(const std::string &dir_list, const std::string &sub_list_dirs);
    QFileSystemWatcher* m_watcher;
};

#endif // FILESYSTEMDRIVER_H
