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
#define SET_DIR "../settings"
#define PATH_SEPARATOR ';' ///Разделитель для списка путей


/*!
    \brief Р РЋРЎвЂљРЎР‚РЎС“Р С”РЎР‚РЎвЂљРЎС“РЎР‚Р В° РЎРѓ Р С—Р В°РЎР‚Р В°Р СР ВµРЎвЂљРЎР‚Р В°Р СР С‘ Р Р…Р В°РЎРѓРЎвЂљРЎР‚Р С•Р ВµР С”
*/
struct Settings{
    std::string sub_list_dirs;
    std::string log_file_path;
    std::string dir_list;
};


std::vector<std::string> split (const std::string&, std::string);
std::vector<std::string> split (const std::string&, char);


/*!
    \brief Р С™Р В»Р В°РЎРѓРЎРѓ Р Т‘Р С•РЎРѓРЎвЂљРЎС“Р С—Р В° Р С” Р Р…Р В°РЎРѓРЎвЂљРЎР‚Р С•Р в„–Р С”Р В°Р С
    \author Zelenskiy V.P.
    \version 1.0
    \date Р СљР В°РЎР‚РЎвЂљ 2021 Р С–Р С•Р Т‘Р В°
    \warning Р вЂќР В°Р Р…Р Р…РЎвЂ№Р в„– Р С”Р В»Р В°РЎРѓРЎРѓ Р Р…Р Вµ РЎРѓР С•Р В·Р Т‘Р В°Р ВµРЎвЂљРЎРѓРЎРЏ, Р В° Р С‘РЎРѓР С—Р С•Р В»РЎРЉР В·РЎС“Р ВµРЎвЂљРЎРѓРЎРЏ РЎвЂЎР ВµРЎР‚Р ВµР В· Р С‘Р Р…РЎвЂљРЎРѓРЎвЂљР В°Р Р…РЎРѓ

    Р С™Р В»Р В°РЎРѓРЎРѓ Р Т‘Р В»РЎРЏ РЎвЂЎРЎвЂљР ВµР Р…Р С‘РЎРЏ Р С‘ Р В·Р В°Р С—Р С‘РЎРѓР С‘ Р Р…Р В°РЎРѓРЎвЂљРЎР‚Р С•Р в„–Р С” Р С—РЎР‚Р С•Р С–РЎР‚Р В°Р СР СРЎвЂ№, РЎР‚Р В°Р В·РЎР‚Р В°Р В±Р С•РЎвЂљР В°Р Р… Р С—Р С• РЎв‚¬Р В°Р В±Р В»Р С•Р Р…РЎС“ singleton
    Р С‘ Р Р…Р Вµ Р Т‘Р С•Р В»Р В¶Р ВµР Р… РЎРѓР С•Р В·Р Т‘Р В°Р Р†Р В°РЎвЂљРЎРЉРЎРѓРЎРЏ Р Р† Р С—РЎР‚Р С•Р С–РЎР‚Р В°Р СР СР Вµ.
    Р СџР ВµРЎР‚Р ВµР Т‘ Р С‘РЎРѓР С—Р С•Р В»РЎРЉР В·Р С•Р Р†Р В°Р Р…Р С‘Р ВµР С Р Р…Р ВµР С•Р В±РЎвЂ¦Р С•Р С‘Р Т‘Р СР С• Р Р†РЎвЂ№Р В·Р Р†Р В°РЎвЂљРЎРЉ Р СР ВµРЎвЂљР С•Р Т‘ read_settings Р С‘Р В»Р С‘ parse_args
    Р Т‘Р В»РЎРЏ РЎРѓРЎвЂЎР С‘РЎвЂљРЎвЂ№Р Р†Р В°Р Р…Р С‘РЎРЏ Р Р…Р В°РЎРѓРЎвЂљРЎР‚Р С•Р ВµР С”.
    Р СџР В°РЎР‚Р В°Р СР ВµРЎвЂљРЎР‚РЎвЂ№ Р Р…Р В°РЎРѓРЎвЂљРЎР‚Р ВµР С•Р С” Р СР С•Р С–РЎС“РЎвЂљ Р С—Р ВµРЎР‚Р ВµР Т‘Р В°Р Р†Р В°РЎвЂљРЎРЉРЎРѓРЎРЏ Р Р† qml Р ВµРЎРѓР В»Р С‘ Р С•Р С—Р ВµРЎР‚Р Т‘Р ВµР В»Р С‘РЎвЂљРЎРЉ Р С•Р В±РЎРЉР ВµР С”РЎвЂљ Р С‘Р Р…РЎРѓРЎвЂљР В°Р Р…РЎРѓР В° Р Р† Р С”Р С•Р Р…РЎвЂљР ВµР С”РЎРѓРЎвЂљ qml.

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

    Settings m_set;
    std::string m_path_to_file;
    bool is_init_arg;
};



#endif // SETTINGSCONTROLLER_H
