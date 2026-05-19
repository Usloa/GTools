#include "Logger.h"

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::addEntry(const QString& level, const QString& message) {
    LogEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.level = level;
    entry.message = message;
    m_entries.prepend(entry);
    if (m_entries.size() > 500) {
        m_entries.removeLast();
    }
    emit logAdded(entry);
}

void Logger::info(const QString& message) {
    addEntry(QString::fromUtf8("INFO"), message);
}

void Logger::warning(const QString& message) {
    addEntry(QString::fromUtf8("WARN"), message);
}

void Logger::error(const QString& message) {
    addEntry(QString::fromUtf8("ERROR"), message);
}

QList<LogEntry> Logger::entries() const {
    return m_entries;
}

void Logger::clear() {
    m_entries.clear();
}
