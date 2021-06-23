#include "emailmodel.h"

EmailModel::EmailModel(std::vector<Abonent> &abonents, QObject *parent):
    QAbstractListModel(parent),
    m_ref_abonents(abonents),
    m_inbox_watchers(nullptr),
    m_outbox_watchers(nullptr),
    m_status(EMPTY),
    m_model_type(UNKNOWN)
{}

EmailModel::~EmailModel()
{
    try {
        if(m_inbox_watchers!=nullptr){
            delete m_inbox_watchers;
            m_inbox_watchers = nullptr;
        }
        if(m_outbox_watchers!=nullptr){
            delete m_outbox_watchers;
            m_outbox_watchers = nullptr;
        }
    }
    catch(std::exception& exep){
        qDebug()<<exep.what();
    }
    catch (...) {

    }

}

void EmailModel::initModelData()
{
    setInputLetters(letters());
    /// А теперь сравнить с тем, что действительно есть сейчас в дерикториях
    /// И внести изменения в БД
    /// Для этого смотрим в таблице let_option все пути и свеояем с тем что находится в дерикториях
    /// если такого еще нет то вставляем новое входящее
    /// можно придумать универсальную функцию проверки в дериктории нового контейнера и вставки его в БД
    /// При этом можно его сравнивать с теми что уже есть в БД
    if(getModel_type()!=FAIL)
        for(auto const &it : m_ref_abonents){
            addNewLettersFromDir(letters(),
                                 QString::fromStdString(
                                     it.inbox_path));
        }
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
        setAbonentsFromRESULT(res, db_abonents);
    ///сравнение абонентов БД с настроечными
        for(auto& it : ref_abonents()){
            auto ab_iter = db_abonents.find(it);
            if(ab_iter != db_abonents.end()){
                it.db_id = ab_iter->db_id;
                if(ab_iter->inbox_path != it.inbox_path ||
                    ab_iter->outbox_path != it.outbox_path){
                    ///update inbox and outbox path
                    db::RESULT upd_res =
                            db::DatabaseQuery::
                            generate_update_abonent_path(it);
                    ///обработка ошибки обновления
                    if(upd_res.first!=db::DBResult::ISOK){
                        qDebug()<<"update outbox, inbox "
                                    + QString::fromStdString(it)
                                    + " is failed!";
                    }
                }
                if(ab_iter->db_type_id != it.db_type_id){
                 ///update type_id
                    db::RESULT upd_res = db::DatabaseQuery::
                            generate_update_abonent_type(it);
                    ///обработка ошибки обновления
                    if(upd_res.first!=db::DBResult::ISOK){
                        qDebug()<<"update type id"
                                    + QString::fromStdString(it)
                                    + " is failed!";
                    }
                }
                db_abonents.erase(ab_iter);
            }
            else{
                it.db_id = db::DatabaseQuery::
                        generate_select_abonent_count()+1;
                db::RESULT upd_res = db::DatabaseQuery::
                        generate_insert_abonent(it);
                ///если не нашли такой элемент вставляем его
                if(upd_res.first!=db::DBResult::ISOK){
                    qDebug()<<"insert "
                                + QString::fromStdString(it)
                                + " is failed!";
                }
            }
        }
        if(db_abonents.size()>0){
            for(auto it : db_abonents){
                try {
                    std::string type_name = "Неопределен";
                    it.db_type_id = SettingsController::
                            get_instanse().get_settings().
                            cl_abonent_type.at(type_name);

                }  catch (const std::exception& exp) {
                    qDebug()<<"Uncnown type"<<exp.what();
                    it.db_type_id=0;
                }
                db::RESULT upd_res = db::DatabaseQuery::
                        generate_update_abonent_type(it);
                ///обработка ошибки обновления
                if(upd_res.first!=db::DBResult::ISOK){
                    qDebug()<<"update abonent type "
                                + QString::fromStdString(it)
                                + " is failed!";
                }
            }
            ///если остались другие абоненты в БД то изменяем их тип на неопределенный
            }
        ///Включение слежения за дерикториями входящих и исходящих
            initWatchers();
        }
    else {
        setStatus(INIT_AB_ERROR);
        return;
        ///Ошибка синхронизации с БД. Почтовая служба не работает
    }
    setStatus(IS_OK);
}

void EmailModel::initWatchers()
{
    try {
        if(m_inbox_watchers!=nullptr){
            delete m_inbox_watchers;
            m_inbox_watchers = nullptr;
        }
        if(m_outbox_watchers!=nullptr){
            delete m_outbox_watchers;
            m_outbox_watchers = nullptr;
        }
    }
    catch(std::exception& exep){
        qDebug()<<exep.what();
        setStatus(STATUS::INIT_FSW_ERROR);
        return;
    }
    catch (...) {
        qDebug()<<"unknown exeption";
        setStatus(STATUS::INIT_FSW_ERROR);
        return;
    }

    m_inbox_watchers = new QFileSystemWatcher;
    m_outbox_watchers = new QFileSystemWatcher;

    for(const auto &it : ref_abonents()){
        m_inbox_watchers->
                addPath(QString::fromStdString(it.inbox_path));
        m_outbox_watchers->
                addPath(QString::fromStdString(it.outbox_path));
    }
    connect(m_inbox_watchers, SIGNAL(directoryChanged(QString)),
            this, SLOT(getNewInMessage(QString)));
    connect(m_outbox_watchers, SIGNAL(directoryChanged(QString)),
            this, SLOT(getNewOutMessage(QString)));
}

enum EmailModel::STATUS EmailModel::status() const
{
    return m_status;
}

void EmailModel::setOutputList()
{
    setOutputLetters(letters());
}

void EmailModel::setInputList()
{
    setInputLetters(letters());
}

void EmailModel::setStatus(enum STATUS status)
{
    m_status = status;
}

void EmailModel::setOutputLetters(std::vector<Letter> &letters)
{
    db::RESULT res = db::DatabaseQuery::generate_select_output_letters();
    ///изначально считываем только входящие
    if(res.first==db::DBResult::ISOK){
        beginResetModel();
        letters.clear();
        setLettersFromRESULT(res, letters);
        endResetModel();
        setModel_type(OUTBOX);
    }
    else {
        setModel_type(FAIL);
    }
}

void EmailModel::setInputLetters(std::vector<Letter> &letters)
{
    db::RESULT res = db::DatabaseQuery::generate_select_input_letters();
    ///изначально считываем только входящие
    if(res.first==db::DBResult::ISOK){
        beginResetModel();
        letters.clear();
        setLettersFromRESULT(res, letters);
        endResetModel();
        setModel_type(INBOX);
    }
    else {
        setModel_type(FAIL);
    }
}

void EmailModel::update()
{

}

void EmailModel::updateAbonents()
{

}

void EmailModel::getNewInMessage(QString path)
{

}

void EmailModel::getNewOutMessage(QString path)
{

}

void EmailModel::sendMessage()
{

}

void EmailModel::getAttacmentsList(const int index)
{

}

void EmailModel::getAttacments(const int mes_index, const int dir_index)
{

}

void EmailModel::deleteMessage(const int index)
{

}

QModelIndex EmailModel::index(int row,
                              int column,
                              const QModelIndex &parent) const
{
       Q_UNUSED(column)
       if (!parent.isValid())
                   return createIndex(
                               row,
                               0,
                               static_cast<quintptr>(0));

       return createIndex(row, 0, parent.row() + 1);
}

QModelIndex EmailModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalId() == 0)
                   return QModelIndex();

       return createIndex(
                   child.internalId() - 1,
                   0,
                   static_cast<quintptr>(0));
}

int EmailModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_letters.size();
}

int EmailModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant EmailModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < m_letters.size() && index.row()>= 0){
        auto finder = std::find_if(
                    m_ref_abonents.begin(),
                    m_ref_abonents.end() ,
                    [&](const Abonent& it){
                    return it.db_id==m_letters.at(index.row()).from_id;
                    }
        );
        switch (role) {
        case TITLE:
            return QString::fromStdString(
                        m_letters.at(index.row()).title);
            break;
        case DATE:
            return QString::fromStdString(
                        m_letters.at(index.row()).date);
            break;
        case ATACH_COUNT:
            return m_letters.at(index.row()).attach_count;
            break;
        case ICON:
            if(finder!=m_ref_abonents.end())
                return QString::fromStdString(
                            finder->icon_path);
            else return ":/../icons/contacts.png";
            break;
        case SENDER:
            if(finder!=m_ref_abonents.end())
                return QString::fromStdString(
                            finder->sys_name);
            else{
                std::string str = "Неизвестный абонент";
                return QString::fromStdString(str);
            }
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> EmailModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(TITLE,         QByteArray("title"));
    roles.insert(STATUS,        QByteArray("status"));
    roles.insert(DATE,          QByteArray("date"));
    roles.insert(ICON,          QByteArray("icon"));
    roles.insert(ATACH_COUNT,   QByteArray("atach_count"));
    roles.insert(SENDER,        QByteArray("sender"));
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
        qDebug()<< QString::fromStdString(db_ab);
        ab_arr.insert(std::move_if_noexcept(db_ab));
    }
}

void EmailModel::setLettersFromRESULT(db::RESULT &result, std::vector<Letter> &let_arr)
{
    while(result.second.next()){
        Letter let;
        let.let_id = result.second.value(0).toUInt();
        let.let_dir_type = result.second.value(1).toUInt();
        let.to_id = result.second.value(2).toUInt();
        let.from_id = result.second.value(3).toUInt();
        let.let_status = result.second.value(4).toUInt();
        let.date = result.second.value(5).toString().toStdString();
        let.title = result.second.value(6).toString().toStdString();
        let.let_path = result.second.value(7).toString().toStdString();
        let.attach_count = result.second.value(8).toUInt();
        let_arr.push_back(std::move_if_noexcept(let));
        qDebug()<< let.from_id <<QString::fromStdString(let_arr.at(let_arr.size()-1));
    }
}

void EmailModel::addNewLettersFromDir(std::vector<Letter> &let_arr, const QString &path)
{
    std::vector<Letter> lettersFromDir;
    ///Определить функцию получения информации из контейнера
    makeLettersFromDir(lettersFromDir, path);
    for(const auto &it: lettersFromDir){
        auto finder = std::find_if(
                    let_arr.begin(),
                    let_arr.end() ,
                    [&](const Letter& let){
                        return let.let_path==it.let_path;
                    }
        );
        if(finder==let_arr.end()){
            auto res = db::DatabaseQuery::generate_insert_letters(*finder);
            if(res.first==db::DBResult::ISOK){
                let_arr.push_back(std::move(*finder));
            }
            else{
                qDebug()<<"Fail with insert mess: " << QString::fromStdString(*finder);
            }
        }
    }
}

void EmailModel::makeLettersFromDir(std::vector<Letter> &empty_let_arr, const QString &path)
{

}

EmailModel::MODEL_TYPE EmailModel::getModel_type() const
{
    return m_model_type;
}

void EmailModel::setModel_type(MODEL_TYPE model_type)
{
    m_model_type = model_type;
}

std::vector<Letter>& EmailModel::letters()
{
    return m_letters;
}

void EmailModel::setLetters(const std::vector<Letter> &letters)
{
    m_letters = letters;
}

std::vector<Abonent> &EmailModel::ref_abonents() const
{
    return m_ref_abonents;
}

void EmailModel::setRef_abonents(const std::vector<Abonent> &ref_abonents)
{
    m_ref_abonents = ref_abonents;
}


AbonentModel::AbonentModel(std::vector<Abonent> &abonents, QObject *parent):
    QAbstractListModel(parent), m_ref_abonents(abonents)
{}

AbonentModel::~AbonentModel()
{}

void AbonentModel::Init()
{

}

QModelIndex AbonentModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column)
    if (!parent.isValid())
                return createIndex(row, 0, static_cast<quintptr>(0));

    return createIndex(row, 0, parent.row() + 1);
}

QModelIndex AbonentModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalId() == 0)
                   return QModelIndex();

    return createIndex(child.internalId() - 1, 0, static_cast<quintptr>(0));
}

int AbonentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_ref_abonents.size();
}

int AbonentModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant AbonentModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < m_ref_abonents.size() && index.row()>= 0)
        switch (role) {
        case NAME:
            return QString::fromStdString(
                        m_ref_abonents.at(index.row()).sys_name);
            break;
        case ICON:
            return QString::fromStdString(
                        m_ref_abonents.at(index.row()).icon_path);
            break;
        case FROM:
            return QString::fromStdString(
                        m_ref_abonents.at(index.row()).inbox_path);
            break;
        case TO:
            return QString::fromStdString(
                        m_ref_abonents.at(index.row()).outbox_path);
            break;
        }
    return QVariant();
}

QHash<int, QByteArray> AbonentModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ICON,  QByteArray("icon"));
    roles.insert(NAME,  QByteArray("name"));
    roles.insert(FROM,  QByteArray("from"));
    roles.insert(TO,    QByteArray("to"));
    return roles;
}

std::vector<Abonent> &AbonentModel::ref_abonents() const
{
    return m_ref_abonents;
}

void AbonentModel::setRef_abonents(const std::vector<Abonent> &ref_abonents)
{
    m_ref_abonents = ref_abonents;
}
