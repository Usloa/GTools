#include <QApplication>
#include <QIcon>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QMenu>
#include "app/LauncherWindow.h"
#include "core/Config.h"
#include "core/ToolRegistry.h"
#include "utils/HotkeyManager.h"
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

    Config::instance().load();

    // Register tools
    auto &reg = ToolRegistry::instance();
    reg.registerTool(std::make_unique<OcrTool>());
    reg.registerTool(std::make_unique<ScreenshotTool>());
    reg.registerTool(std::make_unique<ColorPickerTool>());
    reg.registerTool(std::make_unique<MarkdownTool>());
    reg.registerTool(std::make_unique<TranslateTool>());

    // Launcher window
    LauncherWindow launcher;

    // Global hotkey: Alt+Space
    HotkeyManager hotkey;
    if (!hotkey.registerHotkey(1, MOD_ALT, VK_SPACE)) {
        // Fallback: try Alt+Q if Alt+Space is taken
        hotkey.registerHotkey(1, MOD_ALT, 'Q');
    }
    QObject::connect(&hotkey, &HotkeyManager::hotkeyPressed, [&](int id) {
        if (id == 1) {
            if (launcher.isVisible())
                launcher.hideLauncher();
            else
                launcher.showLauncher();
        }
    });

    // Show launcher on first start
    launcher.showLauncher();

    // System tray
    QSystemTrayIcon tray;
    tray.setIcon(QIcon(":/icons/gtools.png"));
    tray.setToolTip("GTools - Alt+Space to open");

    QMenu trayMenu;
    trayMenu.addAction(QString::fromUtf8("显示"), [&]() { launcher.showLauncher(); });
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
