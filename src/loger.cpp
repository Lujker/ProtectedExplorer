#include "loger.h"

/*!
 * \brief Loger::init создание или открытие файла логов
 * \param logFilePath путь к файлу логов
 * \return успешность инициализации
 */
bool Loger::init(const std::string &logFilePath)
{
        if(logFilePath==""||logFilePath.empty()){
            std::cout<<"Log file path is empty\n";
            return false;
        }
        if(m_out.is_open()) m_out.close();

        if(logFileIsValid(logFilePath)){
        }
        else{
            if(createLogFile(logFilePath)){
            }
            else{
                std::cout<<"Fail to create log file!\n";
                return m_isInit = false;
            }
        }
        m_out.open(logFilePath,std::ios::app);
        return m_isInit = true;
}

/*!
 * \brief Loger::getInstanse получение обьекта класса как сингелтона
 * \return статический обьект
 */
Loger &Loger::getInstanse()
{
    static Loger log;
    return log;
}

/*!
 * \brief Loger::writeMessage метод записи лог сообщения
 * \param mes сообщение для записи
 * \param type тип сообщения из перечисление типов логов
 * \return успешность записи сообщения
 */
bool Loger::writeMessage(const char *mes, LOG_TYPE type)
{
    return writeMessage(std::string(mes), type);
}

bool Loger::writeMessage(const std::string &mes, LOG_TYPE type)
{
        std::string log_msg = "";
        time_t now = time(0);
        std::string dt = "[" + std::string(ctime(&now)); ///текущая дата и время
        dt.at(dt.size()-1)=']';
        log_msg = dt + get_type(type) + ("\t") + mes;

        ///Заполенения данными
        std::lock_guard<std::mutex>  guard(m_mutex);
        m_out << log_msg << std::endl;
        return true;
}

bool Loger::isInit()
{
    return m_isInit;
}

/*!
 * \brief Loger::createLogFile создание файлов логов
 * \param logFilePath путь к лог файлу
 * \return успешность создания файла
 */
bool Loger::createLogFile(const std::string &logFilePath)
{
    std::ofstream file(logFilePath, std::ios_base::out | std::ios_base::trunc);
    if(file.is_open()){
        file.close();
        return true;
    }
    else{
        return false;
    }

    return false;
}

bool Loger::logFileIsValid(const std::string &logFilePath)
{
    std::ifstream file;
    file.open(logFilePath);
    if(!file){
        return false;
    }
    else{
        file.close();
        return true;
    }

    return false;
}

/*!
 * \brief Loger::get_type получение строки информации по типу логов
 * \param type тип из перечисления
 * \return строка для записи типа
 */
std::string Loger::get_type(const LOG_TYPE &type)
{
    std::string ret_type;
    switch (type) {
    case (LOG_TYPE::DEB):
        ret_type = "<DEBUG> ";
        break;
    case (LOG_TYPE::WAR):
        ret_type = "<WARNING> ";
        break;
    case (LOG_TYPE::CRIT):
        ret_type = "<!CRITICAL!> ";
        break;
    case (LOG_TYPE::FAT):
        ret_type = "<!!!FATAL!!!> ";
        break;
    default:
        ret_type = "<DEBUG> ";
        break;
    }
    return ret_type;
}

Loger::~Loger()
{
    if(m_out.is_open()) m_out.close();
}

Loger::Loger()
{}
