#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H

///Qt libs
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <memory>


namespace db {
enum DBState{
                     OK=20,
           ERROR_NO_DRIVER,
             ERROR_OPENING,
               ERROR_QUERY,
  ERROR_NOT_EXIST_SETTINGS
};


enum class DBResult{
    ISOK=30,FAIL=31
};

using RESULT = std::pair<DBResult,QSqlQuery>; ///возвращаем результат в виде информации об успешности и запрос для получения информации

namespace
{
    class DBCloser {
    public:
        void operator() (QSqlDatabase* db) {
            if(db!=nullptr && db->isOpen()){
                db->close();
                QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
                delete db;
            }
        }
    };
}

/*!
 *  \brief Класс DatabaseAccessor написан на основе паттерна проектирования singleton.
 *
 *  \details Для получения подробной информации см. "singleton meyers".
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
 *  \version 1.2
 *  \date 10.06.2021
 * */
class DatabaseAccessor: public QObject
{

    Q_OBJECT
public:
    ///получение экземпляра базы данных
    static DatabaseAccessor* getInstanse();
    ///параметры настроек для подключения к базе данных
    static QString   dbName;
    static int      m_state;


public:
    RESULT executeQuery(const QString &query);
    bool try_open();
    bool is_open();
    bool db_is_valid();
    bool check_tables();
    bool create_tables();
    bool driver_check();
    static void setState(int state);
    static int getState();
    void close();


private:
    DatabaseAccessor();
    ~DatabaseAccessor();
    DatabaseAccessor(const DatabaseAccessor&) =delete;
    DatabaseAccessor& operator=(const DatabaseAccessor& )=delete;
    //database referense
    std::unique_ptr<QSqlDatabase,DBCloser> db;
};

}
#endif // DATABASEACCESSOR_H
