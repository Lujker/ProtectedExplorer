#ifndef FOLDEREXPL_H
#define FOLDEREXPL_H

/// модули библиотеки Qt
#include <QObject>
#include <QDebug>
#include <QList>
#include <QDateTime>
#include <QAbstractListModel>
///модули QFilesystem
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QFileIconProvider>
#include <QDesktopServices>
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
#include <databaseaccessor.h>
#include <databasequery.h>
#include <emailmodel.h>


class DirsModel;
class IconProvider;
class EmailModel;

/*!
 * \brief The FolderExpl class
 * \details Класс обертка для моделей таблиц файловой системы.
 * Класс предназначен для установлние связей (QObject::connect)
 * между двумя моделями и почтовой моделью, и их инициализации.
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
    void init(DirsModel *subs, DirsModel* dirs, EmailModel* mail = nullptr);
    void clear_members();

    DirsModel *sub_model() const;
    DirsModel *dir_model() const;

    IconProvider *getProvider() const;
    void setProvider(IconProvider *value);
private:
    DirsModel*      m_sub_model;
    DirsModel*      m_dir_model;
    EmailModel*     m_mail_model;
    IconProvider*       provider;
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

    Q_PROPERTY(QString current_dir READ current_dir NOTIFY current_dir_change)

public:
    explicit DirsModel(std::vector<std::pair<std::string,std::string>> dirs, QObject* parent = nullptr);
    QString current_dir();

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
    void moveFile(int index);
    void signedFile(int index);
    void copyFrom(QString path);
    void copyTo(QString path);

    //Работа с дерикториями и файлами
    void derictoryChange(const QString& path);
    void addFile(QString name);
    void deleteFile(int index);
    void deleteFiles(int start, int end);
    void addFolder(QString name);
    void deleteFolder(int index);
    void renameFile(int index, QString name);
    //Сортировка файлов таблциы
    void setSorting(int column, int order);
    void sortByName(bool lower = false);
    void sortByDate(bool lower = false);
    void sortBySize(bool lower = false);
    void sortBySuffix(bool lower = false);
    void copyPath(QString src, QString dst);


    /// QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void copyFile(QString);
    void current_dir_change(QString);

private:
        ///список даступных папок с их вложением
        std::vector<std::pair<std::string,std::string>> m_dirs;
        ///открытая сейчас дериктория
        QDir* m_folder;
        ///списко имен папок и файлов для отображения
        QList<QString> m_filenames;
        ///уровень вхождения
        int m_level_count;
        QFileSystemWatcher* m_watcher;
        std::string m_current_dir;

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
};

#endif // FOLDEREXPL_H
