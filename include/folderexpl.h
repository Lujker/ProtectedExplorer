#ifndef FOLDEREXPL_H
#define FOLDEREXPL_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QList>
#include <QAbstractListModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QFileIconProvider>
#include <list>

#include <settingscontroller.h>
#include <loger.h>

class DirsModel;

class FolderExpl : public QObject
{
    Q_OBJECT
public:
    explicit FolderExpl(QObject *parent = nullptr);
    void initFromSettings();
    void init(DirsModel *subs, DirsModel* dirs);
    void clear_members();

    DirsModel *sub_model() const;
    DirsModel *dir_model() const;

signals:

private:
    DirsModel* m_sub_model;
    DirsModel* m_dir_model;
};

class DirsModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DirsModel(std::list<std::string> dirs, QObject* parent = nullptr);

public slots:
    void refreshModel();

    void openFolder(int index);
    void comeBack();
    void comeToBeginning();

    void copySelections(int start, int end);
    void copyFrom(QString path);
    void copyTo(QString path);

    void derictoryChange(const QString& path);
    void addFile();
    void deleteFile(int index);
    void deleteFiles(int start, int end);
    void addFolder();
    void deleteFolder(int index);


private:
    ///список даступных папок с их вложением
    QList<QString> m_dirs;
    ///открытая сейчас дериктория
    QDir* folder;
    ///списко имен папок и файлов для отображения
    QList<QString> m_filenames;
    ///уровень вхождения
    int m_level_count;
    QFileSystemWatcher* watcher;

    enum DirsRoles
        {
            IconRole = Qt::UserRole+1,
            NameRole,
            SuffixRole,
            SizeRole,
            isFolderRole,
            DateRole
        };

    void copyPath(QString src, QString dst);

    /// QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:
    void copyFile(QString);
};


#endif // FOLDEREXPL_H
