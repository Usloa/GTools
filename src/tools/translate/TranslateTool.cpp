#include "TranslateTool.h"
#include "TencentTranslateClient.h"
#include "tools/ocr/BaiduOcrClient.h"
#include "tools/screenshot/CaptureOverlay.h"
#include "utils/Logger.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QClipboard>
#include <QTimer>

TranslateTool::TranslateTool()
    : m_client(new TencentTranslateClient(this))
    , m_ocrClient(new BaiduOcrClient(this)) {}

void TranslateTool::activate(QWidget *) {
    connect(m_ocrClient, &BaiduOcrClient::recognized, this, &TranslateTool::showResult,
            Qt::UniqueConnection);
    connect(m_ocrClient, &BaiduOcrClient::error, this, [](const QString &msg) {
        Logger::instance().error(QString::fromUtf8("快速翻译 OCR 失败: %1").arg(msg));
        auto *win = new QWidget(nullptr);
        win->setWindowTitle(QString::fromUtf8("快速翻译 - 错误"));
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
        auto *layout = new QVBoxLayout(win);
        auto *label = new QLabel(QString::fromUtf8("OCR识别失败: %1").arg(msg));
        layout->addWidget(label);
        win->show();
    }, Qt::UniqueConnection);

    QTimer::singleShot(200, this, [this]() {
        auto *overlay = new CaptureOverlay;
        connect(overlay, &CaptureOverlay::regionCaptured, this, [this](const QPixmap &pix) {
            Logger::instance().info(QString::fromUtf8("快速翻译: 截屏完成，开始OCR识别"));
            m_ocrClient->recognize(pix);
        });
        connect(overlay, &CaptureOverlay::cancelled, this, []() {
            Logger::instance().info(QString::fromUtf8("快速翻译: 截屏已取消"));
        });
        overlay->start();
    });
}

void TranslateTool::showResult(const QString &ocrText) {
    auto *win = new QWidget(nullptr);
    win->setWindowTitle(QString::fromUtf8("快速翻译"));
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setMinimumSize(520, 440);
    win->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    win->setStyleSheet(
        "QWidget#translateWin { background: #f8f9fa; }"
        "QTextEdit {"
        "  background: #ffffff;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "  selection-background-color: #667eea;"
        "}"
        "QTextEdit:focus { border: 1.5px solid #667eea; }"
        "QComboBox {"
        "  background: #ffffff;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 6px;"
        "  padding: 6px 12px;"
        "  font-size: 13px;"
        "  min-width: 100px;"
        "}"
        "QComboBox:hover { border-color: #667eea; }"
        "QComboBox::drop-down {"
        "  border: none;"
        "  width: 20px;"
        "}"
        "QPushButton {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #667eea,stop:1 #764ba2);"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 20px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #5a6fd6,stop:1 #6a4190);"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4e5fc0,stop:1 #5e3780);"
        "}"
        "QPushButton#copyBtn {"
        "  background: #ffffff;"
        "  color: #667eea;"
        "  border: 1.5px solid #667eea;"
        "}"
        "QPushButton#copyBtn:hover {"
        "  background: #f0f2ff;"
        "}"
        "QLabel#arrowLabel {"
        "  font-size: 18px;"
        "  color: #667eea;"
        "  font-weight: bold;"
        "}"
        "QLabel#titleLabel {"
        "  font-size: 12px;"
        "  color: #888888;"
        "  font-weight: bold;"
        "  margin-bottom: 2px;"
        "}");
    win->setObjectName("translateWin");

    auto *layout = new QVBoxLayout(win);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    // Language selectors
    auto *langLayout = new QHBoxLayout;
    langLayout->setSpacing(10);
    auto *srcLang = new QComboBox;
    srcLang->addItem(QString::fromUtf8("自动检测"), "auto");
    srcLang->addItem(QString::fromUtf8("中文"), "zh");
    srcLang->addItem(QString::fromUtf8("英语"), "en");
    srcLang->addItem(QString::fromUtf8("日语"), "ja");
    srcLang->addItem(QString::fromUtf8("韩语"), "ko");
    srcLang->addItem(QString::fromUtf8("法语"), "fr");
    srcLang->addItem(QString::fromUtf8("德语"), "de");

    auto *arrow = new QLabel(QString::fromUtf8("→"));
    arrow->setObjectName("arrowLabel");
    auto *tgtLang = new QComboBox;
    tgtLang->addItem(QString::fromUtf8("中文"), "zh");
    tgtLang->addItem(QString::fromUtf8("英语"), "en");
    tgtLang->addItem(QString::fromUtf8("日语"), "ja");
    tgtLang->addItem(QString::fromUtf8("韩语"), "ko");
    tgtLang->addItem(QString::fromUtf8("法语"), "fr");
    tgtLang->addItem(QString::fromUtf8("德语"), "de");

    langLayout->addWidget(srcLang);
    langLayout->addWidget(arrow);
    langLayout->addWidget(tgtLang);
    langLayout->addStretch();
    layout->addLayout(langLayout);

    // Source label + input
    auto *srcLabel = new QLabel(QString::fromUtf8("识别文本"));
    srcLabel->setObjectName("titleLabel");
    layout->addWidget(srcLabel);

    auto *input = new QTextEdit;
    input->setPlainText(ocrText);
    input->setMaximumHeight(120);
    layout->addWidget(input);

    // Buttons
    auto *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    auto *translateBtn = new QPushButton(QString::fromUtf8("翻译"));
    auto *copyBtn = new QPushButton(QString::fromUtf8("复制结果"));
    copyBtn->setObjectName("copyBtn");
    btnLayout->addStretch();
    btnLayout->addWidget(translateBtn);
    btnLayout->addWidget(copyBtn);
    layout->addLayout(btnLayout);

    // Output label + text
    auto *tgtLabel = new QLabel(QString::fromUtf8("翻译结果"));
    tgtLabel->setObjectName("titleLabel");
    layout->addWidget(tgtLabel);

    auto *output = new QTextEdit;
    output->setReadOnly(true);
    output->setPlaceholderText(QString::fromUtf8("翻译结果将显示在这里..."));
    layout->addWidget(output);

    connect(m_client, &TencentTranslateClient::translated, output, &QTextEdit::setPlainText);
    connect(m_client, &TencentTranslateClient::error, output, &QTextEdit::setPlainText);

    connect(translateBtn, &QPushButton::clicked, [this, input, output, srcLang, tgtLang]() {
        QString text = input->toPlainText().trimmed();
        if (text.isEmpty()) return;
        QString src = srcLang->currentData().toString();
        QString tgt = tgtLang->currentData().toString();
        output->setPlainText(QString::fromUtf8("翻译中..."));
        m_client->translate(text, src, tgt);
    });

    connect(copyBtn, &QPushButton::clicked, [=]() {
        QString text = output->toPlainText();
        if (!text.isEmpty()) {
            QApplication::clipboard()->setText(text);
        }
    });

    win->show();
    win->activateWindow();

    // Auto-translate on open
    if (!ocrText.trimmed().isEmpty()) {
        QString src = srcLang->currentData().toString();
        QString tgt = tgtLang->currentData().toString();
        output->setPlainText(QString::fromUtf8("翻译中..."));
        m_client->translate(ocrText.trimmed(), src, tgt);
    }
}