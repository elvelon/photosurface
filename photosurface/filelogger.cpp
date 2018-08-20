/****************************************************************************
**
** Copyright Kai Hinderer
** Contact: kai.hinderer|AT|gmail.com
**
****************************************************************************/

#include "filelogger.h"
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

FileLogger::FileLogger(QString version, QString logFileName, QObject *parent) : QObject(parent)
{
    generateLogFile(version, logFileName);
}


void FileLogger::generateLogFile(QString version, QString fn)
{
    QString time = QDateTime::currentDateTime().toString("yyMMddhhmmss");
    log_file = new QFile(fn + "_" + time + ".log");
    onFileLog("Version: " + version);
}

void FileLogger::onFileLog(QString msg)
{
    static QString temp_log = "";
    static QString eolSequence = "\r\n";
    QString time = QDateTime::currentDateTime().toString("yyyy:MM:dd_hh:mm:ss.zzz");

    if(!log_file)
    {
        temp_log += time + ": " + msg + eolSequence;
        return;
    }

    if(!log_file->isOpen())
        log_file->open(QIODevice::WriteOnly | QIODevice::Append);

    if(!log_file->isWritable())
    {
        qDebug() << "file is not writable";
        return;
    }

    QTextStream out(log_file);
    if(!temp_log.isEmpty())
    {
        out << temp_log  << eolSequence;
        temp_log = "";
    }
    if(!msg.isEmpty())
        out << time << ": " << msg << eolSequence;
    log_file->flush();
    log_file->close();

    qDebug() << msg;
}
