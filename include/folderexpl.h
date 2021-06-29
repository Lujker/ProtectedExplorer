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

class FileSystemModel;
class EmailModel;
class IconProvider;

/*!
 * \brief The FolderExpl class
 * \details Класс обертка для моделей таблиц файловой системы.
 * Класс предназначен для установлние связей (QObject::connect)
 * между двумя моделями и почтовой моделью, и их инициализации.
 *
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 01.06.2021
 * \warning Класс не допускает возможности наследования
 */
class FolderExpl final: public QObject
{
    Q_OBJECT
public:
    explicit FolderExpl(QObject *parent = nullptr);
    ~FolderExpl();
    void initFromSettings();
    void init(FileSystemModel *subs, FileSystemModel* dirs,
              EmailModel* leftModel, EmailModel* rightModel,
              AbonentModel *abonentModel);
    void clear_members();


    IconProvider *getProvider() const;
    void setProvider(IconProvider *value);

    std::pair<std::shared_ptr<FileSystemModel>, std::shared_ptr<FileSystemModel> > getDir_models() const;
    std::pair<std::shared_ptr<EmailModel>, std::shared_ptr<EmailModel>> getEmail_models() const;
    std::shared_ptr<AbonentModel>       getAbonent_model() const;


private:
    std::pair<
    std::shared_ptr<FileSystemModel>,
    std::shared_ptr<FileSystemModel>>     m_dir_models;

    std::pair<
    std::shared_ptr<EmailModel>,
    std::shared_ptr<EmailModel>>    m_email_models;

    std::shared_ptr<AbonentModel>   m_abonent_model;

    IconProvider*                   provider;
};

/*!
 * \brief The FileSystemModel class интерфейс для класса отображения фаловой системы
 * \details Данный класс является интерфейсом для отображения фаловой системы в qml
 */
class FileSystemModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    FileSystemModel(QObject* parent = nullptr);
    virtual ~FileSystemModel() = 0;
    virtual QString current_dir() = 0;

public slots:
    ///обновляет все данные в view qml
    virtual void refreshModel() = 0;
    ///Установка стандартных путей в таблице
    virtual void setAsSubModel() = 0;
    virtual void setAsDirModel() = 0;

    ///Переход по дерикториями
    virtual void openFolder(int index) = 0;
    ///преейти на уровень выше
    virtual void comeBack() = 0;
    ///Вернуться к стандартным дерикториям из настроек
    virtual void comeToBeginning() = 0;

    ///Копирование файлов
    virtual void copySelections(int start, int end) = 0;
    virtual void copyFile(int index) = 0;
    virtual void copyFrom(QString path) = 0;
    virtual void copyTo(QString path) = 0;
    ///перемещение фалов
    virtual void moveFile(int index) = 0;
    ///подпись файлов
    virtual void signedFile(int index) = 0;

    ///Работа с дерикториями и файлами
    virtual void derictoryChange(QString path) = 0;
    virtual void addFile(QString name) = 0;
    virtual void deleteFile(int index) = 0;
    virtual void deleteFiles(int start, int end) = 0;
    virtual void addFolder(QString name) = 0;
    virtual void deleteFolder(int index) = 0;
    virtual void renameFile(int index, QString name) = 0;
    ///Сортировка файлов таблицы
    virtual void setSorting(int column, int order) = 0;
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
 * \warning Возможны баги, класс не проходил тестирование. Насоедование заперщено (есть интерфейс)
 * \todo
 */
class DirsModel final: public FileSystemModel
{
    Q_OBJECT
    Q_PROPERTY(QString current_dir READ current_dir NOTIFY current_dir_change)

public:
    explicit DirsModel(std::vector<std::pair<std::string,std::string>> dirs, QObject* parent = nullptr);
    DirsModel(const DirsModel& ) = delete;
    ~DirsModel();
    QString current_dir() override;

public slots:
    //Установка стандартных путей в таблице
    void refreshModel() override;
    void setAsSubModel() override;
    void setAsDirModel() override;

    //Переход по дерикториями
    void openFolder(int index) override;
    void comeBack() override;
    void comeToBeginning() override;
    //Копирование файлов
    void copySelections(int start, int end) override;
    void copyFile(int index) override;
    void moveFile(int index) override;
    void signedFile(int index) override;
    void copyFrom(QString path) override;
    void copyTo(QString path) override;

    //Работа с дерикториями и файлами
    void derictoryChange(QString path) override;
    void addFile(QString name) override;
    void deleteFile(int index) override;
    void deleteFiles(int start, int end) override;
    void addFolder(QString name) override;
    void deleteFolder(int index) override;
    void renameFile(int index, QString name) override;
    //Сортировка файлов таблцы
    void setSorting(int column, int order) override;
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
    ///отправляем сигнал на соседнюю модель если пользователь копирует
    void copyFile(QString);
    ///отслеживаем изменения в текущей дериктории
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
        ///отслеживает текущую дерикторию и обновляет если изменилась
        QFileSystemWatcher* m_watcher;
        ///путь к текущей дериктории
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
class IconProvider final: public QQuickImageProvider
{
public:
    IconProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
protected:
    QFileIconProvider m_provider;
    QMimeDatabase m_mimeDB;
};

#endif // FOLDEREXPL_H
