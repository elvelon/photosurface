#include "functionality.h"



Functionality::Functionality(QString ln, QObject *parent) : QObject(parent)
{
    QString cmd_arg = "";
    QStringList cmd_lst = ln.split('%');
    QString cmd_called = cmd_lst[0];
    if(cmd_lst[1] != "")
    {
        cmd_arg = cmd_lst[1];
    }

    if(cmd_called == "delete")
    {
        qDebug() << cmd_arg;
//        if QFile::exists(){
//            QFile::remove();
//        }

        file = new QFile(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/" + cmd_arg);
        if(file->exists()){
            file->remove();
            qDebug() << "File removed!";
        }else{
            qDebug() << "Doesn't exist!";
        }
    }

    //ends thread

}
