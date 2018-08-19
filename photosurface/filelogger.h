#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileLogger : public QObject
{
    Q_OBJECT
public:
    explicit FileLogger(QString version, QString logFileName, QObject *parent = 0);

public slots:
    void onFileLog(QString msg_stream);

private:
    void generateLogFile(QString version, QString fn);
    QFile *log_file;
};

#endif // FILELOGGER_H
