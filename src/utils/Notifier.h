#pragma once
#include <QString>

class Notifier {
public:
    static void show(const QString &title, const QString &message);
    static void showError(const QString &message);
    static void showSuccess(const QString &message);
};
