#pragma once
#include <QString>
#include <QList>
#include <QDateTime>
#include <QObject>

struct LogEntry {
    QDateTime timestamp;
    QString level;
    QString message;
};

class Logger : public QObject {
    Q_OBJECT
public:
    static Logger& instance();

    void info(const QString& message);
    void warning(const QString& message);
    void error(const QString& message);

    QList<LogEntry> entries() const;
    void clear();

signals:
    void logAdded(const LogEntry& entry);

private:
    Logger() = default;
    QList<LogEntry> m_entries;
    void addEntry(const QString& level, const QString& message);
};
