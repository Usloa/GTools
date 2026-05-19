#include "LauncherWindow.h"
#include "core/ToolRegistry.h"
#include <QKeyEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QGraphicsDropShadowEffect>
#include <QApplication>

LauncherWindow::LauncherWindow(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedWidth(600);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->setSpacing(4);

    m_input = new QLineEdit(this);
    m_input->setPlaceholderText(QString::fromUtf8("输入工具名称..."));
    m_input->setFixedHeight(44);
    m_input->setStyleSheet(
        "QLineEdit {"
        "  background: #ffffff;"
        "  border: 2px solid #e0e0e0;"
        "  border-radius: 10px;"
        "  padding: 0 16px;"
        "  font-size: 15px;"
        "  selection-background-color: #667eea;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #667eea;"
        "}");
    m_layout->addWidget(m_input);

    m_results = new QListWidget(this);
    m_results->setStyleSheet(
        "QListWidget {"
        "  background: #ffffff;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  padding: 4px;"
        "  outline: none;"
        "}"
        "QListWidget::item {"
        "  padding: 10px 12px;"
        "  border-radius: 6px;"
        "  margin: 2px 0;"
        "  background: transparent;"
        "}"
        "QListWidget::item:hover {"
        "  background: #f5f7fa;"
        "}"
        "QListWidget::item:selected {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2);"
        "  color: white;"
        "}"
        "QListWidget::item:selected:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #5a6fd6, stop:1 #6a4190);"
        "}");
    m_results->setFixedHeight(240);
    m_results->hide();
    m_layout->addWidget(m_results);

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 60));
    setGraphicsEffect(shadow);

    connect(m_input, &QLineEdit::textChanged, this, &LauncherWindow::onInputChanged);
    connect(m_results, &QListWidget::itemActivated, this, &LauncherWindow::onItemActivated);
}

void LauncherWindow::showLauncher() {
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (!screen) screen = QGuiApplication::primaryScreen();
    QRect geo = screen->availableGeometry();
    int x = geo.x() + (geo.width() - width()) / 2;
    int y = geo.y() + geo.height() / 4;
    move(x, y);

    m_input->clear();
    m_results->hide();
    adjustSize();
    show();
    activateWindow();
    m_input->setFocus();
    updateResults("");
}

void LauncherWindow::hideLauncher() {
    hide();
}

bool LauncherWindow::event(QEvent *e) {
    if (e->type() == QEvent::WindowDeactivate) {
        hideLauncher();
        return true;
    }
    return QWidget::event(e);
}

void LauncherWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        hideLauncher();
    } else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        auto *item = m_results->currentItem();
        if (item) onItemActivated(item);
    } else if (e->key() == Qt::Key_Down) {
        int row = m_results->currentRow();
        if (row < m_results->count() - 1) m_results->setCurrentRow(row + 1);
    } else if (e->key() == Qt::Key_Up) {
        int row = m_results->currentRow();
        if (row > 0) m_results->setCurrentRow(row - 1);
    } else {
        QWidget::keyPressEvent(e);
    }
}

void LauncherWindow::onInputChanged(const QString &text) {
    updateResults(text);
}

void LauncherWindow::onItemActivated(QListWidgetItem *item) {
    QString toolId = item->data(Qt::UserRole).toString();
    Tool *tool = ToolRegistry::instance().findById(toolId);
    if (tool) {
        hideLauncher();
        tool->activate(this);
    }
}

void LauncherWindow::updateResults(const QString &query) {
    m_results->clear();
    auto tools = ToolRegistry::instance().search(query);
    for (Tool *t : tools) {
        auto *item = new QListWidgetItem(t->icon(), t->name());
        item->setData(Qt::UserRole, t->id());
        m_results->addItem(item);
    }
    if (m_results->count() > 0) {
        m_results->show();
        m_results->setCurrentRow(0);
        adjustSize();
    } else {
        m_results->hide();
        adjustSize();
    }
}
