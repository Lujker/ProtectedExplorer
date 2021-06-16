#include "emailmodel.h"

EmailModel::EmailModel(const std::vector<Abonent> &abonents, QObject *parent):
    m_ref_abonents(abonents), m_status(EMPTY)
{}

void EmailModel::initModelData()
{
    db::RESULT res = db::DatabaseQuery::generate_select_input_letters();
    ///изначально считываем только входящие
}

/*!
 * \brief EmailModel::initAddressBook считываем всех абонентов из БД и синхронизируем их
 * с списком из настроек
 */
void EmailModel::initAddressBook()
{
    db::RESULT res = db::DatabaseQuery::generate_select_abonents();
    std::set<Abonent> db_abonents;
    size_t abonents_count = 0;
    if(res.first==db::DBResult::ISOK){
        while(res.second.next()){
            Abonent db_ab;
            db_ab.db_id = res.second.value(0).toInt();
            db_ab.sys_name = res.second.value(1).toString().toStdString();
            db_ab.inbox_path = res.second.value(2).toString().toStdString();
            db_ab.outbox_path = res.second.value(3).toString().toStdString();
            db_ab.db_type_id = res.second.value(4).toInt();
            qDebug()<<res.second.value(1).toString();
            db_abonents.insert(std::move_if_noexcept(db_ab));
        }
        abonents_count = db_abonents.size();
    }
    else {
        setStatus(INIT_AB_ERROR);
        return;
        ///Ошибка синхронизации с БД. Почтовая служба не работает
    }
    ///сравнение абонентов БД с настроечными
    for( const auto& it : m_ref_abonents){
        auto ab_iter = db_abonents.find(it);
        if(ab_iter != db_abonents.end()){
            if(ab_iter->inbox_path != it.inbox_path ||
                ab_iter->outbox_path != it.outbox_path){
                ///update inbox and outbox path
                db::RESULT upd_res = db::DatabaseQuery::generate_update_abonent_path(
                            it.inbox_path, it.outbox_path, it.sys_name);
                ///обработка ошибки обновления
            }
            if(ab_iter->db_type_id != it.db_type_id){
             ///update type_id
                db::RESULT upd_res = db::DatabaseQuery::generate_update_abonent_type(
                            it.db_type_id, it.sys_name);
                ///обработка ошибки обновления
            }
            db_abonents.erase(ab_iter);
        }
        else{
            db::RESULT upd_res = db::DatabaseQuery::generate_insert_abonent(++abonents_count,
                        it.sys_name, it.inbox_path, it.outbox_path, it.db_type_id);
            ///если не нашли такой элемент вставляем его
        }
    }
    if(db_abonents.size()>0){
        for(const auto& it : db_abonents){
            db::RESULT upd_res = db::DatabaseQuery::generate_update_abonent_type(
                        AbonentTypes::UNDEFINED, it.sys_name);
            ///обработка ошибки обновления
        }
        ///если остались другие абоненты в БД то изменяем их тип на неопределенный
    }
    setStatus(IS_OK);
}

int EmailModel::status() const
{
    return m_status;
}

void EmailModel::setStatus(int status)
{
    m_status = status;
}

void EmailModel::setOutputLetters()
{

}

void EmailModel::setInputLetters()
{
    db::RESULT res = db::DatabaseQuery::generate_select_input_letters();

}

QModelIndex EmailModel::index(int row, int column, const QModelIndex &parent) const
{
       Q_UNUSED(column)
       if (!parent.isValid())
                   return createIndex(row, 0, static_cast<quintptr>(0));

       return createIndex(row, 0, parent.row() + 1);
}

QModelIndex EmailModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalId() == 0)
                   return QModelIndex();

       return createIndex(child.internalId() - 1, 0, static_cast<quintptr>(0));
}

int EmailModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int EmailModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant EmailModel::data(const QModelIndex &index, int role) const
{
    return 0;
}

QHash<int, QByteArray> EmailModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    return roles;
}

