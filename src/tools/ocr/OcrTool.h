#pragma once
#include "core/Tool.h"
#include <QObject>

class BaiduOcrClient;

class OcrTool : public QObject, public Tool {
    Q_OBJECT
public:
    OcrTool();

    QString id() const override { return "ocr"; }
    QString name() const override { return QString::fromUtf8("OCR 文字识别"); }
    QString keyword() const override { return "ocr wenzi shibie"; }
    QIcon icon() const override { return QIcon(":/icons/ocr.png"); }

    void activate(QWidget *parent) override;

private:
    BaiduOcrClient *m_client;
};
