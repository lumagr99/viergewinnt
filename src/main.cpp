#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

#include "myglitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QTranslator translator;
    if (translator.load("Julian_Witte_MM2020_de_DE.qm", ":/translations"))
        app.installTranslator(&translator);

    QQmlApplicationEngine engine;
    qmlRegisterType<MyGLItem>("GlComponents", 1,0, "MyGLItem");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
