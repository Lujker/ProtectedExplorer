#include "settingscontroller.h"

std::vector<std::string> split(const std::string &s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}


SettingsController &SettingsController::get_instanse()
{
    static SettingsController sett;
    return sett;
}

void SettingsController::parse_args(int argc, char *argv[])
{
    ///need settings args
}

void SettingsController::save_settings(std::string path_to_save_file)
{
    QFile file(QString::fromStdString(path_to_save_file));
    if(!file.open(QIODevice::WriteOnly)){
        QDir dir(SET_DIR);
        dir.mkpath(QString::fromStdString(SET_DIR));
        file.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);  // Устанавливаем автоформатирование текста
    xmlWriter.writeStartDocument();     // Запускаем запись в документ
    xmlWriter.writeStartElement("Settings");   // Записываем первый элемент с его именем

    xmlWriter.writeStartElement("save_dump_dir");  // Записываем тег с именем для первого чекбокса
    xmlWriter.writeAttribute("string", QString::fromStdString(m_set.save_dump_dir));
    xmlWriter.writeEndElement();        // Закрываем тег

    xmlWriter.writeStartElement("log_file_path");  // Записываем тег с именем для первого чекбокса
    if(m_set.log_file_path.empty())
        xmlWriter.writeAttribute("string", "../logfile.log");
    else
        xmlWriter.writeAttribute("string", QString::fromStdString(m_set.log_file_path));
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("dir_list");  // Записываем тег с именем для первого чекбокса
    xmlWriter.writeAttribute("string", QString::fromStdString(m_set.dir_list));
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    /* Завершаем запись в документ
     * */
    xmlWriter.writeEndDocument();
    file.close();   // Закрываем файл
}

void SettingsController::read_settings()
{
    QString path;
    if(m_path_to_file.empty())
       path = SET_PATH;
    else path = QString::fromStdString(m_path_to_file);

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        ///FAIL
        return;
     }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();   // Переходит к первому элементу в файле

    while(!xmlReader.atEnd()){
                /* Проверяем, является ли элемент началом тега
                 * */
                if(xmlReader.isStartElement())
                {
                    if(xmlReader.name() == "save_dump_dir")
                        for(const auto &attr : xmlReader.attributes()) {
                            if (attr.name().toString() == "string") {
                                QString attribute_value = attr.value().toString();
                                m_set.save_dump_dir = attribute_value.toStdString();
                            }
                        }

                    if(xmlReader.name() == "log_file_path")
                        for(const auto &attr : xmlReader.attributes()) {
                            if (attr.name().toString() == "string") {
                                QString attribute_value = attr.value().toString();
                                if(attribute_value.isEmpty())
                                   m_set.log_file_path = "../logfile.log";
                                else
                                    m_set.log_file_path = attribute_value.toStdString();
                            }
                        }

                    if(xmlReader.name() == "dir_list")
                        for(const auto &attr : xmlReader.attributes()) {
                            if (attr.name().toString() == "string") {
                                QString attribute_value = attr.value().toString();
                                m_set.dir_list = attribute_value.toInt();
                            }
                        }
                }
                xmlReader.readNext(); // Переходим к следующему элементу файла
    }
    file.close(); // Закрываем файл
}

bool SettingsController::is_init()
{
    return this->is_init_arg;
}

const Settings &SettingsController::get_settings()
{
    return this->m_set;
}

QString SettingsController::save_dump_dir()
{
    return QString::fromStdString(m_set.save_dump_dir);
}

QString SettingsController::log_file_path()
{
    return QString::fromStdString(m_set.log_file_path);
}

QString SettingsController::dir_list()
{
    return QString::fromStdString(m_set.dir_list);
}

std::list<std::string> SettingsController::parse_dump_list()
{
    std::list<std::string> lst;
    if(!m_set.dir_list.empty()){

    }
    return lst;
}

void SettingsController::set_save_dump_dir(const QString &name)
{
    auto list = name.split("///");
    m_set.save_dump_dir = list.at(list.size()-1).toStdString();
    emit save_dump_dir_change(QString::fromStdString(m_set.save_dump_dir));
}

void SettingsController::save_app_settings()
{
    if(m_path_to_file.empty())
        save_settings(SET_PATH);
    else  save_settings(m_path_to_file);
}

void SettingsController::set_log_file_path(const QString &path)
{
    if(path.isEmpty()) return;
    m_set.log_file_path = path.toStdString();
    emit log_file_path_change(path);
}

void SettingsController::set_dir_list(const QString &dir_list)
{
    if(!dir_list.isEmpty()){
        m_set.dir_list = dir_list.toStdString();
        emit dir_list_change(dir_list);
    }
}

SettingsController::SettingsController(QObject *parent)
{
    Q_UNUSED(parent)
}

SettingsController::~SettingsController()
{}
