#include <QUrl>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QDebug>

#include "imagecontext.h"


ImageContext::ImageContext(QObject *parent) : QObject(parent)
{

}

QStringList ImageContext::imageNameFilters(QUrl pic_location)
{
    QStringList rawSortedList;
    QStringList result;
    QDir dir(pic_location.toString());
    QString now = "*" + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "*";

    //raw list
    QStringList basicFilter;
    basicFilter << "*.jpg";
    rawSortedList = dir.entryList(basicFilter, QDir::Files, QDir::Name);

    //now files
    QStringList filter;
    QStringList filteredDir;

    filter << now;
    filteredDir<< dir.entryList(filter, QDir::Files, QDir::Name);

    for (int i = 0; i < filteredDir.size(); i++)
    {
        QString fn = dir.relativeFilePath(filteredDir.at(i));
        result << fn;
        rawSortedList.removeLast();
    }

    //15 most recent
    int j = 15;
    while(rawSortedList.size() && (j--))
    {
        result << dir.relativeFilePath(rawSortedList.last());
        rawSortedList.removeLast();
    }

    //25 random
    int k = 25;
    while(rawSortedList.size() && (k--))
    {
        int index = qrand() % rawSortedList.size();
        result << dir.relativeFilePath(rawSortedList.at(index));
        rawSortedList.removeAt(index);
    }

    qDebug() << "Nr. of pics: " << result.size();
    return result;
}

void ImageContext::setContext(QQmlContext* context)
{
    app_context = context;
    QUrl picturesLocationUrl = QUrl::fromLocalFile(QDir::homePath());
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    const QStringList nameFilters = imageNameFilters(picturesLocations.first());

    if (!picturesLocations.isEmpty()) {
        picturesLocationUrl = QUrl::fromLocalFile(picturesLocations.first());
        if (!QDir(picturesLocations.first()).entryInfoList(nameFilters, QDir::Files).isEmpty())
        {
            QUrl initialUrl = picturesLocationUrl;
            app_context->setContextProperty(QStringLiteral("contextInitialUrl"), initialUrl);
        }else
        {
            app_context->setContextProperty(QStringLiteral("contextInitialUrl"), "undefined");
        }
    }

    app_context->setContextProperty(QStringLiteral("contextPicturesLocation"), picturesLocationUrl);
    app_context->setContextProperty(QStringLiteral("contextImageNameFilters"), nameFilters);
}

void ImageContext::onUpdateContext(void)
{
    setContext(app_context);
}
