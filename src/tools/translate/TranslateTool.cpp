#include "TranslateTool.h"
#include "TencentTranslateClient.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QClipboard>

TranslateTool::TranslateTool() : m_client(new TencentTranslateClient(this)) {}

void TranslateTool::activate(QWidget *parent) {
    auto *win = new QWidget(nullptr);
    win->setWindowTitle(QString::fromUtf8("快速翻译"));
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setMinimumSize(500, 400);
    win->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    auto *layout = new QVBoxLayout(win);

    // Language selectors
    auto *langLayout = new QHBoxLayout;
    auto *srcLang = new QComboBox;
    srcLang->addItem(QString::fromUtf8("自动检测"), "auto");
    srcLang->addItem(QString::fromUtf8("中文"), "zh");
    srcLang->addItem(QString::fromUtf8("英语"), "en");
    srcLang->addItem(QString::fromUtf8("日语"), "ja");
    srcLang->addItem(QString::fromUtf8("韩语"), "ko");
    srcLang->addItem(QString::fromUtf8("法语"), "fr");
    srcLang->addItem(QString::fromUtf8("德语"), "de");

    auto *arrow = new QLabel(" → ");
    auto *tgtLang = new QComboBox;
    tgtLang->addItem(QString::fromUtf8("英语"), "en");
    tgtLang->addItem(QString::fromUtf8("中文"), "zh");
    tgtLang->addItem(QString::fromUtf8("日语"), "ja");
    tgtLang->addItem(QString::fromUtf8("韩语"), "ko");
    tgtLang->addItem(QString::fromUtf8("法语"), "fr");
    tgtLang->addItem(QString::fromUtf8("德语"), "de");

    langLayout->addWidget(srcLang);
    langLayout->addWidget(arrow);
    langLayout->addWidget(tgtLang);
    langLayout->addStretch();
    layout->addLayout(langLayout);

    // Input
    auto *input = new QTextEdit;
    input->setPlaceholderText(QString::fromUtf8("输入要翻译的文本..."));
    layout->addWidget(input);

    // Buttons
    auto *btnLayout = new QHBoxLayout;
    auto *translateBtn = new QPushButton(QString::fromUtf8("翻译"));
    auto *copyBtn = new QPushButton(QString::fromUtf8("复制结果"));
    btnLayout->addStretch();
    btnLayout->addWidget(translateBtn);
    btnLayout->addWidget(copyBtn);
    layout->addLayout(btnLayout);

    // Output
    auto *output = new QTextEdit;
    output->setReadOnly(true);
    output->setPlaceholderText(QString::fromUtf8("翻译结果..."));
    layout->addWidget(output);

    // Connections
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
}
