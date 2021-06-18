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

/*!
 * \brief SettingsController::get_instanse получение сингелтон обьекта
 * \return возвращает статический обьект
 */
SettingsController &SettingsController::get_instanse()
{
    static SettingsController sett;
    return sett;
}

/*!
 * \brief SettingsController::parse_args парсинг аргументов командной строки для поиска пути к файлу настроек
 * \param app обьект ядра приложения Qt
 */
void SettingsController::parse_args(const QCoreApplication &app)
{
    QCommandLineParser parser;
      parser.setApplicationDescription("open protected file explorer");
      parser.addHelpOption();

      parser.addVersionOption();
      QCommandLineOption set_file(QStringList() << "s", "path to settings .xml file", "path");
      parser.addOption(set_file);

      parser.process(app);

      if(parser.isSet(set_file)){
          get_instanse().m_path_to_file = parser.value("s").toStdString();
      }
}

/*!
 * \brief SettingsController::save_settings сохранить XML файл настроек из структуры
 * \param path_to_save_file путь к файлу настреок для записи
 */
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
    xmlWriter.writeStartElement("work_dirs");

    for(const auto &it : m_set.dir_list){
        xmlWriter.writeStartElement("dir");  // Записываем тег с именем для первого чекбокса
        xmlWriter.writeAttribute("title", QString::fromStdString(it.first));
        xmlWriter.writeAttribute("driver", "localdir");
        xmlWriter.writeCharacters(QString::fromStdString(it.second));
        xmlWriter.writeEndElement();        // Закрываем тег
    }

    for(const auto &it : m_set.shared_list){
        xmlWriter.writeStartElement("dir");  // Записываем тег с именем для первого чекбокса
        xmlWriter.writeAttribute("title", QString::fromStdString(it.first));
        xmlWriter.writeAttribute("driver", "ftp");
        xmlWriter.writeCharacters(QString::fromStdString(it.second));
        xmlWriter.writeEndElement();        // Закрываем тег
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("mail");
    xmlWriter.writeStartElement("addressbook");
    for(const auto &it : m_set.abonents){
        xmlWriter.writeStartElement("system");  // Записываем тег с именем для первого чекбокса
        xmlWriter.writeAttribute("type_id", QString::number(it.db_type_id));
        xmlWriter.writeAttribute("name", QString::fromStdString(it.sys_name));
        xmlWriter.writeStartElement("outbox");
        xmlWriter.writeCharacters(QString::fromStdString(it.outbox_path));
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("inbox");
        xmlWriter.writeCharacters(QString::fromStdString(it.inbox_path));
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();        // Закрываем тег
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("log_file");  // Записываем тег с именем для первого чекбокса
    if(m_set.log_file_path.empty())
        xmlWriter.writeAttribute("path", "../logfile.log");
    else
        xmlWriter.writeAttribute("path", QString::fromStdString(m_set.log_file_path));
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    /* Завершаем запись в документ
     * */
    xmlWriter.writeEndDocument();
    file.close();   // Закрываем файл
}

/*!
 * \brief SettingsController::read_settings чтение файла настроек по xml
 */
void SettingsController::read_settings()
{
    QString path;
    if(m_path_to_file.empty()){
        if(QFile::exists(SET_PATH))
            path = SET_PATH;
        else
            path = RESERV_SET_PATH;
    }
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
                    if(xmlReader.name() == "work_dirs"){
                        xmlReader.readNextStartElement();
                         while(xmlReader.name() == "dir"){
                             QString title;
                             QString driver;
                             QString path;
                             for(const auto &attr : xmlReader.attributes()) {
                                 if (attr.name().toString() == "title") {
                                     title = attr.value().toString();
                                 }
                                 else if(attr.name().toString() == "driver"){
                                     driver = attr.value().toString();
                                 }
                             }
                             path = xmlReader.readElementText();
                             if(driver == "ftp"){
                                 m_set.shared_list.push_back(
                                             std::make_pair(title.toStdString(), path.toStdString()));
                             }
                             else if (driver == "localdir"){
                                 m_set.dir_list.push_back(
                                             std::make_pair(title.toStdString(), checkFolder(path.toStdString())));
                             }
                             xmlReader.readNextStartElement();
                         }
                    }
                    if(xmlReader.name() == "addressbook"){
                        xmlReader.readNextStartElement();
                        while(xmlReader.name()=="system"){
                          Abonent new_abonent;
                          for(const auto &attr : xmlReader.attributes()) {
                              if (attr.name().toString() == "name") {
                                  new_abonent.sys_name = attr.value().toString().toStdString();
                              }
                              else if(attr.name().toString() == "type_id"){
                                  new_abonent.db_type_id = attr.value().toInt();
                              }
                          }
                          for(int i=0;i<2;++i){
                              xmlReader.readNextStartElement();
                              if(xmlReader.name()=="outbox")
                                  new_abonent.outbox_path=xmlReader.readElementText().toStdString();
                              else if(xmlReader.name()=="inbox")
                                  new_abonent.inbox_path=xmlReader.readElementText().toStdString();
                          }
//                          qDebug()<<new_abonent.db_id << " " << QString::fromStdString(new_abonent.sys_name) << " " << QString::fromStdString(new_abonent.inbox_path)
//                                 << " "<< QString::fromStdString(new_abonent.outbox_path);
                          m_set.abonents.push_back(new_abonent);

                          xmlReader.readNextStartElement();
                          xmlReader.readNextStartElement();
                      }
                    }
                    if(xmlReader.name() == "log_file")
                        for(const auto &attr : xmlReader.attributes()) {
                            if (attr.name().toString() == "path") {
                                QString attribute_value = attr.value().toString();
                                if(attribute_value.isEmpty())
                                   m_set.log_file_path = "../logfile.log";
                                else
                                    m_set.log_file_path = attribute_value.toStdString();
                            }
                    }
                }
                xmlReader.readNext();
    }
    file.close(); // Закрываем файл

    set_cl(); ///Заполнение класификаторов из базы данных
}

bool SettingsController::is_init()
{
    return this->is_init_arg;
}

const Settings &SettingsController::get_settings()
{
    return this->m_set;
}

void SettingsController::set_cl()
{
    db::DatabaseQuery::generate_select_cl_abonent_type(m_set.cl_abonent_type);
    db::DatabaseQuery::generate_select_cl_status(m_set.cl_status);
}

std::vector<std::pair<std::string,std::string>> SettingsController::sub_list_dirs()
{
    return m_set.shared_list;
}

QString SettingsController::log_file_path()
{
    return QString::fromStdString(m_set.log_file_path);
}

std::vector<std::pair<std::string,std::string>> SettingsController::dir_list()
{
    return m_set.dir_list;
}

std::vector<Abonent>& SettingsController::abonents()
{
    return m_set.abonents;
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

SettingsController::SettingsController(QObject *parent)
{
    Q_UNUSED(parent)
}

SettingsController::~SettingsController()
{}

std::string SettingsController::checkFolder(const std::string &absPath)
{
    ///проверка имени дериктории по абсолютному пути
    /// если такой нет то создаем ее в home дериктории пользователя если ее еще не создали

    QDir dir(QString::fromStdString(absPath));
    if(!dir.exists()){
        QString iter_dir_path(QDir::homePath()+
                              QDir::separator()+
                              QString::fromStdString(absPath));
        QDir dir_in_home(iter_dir_path);
        if(!dir_in_home.exists()){
            QDir home = QDir::home();
            home.mkpath(QString::fromStdString(absPath));
        }
        return iter_dir_path.toStdString();
    }
    return absPath;
}

Abonent::operator std::string()
{
    return std::string {"Abonnent: with id '"
                + std::to_string(this->db_id)
                + "' and name '"
                + this->sys_name + "'"};
}

bool Abonent::operator==(const Abonent &ab) const
{
    return this->sys_name==ab.sys_name;
}

bool Abonent::operator!=(const Abonent &ab) const
{
    return !(*this==ab);
}

bool Abonent::operator<(const Abonent &ab) const
{
    return this->sys_name<ab.sys_name;
}

Letter::operator std::string()
{
    return std::string {"Letter: with id '"
                + std::to_string(this->let_id)
                + "' and title '"
                + this->title + "'"};
}

bool Letter::operator==(const Letter &let) const
{
    return this->let_id == let.let_id;
}

bool Letter::operator!=(const Letter &let) const
{
    return !(*this==let);
}

bool Letter::operator<(const Letter &let) const
{
    return this->let_id<let.let_id;
}
