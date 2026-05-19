#pragma once
#include "core/Tool.h"
#include <QObject>

class MarkdownTool : public QObject, public Tool {
    Q_OBJECT
public:
    QString id() const override { return "markdown"; }
    QString name() const override { return QString::fromUtf8("Markdown 编辑器"); }
    QString keyword() const override { return "markdown md bianji"; }
    QIcon icon() const override { return QIcon(":/icons/markdown.png"); }

    void activate(QWidget *parent) override;
};
