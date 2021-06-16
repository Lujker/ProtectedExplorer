#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

///Incl moduls
#include "databaseaccessor.h"

///Qt libs
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>

///C and C++ libs
#include <cmath>
#include <map>
#include <set>
#include <iostream>
#include <iterator>

namespace db {

/*!
  \brief Класс DatabaseQuery содержит набор статичных методов для обращения к подключенной базе данных

  \details Объект данного класса невозможно создать т.к. он не несет информации а служит лишь для выполнения запросов
  Данный класс находится в namespace db и при необходимости может быть расширен.

  Перечисление DBResult сообщает о состоянни запроса после его выполнения.
  Если во время выполнения запроса не возникло ошибок в паре RESULT первый аргумент будет = ISOK.

 *  \author Z.V.P
 *  \version 1.2
 *  \date 10.06.2021

 * */
class DatabaseQuery
{
public:
    ///Abonents_table
    static RESULT generate_select_abonents();
    static RESULT generate_select_abonent_input_dir();
    static RESULT generate_select_abonent_output_dir();
    static RESULT generate_update_abonent_type(const size_t &type_id, const std::string &sys_name);
    static RESULT generate_update_abonent_path(const std::string& input, const std::string& output, const std::string &sys_name);

    static RESULT generate_delete_abonent();
    static RESULT generate_insert_abonent(const size_t &db_id, const std::string &sys_name, const std::string& input, const std::string& output, const size_t &type_id);
    static RESULT generate_update_abonent();

    static RESULT generate_select_all_letters();
    static RESULT generate_select_input_letters();
    static RESULT generate_select_output_letters();
    static RESULT generate_insert_letters();
    static RESULT generate_delete_letters();
    static RESULT generate_update_letters();

    static RESULT generate_create_table();
    static RESULT generate_check_table();
private:
    DatabaseQuery()=delete;
    DatabaseQuery(DatabaseQuery&)=delete;
    ~DatabaseQuery()=delete;
};
}
#endif // DATABASEQUERY_H
