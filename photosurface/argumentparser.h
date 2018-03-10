#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QtCore>

class ArgumentParser : public QObject
{
	Q_OBJECT

private:
    QHash<QString, QString>	parameter;
	QStringList		options;
    QString			executeableName;

public:
    explicit ArgumentParser(QStringList &optionsList,
                QStringList &paramList,
                QObject *parent = 0);

	
	bool	isEmpty();
	QString executableName();

	bool	paramExists(const QString &key);
	QString param(const QString &key) const;
	bool	option(const QString &key);
};

#endif // ARGUMENTPARSER_H
