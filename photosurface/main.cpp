/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
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
    QCoreApplication::setApplicationVersion(QLatin1String(QT_VERSION_STR));

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

    //Start Downloader!
    Downloader d(m_user);
    QTimer::singleShot(30000, &d, SLOT(execute()));

//    PingScript ping;
    Ping ping;
    QTimer *timerPing = new QTimer();
    QObject::connect(timerPing, SIGNAL(timeout()), &ping, SLOT(onPing()));
    timerPing->start(60000);

    QObject::connect(&d, &Downloader::success, &ping, &Ping::deletePicList);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    ImageContext img_con;
    img_con.setContext(context);
    context->setContextProperty(("img_con"), &img_con); //Could be all run over this context.

    QTimer *timerContext = new QTimer();
    QObject::connect(timerContext, SIGNAL(timeout()), &img_con, SLOT(onUpdateContext()));
    timerContext->start(360000); //every hour

    QObject::connect(&d, SIGNAL(success()), &img_con, SLOT(onUpdateContext()));

    engine.load(QUrl("qrc:///photosurface.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
