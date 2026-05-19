#pragma once
#include "core/Tool.h"
#include <QObject>

class TencentTranslateClient;

class TranslateTool : public QObject, public Tool {
    Q_OBJECT
public:
    TranslateTool();

    QString id() const override { return "translate"; }
    QString name() const override { return QString::fromUtf8("快速翻译"); }
    QString keyword() const override { return "translate fanyi"; }
    QIcon icon() const override { return QIcon(":/icons/translate.png"); }

    void activate(QWidget *parent) override;

private:
    TencentTranslateClient *m_client;
};
