#pragma once
#include <QWidget>
#include <QColor>
#include <QPixmap>

class MagnifierWidget : public QWidget {
    Q_OBJECT
public:
    explicit MagnifierWidget(QWidget *parent = nullptr);
    void updatePosition(const QPoint &cursorPos);
    void updatePixels(const QPixmap &region, const QColor &centerColor);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QPixmap m_region;
    QColor m_centerColor;
};
