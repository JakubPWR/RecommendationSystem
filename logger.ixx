export module Logger;

import <QString>;
import <QFile>;
import <QTextStream>;
import <QDateTime>;
import <QDir>;
import <QCoreApplication>;

export class Logger {
public:
    static void log(const QString& operation);
};

void Logger::log(const QString& operation) {
    QString logDirPath = QDir(QCoreApplication::applicationDirPath()).filePath("logs");

    QDir dir;
    if (!dir.exists(logDirPath)) {
        dir.mkpath(logDirPath);
    }

    QString logFilePath = logDirPath + "/log.txt";

    QFile file(logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        out << timestamp << " | " << operation << "\n";
        file.close();
    }
}
