#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextCodec>
#include <folderexpl.h>
/*!
 * \author Zelenskiy Vladislav, email: vladzzp10@yandex.ru
 */

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    app.setApplicationName("Модуль взаимодействия со шлюзом обмена данными ");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationDomain("RusBITeh");
    app.setWindowIcon(QIcon(":/appIcon.ico"));

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); //изменения

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("windows-1251")); //изменения
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); //изменения
    ///Чтение переданных параметров командной строки и инициализцация настроек
    SettingsController::get_instanse().parse_args(app);
    SettingsController::get_instanse().read_settings();
    ///Инициализация лог файла приложения
    Loger::getInstanse().init(
                SettingsController::get_instanse().
                log_file_path().toStdString());
    ///Инициализация проводника
    FolderExpl expl;
    expl.initFromSettings();
    ///Запуск движка QML
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    ///Загрузка объектов в контекст QML интерфейса
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty(QLatin1String("AppSettings"), &SettingsController::get_instanse());
    ctxt->setContextProperty(QLatin1String("DirModel"), expl.getDir_models().first.get());
    ctxt->setContextProperty(QLatin1String("SubModel"), expl.getDir_models().second.get());
    ctxt->setContextProperty(QLatin1String("EmailModelLeft"), expl.getEmail_models().first.get());
    ctxt->setContextProperty(QLatin1String("EmailModelRight"), expl.getEmail_models().second.get());
    ctxt->setContextProperty(QLatin1String("AbonentModel"), expl.getAbonent_model().get());
    ///устновка постовщика изображений
    engine.addImageProvider(QLatin1String("iconProvider"), expl.getProvider());
    ///загрузка входного модуля
    engine.load(url);

    return app.exec();
}
