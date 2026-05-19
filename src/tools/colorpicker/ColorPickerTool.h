#pragma once
#include "core/Tool.h"
#include <QObject>
#include <QTimer>

class MagnifierWidget;

class ColorPickerTool : public QObject, public Tool {
    Q_OBJECT
public:
    ColorPickerTool();
    ~ColorPickerTool() override;

    QString id() const override { return "colorpicker"; }
    QString name() const override { return QString::fromUtf8("屏幕取色"); }
    QString keyword() const override { return "color quese yanse"; }
    QIcon icon() const override { return QIcon(":/icons/colorpicker.png"); }

    void activate(QWidget *parent) override;

private:
    void startPicking();
    void stopPicking();
    void samplePixel();

    QTimer *m_timer = nullptr;
    MagnifierWidget *m_magnifier = nullptr;
    bool m_picking = false;
};
