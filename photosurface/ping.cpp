#include "ping.h"
#include <QDebug>

Ping::Ping(QObject *parent) : QObject(parent)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(dl_replyFinished(QNetworkReply*)));

    connect(&timer, SIGNAL(timeout()), this, SLOT(onPing()));
    timer.start(1000); //start quick
}

void Ping::onPing()
{
    manager.get(QNetworkRequest(QUrl("http://www.reha-daheim.de/GrannysGallery/attach_extract.php")));   //Opti
}

void Ping::dl_replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "LIST_ERROR!";
    }
    else
    {
        while(!(reply->atEnd())){
            QString line = reply->readLine();
            qDebug() << line;
        }
        if(reply->atEnd()){
            qDebug() << "@End_LIST";
        }
    }
    timer.start(60000);
    reply->deleteLater();
}
