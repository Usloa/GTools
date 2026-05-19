#include "OcrTool.h"
#include "BaiduOcrClient.h"
#include "tools/screenshot/CaptureOverlay.h"
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QTimer>

OcrTool::OcrTool() : m_client(new BaiduOcrClient(this)) {
    connect(m_client, &BaiduOcrClient::recognized, this, [](const QString &text) {
        QApplication::clipboard()->setText(text);
    });
    connect(m_client, &BaiduOcrClient::error, this, [](const QString &msg) {
        QMessageBox::warning(nullptr, "OCR", msg);
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
