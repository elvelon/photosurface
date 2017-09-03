#ifndef IMAGECONTEXT_H
#define IMAGECONTEXT_H

#include <QObject>
#include <QQmlContext>

class ImageContext : public QObject
{
    Q_OBJECT
public:
    explicit ImageContext(QObject *parent=Q_NULLPTR);
    void setContext(QQmlContext *context);

private:
    QStringList imageNameFilters(QUrl pic_location);
    QQmlContext *app_context;

public slots:
    void onUpdateContext( void );
};

#endif // IMAGECONTEXT_H
