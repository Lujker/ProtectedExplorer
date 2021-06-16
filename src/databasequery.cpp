#include "databasequery.h"

namespace db {

/*!
 * \brief DatabaseQuery::generate_select_abonents считывает всех абонентов из базы данных
 * \return RESULT
 */
RESULT DatabaseQuery::generate_select_abonents()
{
    QString str = "SELECT * FROM abonents;";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_update_abonent_type(const size_t& type_id, const std::string &sys_name)
{
    QString str = "UPDATE abonents SET type = " + QString::number(type_id) + " WHERE Name = '" + QString::fromStdString(sys_name) + "';";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_update_abonent_path(const std::string &input, const std::string &output, const std::string &sys_name)
{
    QString str = "UPDATE abonents SET inbox = " + QString::fromStdString(input) + ", outbox = " + QString::fromStdString(output) +
    " WHERE Name = '" + QString::fromStdString(sys_name) + "';";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_insert_abonent(const size_t &db_id, const std::string &sys_name, const std::string &input, const std::string &output, const size_t &type_id)
{
    QString str = "INSERT INTO abonents (id, Name, inbox, outbox, type) VALUES (" + QString::number(db_id) +", '" + QString::fromStdString(sys_name) + "', '" + QString::fromStdString(input) +
    "', '" + QString::fromStdString(output) + "', " + QString::number(type_id) + ");";
    qDebug()<<str;
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_select_input_letters()
{
    QString str;
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}


}
