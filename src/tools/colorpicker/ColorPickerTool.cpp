#include "ColorPickerTool.h"
#include "MagnifierWidget.h"
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QScreen>
#include <QGuiApplication>
#include <QMouseEvent>
#include <windows.h>

ColorPickerTool::ColorPickerTool() {}

ColorPickerTool::~ColorPickerTool() {
    stopPicking();
}

void ColorPickerTool::activate(QWidget *) {
    startPicking();
}

void ColorPickerTool::startPicking() {
    if (m_picking) return;
    m_picking = true;

    m_magnifier = new MagnifierWidget;
    m_magnifier->show();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ColorPickerTool::samplePixel);
    m_timer->start(30);

    qApp->installEventFilter(this);
}

void ColorPickerTool::stopPicking() {
    m_picking = false;
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if (m_magnifier) {
        m_magnifier->close();
        delete m_magnifier;
        m_magnifier = nullptr;
    }
    qApp->removeEventFilter(this);
}

void ColorPickerTool::samplePixel() {
    QPoint pos = QCursor::pos();

    // Get pixel color using Win32
    HDC hdc = GetDC(nullptr);
    COLORREF color = GetPixel(hdc, pos.x(), pos.y());
    ReleaseDC(nullptr, hdc);
    QColor c(GetRValue(color), GetGValue(color), GetBValue(color));

    // Grab small region for magnifier
    QScreen *screen = QGuiApplication::screenAt(pos);
    if (screen) {
        QPixmap region = screen->grabWindow(0,
            pos.x() - screen->geometry().x() - 7,
            pos.y() - screen->geometry().y() - 7, 15, 15);
        m_magnifier->updatePixels(region, c);
    }
    m_magnifier->updatePosition(pos);

    // Check for mouse click (left button)
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        QApplication::clipboard()->setText(c.name().toUpper());
        stopPicking();
    }
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        stopPicking();
    }
}
