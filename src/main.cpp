#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <folderexpl.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    app.setApplicationName("Модуль взаимодействия со шлюзом обмена данными ");
    app.setApplicationVersion("1.0.0");
    app.setWindowIcon(QIcon(":/appIcon.ico"));
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
    ctxt->setContextProperty(QLatin1String("DirModel"), expl.getDir_models().at(0));
    ctxt->setContextProperty(QLatin1String("SubModel"), expl.getDir_models().at(1));
    ctxt->setContextProperty(QLatin1String("EmailModelLeft"), expl.getEmail_models().at(0));
    ctxt->setContextProperty(QLatin1String("EmailModelRight"), expl.getEmail_models().at(0));
    ///устновка постовщика изображений
    engine.addImageProvider(QLatin1String("iconProvider"), expl.getProvider());
    ///загрузка входного модуля
    engine.load(url);

    return app.exec();
}
