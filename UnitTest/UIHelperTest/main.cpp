#include "uihelpertest.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<MsgBoxIcon>("MsgBoxIcon", 1, 0, "MsgBoxIcon");
    UIOperation::getIns()->setContextProperty(engine);

    UIHelperTest uiHelperTest;
    UIHelperThreadTest uihelperThdTest;
    engine.rootContext()->setContextProperty("uiHelperTest", &uiHelperTest);
    engine.rootContext()->setContextProperty("uihelperThdTest", &uihelperThdTest);

    qDebug() << "main thread id:" << QThread::currentThreadId();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
