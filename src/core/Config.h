#pragma once
#include <QJsonObject>
#include <QString>

class Config {
public:
    static Config &instance();

    void load();
    void save();

    QString baiduApiKey() const;
    QString baiduSecretKey() const;
    void setBaiduCredentials(const QString &apiKey, const QString &secretKey);

    QString tencentSecretId() const;
    QString tencentSecretKey() const;
    void setTencentCredentials(const QString &secretId, const QString &secretKey);

    bool startOnBoot() const;
    void setStartOnBoot(bool enabled);

private:
    Config() = default;
    QString configPath() const;

    QJsonObject m_data;
};
