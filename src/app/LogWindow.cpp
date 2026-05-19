#include "LogWindow.h"
#include "utils/Logger.h"
#include <QHeaderView>
#include <QDateTime>
#include <QClipboard>
#include <QApplication>
#include <QMenu>

LogWindow::LogWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    refreshLogs();
    connect(&Logger::instance(), &Logger::logAdded, this, &LogWindow::onLogAdded);
}

void LogWindow::setupUI() {
    setWindowTitle(QString::fromUtf8("GTools 日志"));
    setWindowFlags(Qt::Window);
    resize(800, 500);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({
        QString::fromUtf8("时间"),
        QString::fromUtf8("级别"),
        QString::fromUtf8("消息")
    });
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet(
        "QTableWidget {"
        "  background: #ffffff;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 6px;"
        "  gridline-color: #f0f0f0;"
        "}"
        "QTableWidget::item {"
        "  padding: 6px;"
        "}"
        "QTableWidget::item:selected {"
        "  background: #667eea;"
        "  color: white;"
        "}"
        "QHeaderView::section {"
        "  background: #f5f7fa;"
        "  padding: 8px;"
        "  border: none;"
        "  border-bottom: 1px solid #e0e0e0;"
        "  font-weight: bold;"
        "}"
    );
    m_table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_table, &QTableWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        auto *item = m_table->itemAt(pos);
        if (!item) return;
        int row = item->row();
        auto *msgItem = m_table->item(row, 2);
        if (!msgItem) return;
        QMenu menu;
        auto *copyAction = menu.addAction(QString::fromUtf8("复制消息"));
        connect(copyAction, &QAction::triggered, this, [msgItem]() {
            QApplication::clipboard()->setText(msgItem->text());
        });
        menu.exec(m_table->viewport()->mapToGlobal(pos));
    });
    layout->addWidget(m_table);

    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_refreshBtn = new QPushButton(QString::fromUtf8("刷新"), this);
    m_refreshBtn->setFixedWidth(80);
    m_refreshBtn->setStyleSheet(
        "QPushButton {"
        "  background: #667eea;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "  background: #5a6fd6;"
        "}"
        "QPushButton:pressed {"
        "  background: #4a5fc6;"
        "}"
    );
    connect(m_refreshBtn, &QPushButton::clicked, this, &LogWindow::onRefreshClicked);
    btnLayout->addWidget(m_refreshBtn);

    m_clearBtn = new QPushButton(QString::fromUtf8("清空"), this);
    m_clearBtn->setFixedWidth(80);
    m_clearBtn->setStyleSheet(
        "QPushButton {"
        "  background: #ff6b6b;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "  background: #ee5a5a;"
        "}"
        "QPushButton:pressed {"
        "  background: #dd4a4a;"
        "}"
    );
    connect(m_clearBtn, &QPushButton::clicked, this, &LogWindow::onClearClicked);
    btnLayout->addWidget(m_clearBtn);

    layout->addLayout(btnLayout);
}

void LogWindow::refreshLogs() {
    m_table->setRowCount(0);
    auto entries = Logger::instance().entries();
    for (int i = 0; i < entries.size(); ++i) {
        const auto &entry = entries[i];
        int row = m_table->rowCount();
        m_table->insertRow(row);

        auto *timeItem = new QTableWidgetItem(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss"));
        timeItem->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(row, 0, timeItem);

        auto *levelItem = new QTableWidgetItem(entry.level);
        levelItem->setTextAlignment(Qt::AlignCenter);
        QString color = levelColor(entry.level);
        levelItem->setForeground(QColor(color));
        m_table->setItem(row, 1, levelItem);

        auto *msgItem = new QTableWidgetItem(entry.message);
        m_table->setItem(row, 2, msgItem);
    }
}

QString LogWindow::levelColor(const QString& level) const {
    if (level == "ERROR") return "#ff6b6b";
    if (level == "WARN") return "#ffa502";
    return "#26de81";
}

void LogWindow::onLogAdded() {
    refreshLogs();
}

void LogWindow::onClearClicked() {
    Logger::instance().clear();
    m_table->setRowCount(0);
}

void LogWindow::onRefreshClicked() {
    refreshLogs();
}
