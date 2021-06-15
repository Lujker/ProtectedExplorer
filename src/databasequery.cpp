#include "databasequery.h"

namespace db {

/*!
 * \brief DatabaseQuery::generate_select_abonents считывает всех абонентов из базы данных
 * \return RESULT
 */
RESULT DatabaseQuery::generate_select_abonents()
{
    QString str = "SELECT Name, inbox, outbox, type FROM abonents;";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_select_input_letters()
{
    QString str;
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}


}
