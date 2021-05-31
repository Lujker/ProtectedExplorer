#ifndef FOLDEREXPL_H
#define FOLDEREXPL_H

/// модули библиотеки Qt
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
#include <QDesktopServices>
#include <QUrl>
#include <QQuickImageProvider>
#include <QFileIconProvider>
#include <QMimeDatabase>
#include <QPixmap>

/// stl модули
#include <list>
#include <algorithm>

/// написанные модули
#include <settingscontroller.h>
#include <loger.h>


class DirsModel;
class IconProvider;

/*!
 * \brief The FolderExpl class
 * \details Класс обертка для моделей таблиц файловой системы.
 * Класс предназначен для установлние связей (QObject::connect)
 * между двумя моделями и их инициализации.
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 01.06.2021
 * \warning Класс не допускает возможности наследования
 */
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

    IconProvider *getProvider() const;
    void setProvider(IconProvider *value);

signals:

private:
    DirsModel* m_sub_model;
    DirsModel* m_dir_model;
    IconProvider* provider;
};

/*!
 * \brief The DirsModel class
 * \details Класс явялется моделью для таблицы файловой системы.
 * Класс наследуется от абстрактной табличной модели Qt
 * и переопредеяет полностью абстарктные методы доступа к значениям таблицы.
 * Класс констурируется получая стандартные пути доступа к папкам
 * из класса обеспечивающий чтение и запись настроек.
 * Класс определяет множество стандартных методов работы с файловой системой.
 *
 * Класс содержит списки со стандартными путями дерикторий
 * и списко файлов открытой дериктории.
 * Также класс содерит QDir и QFileSystemWatcher обьекты
 * которые содержат открытую сейчас дерикторию и обьект для отслеживания
 * изменения в открытой дериктории.
 * Уровень вложенности входа в дерикторию отслежтвается
 * счетчиком m_level_count и обеспечивает возможность замкнутости
 * дерикторий. Когда счетчик равен 0 не допускается выход из папки.
 *
 *
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 01.06.2021
 * \warning Возможны баги, класс не проходил тестирование
 */
class DirsModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DirsModel(std::list<std::string> dirs, QObject* parent = nullptr);

public slots:
    //Установка стандартных путей в таблице
    void refreshModel();
    void setAsSubModel();
    void setAsDirModel();

    //Переход по дерикториями
    void openFolder(int index);
    void comeBack();
    void comeToBeginning();
    //Копирование файлов
    void copySelections(int start, int end);
    void copyFile(int index);
    void signedFile(int index);
    void copyFrom(QString path);
    void copyTo(QString path);

    //Работа с дерикториями и файлами
    void derictoryChange(const QString& path);
    void addFile();
    void deleteFile(int index);
    void deleteFiles(int start, int end);
    void addFolder();
    void deleteFolder(int index);
    void renameFile(int index, QString name);
    //Сортировка файлов таблциы
    void setSorting(int column, int order);
    void sortByName(bool lower = false);
    void sortByDate(bool lower = false);
    void sortBySize(bool lower = false);
    void sortBySuffix(bool lower = false);


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

    ///Перечисление для доступа к информации в qml
    enum DirsRoles
        {
            FilePathRole = Qt::UserRole+1,
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

/*!
 * \brief The IconProvider class
 * \details Класс переопределяет проводник между файловой системой и приложением.
 * По индексу элемента таблицы мы получаем полный путь к файлу
 * и получаем его икноку из файловой сисетмы.
 *
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 01.06.2021
 * \warning Загрузка иконок проходит медленно
 */
class IconProvider : public QQuickImageProvider
{
public:
    IconProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
protected:
    QFileIconProvider m_provider;
    QMimeDatabase m_mimeDB;
signals:
public slots:
};

#endif // FOLDEREXPL_H
