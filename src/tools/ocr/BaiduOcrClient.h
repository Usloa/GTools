#pragma once
#include <QObject>
#include <QPixmap>
#include <QDateTime>

class BaiduOcrClient : public QObject {
    Q_OBJECT
public:
    explicit BaiduOcrClient(QObject *parent = nullptr);
    void recognize(const QPixmap &image);

signals:
    void recognized(const QString &text);
    void error(const QString &message);

private:
    void refreshToken();
    void doRecognize(const QByteArray &imageBase64);

    QString m_accessToken;
    QDateTime m_tokenExpiry;
    QByteArray m_pendingImage;
};
