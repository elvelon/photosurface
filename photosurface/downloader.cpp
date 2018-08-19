#include "downloader.h"
#include <QDebug>
#include <QThread>
#include <QString>

Downloader::Downloader(QString user, QString url)  :
    m_user(user)
{
    dataThereFlag = false;
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(onNetworkReplyAvailable(QNetworkReply*)));
    ftpUrl = url;
    QObject::connect(&timer, &QTimer::timeout, this, &Downloader::timedDownloadExecution);
    QObject::connect(this, &Downloader::startQueuedDownload, this, &Downloader::doDownload);
}

void Downloader::timedDownloadExecution()
{
    QString pic_list = ftpUrl + "/private/pics/" + m_user + "/pic_list.txt";

    QUrl url = QUrl::fromEncoded(pic_list.toLocal8Bit());
    if (ListOfPicUrlsReadyToDownload.isEmpty())
    {
        doDownload(url);
    }
    if(!timer.isActive())
    {
        timer.start(60000);
    }
}

void Downloader::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply;
    reply = manager.get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif
}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_SSL
    foreach (const QSslError &error, sslErrors)
    {
        emit logToFile(QString("SSL error: %1\n")
                          .arg(qPrintable(error.errorString())));
    }
#else
    Q_UNUSED(sslErrors);
#endif
}

void Downloader::onNetworkReplyAvailable(QNetworkReply *reply)
{
    if (reply->error())
    {
        emit logToFile(QString("Download of %1 failed: %2\n")
                          .arg(reply->url().toEncoded().constData())
                           .arg(qPrintable(reply->errorString())));
        return;
    }

    QString filename = extractFileNameFromReply(reply->url());
    if(filename == "")
    {
        return;
    }

    handlePayloadOfReply(reply, filename);

    if(ListOfPicUrlsReadyToDownload.isEmpty())
    {
        emit allAvailableDownloadsHandled();
        emit logToFile("Downloads finished");
    }
    else{
        emit startQueuedDownload(QUrl::fromEncoded(ListOfPicUrlsReadyToDownload.last().toLocal8Bit()));
    }
    reply->deleteLater();
}

QString Downloader::extractFileNameFromReply(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        return "download";

    if (basename == "pic_list.txt")
        return "pic_list.txt";

    if (QFile::exists(basename)) {

        emit logToFile(QString("%1 already exists").arg(basename));
        return "";
    }
    return basename;
}

void Downloader::handlePayloadOfReply(QNetworkReply *reply, QString filename)
{
    if(fileIsPicList(filename))
    {
        ListOfPicUrlsReadyToDownload = processPicList(reply);
    }
    else
    {
        saveImageFromReplyToDisk(filename, reply);
        ListOfPicUrlsReadyToDownload.removeFirst();
    }
}

bool Downloader::fileIsPicList(const QString fn)
{
    return fn == "pic_list.txt";
}

QStringList Downloader::processPicList(QIODevice *data){
    QStringList pic_urls;
    char buf[1024];
    while(data->canReadLine())
    {
        int nr_of_chars_read = data->readLine(buf, sizeof(buf));
        QString check(buf);
        if(check.startsWith("rm "))
        {
            deletePicture(QString::fromLocal8Bit(buf, nr_of_chars_read-1).mid(3));
            continue;
        }
        pic_urls << ftpUrl + QString::fromLocal8Bit(buf, nr_of_chars_read-1);
        qDebug() << pic_urls;

    }
    return pic_urls;
}

void Downloader::deletePicture(QString fn)
{
    emit logToFile(QString("deleting file: %1").arg(fn));
    QFile file (fn);
    if(file.exists())
        file.remove();
}

bool Downloader::saveImageFromReplyToDisk(const QString &filename, QIODevice *data)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/" + filename);
    if (!file.open(QIODevice::WriteOnly)) {
        emit logToFile(QString("Could not open %1 for writing: %2\n")
                       .arg(qPrintable(filename))
                       .arg(qPrintable(file.errorString())));
        return false;
    }

    file.write(data->readAll());
    file.close();

    emit logToFile(QString("Download of %1 succeeded.")
                   .arg(qPrintable(filename)));
    return true;
}
