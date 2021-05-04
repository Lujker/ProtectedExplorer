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

#define SET_PATH "../settings/set.xml"
#define SET_DIR "../settings"


/*!
    \brief РЎС‚СЂСѓРєСЂС‚СѓСЂР° СЃ РїР°СЂР°РјРµС‚СЂР°РјРё РЅР°СЃС‚СЂРѕРµРє
*/
struct Settings{
    std::string save_dump_dir;
    std::string log_file_path;
    std::string dir_list;
};


std::vector<std::string> split (const std::string&, std::string);
std::vector<std::string> split (const std::string&, char);


/*!
    \brief РљР»Р°СЃСЃ РґРѕСЃС‚СѓРїР° Рє РЅР°СЃС‚СЂРѕР№РєР°Рј
    \author Zelenskiy V.P.
    \version 1.0
    \date РњР°СЂС‚ 2021 РіРѕРґР°
    \warning Р”Р°РЅРЅС‹Р№ РєР»Р°СЃСЃ РЅРµ СЃРѕР·РґР°РµС‚СЃСЏ, Р° РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ С‡РµСЂРµР· РёРЅС‚СЃС‚Р°РЅСЃ

    РљР»Р°СЃСЃ РґР»СЏ С‡С‚РµРЅРёСЏ Рё Р·Р°РїРёСЃРё РЅР°СЃС‚СЂРѕР№Рє РїСЂРѕРіСЂР°РјРјС‹, СЂР°Р·СЂР°Р±РѕС‚Р°РЅ РїРѕ С€Р°Р±Р»РѕРЅСѓ singleton
    Рё РЅРµ РґРѕР»Р¶РµРЅ СЃРѕР·РґР°РІР°С‚СЊСЃСЏ РІ РїСЂРѕРіСЂР°РјРјРµ.
    РџРµСЂРµРґ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёРµРј РЅРµРѕР±С…РѕРёРґРјРѕ РІС‹Р·РІР°С‚СЊ РјРµС‚РѕРґ read_settings РёР»Рё parse_args
    РґР»СЏ СЃС‡РёС‚С‹РІР°РЅРёСЏ РЅР°СЃС‚СЂРѕРµРє.
    РџР°СЂР°РјРµС‚СЂС‹ РЅР°СЃС‚СЂРµРѕРє РјРѕРіСѓС‚ РїРµСЂРµРґР°РІР°С‚СЊСЃСЏ РІ qml РµСЃР»Рё РѕРїРµСЂРґРµР»РёС‚СЊ РѕР±СЊРµРєС‚ РёРЅСЃС‚Р°РЅСЃР° РІ РєРѕРЅС‚РµРєСЃС‚ qml.

*/


class SettingsController: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString save_dump_dir READ save_dump_dir WRITE set_save_dump_dir NOTIFY save_dump_dir_change)

public:
    static SettingsController &get_instanse();
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
