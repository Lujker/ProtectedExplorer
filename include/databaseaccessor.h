#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H

//Qt libs
#include <QObject>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <memory>

#include "databasequery.h"

/*!
 *  \brief Класс DatabaseAccessor написан на основе паттерна проектирования singleton.
 * Для получения подробной информации см. "singleton meyers".
 * Класс обеспечивает единаличный доступ к базе данных без возможности
 * создать копию или инициализировать объект повторно.
 * Кроме того, структура класса основана на идеалогии RAII
 * поэтому при создании объекта не нужно беспокоится об освобождении памяти
 * или закрытии базы данных перед удалении.
 *
 * Перечисление DBState содежрит возможные состояния после выполнения операций над
 * классами в namespace db.
 *
 * Класс DBCloser является функтором который освобождает рессурсы объекта класса
 * DatabaseAccessor, а именно при удалении инстенса класс перед выходом из программы.
 *
 * Некоторые мемберы класса DatabaseAccessor статичны, таким образом мы
 * имеем доступ к настройкам базы данных без обращения к объекту и
 * получения его инстенса. Это используется в конструкторе главного окна.
 *
 *  \author Z.V.P
 *  \version 1.1
 *  \date 04.02.2021
 *
 * */


namespace db {

namespace
{
    class DBCloser {
    public:
        void operator() (QSqlDatabase* db) {
            if(db!=nullptr && db->isOpen()){
//                db::DatabaseQuery::generate_drop_file_table();
//                db::DatabaseQuery::generate_drop_file_options_table();
                db->close();
                QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
//                QFile::remove(db->databaseName());
                delete db;
            }
        }
    };
}

enum DBState{
                     OK=20,
           ERROR_NO_DRIVER,
             ERROR_OPENING,
               ERROR_QUERY,
                ERROR_BASU,
  ERROR_NOT_EXIST_SETTINGS
};

class DatabaseAccessor: public QObject
{

    Q_OBJECT
public:
    static DatabaseAccessor* getInstanse(); ///получение экземпляра базы данных

    ///параметры настроек для подключения к базе данных
    static QString dbHost;
    static QString dbName;
    static QString dbUser;
    static QString dbPass;
    ///названия таблиц которые мы передаем в класс query при запросах
    static QString files_table;
    static QString file_options_table;
    static int     m_state;


public:
    std::pair<DBState,QSqlQuery> executeQuery(const QString &query);
    bool try_open();
    bool is_open();
    bool db_is_valid();
    static bool checkSQLDriver(QString driver_name = "QSQLITE");
    static void setState(int state);
    static int getState();
    static void close();

private:
    DatabaseAccessor();
    ~DatabaseAccessor();
    DatabaseAccessor(const DatabaseAccessor&) =delete;
    DatabaseAccessor& operator=(const DatabaseAccessor& )=delete;
    //move construct and move operator= not create, bec we have constructor&

    //database referense
    std::unique_ptr<QSqlDatabase,DBCloser> db;
};

}
#endif // DATABASEACCESSOR_H
