/****************************************************************************
**
** Copyright Kai Hinderer
** Contact: kai.hinderer|AT|gmail.com
**
****************************************************************************/

#ifndef IMAGECONTEXT_H
#define IMAGECONTEXT_H

#include <QObject>
#include <QQmlContext>

class ImageContext : public QObject
{
    Q_OBJECT
public:
    explicit ImageContext(QObject *parent=Q_NULLPTR);
    QStringList setContext(QQmlContext *context);

public slots:
    void onUpdateContext( void );

private:
    QStringList getFilteredImageChoice(QUrl pic_location);
    QQmlContext *app_context;
};

#endif // IMAGECONTEXT_H
