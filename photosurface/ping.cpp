/****************************************************************************
**
** Copyright Kai Hinderer
** Contact: kai.hinderer|AT|gmail.com
**
****************************************************************************/

#include "ping.h"
#include <QDebug>

Ping::Ping(QObject *parent) : QObject(parent)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(dl_replyFinished(QNetworkReply*)));
}

void Ping::onPing()
{
    manager.get(QNetworkRequest(QUrl("http://www.reha-daheim.de/GrannysGallery/attach_extract.php")));   //Opti
}

void Ping::deletePicList()
{
    manager.get(QNetworkRequest(QUrl("http://www.reha-daheim.de/GrannysGallery/deletePiclist.php")));   //Dopti
}

void Ping::dl_replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        emit logToFile("Ping-Reply error!");
    }
    else
    {
        while(!(reply->atEnd())){
            QString line = reply->readLine();
            emit logToFile(QString("Reply: %1").arg(line));
        }
        if(reply->atEnd()){
            emit logToFile(QString("@End of ping reply to: %1").arg(reply->request().url().toString()));
        }
    }
    reply->deleteLater();
}
