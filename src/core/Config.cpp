#include "Config.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

Config &Config::instance() {
    static Config cfg;
    return cfg;
}

QString Config::configPath() const {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + "/config.json";
}

void Config::load() {
    QFile file(configPath());
    if (file.open(QIODevice::ReadOnly)) {
        m_data = QJsonDocument::fromJson(file.readAll()).object();
    }
}

void Config::save() {
    QFile file(configPath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(m_data).toJson(QJsonDocument::Indented));
    }
}

QString Config::baiduApiKey() const { return m_data["baidu_api_key"].toString(); }
QString Config::baiduSecretKey() const { return m_data["baidu_secret_key"].toString(); }

void Config::setBaiduCredentials(const QString &apiKey, const QString &secretKey) {
    m_data["baidu_api_key"] = apiKey;
    m_data["baidu_secret_key"] = secretKey;
    save();
}

QString Config::tencentSecretId() const { return m_data["tencent_secret_id"].toString(); }
QString Config::tencentSecretKey() const { return m_data["tencent_secret_key"].toString(); }

void Config::setTencentCredentials(const QString &secretId, const QString &secretKey) {
    m_data["tencent_secret_id"] = secretId;
    m_data["tencent_secret_key"] = secretKey;
    save();
}

bool Config::startOnBoot() const { return m_data["start_on_boot"].toBool(false); }

void Config::setStartOnBoot(bool enabled) {
    m_data["start_on_boot"] = enabled;
    save();
}
