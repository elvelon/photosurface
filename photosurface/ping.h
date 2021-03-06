/****************************************************************************
**
** Copyright Kai Hinderer
** Contact: kai.hinderer|AT|gmail.com
**
****************************************************************************/

#ifndef PING_H
#define PING_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

class Ping : public QObject
{
    Q_OBJECT
public:
    explicit Ping(QObject *parent=Q_NULLPTR);

private:
    QNetworkAccessManager manager;


signals:
    void logToFile(QString);

public slots:
    void onPing();
    void deletePicList();
    void dl_replyFinished(QNetworkReply *reply);
};

#endif // PING_H
