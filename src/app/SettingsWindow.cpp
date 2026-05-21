#include "SettingsWindow.h"
#include "core/Config.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QTimer>
#include <QKeySequenceEdit>

SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadSettings();
}

void SettingsWindow::setupUI() {
    setWindowTitle(QString::fromUtf8("GTools 设置"));
    setWindowFlags(Qt::Window);
    setFixedSize(550, 500);

    setStyleSheet(
        "QWidget {"
        "  background: #f8f9fa;"
        "  font-family: 'Microsoft YaHei', 'Segoe UI', sans-serif;"
        "}"
        "QTabWidget::pane {"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  background: #ffffff;"
        "}"
        "QTabBar::tab {"
        "  background: #e0e0e0;"
        "  border: none;"
        "  padding: 10px 20px;"
        "  margin-right: 2px;"
        "  border-top-left-radius: 6px;"
        "  border-top-right-radius: 6px;"
        "}"
        "QTabBar::tab:selected {"
        "  background: #ffffff;"
        "  color: #667eea;"
        "  font-weight: bold;"
        "}"
        "QTabBar::tab:hover:!selected {"
        "  background: #f0f0f0;"
        "}"
        "QGroupBox {"
        "  font-weight: bold;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  margin-top: 12px;"
        "  padding-top: 16px;"
        "  background: #ffffff;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  left: 12px;"
        "  padding: 0 8px;"
        "}"
        "QLineEdit {"
        "  background: #ffffff;"
        "  border: 2px solid #e0e0e0;"
        "  border-radius: 6px;"
        "  padding: 8px 12px;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #667eea;"
        "}"
        "QCheckBox {"
        "  spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "  width: 20px;"
        "  height: 20px;"
        "  border-radius: 4px;"
        "  border: 2px solid #e0e0e0;"
        "  background: #ffffff;"
        "}"
        "QCheckBox::indicator:checked {"
        "  border-color: #667eea;"
        "  image: url(E:/GTools/pic/勾选框已打勾.png);"
        "}"
    );

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_tabWidget = new QTabWidget(this);
    layout->addWidget(m_tabWidget);

    setupApiTab();
    setupGeneralTab();
    setupPreferenceTab();
    setupAdvancedTab();

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: #26de81; font-weight: bold;");
    m_statusLabel->hide();
    layout->addWidget(m_statusLabel);

    m_saveBtn = new QPushButton(QString::fromUtf8("保存"), this);
    m_saveBtn->setFixedHeight(44);
    m_saveBtn->setStyleSheet(
        "QPushButton {"
        "  background: #667eea;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background: #5a6fd6;"
        "}"
        "QPushButton:pressed {"
        "  background: #4a5fc6;"
        "}"
    );
    connect(m_saveBtn, &QPushButton::clicked, this, &SettingsWindow::onSaveClicked);
    layout->addWidget(m_saveBtn);
}

void SettingsWindow::setupApiTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(16);

    auto *baiduGroup = new QGroupBox(QString::fromUtf8("百度 OCR"), tab);
    auto *baiduLayout = new QFormLayout(baiduGroup);
    baiduLayout->setSpacing(10);

    m_baiduApiKey = new QLineEdit();
    m_baiduApiKey->setEchoMode(QLineEdit::Password);
    m_baiduApiKey->setPlaceholderText(QString::fromUtf8("输入 API Key"));
    baiduLayout->addRow(QString::fromUtf8("API Key:"), m_baiduApiKey);

    m_baiduSecretKey = new QLineEdit();
    m_baiduSecretKey->setEchoMode(QLineEdit::Password);
    m_baiduSecretKey->setPlaceholderText(QString::fromUtf8("输入 Secret Key"));
    baiduLayout->addRow(QString::fromUtf8("Secret Key:"), m_baiduSecretKey);

    layout->addWidget(baiduGroup);

    auto *tencentGroup = new QGroupBox(QString::fromUtf8("腾讯翻译"), tab);
    auto *tencentLayout = new QFormLayout(tencentGroup);
    tencentLayout->setSpacing(10);

    m_tencentSecretId = new QLineEdit();
    m_tencentSecretId->setEchoMode(QLineEdit::Password);
    m_tencentSecretId->setPlaceholderText(QString::fromUtf8("输入 SecretId"));
    tencentLayout->addRow(QString::fromUtf8("SecretId:"), m_tencentSecretId);

    m_tencentSecretKey = new QLineEdit();
    m_tencentSecretKey->setEchoMode(QLineEdit::Password);
    m_tencentSecretKey->setPlaceholderText(QString::fromUtf8("输入 SecretKey"));
    tencentLayout->addRow(QString::fromUtf8("SecretKey:"), m_tencentSecretKey);

    layout->addWidget(tencentGroup);
    layout->addStretch();

    m_tabWidget->addTab(tab, QString::fromUtf8("API 配置"));
}

void SettingsWindow::setupGeneralTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(16);

    auto *infoLabel = new QLabel(QString::fromUtf8("通用设置项将在后续版本中添加。"), tab);
    infoLabel->setStyleSheet("color: #888; font-style: italic;");
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);
    layout->addStretch();

    m_tabWidget->addTab(tab, QString::fromUtf8("通用"));
}

void SettingsWindow::setupPreferenceTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(16);

    auto *hotkeyGroup = new QGroupBox(QString::fromUtf8("快捷键设置"), tab);
    auto *hotkeyLayout = new QFormLayout(hotkeyGroup);
    hotkeyLayout->setSpacing(10);

    m_hotkeyShowLauncher = new QLineEdit();
    m_hotkeyShowLauncher->setPlaceholderText(QString::fromUtf8("例如: Alt+Space"));
    hotkeyLayout->addRow(QString::fromUtf8("显示启动器:"), m_hotkeyShowLauncher);

    m_hotkeyDetachWindow = new QLineEdit();
    m_hotkeyDetachWindow->setPlaceholderText(QString::fromUtf8("例如: Alt+D"));
    hotkeyLayout->addRow(QString::fromUtf8("分离窗口:"), m_hotkeyDetachWindow);

    layout->addWidget(hotkeyGroup);
    layout->addStretch();

    m_tabWidget->addTab(tab, QString::fromUtf8("使用偏好"));
}

void SettingsWindow::setupAdvancedTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(16);

    auto *systemGroup = new QGroupBox(QString::fromUtf8("系统设置"), tab);
    auto *systemLayout = new QVBoxLayout(systemGroup);
    systemLayout->setSpacing(12);

    m_startOnBoot = new QCheckBox(QString::fromUtf8("开机自启动"), tab);
    systemLayout->addWidget(m_startOnBoot);

    layout->addWidget(systemGroup);
    layout->addStretch();

    m_tabWidget->addTab(tab, QString::fromUtf8("高级设置"));
}

void SettingsWindow::loadSettings() {
    auto &cfg = Config::instance();
    m_baiduApiKey->setText(cfg.baiduApiKey());
    m_baiduSecretKey->setText(cfg.baiduSecretKey());
    m_tencentSecretId->setText(cfg.tencentSecretId());
    m_tencentSecretKey->setText(cfg.tencentSecretKey());
    m_hotkeyShowLauncher->setText(cfg.hotkeyShowLauncher());
    m_hotkeyDetachWindow->setText(cfg.hotkeyDetachWindow());
    m_startOnBoot->setChecked(cfg.startOnBoot());
}

void SettingsWindow::onSaveClicked() {
    auto &cfg = Config::instance();
    cfg.setBaiduCredentials(m_baiduApiKey->text(), m_baiduSecretKey->text());
    cfg.setTencentCredentials(m_tencentSecretId->text(), m_tencentSecretKey->text());
    cfg.setHotkeyShowLauncher(m_hotkeyShowLauncher->text());
    cfg.setHotkeyDetachWindow(m_hotkeyDetachWindow->text());
    cfg.setStartOnBoot(m_startOnBoot->isChecked());

    m_statusLabel->setText(QString::fromUtf8("✓ 设置已保存"));
    m_statusLabel->show();

    QTimer::singleShot(2000, this, [this]() {
        m_statusLabel->hide();
    });
}
