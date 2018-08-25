#include <QStandardPaths>
#include <QDebug>

#include "testimagecontext.h"

#include "../../imagecontext.h"

TestImageContext::TestImageContext(QQmlContext *context, QObject *parent) : QObject(parent)
{
    ImageContext img_con;

    QStringList imageListFirst = img_con.setContext(context);
    QStringList imageListSecond = img_con.setContext(context);

    QString now_REGEX = "*" + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "*";
    QStringList filteredList = imageListFirst.filter(now_REGEX);

    qDebug() << filteredList;

    Q_ASSERT(imageListFirst != imageListSecond);

    //First 15 should be the same
    QStringList imageListFirst_firstFifteen = imageListFirst.mid(0,15);
    QStringList imageListSecond_firstFifteen = imageListSecond.mid(0,15);
    Q_ASSERT(imageListFirst_firstFifteen == imageListSecond_firstFifteen);

//    Q_ASSERT(imageListFirst.size() <= 41);
//    Q_ASSERT(imageListSecond.size() <= 41);
}
