#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

///Incl moduls
#include "databaseaccessor.h"

//Qt libs
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>

//C and C++ libs
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
    static RESULT generate_delete_ls_vf_id(int vf_id, int ls_cat);
    static RESULT generate_insert_ls_vf(int vf_id, int ls_cat, int count_st, int count_real, int count_list, int count_sick, int count_dead);
    static RESULT generate_delete_vvt_id(int vf_id, std::string ls_cat);
    static RESULT generate_insert_vvt(int vf_id, std::string code_pr, int staff_pr, int presence_pr);

    static RESULT generate_create_table();
    static RESULT generate_check_table();
private:
    DatabaseQuery()=delete;
    DatabaseQuery(DatabaseQuery&)=delete;
    ~DatabaseQuery()=delete;
};
}
#endif // DATABASEQUERY_H
