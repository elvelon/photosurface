/****************************************************************************
**
** Copyright Kai Hinderer
** Contact: kai.hinderer|AT|gmail.com
**
****************************************************************************/

#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>

class Functionality : public QObject
{
    Q_OBJECT
public:
    explicit Functionality(QString ln, QObject *parent = 0);
private:
    QFile *file;

signals:

public slots:
};

#endif // FUNCTIONALITY_H
