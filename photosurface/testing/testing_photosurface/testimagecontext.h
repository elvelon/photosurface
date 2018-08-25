#ifndef TESTIMAGECONTEXT_H
#define TESTIMAGECONTEXT_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtTest/QtTest>

class TestImageContext : public QObject
{
    Q_OBJECT
public:
    explicit TestImageContext(QQmlContext *context, QObject *parent = 0);

signals:

public slots:
};

#endif // TESTIMAGECONTEXT_H
