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

    SettingsController::get_instanse().parse_args(app);
    SettingsController::get_instanse().read_settings();
    Loger::getInstanse().init(
                SettingsController::get_instanse().
                log_file_path().toStdString());


    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("AppSettings", &SettingsController::get_instanse());

    engine.load(url);

    return app.exec();
}
