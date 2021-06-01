﻿#include <QGuiApplication>
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
    ctxt->setContextProperty(QLatin1String("DirModel"), expl.dir_model());
    ctxt->setContextProperty(QLatin1String("SubModel"), expl.sub_model());
    engine.addImageProvider(QLatin1String("iconProvider"), expl.getProvider());
    engine.load(url);

    return app.exec();
}
