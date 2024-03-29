﻿#ifndef EMAILMODEL_H
#define EMAILMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQuickItem>
///фаловая система
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
/// stl модули
#include <list>
#include <algorithm>
#include <set>
/// написанные модули
#include <settingscontroller.h>
#include <loger.h>
#include <databaseaccessor.h>
#include <databasequery.h>
/// модули SQL
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

/*!
 * \brief The EmailModel class
 * \details Класс определяет почтовый виджет для работы с формированием и отправкой/принятем контейнеров
 * в/из черного ящика.
 *
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 10.06.2021
 * \warning не протестированн выриант наследования и переопределения методов получения информации из БД и писем
 * \todo Добавить интерфейс
 */
class EmailModel: public QAbstractListModel
{
    Q_OBJECT
public:

    enum STATUS{
        EMPTY = 0,
        IS_OK = 10,
        INIT_AB_ERROR,
        INIT_LET_ERROR,
        INIT_FSW_ERROR,
        DB_ACCEPT_FAILED
    };

    enum MODEL_TYPE{
        UNKNOWN = 0,
        INBOX,
        OUTBOX,
        FAIL
    };

    enum LETTERS_ROLES{
        TITLE = Qt::UserRole+1,
        STATUS,
        DATE,
        ATACH_COUNT,
        ICON,
        SENDER,
        INDEX
    };

    explicit        EmailModel(std::vector<Abonent>& abonents, QObject* parent = nullptr);
    virtual         ~EmailModel();

    ///\warning Методы ниже бязательно переопределяются в наслединке
    virtual void    initModelData() noexcept;
    virtual void    initWatchers()  noexcept;
    virtual void    initAddressBook();
    virtual void    init();
    ///\brief Установка значений состояния программы (не обязательно переопределять)
    virtual void    setStatus(enum STATUS status);
    virtual void    setModel_type(enum MODEL_TYPE model_type);
    virtual void    setRef_abonents(const std::vector<Abonent> &ref_abonents);
    virtual void    setLetters(const std::vector<Letter> &letters);

    std::vector<Letter>     &letters()      noexcept;
    std::vector<Abonent>    &ref_abonents() const noexcept;
    enum MODEL_TYPE         getModel_type() const noexcept;
    enum STATUS             status()        const noexcept;
public slots:
    ///Для QML comboBox вызывает соответсвующие виртуальные функции
    void setOutputList();
    void setInputList();
    ///\brief Делает запрос в БД и вызывает SetAbonentFromRESULT в случае успеха
    virtual void    setOutputLetters(std::vector<Letter>& letters);
    ///\brief Делает запрос в БД и вызывает SetAbonentFromRESULT в случае успеха
    virtual void    setInputLetters(std::vector<Letter> &letters);
    ///\brief Общая перезагрузка
    virtual void    update();
    ///\brief Обновление списка абонентов и асоциаций с письмами
    virtual void    updateAbonents();
    ///\brief Получение нового входящего (изначально соединяется с QFileSystemWatcher)
    virtual void    getNewInMessage(QString path);
    ///\brief Получение нового исходящего (изначально соединяется с QFileSystemWatcher)
    virtual void    getNewOutMessage(QString path);
    ///\brief Формирование нового исходящего и отправка
    virtual void    sendMessage();
    ///\brief Получение списка файлов во вложении
    virtual void    getAttacmentsList(const int index);
    ///\brief Получение вложения контейнера
    virtual void    getAttacments(const int mes_index, const int dir_index);
    ///\brief Удаление сообщения
    virtual void    deleteMessage(const int index);
    /// QAbstractItemModel interface доступа объектов из QML

    ///\warning Методы ниже бязательно переопределяются в наслединке
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int         rowCount(const QModelIndex &parent) const override;
    virtual int         columnCount(const QModelIndex &parent) const override;
    virtual QVariant    data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    ///\brief служебные функции только для данного класса
private:
    void setAbonentsFromRESULT(db::RESULT& result, std::set<Abonent> &ab_arr);
    void setLettersFromRESULT(db::RESULT& result, std::vector<Letter>& let_arr);
    void syncLettersWithDir(std::vector<Letter>& let_arr, const QString& path);
    void makeLettersFromDir(std::vector<Letter>& empty_let_arr, const QString& path);

private:
    ///Ссылка на список абонентов в классе настроек
    std::vector<Abonent>&           m_ref_abonents;
    ///Список писем
    std::vector<Letter>             m_letters;
    ///Слежка за входящими дерикториями
    QFileSystemWatcher*             m_inbox_watchers;
    ///Слежка за исходящими дерикториями
    QFileSystemWatcher*             m_outbox_watchers;
    enum STATUS                     m_status;
    enum MODEL_TYPE                 m_model_type;

signals:
    void inboxChange();
    void ouboxChange();
};

/*!
 * \brief The AbonentModel class
 * \details Класс определяет виджет списка абонентов
 *
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 10.06.2021
 * \warning
 */
class AbonentModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum AbonentsRoles{
        ICON = Qt::UserRole+1,
        NAME,
        FROM,
        TO,
        INDEX
    };

    explicit        AbonentModel(std::vector<Abonent>& abonents, QObject* parent = nullptr);
    virtual         ~AbonentModel();
    virtual void    init();
    /// QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int         rowCount(const QModelIndex &parent) const override;
    virtual int         columnCount(const QModelIndex &parent) const override;
    virtual QVariant    data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    ///get funcs
    std::vector<Abonent> &ref_abonents() const;
    void setRef_abonents(const std::vector<Abonent> &ref_abonents);

public slots:
    virtual void addAbonent(QString sys_name= "Не указано",
                            QString inbox_path = "...",
                            QString outbox_path="...",
                            QString icon_path = ":/../icons/contacts.png",
                            int db_type_id=0);
    virtual void delAbonent(int index);
    virtual void renameAbonent(int index, QString sys_name);
    virtual void setInPath(int index, QString path);
    virtual void setOutPath(int index, QString path);
    virtual void setIconPath(int index, QString path);
    virtual void updateAll();

private:
    std::vector<Abonent>&           m_ref_abonents; ///Ссылка на список абонентов в классе настроек

signals:
    void abonentsChange();
};

#endif // EMAILMODEL_H
