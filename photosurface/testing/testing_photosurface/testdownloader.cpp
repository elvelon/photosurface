#include <QDebug>

#include "testdownloader.h"

#include "../../downloader.h"

TestDownloader::TestDownloader()
{
    qDebug() << "here";
    Downloader downloader("m_user", "ftp://rehad951:1qay!QAY@ftp.reha-daheim.de");
}
