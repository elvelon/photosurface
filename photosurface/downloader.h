#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QStringList>
#include <QStandardPaths>
#include <QTimer>
#include <QTextStream>
#include <QUrl>

#include <stdio.h>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

QT_USE_NAMESPACE

class Downloader : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;

public:
    explicit Downloader(QString user, QString url);

signals:
    void allAvailableDownloadsHandled();
    void startQueuedDownload(QUrl);
    void logToFile(QString);

public slots:
    void timedDownloadExecution();
    void onNetworkReplyAvailable(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

private slots:
    void doDownload(const QUrl &url);

private:
    bool fileIsPicList(const QString fn);
    void deletePicture(QString fn);
    void extractFilenameFromReply(QNetworkReply *reply);
    bool fileIsValidPicList(const QString fn);
    void handlePayloadOfReply(QNetworkReply *reply, QString filename);
    QString extractFileNameFromReply(const QUrl &url);
    bool saveImageFromReplyToDisk(const QString &filename, QIODevice *data);
    QStringList processPicList(QIODevice *data);
    QString ftpUrl;
    QString m_user;
    QStringList  ListOfPicUrlsReadyToDownload;
    bool dataThereFlag;
    QTimer timer;
//    QString password;
};


#endif // DOWNLOADER_H
