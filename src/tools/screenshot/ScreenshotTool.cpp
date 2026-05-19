#include "ScreenshotTool.h"
#include "CaptureOverlay.h"
#include <QApplication>
#include <QClipboard>
#include <QTimer>

void ScreenshotTool::activate(QWidget *) {
    QTimer::singleShot(200, this, [this]() {
        auto *overlay = new CaptureOverlay;
        connect(overlay, &CaptureOverlay::regionCaptured, this, [](const QPixmap &pix) {
            QApplication::clipboard()->setPixmap(pix);
        });
        overlay->start();
    });
}
