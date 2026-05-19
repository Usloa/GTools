#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>

class Tool {
public:
    virtual ~Tool() = default;

    virtual QString id() const = 0;
    virtual QString name() const = 0;
    virtual QString keyword() const = 0;
    virtual QIcon icon() const = 0;

    virtual void activate(QWidget *parent) = 0;
    virtual QWidget *inlineWidget() { return nullptr; }
};
