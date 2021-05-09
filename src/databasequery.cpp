#include "../include/databasequery.h"

namespace db {

DatabaseQuery::RESULT DatabaseQuery::exec_request(QSqlQuery qu)
{
    if(!qu.exec()&&qu.lastError().isValid()){
//        qCritical()<<qu.lastError().text()<<"\n"<<qu.lastQuery();
        return std::make_pair(db::DBResult::FAIL,qu);
    }
    else{
//                qDebug()<<qu.lastQuery();
        return std::make_pair(db::DBResult::ISOK,qu);
    }
}

DatabaseQuery::RESULT DatabaseQuery::generate_create_files_table()
{
    QSqlQuery query;
    QString str = "CREATE TABLE IF NOT EXISTS " + db::DatabaseAccessor::files_table + " ("
                    "id INTEGER PRIMARY KEY NOT NULL, options INTEGER REFERENCES " +
                    db::DatabaseAccessor::getInstanse()->file_options_table + "(id), "
                    "parentDir INTEGER REFERENCES " + db::DatabaseAccessor::files_table +
                    "(id), name TEXT NOT NULL, isDir INTEGER NOT NULL, isShared INTEGER NOT NULL);";

    if(!query.exec(str) && query.lastError().isValid()){
        return std::make_pair(db::DBResult::FAIL, query);
    }
    else{
        return std::make_pair(db::DBResult::ISOK, query);
    }

    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_crete_file_options_table()
{
    QSqlQuery query;
    QString str = "CREATE TABLE IF NOT EXISTS " + db::DatabaseAccessor::file_options_table + " ("
                    "id INTEGER PRIMARY KEY NOT NULL, size INTEGER, type TEXT"
                    "date TEXT, path TEXT);";

    if(!query.exec(str) && query.lastError().isValid()){
        return std::make_pair(db::DBResult::FAIL, query);
    }
    else{
        return std::make_pair(db::DBResult::ISOK, query);
    }
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_drop_file_table()
{
    QSqlQuery query;
    QString str = "CREATE TABLE IF EXISTS " + db::DatabaseAccessor::files_table + ";";

    if(!query.exec(str) && query.lastError().isValid()){
        return std::make_pair(db::DBResult::FAIL, query);
    }
    else{
        return std::make_pair(db::DBResult::ISOK, query);
    }
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_drop_file_options_table()
{
    QSqlQuery query;
    QString str = "CREATE TABLE IF EXISTS " + db::DatabaseAccessor::file_options_table + ";";

    if(!query.exec(str) && query.lastError().isValid()){
        return std::make_pair(db::DBResult::FAIL, query);
    }
    else{
        return std::make_pair(db::DBResult::ISOK, query);
    }
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_add_file(QFile& file, bool isSub)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_add_dir(QDir& dir, bool isSub)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_delete_file(std::string& path)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_find_file(std::string &path)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_copy_file_to(std::string& path, std::string& dest_path)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_move_file_to(std::string& path, std::string& dest_path)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_select_all_files(std::string &work_dir)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::generate_select_all_sub_files(std::string &work_dir)
{
    QSqlQuery query;
    return std::make_pair(db::DBResult::FAIL, query);
}

DatabaseQuery::RESULT DatabaseQuery::exec_request(const QString &qu)
{
    QSqlQuery query;
    if(!query.exec(qu) && query.lastError().isValid()){
//        qCritical()<<qu.lastError().text()<<"\n"<<qu.lastQuery();
        return std::make_pair(db::DBResult::FAIL, query);
    }
    else{
//                qDebug()<<qu.lastQuery();
        return std::make_pair(db::DBResult::ISOK, query);
    }
}

}
