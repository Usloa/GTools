#include "Notifier.h"
#include <QSystemTrayIcon>
#include <QApplication>

void Notifier::show(const QString &title, const QString &message) {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
        static QSystemTrayIcon *tray = nullptr;
        if (!tray) {
            tray = new QSystemTrayIcon(qApp);
            tray->setIcon(QIcon("E:/GTools/pic/GTools.png"));
            tray->show();
        }
        tray->showMessage(title, message, icon, 3000);
    }
}

void Notifier::showError(const QString &message) {
    show(QString::fromUtf8("错误"), message);
}

void Notifier::showSuccess(const QString &message) {
    show(QString::fromUtf8("成功"), message);
}
