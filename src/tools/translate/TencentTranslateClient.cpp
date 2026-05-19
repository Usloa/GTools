#include "TencentTranslateClient.h"
#include "core/Config.h"
#include "core/NetworkClient.h"
#include "utils/CryptoUtils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QTimeZone>

TencentTranslateClient::TencentTranslateClient(QObject *parent) : QObject(parent) {}

void TencentTranslateClient::translate(const QString &text,
                                        const QString &source,
                                        const QString &target) {
    auto &cfg = Config::instance();
    if (cfg.tencentSecretId().isEmpty() || cfg.tencentSecretKey().isEmpty()) {
        emit error(QString::fromUtf8("请先配置腾讯云 SecretId/SecretKey"));
        return;
    }

    QJsonObject body;
    body["SourceText"] = text;
    body["Source"] = source;
    body["Target"] = target;
    body["ProjectId"] = 0;
    QByteArray payload = QJsonDocument(body).toJson(QJsonDocument::Compact);

    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QByteArray auth = buildAuthorization(payload, timestamp);

    QUrl url("https://tmt.tencentcloudapi.com/");
    QMap<QString, QString> headers;
    headers["Content-Type"] = "application/json; charset=utf-8";
    headers["Host"] = "tmt.tencentcloudapi.com";
    headers["X-TC-Action"] = "TextTranslate";
    headers["X-TC-Version"] = "2018-03-21";
    headers["X-TC-Timestamp"] = QString::number(timestamp);
    headers["X-TC-Region"] = "ap-guangzhou";
    headers["Authorization"] = QString::fromUtf8(auth);

    NetworkClient::global()->post(url, payload, headers,
        [this](const QByteArray &data) {
            QJsonObject obj = QJsonDocument::fromJson(data).object();
            QJsonObject response = obj["Response"].toObject();
            if (response.contains("Error")) {
                emit error(response["Error"].toObject()["Message"].toString());
            } else {
                emit translated(response["TargetText"].toString());
            }
        },
        [this](const QString &err) {
            emit error(QString::fromUtf8("翻译请求失败: ") + err);
        });
}

QByteArray TencentTranslateClient::buildAuthorization(const QByteArray &payload,
                                                       qint64 timestamp) const {
    auto &cfg = Config::instance();
    QByteArray secretId = cfg.tencentSecretId().toUtf8();
    QByteArray secretKey = cfg.tencentSecretKey().toUtf8();

    QString service = "tmt";
    QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp, QTimeZone::utc());
    QString date = dt.toString("yyyy-MM-dd");

    // Step 1: Canonical request
    QString httpMethod = "POST";
    QString canonicalUri = "/";
    QString canonicalQueryString = "";
    QString canonicalHeaders = "content-type:application/json; charset=utf-8\n"
                               "host:tmt.tencentcloudapi.com\n";
    QString signedHeaders = "content-type;host";
    QByteArray hashedPayload = CryptoUtils::sha256Hash(payload).toHex();

    QString canonicalRequest = httpMethod + "\n" + canonicalUri + "\n" +
                               canonicalQueryString + "\n" + canonicalHeaders + "\n" +
                               signedHeaders + "\n" + QString::fromUtf8(hashedPayload);

    // Step 2: String to sign
    QString algorithm = "TC3-HMAC-SHA256";
    QString credentialScope = date + "/" + service + "/tc3_request";
    QByteArray hashedCanonical = CryptoUtils::sha256Hash(canonicalRequest.toUtf8()).toHex();
    QString stringToSign = algorithm + "\n" + QString::number(timestamp) + "\n" +
                           credentialScope + "\n" + QString::fromUtf8(hashedCanonical);

    // Step 3: Signing key
    QByteArray secretDate = CryptoUtils::hmacSha256("TC3" + secretKey, date.toUtf8());
    QByteArray secretService = CryptoUtils::hmacSha256(secretDate, service.toUtf8());
    QByteArray secretSigning = CryptoUtils::hmacSha256(secretService, "tc3_request");

    // Step 4: Signature
    QByteArray signature = CryptoUtils::hmacSha256(secretSigning, stringToSign.toUtf8()).toHex();

    // Step 5: Authorization
    QString authorization = algorithm + " Credential=" + QString::fromUtf8(secretId) + "/" +
                            credentialScope + ", SignedHeaders=" + signedHeaders +
                            ", Signature=" + QString::fromUtf8(signature);

    return authorization.toUtf8();
}
