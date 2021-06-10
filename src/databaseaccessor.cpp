#include "databaseaccessor.h"

namespace db {
///инициализация статических переменных для доступа к ним в других классах проекта
QString DatabaseAccessor::dbName=nullptr;
QString DatabaseAccessor::ls_vf_NamedTable=nullptr;
QString DatabaseAccessor::vvt_NamedTable=nullptr;
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
    DBCloser(db);
}

void DatabaseAccessor::setState(int state)
{
    m_state = state;
}

RESULT DatabaseAccessor::executeQuery(const QString& query)
{
    QSqlQuery qu(*db);
    if(!qu.exec(query)&&qu.lastError().isValid())
        return std::make_pair(DBResult::ISOK,qu);
    else return std::make_pair(DBResult::FAIL,qu);
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

bool DatabaseAccessor::db_is_valid()
{
    return db->isValid();
}

bool DatabaseAccessor::check_tables()
{
    return false;
}

bool DatabaseAccessor::create_tables()
{
    return false;
}

/*!
 * \brief DatabaseAccessor::DatabaseAccessor создает экземпляр и инициализирует подключение
 */
DatabaseAccessor::DatabaseAccessor()
{
    auto database = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    db.reset(database);
    db->setDatabaseName(dbName);

    if(!db->open()){
       qDebug()<<db->lastError().text();
    }
}

DatabaseAccessor::~DatabaseAccessor()
{}

}
