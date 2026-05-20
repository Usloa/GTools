#include "CaptureOverlay.h"
#include "utils/ScreenGrabber.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QGuiApplication>
#include <QScreen>

CaptureOverlay::CaptureOverlay(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::BypassWindowManagerHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);
}

void CaptureOverlay::start() {
    m_desktop = ScreenGrabber::grabAllScreens();
    QRect vr = ScreenGrabber::virtualDesktopRect();
    setGeometry(vr);
    m_cursorPos = QCursor::pos() - vr.topLeft();
    showFullScreen();
    activateWindow();
    setFocus();
    grabMouse();
    grabKeyboard();
}

void CaptureOverlay::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.drawPixmap(0, 0, m_desktop);

    // Dark overlay
    p.fillRect(rect(), QColor(0, 0, 0, 100));

    if (m_selecting || m_selected) {
        QRect sel = QRect(m_startPos, m_endPos).normalized();
        // Clear the selected region to show original image
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.drawPixmap(sel, m_desktop, sel);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);

        // Border
        p.setPen(QPen(QColor(0, 120, 215), 2));
        p.setBrush(Qt::NoBrush);
        p.drawRect(sel);

        // Size label
        QString sizeText = QString("%1 x %2").arg(sel.width()).arg(sel.height());
        QFont f = p.font();
        f.setPixelSize(12);
        p.setFont(f);
        QRect textRect(sel.left(), sel.top() - 20, 100, 18);
        p.setPen(Qt::white);
        p.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, sizeText);
    }

    // Crosshair lines at cursor position
    p.setPen(QPen(QColor(255, 255, 255, 200), 1, Qt::DashLine));
    p.drawLine(m_cursorPos.x(), 0, m_cursorPos.x(), height());
    p.drawLine(0, m_cursorPos.y(), width(), m_cursorPos.y());
}

void CaptureOverlay::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        m_startPos = e->pos();
        m_endPos = e->pos();
        m_selecting = true;
        m_selected = false;
        update();
    } else if (e->button() == Qt::RightButton) {
        releaseMouse();
        releaseKeyboard();
        emit cancelled();
        close();
    }
}

void CaptureOverlay::mouseMoveEvent(QMouseEvent *e) {
    m_cursorPos = e->pos();
    if (m_selecting) {
        m_endPos = e->pos();
    }
    update();
}

void CaptureOverlay::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && m_selecting) {
        m_selecting = false;
        m_endPos = e->pos();
        QRect sel = QRect(m_startPos, m_endPos).normalized();
        if (sel.width() > 3 && sel.height() > 3) {
            QPixmap captured = m_desktop.copy(sel);
            releaseMouse();
            releaseKeyboard();
            emit regionCaptured(captured);
            close();
        } else {
            m_selected = false;
            update();
        }
    }
}

void CaptureOverlay::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        releaseMouse();
        releaseKeyboard();
        emit cancelled();
        close();
    }
}
