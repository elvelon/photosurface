#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QTimer>

#include "functionality.h"

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    void doUpload();
    void process_line(QString ln);
    Functionality * functionality;

signals:

public slots:
    void doDownload(QString url = "");
    void dl_replyFinished (QNetworkReply *reply);
    void pic_replyFinished (QNetworkReply *reply);
    void ul_replyFinished (QNetworkReply *reply);

private:
   qint8 list_there_flag;
   qint8 end_pic_list_flag;
   qint16 line_cnt;
   QString fn;
   QTimer *timer;
   QFile *file2;
   QNetworkAccessManager *manager;
   QNetworkAccessManager *pic_manager;
   QFile *file;
   QList<QString> file_lst;
   QThread functionalityThread;
};

#endif // DOWNLOADER_H
