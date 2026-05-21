#include "ToolPanel.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QEvent>

ToolPanel::ToolPanel(QWidget *parent) : QWidget(parent) {
    setupUI();
}

bool ToolPanel::event(QEvent *e) {
    if (e->type() == QEvent::WindowDeactivate) {
        hide();
        return true;
    }
    return QWidget::event(e);
}

void ToolPanel::setupUI() {
    setWindowTitle(QString::fromUtf8("GTools 工具面板"));
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    setFixedSize(280, 400);
    
    setStyleSheet(
        "QWidget {"
        "  background: #f5f5f5;"
        "  font-family: 'Microsoft YaHei', 'Segoe UI', sans-serif;"
        "}"
        "QPushButton {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 8px;"
        "}"
        "QPushButton:hover {"
        "  background: #e8e8e8;"
        "}"
        "QPushButton:pressed {"
        "  background: #dcdcdc;"
        "}"
        "QLabel {"
        "  color: #333333;"
        "  font-size: 12px;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(8);

    m_gridLayout = new QGridLayout();
    m_gridLayout->setSpacing(8);
    m_gridLayout->setHorizontalSpacing(8);
    m_gridLayout->setVerticalSpacing(12);

    // 添加工具按钮
    addToolButton("screenshot", QString::fromUtf8("截图"), ":/icons/screenshot.png");
    addToolButton("ocr", QString::fromUtf8("OCR"), ":/icons/ocr.png");
    addToolButton("colorpicker", QString::fromUtf8("取色"), ":/icons/colorpicker.png");
    addToolButton("translate", QString::fromUtf8("翻译"), ":/icons/translate.png");
    addToolButton("markdown", QString::fromUtf8("笔记"), ":/icons/markdown.png");

    mainLayout->addLayout(m_gridLayout);
    mainLayout->addStretch();
}

void ToolPanel::addToolButton(const QString &id, const QString &name, const QString &iconPath) {
    auto *btn = new QPushButton(this);
    btn->setFixedSize(80, 80);
    btn->setToolTip(name);
    
    // 创建垂直布局放置图标和文字
    auto *vLayout = new QVBoxLayout(btn);
    vLayout->setContentsMargins(4, 4, 4, 4);
    vLayout->setSpacing(4);
    vLayout->setAlignment(Qt::AlignCenter);
    
    // 图标
    auto *iconLabel = new QLabel(btn);
    QPixmap pixmap(iconPath);
    iconLabel->setPixmap(pixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(40, 40);
    vLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    
    // 文字
    auto *textLabel = new QLabel(name, btn);
    textLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(textLabel, 0, Qt::AlignCenter);
    
    connect(btn, &QPushButton::clicked, this, [this, id]() {
        emit toolActivated(id);
    });
    
    m_gridLayout->addWidget(btn, m_currentRow, m_currentCol);
    
    m_currentCol++;
    if (m_currentCol >= 3) {
        m_currentCol = 0;
        m_currentRow++;
    }
}
