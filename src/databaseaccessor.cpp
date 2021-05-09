#include "../include/databaseaccessor.h"

namespace db {
///инициализация статических переменных для доступа к ним в других файлах
QString DatabaseAccessor::dbHost=nullptr;
QString DatabaseAccessor::dbName=nullptr;
QString DatabaseAccessor::dbPass=nullptr;
QString DatabaseAccessor::dbUser=nullptr;
QString DatabaseAccessor::files_table=nullptr;
QString DatabaseAccessor::file_options_table=nullptr;
int     DatabaseAccessor::m_state=0;


DatabaseAccessor *DatabaseAccessor::getInstanse()
{
    if(!checkSQLDriver()) return nullptr;
    static DatabaseAccessor instanse;
    return &instanse;
}

int DatabaseAccessor::getState()
{
    return m_state;
}

void DatabaseAccessor::close()
{
    DBCloser(db);
}

void DatabaseAccessor::setState(int state)
{
    m_state = state;    
}

std::pair<DBState, QSqlQuery> DatabaseAccessor::executeQuery(const QString& query)
{
    QSqlQuery qu(query,*db);
    if(qu.isActive())
    return std::make_pair(DBState::OK,qu);
    else return std::make_pair(DBState::ERROR_QUERY,qu);
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

bool DatabaseAccessor::checkSQLDriver(QString driver_name)
{
    if (QSqlDatabase::isDriverAvailable(driver_name))
        return true;
    else return false;
    return false;
}

DatabaseAccessor::DatabaseAccessor()
{
    auto database = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db.reset(database);
    db->setHostName(dbHost);
    db->setDatabaseName(dbName);
    db->setUserName(dbUser);
    db->setPassword(dbPass);

    if(!db->open()){        
       qDebug()<<db->lastError().text();
    }
    else{

    }
}

DatabaseAccessor::~DatabaseAccessor()
{}

}
