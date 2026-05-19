#include "OcrTool.h"
#include "BaiduOcrClient.h"
#include "tools/screenshot/CaptureOverlay.h"
#include "utils/Notifier.h"
#include "utils/Logger.h"
#include <QApplication>
#include <QClipboard>
#include <QTimer>

OcrTool::OcrTool() : m_client(new BaiduOcrClient(this)) {
    connect(m_client, &BaiduOcrClient::recognized, this, [](const QString &text) {
        QApplication::clipboard()->setText(text);
        Logger::instance().info(QString::fromUtf8("OCR 识别成功: %1 字符").arg(text.length()));
        Notifier::showSuccess(QString::fromUtf8("文字已复制到剪贴板"));
    });
    connect(m_client, &BaiduOcrClient::error, this, [](const QString &msg) {
        Logger::instance().error(QString::fromUtf8("OCR 失败: %1").arg(msg));
        Notifier::showError(msg);
    });
}

void OcrTool::activate(QWidget *) {
    QTimer::singleShot(200, this, [this]() {
        auto *overlay = new CaptureOverlay;
        connect(overlay, &CaptureOverlay::regionCaptured, this, [this](const QPixmap &pix) {
            m_client->recognize(pix);
        });
        overlay->start();
    });
}
