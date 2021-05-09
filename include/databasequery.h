#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

///Incl moduls
#include "databaseaccessor.h"
//Qt libs
#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
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
/*!
  \brief Класс DatabaseQuery содержит набор статичных методов для обращения к подключенной базе данных
  Объект данного класса невозможно создать т.к. он не несет информации а служит лишь для выполнения запросов
  Данный класс находится в namespace db и при необходимости может быть расширен.

  Перечисление DBResult сообщает о состоянни запроса после его выполнения.
  Если во время выполнения запроса не возникло ошибок в паре RESULT первый аргумент будет = ISOK.

 *  \author Z.V.P
 *  \version 1.1
 *  \date 04.02.2021

 * */
namespace db {

enum class DBResult{
    ISOK=30,FAIL=31
};

class DatabaseQuery
{
public:
    using RESULT = std::pair<DBResult,QSqlQuery>; ///возвращаем результат в виде информации об успешности и запрос для получения информации

    ///Create and del tables functions
    static RESULT generate_create_files_table();
    static RESULT generate_crete_file_options_table();
    static RESULT generate_drop_file_table();
    static RESULT generate_drop_file_options_table();

    ///Edit db functions
    static RESULT generate_add_file(QFile& file, bool isSub = false);
    static RESULT generate_add_dir(QDir& dir, bool isSub = false);
    static RESULT generate_delete_file(std::string& path);
    static RESULT generate_find_file(std::string& path);
    static RESULT generate_copy_file_to(std::string& path, std::string& dest_path);
    static RESULT generate_move_file_to(std::string& path, std::string& dest_path);

    ///Select db finctions
    static RESULT generate_select_all_files(std::string& work_dir);
    static RESULT generate_select_all_sub_files(std::string& work_dir);


    ///Exec query functions
    static RESULT exec_request(QSqlQuery qu);
    static RESULT exec_request(const QString& qu);

private:
    DatabaseQuery()=delete;
    DatabaseQuery(DatabaseQuery&)=delete;
    ~DatabaseQuery()=delete;
};
}
#endif // DATABASEQUERY_H
