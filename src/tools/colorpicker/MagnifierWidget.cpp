#include "MagnifierWidget.h"
#include <QPainter>

MagnifierWidget::MagnifierWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::ToolTip | Qt::BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(140, 170);
}

void MagnifierWidget::updatePosition(const QPoint &cursorPos) {
    move(cursorPos.x() + 20, cursorPos.y() + 20);
}

void MagnifierWidget::updatePixels(const QPixmap &region, const QColor &centerColor) {
    m_region = region;
    m_centerColor = centerColor;
    update();
}

void MagnifierWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    p.setBrush(QColor(40, 40, 40, 230));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(rect(), 8, 8);

    // Magnified pixels
    if (!m_region.isNull()) {
        QRect magRect(10, 10, 120, 120);
        p.drawPixmap(magRect, m_region.scaled(120, 120, Qt::KeepAspectRatio,
                                               Qt::FastTransformation));
        // Crosshair
        int cx = magRect.center().x();
        int cy = magRect.center().y();
        p.setPen(QPen(Qt::white, 1));
        p.drawLine(cx - 6, cy, cx + 6, cy);
        p.drawLine(cx, cy - 6, cx, cy + 6);
    }

    // Color info
    p.setPen(Qt::white);
    QFont f = p.font();
    f.setPixelSize(11);
    p.setFont(f);

    QString hex = m_centerColor.name().toUpper();
    QString rgb = QString("RGB(%1,%2,%3)")
                      .arg(m_centerColor.red())
                      .arg(m_centerColor.green())
                      .arg(m_centerColor.blue());
    p.drawText(QRect(10, 135, 120, 15), Qt::AlignCenter, hex);
    p.drawText(QRect(10, 150, 120, 15), Qt::AlignCenter, rgb);
}
