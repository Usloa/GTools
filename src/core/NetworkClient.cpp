#include "NetworkClient.h"
#include <QNetworkRequest>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent), m_nam(new QNetworkAccessManager(this)) {}

NetworkClient *NetworkClient::global() {
    static NetworkClient *instance = new NetworkClient;
    return instance;
}

void NetworkClient::post(const QUrl &url, const QByteArray &body,
                         const QMap<QString, QString> &headers,
                         SuccessCallback onSuccess, ErrorCallback onError) {
    QNetworkRequest req(url);
    req.setTransferTimeout(15000);
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    auto *reply = m_nam->post(req, body);
    connect(reply, &QNetworkReply::finished, this, [reply, onSuccess, onError]() {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
            if (onSuccess) onSuccess(reply->readAll());
        } else {
            if (onError) onError(reply->errorString());
        }
    });
}

void NetworkClient::get(const QUrl &url, const QMap<QString, QString> &headers,
                        SuccessCallback onSuccess, ErrorCallback onError) {
    QNetworkRequest req(url);
    req.setTransferTimeout(15000);
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    auto *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, onSuccess, onError]() {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
            if (onSuccess) onSuccess(reply->readAll());
        } else {
            if (onError) onError(reply->errorString());
        }
    });
}
