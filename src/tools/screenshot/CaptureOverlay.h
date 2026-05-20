#pragma once
#include <QWidget>
#include <QPixmap>
#include <QPoint>

class CaptureOverlay : public QWidget {
    Q_OBJECT
public:
    explicit CaptureOverlay(QWidget *parent = nullptr);
    void start();

signals:
    void regionCaptured(const QPixmap &pixmap);
    void cancelled();

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    QPixmap m_desktop;
    QPoint m_startPos;
    QPoint m_endPos;
    QPoint m_cursorPos;
    bool m_selecting = false;
    bool m_selected = false;
};
