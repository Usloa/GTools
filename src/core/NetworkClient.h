#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <functional>

class NetworkClient : public QObject {
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);

    using SuccessCallback = std::function<void(const QByteArray &)>;
    using ErrorCallback = std::function<void(const QString &)>;

    void post(const QUrl &url, const QByteArray &body,
              const QMap<QString, QString> &headers,
              SuccessCallback onSuccess, ErrorCallback onError);

    void get(const QUrl &url, const QMap<QString, QString> &headers,
             SuccessCallback onSuccess, ErrorCallback onError);

    static NetworkClient *global();

private:
    QNetworkAccessManager *m_nam;
};
