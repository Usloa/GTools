#include "BaiduOcrClient.h"
#include "core/Config.h"
#include "core/NetworkClient.h"
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

BaiduOcrClient::BaiduOcrClient(QObject *parent) : QObject(parent) {}

void BaiduOcrClient::recognize(const QPixmap &image) {
    // Scale down large images to speed up upload
    QPixmap scaled = image;
    if (scaled.width() > 1920 || scaled.height() > 1080) {
        scaled = scaled.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    scaled.save(&buf, "JPEG", 70);
    m_pendingImage = ba.toBase64();

    if (m_accessToken.isEmpty() || QDateTime::currentDateTime() >= m_tokenExpiry) {
        refreshToken();
    } else {
        doRecognize(m_pendingImage);
    }
}

void BaiduOcrClient::refreshToken() {
    auto &cfg = Config::instance();
    QString apiKey = cfg.baiduApiKey();
    QString secretKey = cfg.baiduSecretKey();

    if (apiKey.isEmpty() || secretKey.isEmpty()) {
        emit error(QString::fromUtf8("请先配置百度 OCR API Key"));
        return;
    }

    QUrl url("https://aip.baidubce.com/oauth/2.0/token");
    QUrlQuery query;
    query.addQueryItem("grant_type", "client_credentials");
    query.addQueryItem("client_id", apiKey);
    query.addQueryItem("client_secret", secretKey);
    url.setQuery(query);

    QMap<QString, QString> headers;
    headers["Content-Type"] = "application/x-www-form-urlencoded";

    NetworkClient::global()->post(url, QByteArray(), headers,
        [this](const QByteArray &data) {
            QJsonObject obj = QJsonDocument::fromJson(data).object();
            m_accessToken = obj["access_token"].toString();
            int expiresIn = obj["expires_in"].toInt(2592000);
            m_tokenExpiry = QDateTime::currentDateTime().addSecs(expiresIn - 600);
            doRecognize(m_pendingImage);
        },
        [this](const QString &err) {
            emit error(QString::fromUtf8("获取 Token 失败: ") + err);
        });
}

void BaiduOcrClient::doRecognize(const QByteArray &imageBase64) {
    QUrl url(QString("https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic"
                     "?access_token=%1").arg(m_accessToken));

    QByteArray body = "image=" + QUrl::toPercentEncoding(imageBase64);

    QMap<QString, QString> headers;
    headers["Content-Type"] = "application/x-www-form-urlencoded";

    NetworkClient::global()->post(url, body, headers,
        [this](const QByteArray &data) {
            QJsonObject obj = QJsonDocument::fromJson(data).object();
            if (obj.contains("error_code")) {
                emit error(obj["error_msg"].toString());
                return;
            }
            QJsonArray words = obj["words_result"].toArray();
            QStringList lines;
            for (const auto &w : words) {
                lines.append(w.toObject()["words"].toString());
            }
            emit recognized(lines.join("\n"));
        },
        [this](const QString &err) {
            emit error(QString::fromUtf8("识别失败: ") + err);
        });
}
