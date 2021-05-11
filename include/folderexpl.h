#ifndef FOLDEREXPL_H
#define FOLDEREXPL_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QAbstractListModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>

#include <settingscontroller.h>
#include <loger.h>

class SubsModel;
class DirsModel;

class FolderExpl : public QObject
{
    Q_OBJECT
public:
    explicit FolderExpl(QObject *parent = nullptr);
    void initFromSettings();
    void init(DirsModel *subs, DirsModel* dirs);
    void clear_members();

signals:

private:
    DirsModel* m_sub_model;
    DirsModel* m_dir_model;
};

class DirsModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DirsModel(std::list<std::string> dirs, QObject* parent = nullptr);

public slots:
    void openFolder(int index);
    void comeBack();
    void comeToBeginning();
    void refreshModel();

private:
    ///список даступных папок с их вложением
    std::list<std::string> m_dirs;
    ///открытая сейчас дериктория
    QDir* folder;
    ///списко имен папок и файлов для отображения
    QList<QString> m_filenames;
    ///уровень вхождения
    int m_level_count;
    QFileSystemWatcher* watcher;

    enum DirsRoles
        {
            NameRole = Qt::UserRole+1,
            SizeRole,
            isFolderRole,
            DateRole
        };

    /// QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
};


#endif // FOLDEREXPL_H
