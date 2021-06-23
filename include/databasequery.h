#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

///Incl moduls
#include "databaseaccessor.h"
#include "settingscontroller.h"

///Qt libs
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>

struct Abonent;
struct Letter;

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
    ///Classificator reading
    static bool generate_select_cl_status(std::map<std::string, size_t> &cl_status);
    static bool generate_select_cl_abonent_type(std::map<std::string, size_t>& cl_abonent_type);

    ///Abonents_table
    static RESULT generate_select_abonents();
    static RESULT generate_select_abonent_input_dir();
    static RESULT generate_select_abonent_output_dir();
    static unsigned int generate_select_abonent_count();

    static RESULT generate_update_abonent_type(const Abonent& abonent);
    static RESULT generate_update_abonent_path(const Abonent& abonent);
    static RESULT generate_delete_abonent(const Abonent& abonent);
    static RESULT generate_insert_abonent(const Abonent& abonent);
    static RESULT generate_update_abonent(const Abonent& abonent);

    ///Letters table
    static RESULT generate_select_all_letters();
    static RESULT generate_select_input_letters();
    static RESULT generate_select_output_letters();
    static unsigned int generate_select_letters_count();

    static RESULT generate_insert_letters(Letter& letter);
    static RESULT generate_delete_letters(Letter& letter);
    static RESULT generate_update_letters(const Letter& letter);

    ///Creating and check func
    static RESULT generate_create_table();
    static RESULT generate_check_table();
private:
    DatabaseQuery()=delete;
    DatabaseQuery(DatabaseQuery&)=delete;
    ~DatabaseQuery()=delete;
};
}
#endif // DATABASEQUERY_H
