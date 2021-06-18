#include "databasequery.h"

namespace db {

/*!
 * \brief DatabaseQuery::generate_select_abonents считывает всех абонентов из базы данных
 * \return RESULT
 */
bool DatabaseQuery::generate_select_cl_status(std::map<std::string, size_t> &cl_status)
{
    RESULT res = db::DatabaseAccessor::getInstanse()->executeQuery("SELECT * FROM cl_status;");
    if(res.first==DBResult::ISOK){
        while (res.second.next()) {
            cl_status.insert(std::make_pair(res.second.value(1).toString().toStdString(),
                                            res.second.value(0).toUInt()));
        }
        return true;
    }
    else return false;
}

bool DatabaseQuery::generate_select_cl_abonent_type(std::map<std::string, size_t> &cl_abonent_type)
{
    RESULT res = db::DatabaseAccessor::getInstanse()->executeQuery("SELECT * FROM cl_abntonent_type;");
    if(res.first==DBResult::ISOK){
        while (res.second.next()) {
            cl_abonent_type.insert(std::make_pair(res.second.value(1).toString().toStdString(),
                                            res.second.value(0).toUInt()));
        }
        return true;
    }
    else return false;
}

RESULT DatabaseQuery::generate_select_abonents()
{
    QString str = "SELECT * FROM abonents;";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

unsigned int DatabaseQuery::generate_select_abonent_count()
{
    QString str = "SELECT COUNT(*) FROM abonents;";
    RESULT res = DatabaseAccessor::getInstanse()->executeQuery(str);
    if(res.first==DBResult::ISOK){
        res.second.next();
        return res.second.value(0).toUInt();
    }
    return 0;
}

RESULT DatabaseQuery::generate_update_abonent_type(const Abonent &abonent)
{
    QString str = "UPDATE abonents SET type = " + QString::number(abonent.db_type_id)
            + " WHERE Name = '" + QString::fromStdString(abonent.sys_name) + "';";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_update_abonent_path(const Abonent &abonent)
{
    QString str = "UPDATE abonents SET inbox = '" + QString::fromStdString(abonent.inbox_path)
            + "', outbox = '" + QString::fromStdString(abonent.outbox_path) +
            "' WHERE Name = '" + QString::fromStdString(abonent.sys_name) + "';";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_delete_abonent(const Abonent &abonent)
{
    QString str = "DELETE FROM abonents WHERE Name = '" + QString::fromStdString(abonent.sys_name) +"';";
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_insert_abonent(const Abonent &abonent)
{
    QString str = "INSERT INTO abonents (id, Name, inbox, outbox, type) VALUES ("
            + QString::number(abonent.db_id) +", '" + QString::fromStdString(abonent.sys_name)
            + "', '" + QString::fromStdString(abonent.inbox_path) + "', '" + QString::fromStdString(abonent.outbox_path)
            + "', " + QString::number(abonent.db_type_id) + ");";
    qDebug()<<str;
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_update_abonent(const Abonent &abonent)
{
    QString str = "UPDATE abonents SET inbox = '" + QString::fromStdString(abonent.inbox_path)
            + "', outbox = '" + QString::fromStdString(abonent.outbox_path) +
            "type = " + QString::number(abonent.db_type_id) +
            "' WHERE Name = '" + QString::fromStdString(abonent.sys_name) + "';";
    qDebug()<<str;
    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_select_all_letters()
{
    QString str = "SELECT DISTINCT"
            " letters.id,"
            " letters.dir_type_id,"
            " letters.to_id,"
            " letters.from_id,"
            " let_status.status_id,"
            " Max(let_status.date),"
            " let_options.let_title,"
            " let_options.let_path,"
            " let_options.attach_count"
            " FROM"
            " letters"
            " INNER JOIN let_status ON letters.id = let_status.id"
            " INNER JOIN let_options ON letters.id = let_options.let_id"
            " GROUP BY"
            " letters.id;";

    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_select_input_letters()
{
    QString str = "SELECT DISTINCT"
            " letters.id,"
            " letters.dir_type_id,"
            " letters.to_id,"
            " letters.from_id,"
            " let_status.status_id,"
            " Max(let_status.date),"
            " let_options.let_title,"
            " let_options.let_path,"
            " let_options.attach_count"
            " FROM"
            " letters"
            " INNER JOIN let_status ON letters.id = let_status.id"
            " INNER JOIN let_options ON letters.id = let_options.let_id"
            " WHERE"
            " letters.dir_type_id = 1"
            " GROUP BY"
            " letters.id;";

    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_select_output_letters()
{
    QString str = "SELECT DISTINCT"
            " letters.id,"
            " letters.dir_type_id,"
            " letters.to_id,"
            " letters.from_id,"
            " let_status.status_id,"
            " Max(let_status.date),"
            " let_options.let_title,"
            " let_options.let_path,"
            " let_options.attach_count"
            " FROM"
            " letters"
            " INNER JOIN let_status ON letters.id = let_status.id"
            " INNER JOIN let_options ON letters.id = let_options.let_id"
            " WHERE"
            " letters.dir_type_id = 2"
            " GROUP BY"
            " letters.id;";

    return DatabaseAccessor::getInstanse()->executeQuery(str);
}

unsigned int DatabaseQuery::generate_select_letters_count()
{
    QString str = "SELECT COUNT(*) FROM letters;";
    RESULT res = DatabaseAccessor::getInstanse()->executeQuery(str);
    if(res.first==DBResult::ISOK){
        res.second.next();
        return res.second.value(0).toUInt();
    }
    return 0;
}

RESULT DatabaseQuery::generate_insert_letters(Letter &letter)
{
    size_t letter_id = generate_select_letters_count();
    if(letter_id!=0){
        QString let_insert = "INSERT INTO letters VALUES ("
                + QString::number(letter_id) + ", "
                + QString::number(letter.let_dir_type) + ", "
                + QString::number(letter.from_id) + ", "
                + QString::number(letter.to_id) + ");";
        RESULT res = db::DatabaseAccessor::getInstanse()->executeQuery(let_insert);
        if(res.first != DBResult::ISOK) return res;

        QString let_status_insert = "INSERT INTO let_status (let_id, date, status_id) VALUES ("
                + QString::number(letter_id) + ", '"
                + QString::fromStdString(letter.date) + "', "
                + QString::number(letter.let_status) + ");";
        res = db::DatabaseAccessor::getInstanse()->executeQuery(let_status_insert);
        if(res.first != DBResult::ISOK) return res;

        QString let_option_insert = "INSERT INTO let_status (let_id, let_path, attach_count, let_title) VALUES ("
                + QString::number(letter_id) + ", '"
                + QString::fromStdString(letter.let_path) + "', "
                + QString::number(letter.attach_count) + ", '"
                + QString::fromStdString(letter.title) + "');";
        res = db::DatabaseAccessor::getInstanse()->executeQuery(let_option_insert);
        if(res.first != DBResult::ISOK) return res;
    }
    return RESULT(DBResult::FAIL, QSqlQuery());
}

RESULT DatabaseQuery::generate_delete_letters(Letter &letter)
{
    QString str;
    return db::DatabaseAccessor::getInstanse()->executeQuery(str);
}

RESULT DatabaseQuery::generate_update_letters(const Letter &letter)
{
    QString str;
    return db::DatabaseAccessor::getInstanse()->executeQuery(str);
}


}
