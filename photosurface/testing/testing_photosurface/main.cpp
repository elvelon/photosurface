#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QDebug>

//#include "testimagecontext.h"
//#include "testdownloader.h"
#include "testping.h"

QQmlContext *context;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    //TEST-CODE//
//    context = engine.rootContext();
//    TestImageContext testImageContext(context);

    TestPing testPing;

//    TestDownloader testDownloader;

//    ImageContext img_con(NULL);
//    img_con.setContext(context);

    return app.exec();
}


