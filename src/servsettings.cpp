#include "servsettings.h"

ServSettings &ServSettings::get_instanse()
{
    static ServSettings sett;
    return sett;
}

void ServSettings::parse_args(int argc, char *argv[])
{
    ///need settings args
}

void ServSettings::save_settings(std::string path_to_save_file)
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

void ServSettings::read_settings()
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

bool ServSettings::is_init()
{
    return this->is_init_arg;
}

const Settings &ServSettings::get_settings()
{
    return this->m_set;
}

QString ServSettings::save_dump_dir()
{
    return QString::fromStdString(m_set.save_dump_dir);
}

QString ServSettings::log_file_path()
{
    return QString::fromStdString(m_set.log_file_path);
}

QString ServSettings::dir_list()
{
    return QString::fromStdString(m_set.dir_list);
}

std::list<std::string> ServSettings::parse_dump_list()
{
    std::list<std::string> lst;
    if(!m_set.dir_list.empty()){

    }
    return lst;
}

void ServSettings::set_save_dump_dir(const QString &name)
{
    auto list = name.split("///");
    m_set.save_dump_dir = list.at(list.size()-1).toStdString();
    emit save_dump_dir_change(QString::fromStdString(m_set.save_dump_dir));
}

void ServSettings::save_app_settings()
{
    if(m_path_to_file.empty())
        save_settings(SET_PATH);
    else  save_settings(m_path_to_file);
}

void ServSettings::set_log_file_path(const QString &path)
{
    if(path.isEmpty()) return;
    m_set.log_file_path = path.toStdString();
    emit log_file_path_change(path);
}

void ServSettings::set_dir_list(const QString &dir_list)
{
    if(!dir_list.isEmpty()){
        m_set.dir_list = dir_list.toStdString();
        emit dir_list_change(dir_list);
    }
}

ServSettings::ServSettings(QObject *parent)
{
    Q_UNUSED(parent)
}

ServSettings::~ServSettings()
{}
