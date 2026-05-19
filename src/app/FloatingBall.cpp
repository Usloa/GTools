#include "FloatingBall.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLinearGradient>

FloatingBall::FloatingBall(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(48, 48);
    setCursor(Qt::PointingHandCursor);
}

void FloatingBall::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient grad(0, 0, width(), height());
    grad.setColorAt(0, QColor("#667eea"));
    grad.setColorAt(1, QColor("#764ba2"));

    p.setPen(Qt::NoPen);
    p.setBrush(grad);
    p.drawEllipse(rect().adjusted(2, 2, -2, -2));

    p.setPen(Qt::white);
    QFont f = font();
    f.setPixelSize(20);
    f.setBold(true);
    p.setFont(f);
    p.drawText(rect(), Qt::AlignCenter, "G");
}

void FloatingBall::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStart = event->globalPosition().toPoint() - frameGeometry().topLeft();
        m_dragging = false;
    }
}

void FloatingBall::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragStart);
        m_dragging = true;
    }
}

void FloatingBall::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !m_dragging) {
        emit clicked();
    }
    m_dragging = false;
}
