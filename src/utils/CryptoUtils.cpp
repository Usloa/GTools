#include "CryptoUtils.h"
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>

namespace CryptoUtils {

QByteArray hmacSha256(const QByteArray &key, const QByteArray &data) {
    return QMessageAuthenticationCode::hash(data, key, QCryptographicHash::Sha256);
}

QByteArray sha256Hash(const QByteArray &data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256);
}

}
