#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <utility>
#include <iostream>
#include <string>

#include "databasequery.h"

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QDir>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QCommandLineParser>

#define SET_PATH "../settings/set.xml"
#define RESERV_SET_PATH "./settings/set.xml"
#define SET_DIR "../settings"
#define PATH_SEPARATOR ';' ///Разделитель для списка путей

/*!
 * \brief The Abonent struct описывает структуру абонента в адресной книге
 */
struct Abonent
{
    Abonent(): db_type_id(0), db_id(0)
    {}
    std::string sys_name;
    std::string outbox_path;
    std::string inbox_path;
    std::string icon_path;
    size_t      db_type_id;
    size_t      db_id;

    operator std::string();

    bool operator==(const Abonent& ab) const;
    bool operator!=(const Abonent& ab) const;
    bool operator<(const Abonent& ab) const;
};

/*!
 * \brief The Letter struct описывает структуру одного сообщения
 */
struct Letter{
    Letter(): let_id(0), let_status(0)
    {}
    size_t              let_id;         ///id сообщения в БД
    size_t              let_dir_type;   ///Тип сообщения (входя/исх/...)
    size_t              let_status;     ///id статуса сообщения
    std::string         let_path;       ///путь к контейнеру письма
    std::string         title;          ///название сообщения
    std::string         date;           ///дата получения/отправки
    size_t              to_id;
    size_t              from_id;
    size_t              attach_count;

    operator std::string();

    bool operator==(const Letter& let) const;
    bool operator!=(const Letter& let) const;
    bool operator<(const Letter& let) const;
};

/*!
    \brief Структура хранящая основные настройки приложения
*/
struct Settings{
    std::vector<std::pair<std::string,std::string>> shared_list;
    std::vector<std::pair<std::string,std::string>> dir_list;
    std::map<QString, size_t> cl_status;
    std::map<QString, size_t> cl_abonent_type;
    std::vector<Abonent> abonents;
    std::string log_file_path;
    bool    helps;
};

///функции получающии из одной строки вектор строк по разделителю
std::vector<std::string> split (const std::string&, std::string);
std::vector<std::string> split (const std::string&, char);


/*!
    \brief Класс доступа к настройкам
    \author Zelenskiy V.P.
    \version 1.0
    \date 01.06.2021
    \warning Класс не допускает наследование и создание на стеке или хипе вручную

    \details Класс предназначен для получения настреок считанных из файла настроек
    Всю информацию полученные из XML файла настроек прилоедние читает в структуру settings.
    Перед использывание необходимо указать файл через аргумент командной строки и вызвать метод
    парсинга значений из командной строки parse_args, если аргуменд не указан то
    брать стандартные пути SET_PATH и SET_DIR
*/


class SettingsController: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool helps READ helps WRITE setHelps NOTIFY helpsChanged)
    static SettingsController &get_instanse();
    static void parse_args(const QCoreApplication &app);
    void save_settings(std::string path_to_save_file = std::string());
    void read_settings();
    bool is_init();
    const Settings &get_settings();
    void set_cl();
    bool helps();
    void setHelps(bool helps);

    ///PROPERTY_GET
    std::vector<std::pair<std::string,std::string>> sub_list_dirs();
    QString log_file_path();
    std::vector<std::pair<std::string,std::string>> dir_list();
    std::vector<Abonent> &abonents();
    ///PROPERTY_SET
public slots:
    void set_log_file_path(const QString& path);
    void save_app_settings();
    ///PROPERTY_NOTIFY
signals:
    void sub_list_dirs_change(const QString &name);
    void dir_list_change(const QString &dir_list);
    void log_file_path_change(const QString &path);
    void helpsChanged(bool helps);

private:
    explicit SettingsController(QObject* parent = nullptr);
    SettingsController(SettingsController&)=delete;
    SettingsController(SettingsController&&)=delete;
    SettingsController& operator=(SettingsController&)=delete;
    ~SettingsController() = default;

    std::string checkFolder(const std::string &absPath);
    std::string checkIcon(const QString &iconPath);

    ///структура для свойств настроек
    Settings m_set;
    ///путь к файлу настроек
    std::string m_path_to_file;
    bool is_init_arg;
};



#endif // SETTINGSCONTROLLER_H
