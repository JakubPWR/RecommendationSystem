#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

class Logger
{
public:
    static void log(const QString& operation);
};

#endif // LOGGER_H
