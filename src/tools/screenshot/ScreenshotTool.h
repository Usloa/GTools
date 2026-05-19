#pragma once
#include "core/Tool.h"

class ScreenshotTool : public QObject, public Tool {
    Q_OBJECT
public:
    QString id() const override { return "screenshot"; }
    QString name() const override { return QString::fromUtf8("截图并复制"); }
    QString keyword() const override { return "screenshot jietu"; }
    QIcon icon() const override { return QIcon(":/icons/screenshot.png"); }

    void activate(QWidget *parent) override;
};
