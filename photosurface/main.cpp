/****************************************************************************
**
** Copyright Kai Hinderer
** Contact: kai.hinderer|AT|gmail.com
**
****************************************************************************/

#ifdef QT_WIDGETS_LIB
#include <QtWidgets/QApplication>
#else
#include <QtGui/QGuiApplication>
#endif
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickWindow>
#include <QtGui/QImageReader>
#include <QtCore/QMimeDatabase>
#include <QDebug>

#include "downloader.h"
#include "ping.h"
#include "imagecontext.h"
#include "argumentparser.h"
#include "filelogger.h"


#define VERSION "0.1"
int main(int argc, char* argv[])
{
    // The reason to use QApplication is that QWidget-based dialogs
    // are the native dialogs on Qt-based platforms like KDE,
    // but they cannot be instantiated if this is a QGuiApplication.
#ifdef QT_WIDGETS_LIB
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif
    QQuickWindow::setDefaultAlphaBuffer(true);

    QCoreApplication::setApplicationName(QStringLiteral("OmasKino"));
    QCoreApplication::setOrganizationName(QStringLiteral("k__h"));
    QCoreApplication::setApplicationVersion(QLatin1String(QT_VERSION_STR) + VERSION);

    QStringList options;
    QStringList parameter;
    QString m_user = "";

    parameter << "user";

    ArgumentParser *arguments = new ArgumentParser(options, parameter);

    if(arguments->paramExists("user"))
    {
        m_user = arguments->param("user");
        qDebug() << "User=" << m_user;
    }
    else
    {
        qDebug() << "Please call with -user=XXX argument";
        return -1;
    }

    FileLogger logger(VERSION, "Logfile");

    Downloader downloader(m_user, "ftp://rehad951:1qay!QAY@ftp.reha-daheim.de");
    QTimer::singleShot(30000, &downloader, SLOT(timedDownloadExecution()));
    QObject::connect(&downloader, &Downloader::logToFile, &logger, &FileLogger::onFileLog);

    Ping ping;
    QTimer *timerPing = new QTimer();
    QObject::connect(timerPing, SIGNAL(timeout()), &ping, SLOT(onPing()));
    QObject::connect(&downloader, &Downloader::allAvailableDownloadsHandled, &ping, &Ping::deletePicList);
    QObject::connect(&ping, &Ping::logToFile, &logger, &FileLogger::onFileLog);
    timerPing->start(60000);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    ImageContext img_con;
    img_con.setContext(context);
    context->setContextProperty(("img_con"), &img_con); //Could be all run over this context.
    QObject::connect(&downloader, SIGNAL(allAvailableDownloadsHandled()), &img_con, SLOT(onUpdateContext()));

    QTimer *timerContext = new QTimer();
    QObject::connect(timerContext, SIGNAL(timeout()), &img_con, SLOT(onUpdateContext()));
    timerContext->start(360000); //every hour

    engine.load(QUrl("qrc:///photosurface.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
