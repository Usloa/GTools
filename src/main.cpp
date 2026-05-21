#include <QApplication>
#include <QIcon>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QMenu>
#include "app/LauncherWindow.h"
#include "app/LogWindow.h"
#include "app/SettingsWindow.h"
#include "app/ToolPanel.h"
#include "core/Config.h"
#include "core/ToolRegistry.h"
#include "utils/HotkeyManager.h"
#include "utils/Logger.h"
#include "tools/screenshot/ScreenshotTool.h"
#include "tools/ocr/OcrTool.h"
#include "tools/colorpicker/ColorPickerTool.h"
#include "tools/translate/TranslateTool.h"
#include "tools/markdown/MarkdownTool.h"
#include <windows.h>

int main(int argc, char *argv[]) {
    QSharedMemory guard("GTools_SingleInstance_Guard");
    if (!guard.create(1)) {
        return 0;
    }

    QApplication app(argc, argv);
    app.setApplicationName("GTools");
    app.setOrganizationName("GTools");
    app.setQuitOnLastWindowClosed(false);
    app.setWindowIcon(QIcon("E:/GTools/pic/GTools.png"));

    Config::instance().load();

    Logger::instance().info(QString::fromUtf8("GTools 启动"));

    // Register tools
    auto &reg = ToolRegistry::instance();
    reg.registerTool(std::make_unique<OcrTool>());
    reg.registerTool(std::make_unique<ScreenshotTool>());
    reg.registerTool(std::make_unique<ColorPickerTool>());
    reg.registerTool(std::make_unique<MarkdownTool>());
    reg.registerTool(std::make_unique<TranslateTool>());

    // Launcher window
    LauncherWindow launcher;

    // Tool panel (竖版工具面板)
    ToolPanel toolPanel;

    // Log window
    LogWindow logWindow;

    // Settings window
    SettingsWindow settingsWindow;

    // Connect launcher menu signals
    QObject::connect(&launcher, &LauncherWindow::settingsRequested, [&]() {
        settingsWindow.show(); settingsWindow.raise(); settingsWindow.activateWindow();
    });
    QObject::connect(&launcher, &LauncherWindow::logRequested, [&]() {
        logWindow.show(); logWindow.raise(); logWindow.activateWindow();
    });

    // Connect tool panel signals
    QObject::connect(&toolPanel, &ToolPanel::toolActivated, [&](const QString &toolId) {
        Tool *tool = ToolRegistry::instance().findById(toolId);
        if (tool) {
            tool->activate(&toolPanel);
        }
    });
    QObject::connect(&toolPanel, &ToolPanel::settingsRequested, [&]() {
        settingsWindow.show();
        settingsWindow.raise();
        settingsWindow.activateWindow();
    });

    // Global hotkey: Alt+Space - 显示工具面板
    HotkeyManager hotkey;
    if (!hotkey.registerHotkey(1, MOD_ALT, VK_SPACE)) {
        hotkey.registerHotkey(1, MOD_ALT, 'Q');
    }
    QObject::connect(&hotkey, &HotkeyManager::hotkeyPressed, [&](int id) {
        if (id == 1) {
            if (toolPanel.isVisible()) {
                toolPanel.hide();
            } else {
                toolPanel.show();
                toolPanel.raise();
                toolPanel.activateWindow();
            }
        }
    });

    // Hotkey 2: Alt+D - 显示/隐藏启动器（分离窗口），同时关闭工具面板
    hotkey.registerHotkey(2, MOD_ALT, 'D');
    QObject::connect(&hotkey, &HotkeyManager::hotkeyPressed, [&](int id) {
        if (id == 2) {
            if (launcher.isVisible()) {
                launcher.hideLauncher();
            } else {
                toolPanel.hide();  // 关闭竖版工具面板
                launcher.setAutoHideOnDeactivate(false);  // 禁用失焦关闭
                launcher.showLauncher();
            }
        }
    });

    // Show tool panel on first start
    toolPanel.show();

    // System tray
    QSystemTrayIcon tray;
    tray.setIcon(QIcon("E:/GTools/pic/GTools.png"));
    tray.setToolTip("GTools - Alt+Space to open");

    QMenu trayMenu;
    trayMenu.addAction(QString::fromUtf8("显示"), [&]() { launcher.showLauncher(); });
    trayMenu.addAction(QString::fromUtf8("设置"), [&]() { settingsWindow.show(); settingsWindow.raise(); settingsWindow.activateWindow(); });
    trayMenu.addAction(QString::fromUtf8("日志"), [&]() { logWindow.show(); logWindow.raise(); logWindow.activateWindow(); });
    trayMenu.addSeparator();
    trayMenu.addAction(QString::fromUtf8("退出"), [&]() { app.quit(); });
    tray.setContextMenu(&trayMenu);
    tray.show();

    QObject::connect(&tray, &QSystemTrayIcon::activated,
                     [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            launcher.showLauncher();
        }
    });

    return app.exec();
}
