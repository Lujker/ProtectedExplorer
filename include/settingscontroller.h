#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <utility>
#include <iostream>

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QCommandLineParser>

#define SET_PATH "../settings/set.xml"
#define RESERV_SET_PATH "./settings/set.xml"
#define SET_DIR "../settings"
#define PATH_SEPARATOR ';' ///����������� ��� ������ �����

struct Abonent
{
    Abonent(): db_type_id(0), db_id(0)
    {}
    std::string sys_name;
    std::string outbox_path;
    std::string inbox_path;
    int db_type_id;
    int db_id;

    bool operator==(const Abonent& ab) const;
    bool operator!=(const Abonent& ab) const;
    bool operator<(const Abonent& ab) const;
};
/*!
    \brief ��������� �������� �������� ��������� ����������
*/
struct Settings{
    std::vector<std::pair<std::string,std::string>> shared_list;
    std::vector<std::pair<std::string,std::string>> dir_list;
    std::vector<Abonent> abonents;
    std::string log_file_path;
};

///������� ���������� �� ����� ������ ������ ����� �� �����������
std::vector<std::string> split (const std::string&, std::string);
std::vector<std::string> split (const std::string&, char);


/*!
    \brief ����� ������� � ����������
    \author Zelenskiy V.P.
    \version 1.0
    \date 01.06.2021
    \warning ����� �� ��������� ������������ � �������� �� ����� ��� ���� �������

    \details ����� ������������ ��� ��������� �������� ��������� �� ����� ��������
    ��� ���������� ���������� �� XML ����� �������� ���������� ������ � ��������� settings.
    ����� ������������� ���������� ������� ���� ����� �������� ��������� ������ � ������� �����
    �������� �������� �� ��������� ������ parse_args, ���� �������� �� ������ ��
    ����� ����������� ���� SET_PATH � SET_DIR
*/


class SettingsController: public QObject
{
    Q_OBJECT
public:
    static SettingsController &get_instanse();
    static void parse_args(const QCoreApplication &app);
    void save_settings(std::string path_to_save_file = std::string());
    void read_settings();
    bool is_init();
    const Settings &get_settings();

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

private:
    explicit SettingsController(QObject* parent = nullptr);
    SettingsController(SettingsController&)=delete;
    SettingsController(SettingsController&&)=delete;
    SettingsController& operator=(SettingsController&)=delete;
    virtual ~SettingsController();
    std::string checkFolder(const std::string &absPath);

    ///��������� ��� ������� ��������
    Settings m_set;
    ///���� � ����� ��������
    std::string m_path_to_file;
    bool is_init_arg;
};



#endif // SETTINGSCONTROLLER_H
