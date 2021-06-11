#include "databasequery.h"

namespace db {
/*!
 * \brief DatabaseQuery::generate_delete_ls_vf_id генерация и выполенение запроса на удаление объекта ЛС из таблицы БД
 * \param vf_id индификатор формирования
 * \param ls_cat категория личного состава
 * \return результат выполнения запроса в виде QSqlQuery
 */
RESULT DatabaseQuery::generate_delete_ls_vf_id(int vf_id, int ls_cat)
{
#ifdef SQLPREPARE
     QSqlQuery qu; qu.prepare("DELETE FROM \"public\"." +
                              db::DatabaseAccessor::ls_vf_NamedTable +
                              " WHERE vf_id=\'" + reinterpret_vf_id(vf_id) + "\' AND ls_cat_id=" + QString::number(reinterpret_cat_id(ls_cat)));
         return exec_request(qu);

#else
     QString str = "DELETE FROM \"public\"."+
             QString(" WHERE vf_id=\'") + (vf_id) +
             QString("\' AND ls_cat_id=") +
             QString::number((ls_cat));

     return DatabaseAccessor::getInstanse()->executeQuery(str);
#endif
}

/*!
 * \brief DatabaseQuery::generate_insert_ls_vf генерация и выполнение запроса на вставку объекта ЛС в БД
 * \param vf_id инидфикатор формирования
 * \param ls_cat категория личного состава (офицеры, прапорщики, ...)
 * \param count_st количество по факту
 * \param count_real количество на лично
 * \param count_list количество по списку
 * \param count_sick количество больных
 * \param count_dead количество умерших
 * \return результат запроса
 */
RESULT DatabaseQuery::generate_insert_ls_vf(int vf_id, int ls_cat,
                                                           int count_st, int count_real,
                                                           int count_list, int count_sick,
                                                           int count_dead)
{
#ifdef SQLPREPARE
    QSqlQuery qu;
    qu.prepare(QString::fromStdString("INSERT INTO \"public\"." + db::DatabaseAccessor::ls_vf_NamedTable.toStdString() + " (vf_id,ls_cat_id,count_st,count_real)"
                           " VALUES (:vf_id ,:ls_cat,:count_st,:count_real)"));
    ls_cat = reinterpret_cat_id(ls_cat);
    if(ls_cat == 0) return std::make_pair(db::DBResult::FAIL,qu);
    if(reinterpret_cat_id(ls_cat)==0)return std::make_pair(db::DBResult::FAIL,qu);


//    qDebug()<<reinterpret_vf_id(vf_id)<<" "<< ls_cat<< " "<< count_st<< " "<< count_real;
    qu.bindValue(":vf_id", reinterpret_vf_id(vf_id));
    qu.bindValue(":ls_cat",reinterpret_cat_id(ls_cat));
    qu.bindValue(":count_st",count_st);
    qu.bindValue(":count_real",count_real);

    return exec_request(qu);
#else
    QString str = "INSERT INTO \"public\"." +
            QString(" (vf_id, ls_cat_id, count_st, count_real, count_list, count_sick, count_dead)") +
            QString(" VALUES (\'") + (vf_id) + "\'," +
            QString::number((ls_cat)) + ","  + QString::number(count_st)+ "," +
            QString::number(count_real) + "," + QString::number(count_list) + "," +
            QString::number(count_sick) + "," +  QString::number(count_dead) + ")";

    return DatabaseAccessor::getInstanse()->executeQuery(str);
#endif
}

/*!
 * \brief DatabaseQuery::generate_delete_vvt_id удаление обекта ВВТ из БД
 * \param vf_id инидфикатор формирования
 * \param ls_cat категория ВВТ формирования
 * \return результат запроса
 */
RESULT DatabaseQuery::generate_delete_vvt_id(int vf_id, std::string ls_cat)
{
    QTextCodec *codec = QTextCodec::codecForName("KOI8-R"); ///из ЕСУТЗ приходит в KOI8-R
    QByteArray b_arr = QByteArray::fromStdString(ls_cat);
    QString n_s = codec->toUnicode(b_arr);
#ifdef SQLPREPARE
    QSqlQuery qu; qu.prepare("DELETE FROM \"public\"." +
                             db::DatabaseAccessor::vvt_NamedTable +
                             " WHERE vf_id=\'" + reinterpret_vf_id(vf_id) + "\' AND vvt_type_id=\'" + n_s + "\'");
    return exec_request(qu);
#else

    QString str = "DELETE FROM \"public\"." +
            QString(" WHERE vf_id=\'") + (vf_id) +
            QString("\' AND vvt_type_id=\'") + n_s + "\'";

    return DatabaseAccessor::getInstanse()->executeQuery(str);
#endif
}

/*!
 * \brief DatabaseQuery::generate_insert_vvt запрос на вставку нового объекта ВВТ в БД
 * \param vf_id индификатор формирования ВВТ
 * \param code_pr код подразделения
 * \param staff_pr количество по списку
 * \param presence_pr колчество в наличии
 * \return результат запроса
 */
RESULT DatabaseQuery::generate_insert_vvt(int vf_id, std::string code_pr, int staff_pr, int presence_pr)
{
#ifdef SQLPREPARE
    QSqlQuery qu;
    qu.prepare(QString::fromStdString("INSERT INTO \"public\"."+ db::DatabaseAccessor::vvt_NamedTable.toStdString() +" (vf_id, vvt_type_id, count_st, count_own)"
                           " VALUES (\':vf_id\' , \':vvt_type_id\', :staff_pr ,:count_own)"));

    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    QByteArray b_arr = QByteArray::fromStdString(code_pr);
    QString n_s = codec->toUnicode(b_arr);

    qu.bindValue(":vf_id", reinterpret_vf_id(vf_id));
    qu.bindValue(":vvt_type_id", n_s);
    qu.bindValue(":staff_pr",staff_pr);
    qu.bindValue(":count_own",presence_pr);

    return exec_request(qu);
#else
    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    QByteArray b_arr = QByteArray::fromStdString(code_pr);
    QString n_s = codec->toUnicode(b_arr);

    QString str = "INSERT INTO \"public\"." +
            QString(" (vf_id, vvt_type_id, count_st, count_own)")+
            QString(" VALUES (\'") + (vf_id) + "\',\'" + n_s + "\'," +
            QString::number(staff_pr)+ "," + QString::number(presence_pr) + ")";

    return DatabaseAccessor::getInstanse()->executeQuery(str);
#endif
}


}
