#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <folderexpl.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    app.setApplicationName("ProtectedExplorer");
    app.setApplicationVersion("1.0.0");

    ///Client code for init app

    SettingsController::get_instanse().parse_args(app);
    SettingsController::get_instanse().read_settings();
    Loger::getInstanse().init(
                SettingsController::get_instanse().
                log_file_path().toStdString());
    FolderExpl expl;
    expl.initFromSettings();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty(QLatin1String("AppSettings"), &SettingsController::get_instanse());
    ctxt->setContextProperty(QLatin1String("DirModel"), expl.dir_model());
    ctxt->setContextProperty(QLatin1String("SubModel"), expl.sub_model());
    engine.addImageProvider(QLatin1String("iconProvider"), new IconProvider);
    engine.load(url);

    return app.exec();
}
