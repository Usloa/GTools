#pragma once
#include <QPixmap>
#include <QRect>

class ScreenGrabber {
public:
    static QPixmap grabAllScreens();
    static QPixmap grabRegion(const QRect &region);
    static QRect virtualDesktopRect();
};
