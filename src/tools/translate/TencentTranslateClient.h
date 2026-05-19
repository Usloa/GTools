#pragma once
#include <QObject>

class TencentTranslateClient : public QObject {
    Q_OBJECT
public:
    explicit TencentTranslateClient(QObject *parent = nullptr);

    void translate(const QString &text, const QString &source, const QString &target);

signals:
    void translated(const QString &result);
    void error(const QString &message);

private:
    QByteArray buildAuthorization(const QByteArray &payload, qint64 timestamp) const;
};
