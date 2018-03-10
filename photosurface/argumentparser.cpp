#include "argumentparser.h"

ArgumentParser::ArgumentParser(QStringList &optionList,
                   QStringList &paramList,
                   QObject *parent) :
    QObject(parent)
{
    QStringList args = QCoreApplication::arguments();
    executeableName = args.at(0);
    args.removeFirst();

    foreach(QString arg, args)
    {
        foreach(QString param, paramList)
        {
            QString key = "-" + param + "=";
            if (arg.indexOf(key) == 0)
            {
                QString value = arg.mid(key.length());
                parameter.insert(param, value);
                break;
            }
        }

        foreach(QString opt, optionList)
        {
            QString key = "-" + opt;
            if (arg == key)
            {
                options.append(opt);
                break;
            }
        }
    }
}

bool ArgumentParser::isEmpty()
{
	return parameter.isEmpty() && options.isEmpty();
}

QString ArgumentParser::executableName()
{
	return executeableName;
}

bool ArgumentParser::paramExists(const QString &key)
{
	return parameter.contains(key);
}

QString ArgumentParser::param(const QString &key) const
{
	return parameter.value(key, QString());
}

bool ArgumentParser::option(const QString &key)
{
	return options.contains(key, Qt::CaseInsensitive);
}
