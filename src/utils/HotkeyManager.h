#pragma once
#include <QObject>
#include <QAbstractNativeEventFilter>
#include <windows.h>

class HotkeyManager : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
public:
    explicit HotkeyManager(QObject *parent = nullptr);
    ~HotkeyManager() override;

    bool registerHotkey(int id, UINT modifiers, UINT vk);
    void unregisterHotkey(int id);

    bool nativeEventFilter(const QByteArray &eventType,
                           void *message, qintptr *result) override;

signals:
    void hotkeyPressed(int id);

private:
    QList<int> m_registeredIds;
};
