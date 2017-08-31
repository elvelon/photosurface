#include "downloader.h"
#include <QDebug>
#include <QThread>

Downloader::Downloader()
{
    dataThereFlag = false;
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
    user = "oma";
    ftpUrl = "ftp://rehad951:1qay!QAY@ftp.reha-daheim.de";
    QObject::connect(&timer, &QTimer::timeout, this, &Downloader::execute);
    timer.start(60000);
}

void Downloader::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply;
    reply = manager.get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif

    currentDownloads.append(reply);
}

QString Downloader::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        return "download";

    if (basename == "pic_list.txt")
        return "pic_list.txt";

    if (QFile::exists(basename)) {
        qDebug() << basename << "already exists";
        return "";
    }
    return basename;
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
{
    //    QFile  file(filename);
    QFile file(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/" + filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open " << qPrintable(filename)
                 <<" for writing: " << qPrintable(file.errorString()) << "\n";
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void Downloader::execute()
{
    QString pic_list = ftpUrl + "/private/pics/" + user + "/pic_list.txt";

    QUrl url = QUrl::fromEncoded(pic_list.toLocal8Bit());
    doDownload(url);
}

void Downloader::downloadRepeater(QStringList args)
{
    foreach (QString arg, args) {
        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
        doDownload(url);
    }
}


void Downloader::processPicList(QIODevice *data){
    QStringList pic_urls;
    char buf[1024];
    while(data->canReadLine())
    {
        int nr_of_chars_read = data->readLine(buf, sizeof(buf));
        pic_urls << ftpUrl + QString::fromLocal8Bit(buf, nr_of_chars_read-1);
        qDebug() << pic_urls;
    }
    downloadRepeater(pic_urls);
}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_SSL
    foreach (const QSslError &error, sslErrors)
        qDebug() << "SSL error: " << qPrintable(error.errorString()) << "\n";
#else
    Q_UNUSED(sslErrors);
#endif
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        qDebug() << "Download of " << url.toEncoded().constData() << " failed: "
                 << qPrintable(reply->errorString()) << "\n";
    } else {
        QString filename = saveFileName(url);
        if((filename != "") && (filename != "pic_list.txt"))
        {
            if (saveToDisk(filename, reply))
            {
                qDebug() << "Download of"  << qPrintable(filename) << "succeeded";
            }
        }else if(filename == "pic_list.txt")
        {
            processPicList(reply);
        }
        dataThereFlag = true;
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if (currentDownloads.isEmpty())
    {
        if(dataThereFlag == true)
        {
            emit success();
            dataThereFlag = false;
        }
        qDebug() << "Downloads finished";
    }
    //        // all downloads finished
    //        QCoreApplication::instance()->quit();
}
