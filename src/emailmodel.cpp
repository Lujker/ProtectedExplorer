#include "emailmodel.h"

EmailModel::EmailModel(std::vector<Abonent> &abonents, QObject *parent):
    m_ref_abonents(abonents), m_status(EMPTY)
{Q_UNUSED(parent);}

void EmailModel::initModelData()
{
    setInputLetters();
}

/*!
 * \brief EmailModel::initAddressBook считываем всех абонентов из БД и синхронизируем их
 * с списком из настроек
 */
void EmailModel::initAddressBook()
{
    db::RESULT res = db::DatabaseQuery::generate_select_abonents();
    if(res.first==db::DBResult::ISOK){
        std::set<Abonent> db_abonents;
        size_t abonents_count = 0;
        setAbonentsFromRESULT(res, db_abonents);
        abonents_count = db_abonents.size();
    ///сравнение абонентов БД с настроечными
        for(auto& it : m_ref_abonents){
            auto ab_iter = db_abonents.find(it);
            if(ab_iter != db_abonents.end()){
                it.db_id = ab_iter->db_id;
                if(ab_iter->inbox_path != it.inbox_path ||
                    ab_iter->outbox_path != it.outbox_path){
                    ///update inbox and outbox path
                    db::RESULT upd_res = db::DatabaseQuery::generate_update_abonent_path(it);
                    ///обработка ошибки обновления
                }
                if(ab_iter->db_type_id != it.db_type_id){
                 ///update type_id
                    db::RESULT upd_res = db::DatabaseQuery::generate_update_abonent_type(it);
                    ///обработка ошибки обновления
                }
                db_abonents.erase(ab_iter);
            }
            else{
                it.db_id = db::DatabaseQuery::generate_select_abonent_count()+1;
                db::RESULT upd_res = db::DatabaseQuery::generate_insert_abonent(it);
                ///если не нашли такой элемент вставляем его
            }
        }
        if(db_abonents.size()>0){
            for(auto it : db_abonents){
                it.db_type_id = SettingsController::get_instanse().get_settings().cl_abonent_type.at("Неопределен");
                db::RESULT upd_res = db::DatabaseQuery::generate_update_abonent_type(it);
                ///обработка ошибки обновления
            }
            ///если остались другие абоненты в БД то изменяем их тип на неопределенный
            }
        }
    else {
        setStatus(INIT_AB_ERROR);
        return;
        ///Ошибка синхронизации с БД. Почтовая служба не работает
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
    db::RESULT res = db::DatabaseQuery::generate_select_output_letters();
    ///изначально считываем только входящие
    if(res.first==db::DBResult::ISOK){
        beginResetModel();
        setLettersFromRESULT(res,m_letters);
        endResetModel();
    }
}

void EmailModel::setInputLetters()
{
    db::RESULT res = db::DatabaseQuery::generate_select_input_letters();
    ///изначально считываем только входящие
    if(res.first==db::DBResult::ISOK){
        beginResetModel();
        setLettersFromRESULT(res,m_letters);
        endResetModel();
    }

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
    return m_letters.size();
}

int EmailModel::columnCount(const QModelIndex &parent) const
{
    return 1;
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

void EmailModel::setAbonentsFromRESULT(db::RESULT &result, std::set<Abonent> &ab_arr)
{
    while(result.second.next()){
        Abonent db_ab;
        db_ab.db_id = result.second.value(0).toInt();
        db_ab.sys_name = result.second.value(1).toString().toStdString();
        db_ab.inbox_path = result.second.value(2).toString().toStdString();
        db_ab.outbox_path = result.second.value(3).toString().toStdString();
        db_ab.db_type_id = result.second.value(4).toInt();
        qDebug()<<result.second.value(1).toString();
        ab_arr.insert(std::move_if_noexcept(db_ab));
    }
}

void EmailModel::setLettersFromRESULT(db::RESULT &result, std::vector<Letter> &let_arr)
{
    while(result.second.next()){
        Letter let;
        let.let_id = result.second.value(0).toUInt();
        let.to_id = result.second.value(1).toUInt();
        let.from_id = result.second.value(2).toUInt();
        let.let_status = result.second.value(3).toUInt();
        let.date = result.second.value(4).toInt();
        let.title = result.second.value(5).toString().toStdString();
        let.let_path = result.second.value(6).toString().toStdString();
        qDebug()<<result.second.value(0).toString();
        let_arr.push_back(std::move_if_noexcept(let));
    }
}

