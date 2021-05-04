#ifndef SERVSETTINGS_H
#define SERVSETTINGS_H
#include <string>
#include <list>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

#define SET_PATH "../settings/set.xml"
#define SET_DIR "../settings"


/*!
    \brief Струкртура с параметрами настроек
*/
struct Settings{
    std::string save_dump_dir;
    std::string log_file_path;
    std::string dir_list;
};

/*!
    \brief Класс доступа к настройкам
    \author Zelenskiy V.P.
    \version 1.0
    \date Март 2021 года
    \warning Данный класс не создается, а используется через интстанс

    Класс для чтения и записи настройк программы, разработан по шаблону singleton
    и не должен создаваться в программе.
    Перед использованием необхоидмо вызвать метод read_settings или parse_args
    для считывания настроек.
    Параметры настреок могут передаваться в qml если оперделить обьект инстанса в контекст qml.

*/
class ServSettings: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString save_dump_dir READ save_dump_dir WRITE set_save_dump_dir NOTIFY save_dump_dir_change)

public:
    static ServSettings &get_instanse();
    static void parse_args(int argc, char *argv[]);
    void save_settings(std::string path_to_save_file = std::string());
    void read_settings();
    bool is_init();
    const Settings &get_settings();

    ///PROPERTY_GET
    QString save_dump_dir();
    QString log_file_path();
    QString dir_list();
    std::list<std::string> parse_dump_list();

    ///PROPERTY_SET
public slots:
    void set_save_dump_dir(const QString &name);
    void set_log_file_path(const QString& path);
    void set_dir_list(const QString& dir_list);
    void save_app_settings();
    ///PROPERTY_NOTIFY
signals:
    void save_dump_dir_change(const QString &name);
    void dir_list_change(const QString &dir_list);
    void log_file_path_change(const QString &path);

private:
    explicit ServSettings(QObject* parent = nullptr);
    ServSettings(ServSettings&)=delete;
    ServSettings(ServSettings&&)=delete;
    ServSettings& operator=(ServSettings&)=delete;
    virtual ~ServSettings();

    Settings m_set;
    std::string m_path_to_file;
    bool is_init_arg;
};

#endif // SERVSETTINGS_H
