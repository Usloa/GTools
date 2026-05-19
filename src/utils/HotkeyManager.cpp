#include "HotkeyManager.h"
#include <QCoreApplication>

HotkeyManager::HotkeyManager(QObject *parent) : QObject(parent) {
    QCoreApplication::instance()->installNativeEventFilter(this);
}

HotkeyManager::~HotkeyManager() {
    for (int id : m_registeredIds) {
        UnregisterHotKey(nullptr, id);
    }
    QCoreApplication::instance()->removeNativeEventFilter(this);
}

bool HotkeyManager::registerHotkey(int id, UINT modifiers, UINT vk) {
    if (RegisterHotKey(nullptr, id, modifiers, vk)) {
        m_registeredIds.append(id);
        return true;
    }
    return false;
}

void HotkeyManager::unregisterHotkey(int id) {
    UnregisterHotKey(nullptr, id);
    m_registeredIds.removeAll(id);
}

bool HotkeyManager::nativeEventFilter(const QByteArray &eventType,
                                       void *message, qintptr *) {
    if (eventType == "windows_generic_MSG") {
        auto *msg = static_cast<MSG *>(message);
        if (msg->message == WM_HOTKEY) {
            emit hotkeyPressed(static_cast<int>(msg->wParam));
            return true;
        }
    }
    return false;
}
