#ifndef PING_H
#define PING_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

class Ping : public QObject
{
    Q_OBJECT
public:
    explicit Ping(QObject *parent = 0);

private:
    QTimer timer;
    QNetworkAccessManager manager;


signals:

public slots:
    void onPing();
    void dl_replyFinished (QNetworkReply *reply);
};

#endif // PING_H
