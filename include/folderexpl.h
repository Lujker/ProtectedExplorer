#ifndef FOLDEREXPL_H
#define FOLDEREXPL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>

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
    void init(SubsModel* subs, DirsModel* dirs);
    void clear_members();

signals:

private:
    SubsModel* m_sub_model;
    DirsModel* m_dir_model;
};

class SubsModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SubsModel(std::list<std::string> subs, QObject* parent = nullptr);

private:
    std::list<std::string> m_subs;
    enum SubsRoles
        {
            NameRole = Qt::UserRole+1,
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

class DirsModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DirsModel(std::list<std::string> dirs, QObject* parent = nullptr);

private:
    std::list<std::string> m_dirs;
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
