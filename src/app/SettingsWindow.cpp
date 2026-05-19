#include "SettingsWindow.h"
#include "core/Config.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QTimer>

SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadSettings();
}

void SettingsWindow::setupUI() {
    setWindowTitle(QString::fromUtf8("GTools 设置"));
    setWindowFlags(Qt::Window);
    setFixedSize(500, 400);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    auto *baiduGroup = new QGroupBox(QString::fromUtf8("百度 OCR"), this);
    baiduGroup->setStyleSheet(
        "QGroupBox {"
        "  font-weight: bold;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  margin-top: 12px;"
        "  padding-top: 16px;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  left: 12px;"
        "  padding: 0 8px;"
        "}"
    );
    auto *baiduLayout = new QFormLayout(baiduGroup);
    baiduLayout->setSpacing(10);

    m_baiduApiKey = new QLineEdit(this);
    m_baiduApiKey->setEchoMode(QLineEdit::Password);
    m_baiduApiKey->setPlaceholderText(QString::fromUtf8("输入 API Key"));
    m_baiduApiKey->setStyleSheet("QLineEdit { padding: 8px; border-radius: 4px; }");
    baiduLayout->addRow(QString::fromUtf8("API Key:"), m_baiduApiKey);

    m_baiduSecretKey = new QLineEdit(this);
    m_baiduSecretKey->setEchoMode(QLineEdit::Password);
    m_baiduSecretKey->setPlaceholderText(QString::fromUtf8("输入 Secret Key"));
    m_baiduSecretKey->setStyleSheet("QLineEdit { padding: 8px; border-radius: 4px; }");
    baiduLayout->addRow(QString::fromUtf8("Secret Key:"), m_baiduSecretKey);

    layout->addWidget(baiduGroup);

    auto *tencentGroup = new QGroupBox(QString::fromUtf8("腾讯翻译"), this);
    tencentGroup->setStyleSheet(
        "QGroupBox {"
        "  font-weight: bold;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  margin-top: 12px;"
        "  padding-top: 16px;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  left: 12px;"
        "  padding: 0 8px;"
        "}"
    );
    auto *tencentLayout = new QFormLayout(tencentGroup);
    tencentLayout->setSpacing(10);

    m_tencentSecretId = new QLineEdit(this);
    m_tencentSecretId->setEchoMode(QLineEdit::Password);
    m_tencentSecretId->setPlaceholderText(QString::fromUtf8("输入 SecretId"));
    m_tencentSecretId->setStyleSheet("QLineEdit { padding: 8px; border-radius: 4px; }");
    tencentLayout->addRow(QString::fromUtf8("SecretId:"), m_tencentSecretId);

    m_tencentSecretKey = new QLineEdit(this);
    m_tencentSecretKey->setEchoMode(QLineEdit::Password);
    m_tencentSecretKey->setPlaceholderText(QString::fromUtf8("输入 SecretKey"));
    m_tencentSecretKey->setStyleSheet("QLineEdit { padding: 8px; border-radius: 4px; }");
    tencentLayout->addRow(QString::fromUtf8("SecretKey:"), m_tencentSecretKey);

    layout->addWidget(tencentGroup);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: #26de81; font-weight: bold;");
    m_statusLabel->hide();
    layout->addWidget(m_statusLabel);

    layout->addStretch();

    m_saveBtn = new QPushButton(QString::fromUtf8("保存"), this);
    m_saveBtn->setFixedHeight(40);
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

void SettingsWindow::loadSettings() {
    auto &cfg = Config::instance();
    m_baiduApiKey->setText(cfg.baiduApiKey());
    m_baiduSecretKey->setText(cfg.baiduSecretKey());
    m_tencentSecretId->setText(cfg.tencentSecretId());
    m_tencentSecretKey->setText(cfg.tencentSecretKey());
}

void SettingsWindow::onSaveClicked() {
    auto &cfg = Config::instance();
    cfg.setBaiduCredentials(m_baiduApiKey->text(), m_baiduSecretKey->text());
    cfg.setTencentCredentials(m_tencentSecretId->text(), m_tencentSecretKey->text());

    m_statusLabel->setText(QString::fromUtf8("✓ 设置已保存"));
    m_statusLabel->show();

    QTimer::singleShot(2000, this, [this]() {
        m_statusLabel->hide();
    });
}
