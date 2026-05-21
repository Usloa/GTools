#include "ScreenGrabber.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <windows.h>

QRect ScreenGrabber::virtualDesktopRect() {
    int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    return QRect(x, y, w, h);
}

QPixmap ScreenGrabber::grabAllScreens() {
    QRect vr = virtualDesktopRect();
    HDC hScreen = GetDC(nullptr);
    HDC hMem = CreateCompatibleDC(hScreen);
    HBITMAP hBmp = CreateCompatibleBitmap(hScreen, vr.width(), vr.height());
    SelectObject(hMem, hBmp);
    BitBlt(hMem, 0, 0, vr.width(), vr.height(),
           hScreen, vr.x(), vr.y(), SRCCOPY);

    QImage img(vr.width(), vr.height(), QImage::Format_RGB32);
    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(bi);
    bi.biWidth = vr.width();
    bi.biHeight = -vr.height();
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    GetDIBits(hMem, hBmp, 0, vr.height(), img.bits(),
              reinterpret_cast<BITMAPINFO *>(&bi), DIB_RGB_COLORS);

    DeleteObject(hBmp);
    DeleteDC(hMem);
    ReleaseDC(nullptr, hScreen);

    QPixmap pix = QPixmap::fromImage(img);
    qreal dpr = QGuiApplication::primaryScreen()->devicePixelRatio();
    pix.setDevicePixelRatio(dpr);
    return pix;
}

QPixmap ScreenGrabber::grabRegion(const QRect &region) {
    QPixmap full = grabAllScreens();
    QRect vr = virtualDesktopRect();
    QRect adjusted = region.translated(-vr.topLeft());
    return full.copy(adjusted);
}
