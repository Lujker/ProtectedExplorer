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
    virtual void setPath(std::string &dir_list, std::string &sub_list_dirs);
    virtual void updateDataBase() = 0;

public slots:
    virtual bool addFile(QString &path);
    virtual bool addFolder(QString &path);
    virtual bool deleteFile(QString &path);
    virtual bool deleteFolder(QString &path);
    virtual bool copyFile(QString &path, QString &dist_path);
    virtual bool moveFile(QString &path, QString &dist_path);

signals:
    void updateBase();

protected:
    virtual void createDataBase() = 0;
private:
    QStringList allPath(const QString& dir_name);

};

class FileSystemDriver: public Driver
{
    Q_OBJECT
public:
    FileSystemDriver(QObject* parent = nullptr);

public slots:
    void newFile(QString path);

private:
    QFileSystemWatcher* m_watcher;
};

#endif // FILESYSTEMDRIVER_H
