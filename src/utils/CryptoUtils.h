#pragma once
#include <QByteArray>

namespace CryptoUtils {
    QByteArray hmacSha256(const QByteArray &key, const QByteArray &data);
    QByteArray sha256Hash(const QByteArray &data);
}
