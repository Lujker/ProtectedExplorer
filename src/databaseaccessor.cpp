#include "databaseaccessor.h"

namespace db {
///инициализация статических переменных для доступа к ним в других классах проекта
QString DatabaseAccessor::dbName="prtexp.db";
int     DatabaseAccessor::m_state=0;

/*!
 * \brief DatabaseAccessor::getInstanse получение signleton экземплаяра класса
 * \return указатель на экземпляр
 */
DatabaseAccessor *DatabaseAccessor::getInstanse()
{
 static DatabaseAccessor instanse; ///если обьект не создан вызывается привытный конструктор
 return &instanse;
}

int DatabaseAccessor::getState()
{
    return m_state;
}

/*!
 * \brief DatabaseAccessor::close закрытие подключения к базе данных через функтор
 */
void DatabaseAccessor::close()
{
    db->close();
    db.get_deleter()(this->db.get());
}

void DatabaseAccessor::setState(int state)
{
    m_state = state;
}

RESULT DatabaseAccessor::executeQuery(const QString& query)
{
    QSqlQuery qu(*db);

    if(query.isEmpty()) return std::make_pair(DBResult::FAIL,qu);
    if(qu.exec(query) && !qu.lastError().isValid()){
        return std::make_pair(DBResult::ISOK,qu);
    }
    else{
        qDebug()<<qu.lastError().text();
        return std::make_pair(DBResult::FAIL,qu);
    }
}

bool DatabaseAccessor::try_open()
{
    if(!db->open()){
       qDebug()<<db->lastError().text();
       return false;
    }
    else{
       return true;
    }
}

bool DatabaseAccessor::is_open()
{
    return db->isOpen();
}

///\brief проврека наличия и подключения к БД
bool DatabaseAccessor::db_is_valid()
{
    return db->isValid();
}

///\brief провекра наличия таблиц
bool DatabaseAccessor::check_tables()
{
    return false;
}

///\brief запускаем скрипт создания базы данных
bool DatabaseAccessor::create_tables()
{
    return false;
}

///\brief проверка наличия драйвера
bool DatabaseAccessor::driver_check()
{
    return false;
}

/*!
 * \brief DatabaseAccessor::DatabaseAccessor создает экземпляр и инициализирует подключение
 */
DatabaseAccessor::DatabaseAccessor()
{
    auto database = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db.reset(database);
    db->setDatabaseName(dbName);
    if(!db->open()){
       qDebug()<<db->lastError().text();
    }
}

DatabaseAccessor::~DatabaseAccessor()
{}

}
