#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <string>
#include <sstream>
#include <list>
#include <vector>

#include <QObject>
#include <QFile>
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
    \brief Структура хранящая основные настройки приложения
*/
struct Settings{
    std::string sub_list_dirs;
    std::string log_file_path;
    std::string dir_list;
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

    Q_PROPERTY(QString sub_list_dirs READ sub_list_dirs WRITE set_sub_list_dirs NOTIFY sub_list_dirs_change)
    Q_PROPERTY(QString dir_list READ dir_list WRITE set_dir_list NOTIFY dir_list_change)

public:
    static SettingsController &get_instanse();
    static void parse_args(const QCoreApplication &app);
    void save_settings(std::string path_to_save_file = std::string());
    void read_settings();
    bool is_init();
    const Settings &get_settings();

    ///PROPERTY_GET
    QString sub_list_dirs();
    QString log_file_path();
    QString dir_list();
    std::list<std::string> parse_dir_list();
    std::list<std::string> parse_sub_list();

    ///PROPERTY_SET
public slots:
    void set_sub_list_dirs(const QString &name);
    void set_log_file_path(const QString& path);
    void set_dir_list(const QString& dir_list);
    void save_app_settings();
    ///PROPERTY_NOTIFY
signals:
    void sub_list_dirs_change(const QString &name);
    void dir_list_change(const QString &dir_list);
    void log_file_path_change(const QString &path);

private:
    explicit SettingsController(QObject* parent = nullptr);
    SettingsController(SettingsController&)=delete;
    SettingsController(SettingsController&&)=delete;
    SettingsController& operator=(SettingsController&)=delete;
    virtual ~SettingsController();

    ///структура для свойств настроек
    Settings m_set;
    ///путь к файлу настроек
    std::string m_path_to_file;
    bool is_init_arg;
};



#endif // SETTINGSCONTROLLER_H
